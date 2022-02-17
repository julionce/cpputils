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

#include <once/cpputils/type_traits/type_container.hpp>

#include <catch2/catch.hpp>

using namespace once;

SCENARIO("type_container includes")
{
  REQUIRE(type_container<>::includes<>::value);
  REQUIRE(type_container<>::includes_v<>);
  REQUIRE_FALSE(type_container<>::includes_v<int>);

  REQUIRE(type_container<int>::includes_v<>);
  REQUIRE(type_container<int>::includes_v<int>);
  REQUIRE_FALSE(type_container<int>::includes_v<float>);

  REQUIRE(type_container<int, float>::includes_v<int>);
  REQUIRE(type_container<int, float>::includes_v<int, float>);
  REQUIRE_FALSE(type_container<int, float>::includes_v<int, double>);
}

SCENARIO("type_container is_empty")
{
  REQUIRE(type_container<>::is_empty::value);
  REQUIRE(type_container<>::is_empty_v);
  REQUIRE_FALSE(type_container<int>::is_empty_v);
}

SCENARIO("type_container size")
{
  REQUIRE(type_container<>::size::value == 0);
  REQUIRE(type_container<>::size_v == 0);
  REQUIRE(type_container<int>::size_v == 1);
  REQUIRE(type_container<int, int>::size_v == 2);
}

SCENARIO("type_container insert")
{
  {
    using container_expected = type_container<int, float>;
    using container_result = type_container<int>::insert_t<float>;
    REQUIRE(std::is_same_v<container_expected, container_result>);
  }

  {
    using container_expected = type_container<int, double, float, bool>;
    using container_result = type_container<int, double>::insert_t<float, bool>;
    REQUIRE(std::is_same_v<container_expected, container_result>);
  }

  {
    using container_expected = type_container<int, double, float, bool>;
    using container_result = type_container<int, bool>::insert_t<float, double>;
    REQUIRE_FALSE(std::is_same_v<container_expected, container_result>);
  }
}

SCENARIO("is_type_container")
{
  REQUIRE(is_type_container<type_container<>>::value);
  REQUIRE(is_type_container_v<type_container<>>);
  REQUIRE(is_type_container_v<type_container<int>>);
  REQUIRE(is_type_container_v<type_container<int, float>>);
  REQUIRE_FALSE(is_type_container_v<int>);
}