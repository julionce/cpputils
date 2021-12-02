# Copyright 2021-present Julián Bermúdez Ortega
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

###############################################################################
# Options.
###############################################################################
option(CPPUTILS_BUILD_TESTS "Build tests." OFF)

###############################################################################
# Project.
###############################################################################
project(cpputils VERSION "0.1.0" LANGUAGES CXX)

###############################################################################
# Configuration.
###############################################################################
include(GNUInstallDirs)
set(DATA_INSTALL_DIR    ${CMAKE_INSTALL_DATADIR}    CACHE PATH "Data install directory.")
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR} CACHE PATH "Headers install directory.")

###############################################################################
# Dependencies.
###############################################################################
include(FetchContent)
FetchContent_Declare(harvester
    GIT_REPOSITORY
        https://github.com/julibert/harvester.git
    GIT_TAG
        master
    )
FetchContent_GetProperties(harvester)
if(NOT harvester_POPULATED)
    FetchContent_Populate(harvester)
endif()
list(APPEND CMAKE_MODULE_PATH "${harvester_SOURCE_DIR}")
include(HarvestPackage)

###############################################################################
# Library.
###############################################################################
add_library(${PROJECT_NAME} INTERFACE)
add_library(julibert::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    )

###############################################################################
# Tests.
###############################################################################
if(CPPUTILS_BUILD_TESTS)
    harvest_package(Catch2)
    include(CTest)
    include(Catch)

    add_subdirectory(${PROJECT_SOURCE_DIR}/test/reference)
    add_subdirectory(${PROJECT_SOURCE_DIR}/test/shared_mutex)
    add_subdirectory(${PROJECT_SOURCE_DIR}/test/tree)
    add_subdirectory(${PROJECT_SOURCE_DIR}/test/stream)
endif()

###############################################################################
# Packaging.
###############################################################################
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/packaging/config.cmake.in
        ${PROJECT_BINARY_DIR}/cmake/config/${PROJECT_NAME}-config.cmake
    INSTALL_DESTINATION
        ${DATA_INSTALL_DIR}/${PROJECT_NAME}/cmake
    PATH_VARS
        INCLUDE_INSTALL_DIR
    )

install(
    FILES
        ${PROJECT_BINARY_DIR}/cmake/config/${PROJECT_NAME}-config.cmake
    DESTINATION
        ${DATA_INSTALL_DIR}/${PROJECT_NAME}/cmake
    )

install(
    TARGETS
        ${PROJECT_NAME}
    EXPORT
        ${PROJECT_NAME}-targets
    )

install(
    EXPORT
        ${PROJECT_NAME}-targets
    NAMESPACE
        julibert::
    DESTINATION
        ${DATA_INSTALL_DIR}/${PROJECT_NAME}/cmake
    )

install(
    DIRECTORY
        ${PROJECT_SOURCE_DIR}/include/
    DESTINATION
        ${INCLUDE_INSTALL_DIR}
    )