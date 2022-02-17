/*
 * Copyright 2021-present Julián Bermúdez Ortega
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <once/cpputils/type_traits/type_set.hpp>

#include <catch2/catch.hpp>

using namespace once;

SCENARIO("type_set construction")
{
  REQUIRE(std::is_base_of_v<type_container<int>, type_set<int>>);
  REQUIRE(std::is_base_of_v<type_container<int>, type_set<int, int>>);
  REQUIRE(std::is_base_of_v<type_container<int, double>,
                            type_set<int, int, double, int>>);
}

SCENARIO("type_set equal")
{
  REQUIRE(type_set<>::equal<type_set<>>::value);
  REQUIRE(type_set<>::equal_v<type_set<>>);
  REQUIRE(type_set<int>::equal_v<type_set<int>>);
  REQUIRE(type_set<int>::equal_v<type_set<int, int>>);
  REQUIRE(type_set<int, double>::equal_v<type_set<int, double>>);
  REQUIRE(type_set<int, double>::equal_v<type_set<double, int>>);
}

SCENARIO("type_set insert")
{
  REQUIRE(type_set<>::insert<int>::type::equal<type_set<int>>::value);
  REQUIRE(type_set<>::insert_t<int>::equal_v<type_set<int>>);
  REQUIRE(type_set<int>::insert_t<int>::equal_v<type_set<int, int>>);
  REQUIRE(type_set<int>::insert_t<double>::equal_v<type_set<int, double>>);
  REQUIRE_FALSE(
    type_set<float>::insert_t<double>::equal_v<type_set<int, double>>);
}

SCENARIO("underlying_type_set")
{
  REQUIRE(underlying_type_set_t<std::variant<int>>::equal_v<type_set<int>>);
  REQUIRE(underlying_type_set_t<std::variant<double, int>>::equal_v<
          type_set<int, double>>);
  REQUIRE_FALSE(
    underlying_type_set_t<std::variant<int, bool>>::equal_v<type_set<int>>);

  REQUIRE(underlying_type_set_t<std::tuple<double, int, int>>::equal_v<
          type_set<int, double>>);
  REQUIRE_FALSE(
    underlying_type_set_t<std::tuple<double>>::equal_v<type_set<bool>>);
}