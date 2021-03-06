/*
  Copyright (C) 2007 National Institute For Space Research (INPE) - Brazil.

  This file is part of TerraMA2 - a free and open source computational
  platform for analysis, monitoring, and alert of geo-environmental extremes.

  TerraMA2 is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License,
  or (at your option) any later version.

  TerraMA2 is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with TerraMA2. See LICENSE. If not, write to
  TerraMA2 Team at <terrama2-team@dpi.inpe.br>.
*/

/*!
  \file src/terrama2/services/view/core/View.cpp

  \brief View service functions

  \author Vinicius Campanha
*/

// STL
#include <cmath>
#include <algorithm>

// TerraLib
#include <terralib/common/StringUtils.h>
#include <terralib/qt/widgets/canvas/Canvas.h>

// TerraMA2
#include "View.hpp"
#include "ViewStyle.hpp"
#include "MemoryDataSetLayer.hpp"
#include "DataManager.hpp"
#include "Exception.hpp"

#include "../../../core/data-model/DataSeries.hpp"
#include "../../../core/data-model/DataSet.hpp"
#include "../../../core/data-model/Filter.hpp"

#include "../../../core/data-access/DataAccessor.hpp"
#include "../../../core/data-access/DataStorager.hpp"

#include "../../../core/utility/Timer.hpp"
#include "../../../core/utility/TimeUtils.hpp"
#include "../../../core/utility/Logger.hpp"
#include "../../../core/utility/DataAccessorFactory.hpp"
#include "../../../core/utility/DataStoragerFactory.hpp"
#include "../../../core/utility/ServiceManager.hpp"

void terrama2::services::view::core::makeView(ViewId viewId, std::shared_ptr< terrama2::services::view::core::ViewLogger > logger, std::weak_ptr<DataManager> weakDataManager)
{
  auto dataManager = weakDataManager.lock();
  if(!dataManager.get())
  {
    TERRAMA2_LOG_ERROR() << QObject::tr("Unable to access DataManager");
    return;
  }

  try
  {
    RegisterId logId = 0;
    if(logger.get())
      logId = logger->start(viewId);

    TERRAMA2_LOG_DEBUG() << QObject::tr("Starting view %1 generation.").arg(viewId);

    auto lock = dataManager->getLock();

    auto viewPtr = dataManager->findView(viewId);

    if(viewPtr->dataSeriesList.size() != viewPtr->filtersPerDataSeries.size())
    {
      QString message = QObject::tr("View %1 do not have the right number of filters for data.").arg(viewId);
      if(logger.get())
        logger->error(message.toStdString(), viewId);
      TERRAMA2_LOG_ERROR() << message;
      throw Exception() << ErrorDescription(message);
    }

    std::vector<std::unordered_map<terrama2::core::DataSetPtr, terrama2::core::DataSetSeries>> seriesList;

    for(auto dataSeriesId : viewPtr->dataSeriesList)
    {
      terrama2::core::DataSeriesPtr inputDataSeries = dataManager->findDataSeries(dataSeriesId);
      terrama2::core::DataProviderPtr inputDataProvider = dataManager->findDataProvider(inputDataSeries->dataProviderId);

      auto dataAccessor = terrama2::core::DataAccessorFactory::getInstance().make(inputDataProvider, inputDataSeries);

      terrama2::core::Filter filter(viewPtr->filtersPerDataSeries.at(dataSeriesId));
      std::unordered_map<terrama2::core::DataSetPtr, terrama2::core::DataSetSeries > series = dataAccessor->getSeries(filter);

      seriesList.push_back(series);
    }

    lock.unlock();

    if(seriesList.size() > 0)
    {
      drawSeriesList(viewId, logger, seriesList, viewPtr->resolutionWidth, viewPtr->resolutionHeight, viewPtr->srid);

      TERRAMA2_LOG_INFO() << QObject::tr("View %1 generated successfully.").arg(viewId);
    }
    else
    {
      QString message = QObject::tr("View %1 has no associated data to be generated.").arg(viewId);
      if(logger.get())
        logger->info(message.toStdString(), viewId);
      TERRAMA2_LOG_INFO() << message;
    }

    if(logger.get())
      logger->done(terrama2::core::TimeUtils::nowUTC(), logId);
  }
  catch(const terrama2::Exception& e)
  {
    TERRAMA2_LOG_ERROR() << boost::get_error_info<terrama2::ErrorDescription>(e) << std::endl;
    TERRAMA2_LOG_INFO() << QObject::tr("Build of view %1 finished with error(s).").arg(viewId);
  }
  catch(const boost::exception& e)
  {
    TERRAMA2_LOG_ERROR() << boost::get_error_info<terrama2::ErrorDescription>(e);
    TERRAMA2_LOG_INFO() << QObject::tr("Build of view %1 finished with error(s).").arg(viewId);
  }
  catch(const std::exception& e)
  {
    TERRAMA2_LOG_ERROR() << e.what();
    TERRAMA2_LOG_INFO() << QObject::tr("Build of view %1 finished with error(s).").arg(viewId);
  }
  catch(...)
  {
    TERRAMA2_LOG_ERROR() << QObject::tr("Unkown error.");
    TERRAMA2_LOG_INFO() << QObject::tr("Build of view %1 finished with error(s).").arg(viewId);
  }
}


