# Copyright 2019-present Julián Bermúdez Ortega.
#
# This file is part of julibert::idlcom.
#
# julibert::idlcom is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# julibert::idlcom is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with julibert::idlcom.  If not, see <https://www.gnu.org/licenses/>.

set(_test_name "unit_test_asset_cpp_stream")

add_executable(${_test_name} ./stream.cpp)

target_link_libraries(${_test_name}
  PRIVATE
    once::cpputils
    Catch2::Catch2
  )

set_target_properties(${_test_name} PROPERTIES
  CXX_STANDARD
    17
  CXX_STANDARD_REQUIRED
    YES
  )

catch_discover_tests(${_test_name})