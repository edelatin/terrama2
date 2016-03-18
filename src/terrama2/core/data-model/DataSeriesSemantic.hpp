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
  \file terrama2/core/data-model/DataSeriesSemantic.hpp

  \brief

  \author Jano Simas
*/

#ifndef __TERRAMA2_CORE_DATA_MODEL_DATA_SERIES_SEMANTIC_HPP__
#define __TERRAMA2_CORE_DATA_MODEL_DATA_SERIES_SEMANTIC_HPP__

#include <string>

namespace terrama2
{
  namespace core
  {
    typedef uint64_t DataFormat;
    struct DataSeriesSemantic
    {
      enum MacroType {
        DCP,
        OCCURRENCE,
        GRID,
        MONITORED,
        STATIC
      };

      std::string name;
      MacroType macroType = STATIC;
      DataFormat dataFormat = 0;
    };
  } /* core */
} /* terrama2 */

#endif //__TERRAMA2_CORE_DATA_MODEL_DATA_SERIES_SEMANTIC_HPP__