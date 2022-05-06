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

#include <once/cpputils/type_traits/hashable.hpp>

#include <catch2/catch.hpp>

using namespace once;

struct Foo
{};

template<>
struct std::hash<Foo>
{
  std::size_t operator()(Foo const&) { return 0; }
};

struct Bar
{};

template<>
struct std::hash<Bar>
{
  void operator()(Bar const&) {}
};

SCENARIO("is_hashable trait")
{
  GIVEN("standard hashable type")
  {
    THEN("the trait value shall be true")
    {
      REQUIRE(is_hashable<int>::value);
      REQUIRE(is_hashable<float>());
      REQUIRE(is_hashable_v<std::string>);
    }
  }

  GIVEN("a type for which there is a hash specialization")
  {
    WHEN("the specializatio is properly defined")
    {
      THEN("the trait value shall be true")
      {
        REQUIRE(is_hashable_v<Foo>);
      }
    }

    WHEN("the specialization is not properly defined")
    {
      THEN("the trait value shall be false")
      {
        REQUIRE_FALSE(is_hashable_v<Bar>);
      }
    }
  }
}