#
#  Copyright (C) 2007 National Institute For Space Research (INPE) - Brazil.
#
#  This file is part of TerraMA2 - a free and open source computational
#  platform for analysis, monitoring, and alert of geo-environmental extremes.
#
#  TerraMA2 is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License,
#  or (at your option) any later version.
#
#  TerraMA2 is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with TerraMA2. See LICENSE. If not, write to
#  TerraMA2 Team at <terrama2-team@dpi.inpe.br>.
#
#
#  Description: CMake script for the TerraMA2 Analysis Core Runtime Module.
#
#  Author: Gilberto Ribeiro de Queiroz
#

#if(WIN32)
#  add_definitions(-DTMA_CORE_DLL)
#endif()

file(GLOB TERRAMA2_SRC_FILES ${TERRAMA2_ABSOLUTE_ROOT_DIR}/src/terrama2/services/analysis/core/*.cpp)
file(GLOB TERRAMA2_HDR_FILES ${TERRAMA2_ABSOLUTE_ROOT_DIR}/src/terrama2/services/analysis/core/*.hpp)

file(GLOB TERRAMA2_DCP_SRC_FILES ${TERRAMA2_ABSOLUTE_ROOT_DIR}/src/terrama2/services/analysis/core/dcp/*.cpp)
file(GLOB TERRAMA2_DCP_HDR_FILES ${TERRAMA2_ABSOLUTE_ROOT_DIR}/src/terrama2/services/analysis/core/dcp/*.hpp)

file(GLOB TERRAMA2_DCP_HISTORY_SRC_FILES ${TERRAMA2_ABSOLUTE_ROOT_DIR}/src/terrama2/services/analysis/core/dcp/history/*.cpp)
file(GLOB TERRAMA2_DCP_HISTORY_HDR_FILES ${TERRAMA2_ABSOLUTE_ROOT_DIR}/src/terrama2/services/analysis/core/dcp/history/*.hpp)

file(GLOB TERRAMA2_OCCURRENCE_SRC_FILES ${TERRAMA2_ABSOLUTE_ROOT_DIR}/src/terrama2/services/analysis/core/occurrence/*.cpp)
file(GLOB TERRAMA2_OCCURRENCE_HDR_FILES ${TERRAMA2_ABSOLUTE_ROOT_DIR}/src/terrama2/services/analysis/core/occurrence/*.hpp)

file(GLOB TERRAMA2_OCCURRENCE_AGRREGATION_SRC_FILES ${TERRAMA2_ABSOLUTE_ROOT_DIR}/src/terrama2/services/analysis/core/occurrence/aggregation/*.cpp)
file(GLOB TERRAMA2_OCCURRENCE_AGGREGATION_HDR_FILES ${TERRAMA2_ABSOLUTE_ROOT_DIR}/src/terrama2/services/analysis/core/occurrence/aggregation/*.hpp)

file(GLOB TERRAMA2_GRID_SRC_FILES ${TERRAMA2_ABSOLUTE_ROOT_DIR}/src/terrama2/services/analysis/core/grid/*.cpp)
file(GLOB TERRAMA2_GRID_HDR_FILES ${TERRAMA2_ABSOLUTE_ROOT_DIR}/src/terrama2/services/analysis/core/grid/*.hpp)



source_group("Source Files"  FILES ${TERRAMA2_SRC_FILES})
source_group("Header Files"  FILES ${TERRAMA2_HDR_FILES})
source_group("Source Files\\dcp"  FILES ${TERRAMA2_DCP_SRC_FILES})
source_group("Header Files\\dcp"  FILES ${TERRAMA2_DCP_HDR_FILES})
source_group("Source Files\\dcp\\history"  FILES ${TERRAMA2_DCP_HISTORY_SRC_FILES})
source_group("Header Files\\dcp\\history"  FILES ${TERRAMA2_DCP_HISTORY_HDR_FILES})
source_group("Source Files\\occurrence"  FILES ${TERRAMA2_OCCURRENCE_SRC_FILES})
source_group("Header Files\\occurrence"  FILES ${TERRAMA2_OCCURRENCE_HDR_FILES})
source_group("Source Files\\occurrence\\aggregation"  FILES ${TERRAMA2_OCCURRENCE_AGRREGATION_SRC_FILES})
source_group("Header Files\\occurrence\\aggregation"  FILES ${TERRAMA2_OCCURRENCE_AGGREGATION_HDR_FILES})
source_group("Source Files\\grid"  FILES ${TERRAMA2_GRID_SRC_FILES})
source_group("Header Files\\grid"  FILES ${TERRAMA2_GRID_HDR_FILES})

include_directories ( SYSTEM
  ${Boost_INCLUDE_DIR}
  ${terralib_INCLUDE_DIRS}
  ${terralib_DIR}
  ${PYTHON_INCLUDE_DIRS}
  ${Qt5Core_INCLUDE_DIRS}
)

set(CMAKE_AUTOMOC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(terrama2_analysis_core SHARED ${TERRAMA2_SRC_FILES}
                                          ${TERRAMA2_HDR_FILES}
                                          ${TERRAMA2_DCP_SRC_FILES}
                                          ${TERRAMA2_DCP_HDR_FILES}
                                          ${TERRAMA2_DCP_HISTORY_SRC_FILES}
                                          ${TERRAMA2_DCP_HISTORY_HDR_FILES}
                                          ${TERRAMA2_OCCURRENCE_SRC_FILES}
                                          ${TERRAMA2_OCCURRENCE_HDR_FILES}
                                          ${TERRAMA2_OCCURRENCE_AGRREGATION_SRC_FILES}
                                          ${TERRAMA2_OCCURRENCE_AGGREGATION_HDR_FILES_SRC_FILES}
                                          ${TERRAMA2_GRID_SRC_FILES}
                                          ${TERRAMA2_GRID_HDR_FILES})

qt5_use_modules(terrama2_analysis_core Core)



target_link_libraries(terrama2_analysis_core  terrama2_core
                                              terrama2_impl
                                              terralib_mod_dataaccess
                                              terralib_mod_memory
                                              terralib_mod_vp_core
                                              terralib_mod_rp
                                              ${PYTHON_LIBRARIES}
                                              ${Boost_PYTHON_LIBRARY})


PYTHON_ADD_MODULE(terrama2 ${TERRAMA2_ABSOLUTE_ROOT_DIR}/src/terrama2/services/analysis/core/PythonInterpreter.cpp)

qt5_use_modules(terrama2 Core)


target_link_libraries(terrama2  terrama2_analysis_core
                                terrama2_core
                                terrama2_impl
                                terralib_mod_dataaccess
                                terralib_mod_memory
                                terralib_mod_vp_core
                                terralib_mod_rp
                                ${PYTHON_LIBRARIES}
                                ${Boost_PYTHON_LIBRARY})

set_target_properties(terrama2_analysis_core
                      PROPERTIES VERSION ${TERRAMA2_VERSION_MAJOR}.${TERRAMA2_VERSION_MINOR}
                                 SOVERSION ${TERRAMA2_VERSION_MAJOR}.${TERRAMA2_VERSION_MINOR}
                                 INSTALL_NAME_DIR "@executable_path/../lib")

install(TARGETS terrama2_analysis_core
        EXPORT terrama2-targets
        RUNTIME DESTINATION ${TERRAMA2_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRAMA2_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRAMA2_DESTINATION_ARCHIVE} COMPONENT runtime)

install(FILES ${TERRAMA2_HDR_FILES}
DESTINATION ${TERRAMA2_DESTINATION_HEADERS}/terrama2/analysis COMPONENT devel)

export(TARGETS terrama2_analysis_core APPEND FILE ${CMAKE_BINARY_DIR}/terrama2-exports.cmake)
