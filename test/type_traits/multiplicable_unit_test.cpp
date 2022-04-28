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

#include <once/cpputils/type_traits/multiplicable.hpp>

#include <catch2/catch.hpp>

using namespace once;

struct Bar
{
  int data;
};

Bar
operator*(Bar const& lhs, Bar const& rhs)
{
  return Bar{ lhs.data * rhs.data };
}

SCENARIO("is_multiplicable trait")
{
  GIVEN("an arithmetic type")
  {
    THEN("the trait value shall be true")
    {
      REQUIRE(is_multiplicable<bool>::value);
      REQUIRE(is_multiplicable<char>());
      REQUIRE(is_multiplicable_v<int>);
    }
  }

  GIVEN("an enum type")
  {
    enum class MyEnum
    {
      red,
      green,
      blue
    };

    THEN("the trait value shall be false")
    {
      REQUIRE_FALSE(is_multiplicable_v<MyEnum>);
    }
  }

  GIVEN("a pointer type")
  {
    THEN("the trait value shall be false")
    {
      struct Foo
      {};
      REQUIRE_FALSE(is_multiplicable_v<std::add_pointer_t<Foo>>);
    }
  }

  GIVEN("a type with a user-defined operator+")
  {
    THEN("the trait value shall be true")
    {
      struct Foo
      {
        int data;

        Foo operator*(Foo other) const { return Foo{ data * other.data }; }
      };
      REQUIRE(is_multiplicable_v<Foo>);
      REQUIRE(is_multiplicable_v<Bar>);
    }
  }
}