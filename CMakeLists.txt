# Copyright 2019-present Julián Bermúdez Ortega.
#
# This file is part of julibert::cpputils.
#
# julibert::cpputils is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# julibert::cpputils is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with julibert::cpputils.  If not, see <https://www.gnu.org/licenses/>.

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
        git@github.com:julibert/harvester.git
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
    COMPONENT
        library
    )

install(
    TARGETS
        cpputils
    EXPORT
        cpputils-targets
    )

install(
    EXPORT
        cpputils-targets
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