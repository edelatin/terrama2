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
  \file terrama2/core/impl/Utils.hpp

  \brief Utility funtions for impl classes.

  \author Paulo R. M. Oliveira
 */

#include "Utils.hpp"

#include "DataAccessorDcpInpe.hpp"
#include "DataAccessorDcpToa5.hpp"
#include "DataAccessorDcpPostGIS.hpp"
#include "DataAccessorGeoTiff.hpp"
#include "DataAccessorOccurrenceWfp.hpp"
#include "DataAccessorOccurrencePostGis.hpp"
#include "DataAccessorStaticDataOGR.hpp"
#include "DataAccessorAnalysisPostGis.hpp"

#include "DataStoragerPostGis.hpp"

#include "DataRetrieverFTP.hpp"

#include "../core/utility/DataAccessorFactory.hpp"
#include "../core/utility/DataStoragerFactory.hpp"
#include "../core/utility/DataRetrieverFactory.hpp"

void terrama2::core::registerFactories()
{
  // Data access
  terrama2::core::DataAccessorFactory::getInstance().add(terrama2::core::DataAccessorDcpInpe::dataAccessorType(), terrama2::core::DataAccessorDcpInpe::make);
  terrama2::core::DataAccessorFactory::getInstance().add(terrama2::core::DataAccessorDcpToa5::dataAccessorType(), terrama2::core::DataAccessorDcpToa5::make);
  terrama2::core::DataAccessorFactory::getInstance().add(terrama2::core::DataAccessorDcpPostGIS::dataAccessorType(), terrama2::core::DataAccessorDcpPostGIS::make);
  terrama2::core::DataAccessorFactory::getInstance().add(terrama2::core::DataAccessorGeoTiff::dataAccessorType(), terrama2::core::DataAccessorGeoTiff::make);
  terrama2::core::DataAccessorFactory::getInstance().add(terrama2::core::DataAccessorOccurrenceWfp::dataAccessorType(), terrama2::core::DataAccessorOccurrenceWfp::make);
  terrama2::core::DataAccessorFactory::getInstance().add(terrama2::core::DataAccessorOccurrencePostGis::dataAccessorType(), terrama2::core::DataAccessorOccurrencePostGis::make);
  terrama2::core::DataAccessorFactory::getInstance().add(terrama2::core::DataAccessorStaticDataOGR::dataAccessorType(), terrama2::core::DataAccessorStaticDataOGR::make);
  terrama2::core::DataAccessorFactory::getInstance().add(terrama2::core::DataAccessorAnalysisPostGis::dataAccessorType(), terrama2::core::DataAccessorAnalysisPostGis::make);

  // Data storager
  terrama2::core::DataStoragerFactory::getInstance().add(terrama2::core::DataStoragerPostGis::dataStoragerType(), terrama2::core::DataStoragerPostGis::make);

  terrama2::core::DataRetrieverFactory::getInstance().add(terrama2::core::DataRetrieverFTP::dataRetrieverType(), terrama2::core::DataRetrieverFTP::make);
}
