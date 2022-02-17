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

set(_test_name "unit_test_type_traits")

add_executable(${_test_name} ${CMAKE_CURRENT_SOURCE_DIR}/type_traits.cpp
                             ${CMAKE_CURRENT_SOURCE_DIR}/less_than_comparable.cpp
                             ${CMAKE_CURRENT_SOURCE_DIR}/equality_comparable.cpp
                             ${CMAKE_CURRENT_SOURCE_DIR}/aggregate_initializable.cpp
                             ${CMAKE_CURRENT_SOURCE_DIR}/type_container_unit_test.cpp
    )

target_link_libraries(${_test_name}
    PRIVATE
        once::cpputils
        Catch2::Catch2
    )

set_target_properties(${_test_name} PROPERTIES
    CXX_STANDARD
        17
    CMAKE_CXX_STANDARD_REQUIRED
        YES
    )

catch_discover_tests(${_test_name})