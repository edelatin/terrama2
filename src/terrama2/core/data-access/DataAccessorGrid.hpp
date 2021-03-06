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
  \file terrama2/core/data-access/DataAccessorGrid.hpp

  \brief Base class to access data from a Grid DataSeries.

  \author Jano Simas
 */

#ifndef __TERRAMA2_CORE_DATA_ACCESS_DATA_ACCESSOR_GRID_HPP__
#define __TERRAMA2_CORE_DATA_ACCESS_DATA_ACCESSOR_GRID_HPP__

//TerraMA2
#include "../Shared.hpp"
#include "DataAccessor.hpp"
#include "DcpSeries.hpp"

namespace terrama2
{
  namespace core
  {
    /*!
      \class DataAccessorGrid
      \brief Base class to access data from a Grid DataSeries.

      Grid data are data organized in a grid with horizontal as vertical spatial indexing.

      This could be a GeoTiff, Ascii-Grid, Grads, WCS or similar data.

    */
    class DataAccessorGrid : public virtual DataAccessor
    {
      public:
        //! Default constructor
        DataAccessorGrid(DataProviderPtr dataProvider, DataSeriesPtr dataSeries, Filter filter = Filter()) : DataAccessor(dataProvider, dataSeries, filter) {}
        //! Default destructor.
        virtual ~DataAccessorGrid() = default;
        //! Default copy constructor
        DataAccessorGrid(const DataAccessorGrid& other) = default;
        //! Default move constructor
        DataAccessorGrid(DataAccessorGrid&& other) = default;
        //! Default const assignment operator
        DataAccessorGrid& operator=(const DataAccessorGrid& other) = default;
        //! Default assignment operator
        DataAccessorGrid& operator=(DataAccessorGrid&& other) = default;

        //! Returns a GridSeries filtered by Filter
        virtual GridSeriesPtr getGridSeries(const Filter& filter);

      protected:
        // Doc in base class
        virtual void addColumns(std::shared_ptr<te::da::DataSetTypeConverter> converter, const std::shared_ptr<te::da::DataSetType>& datasetType) const override;
    };
  }
}

#endif // __TERRAMA2_CORE_DATA_ACCESS_DATA_ACCESSOR_GRID_HPP__