void terrama2::services::view::core::drawSeriesList(ViewId viewId, std::shared_ptr< terrama2::services::view::core::ViewLogger > logger, std::vector<std::unordered_map<terrama2::core::DataSetPtr, terrama2::core::DataSetSeries>>& seriesList, uint32_t resolutionWidth, uint32_t resolutionHeigth, uint32_t srid)
{
  std::vector< std::shared_ptr<te::map::MemoryDataSetLayer> > layersList;
  uint32_t layerID = 0;

  if(resolutionWidth == 0 ||  resolutionHeigth == 0)
  {
    QString message = QObject::tr("Invalid resolution for View %1.").arg(viewId);
    logger->error(message.toStdString(), viewId);
    throw Exception() << ErrorDescription(message);
  }

  // Create layers from series
  for(auto& series : seriesList)
  {
    for(auto& serie : series)
    {
      terrama2::core::DataSetPtr dataset = serie.first;
      std::shared_ptr<te::da::DataSet> teDataSet = serie.second.syncDataSet->dataset();
      std::shared_ptr<te::da::DataSetType> teDataSetType = serie.second.teDataSetType;

      if(!teDataSetType->hasRaster() && !teDataSetType->hasGeom())
      {
        QString message = QObject::tr("DataSet %1 has no drawable data.").arg(QString::fromStdString(teDataSetType->getDatasetName()));
        logger->error(message.toStdString(), viewId);
      }

      if(teDataSetType->hasRaster())
      {
        // TODO: A terralib dataset can have more than one raster field in it?
        std::size_t rpos = te::da::GetFirstPropertyPos(teDataSet.get(), te::dt::RASTER_TYPE);

        if(!teDataSet->moveFirst())
        {
          QString message = QObject::tr("Can not access DataSet %1 raster data.").arg(QString::fromStdString(teDataSetType->getDatasetName()));
          logger->error(message.toStdString(), viewId);
        }
        else
        {
          auto raster(teDataSet->getRaster(rpos));

          te::gm::Envelope* extent = raster->getExtent();

          // Creates a DataSetLayer of raster
          std::shared_ptr<te::map::MemoryDataSetLayer> rasterLayer(new te::map::MemoryDataSetLayer(te::common::Convert2String(++layerID), raster->getName(), teDataSet, teDataSetType));
          rasterLayer->setDataSetName(teDataSetType->getDatasetName());
          rasterLayer->setExtent(*extent);
          rasterLayer->setRendererType("ABSTRACT_LAYER_RENDERER");

          // if dataset SRID is not setted, try to use the SRID from layer
          if(dataset->format.find("srid") == dataset->format.end())
            rasterLayer->setSRID(raster->getSRID());
          else
            rasterLayer->setSRID(std::stoi(dataset->format.at("srid")));

          // VINICIUS: Set Style
          MONO_0_Style(rasterLayer);

          layersList.push_back(rasterLayer);
        }
      }

      if(teDataSetType->hasGeom())
      {
        // TODO: A terralib dataset can have more than one geometry field in it?
        auto geomProperty = te::da::GetFirstGeomProperty(teDataSetType.get());

        if(!teDataSet->moveFirst())
        {
          QString message = QObject::tr("Can not access DataSet %1 geometry data.").arg(QString::fromStdString(teDataSetType->getDatasetName()));
          logger->error(message.toStdString(), viewId);
        }
        else
        {
          std::shared_ptr< te::gm::Envelope > extent(teDataSet->getExtent(teDataSetType->getPropertyPosition(geomProperty)));

          // Creates a Layer
          std::shared_ptr< te::map::MemoryDataSetLayer > geomLayer(new te::map::MemoryDataSetLayer(te::common::Convert2String(++layerID), geomProperty->getName(), teDataSet, teDataSetType));
          geomLayer->setDataSetName(teDataSetType->getName());
          geomLayer->setVisibility(te::map::VISIBLE);
          geomLayer->setExtent(*extent);
          geomLayer->setRendererType("ABSTRACT_LAYER_RENDERER");

          // if dataset SRID is not setted, try to use the SRID from layer
          if(dataset->format.find("srid") == dataset->format.end())
            geomLayer->setSRID(geomProperty->getSRID());
          else
            geomLayer->setSRID(std::stoi(dataset->format.at("srid")));

          // VINICIUS: set style
          geomLayer->setStyle(CreateFeatureTypeStyle(geomProperty->getGeometryType()));

          layersList.push_back(geomLayer);
        }
      }
    }
  }

  // Draw layers

  if(layersList.size() > 0)
  {
    te::gm::Envelope extent;

    for(auto& layer : layersList)
    {
      if(!extent.isValid())
        extent = layer->getExtent();
      else
        extent.Union(layer->getExtent());

      // If the SRID was not setted, use the SRID from the first layer
      if(srid == 0)
        srid = layer->getSRID();
    }

    // Creates a canvas
    double llx = extent.m_llx;
    double lly = extent.m_lly;
    double urx = extent.m_urx;
    double ury = extent.m_ury;

    std::unique_ptr<te::qt::widgets::Canvas> canvas(new te::qt::widgets::Canvas(resolutionWidth, resolutionHeigth));
    canvas->calcAspectRatio(llx, lly, urx, ury);
    canvas->setWindow(llx, lly, urx, ury);
    canvas->setBackgroundColor(te::color::RGBAColor(255, 255, 255, TE_OPAQUE));

    bool cancel = false;

    for(auto& layer : layersList)
    {
      layer->draw(canvas.get(), extent, srid, 0, &cancel);
    }

    // Save view

    canvas->save("GeneretadImage", te::map::PNG);

    canvas->clear();
  }
  else
  {
    QString message = QObject::tr("View %1 could not find any data.").arg(viewId);
    logger->error(message.toStdString(), viewId);
    throw Exception() << ErrorDescription(message);
  }
}


