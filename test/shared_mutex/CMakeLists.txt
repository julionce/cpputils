# Copyright 2019-present Julián Bermúdez Ortega.
#
# This file is part of VaneinsCpputils.
# 
# VaneinsCpputils is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# VaneinsCpputils is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with VaneinsCpputils.  If not, see <https://www.gnu.org/licenses/>.

include(CTest)
include(${PROJECT_SOURCE_DIR}/thirdparty/catch2/contrib/Catch.cmake)
include(${PROJECT_SOURCE_DIR}/thirdparty/catch2/contrib/ParseAndAddCatchTests.cmake)

find_package(Threads REQUIRED)

set(_test_name "test-shared-mutex")

add_executable(${_test_name} ${PROJECT_SOURCE_DIR}/test/shared_mutex.cpp)

target_link_libraries(${_test_name}
    PRIVATE
        vaneins::cpputils
        Catch2::Catch2
        Threads::Threads
    )

set_target_properties(${_test_name} PROPERTIES
    CXX_STANDARD
        11
    CXX_STANDARD_REQUIRED
        YES
    )

catch_discover_tests(${_test_name})