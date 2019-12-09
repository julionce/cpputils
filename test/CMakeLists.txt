# Copyright 2019-present Julián Bermúdez Ortega.
#
# This file is part of vaneins::cpputils.
#
# vaneins::cpputils is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# vaneins::cpputils is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with vaneins::cpputils.  If not, see <https://www.gnu.org/licenses/>.

include(CTest)
include(${PROJECT_SOURCE_DIR}/thirdparty/catch2/contrib/Catch.cmake)
include(${PROJECT_SOURCE_DIR}/thirdparty/catch2/contrib/ParseAndAddCatchTests.cmake)

add_subdirectory(reference)