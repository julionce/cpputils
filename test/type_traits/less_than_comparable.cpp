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

#include <once/cpputils/type_traits/less_than_comparable.hpp>

#include <catch2/catch.hpp>
#include <cstdint>

using namespace once;

struct Bar
{
  int data;
};

bool
operator<(Bar const& lhs, Bar const& rhs)
{
  return lhs.data < rhs.data;
}

struct NonConstLessComparableBar
{
  int data;
};

bool
operator<(NonConstLessComparableBar const& lhs, NonConstLessComparableBar& rhs)
{
  return lhs.data < rhs.data;
}

struct NonMatchedLessComparableBar
{
  int data;
};

bool
operator<(NonConstLessComparableBar const& lhs, int& rhs)
{
  return lhs.data < rhs;
}

SCENARIO("is_less_than_comparable trait")
{
  GIVEN("an arithmetic type")
  {
    THEN("the trait value shall be true")
    {
      REQUIRE(is_less_than_comparable<bool>::value);
      REQUIRE(is_less_than_comparable<char>());
      REQUIRE(is_less_than_comparable_v<int>);
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

    THEN("the trait value shall be true")
    {
      REQUIRE(is_less_than_comparable_v<MyEnum>);
    }
  }

  GIVEN("a pointer type")
  {
    THEN("the trait value shall be true")
    {
      struct Foo;
      REQUIRE(is_less_than_comparable_v<std::add_pointer_t<Foo>>);
    }
  }

  GIVEN("a type with a user-defined operator<")
  {

    WHEN("it is properly defined")
    {
      THEN("the trait value shall be true")
      {
        struct Foo
        {
          int data;
          constexpr bool operator<(Foo const& other) const
          {
            return data < other.data;
          }

          constexpr bool operator<(int const& other) const
          {
            return data < other;
          }
        };
        REQUIRE(is_less_than_comparable_v<Foo>);
        REQUIRE(is_less_than_comparable_v<Foo, int>);
        REQUIRE_FALSE(is_less_than_comparable_v<Foo, Bar>);
        REQUIRE(is_less_than_comparable_v<Bar>);
      }
    }

    WHEN("some parameters' type is not a const&")
    {
      THEN("the trait value shall be false")
      {
        struct Foo
        {
          int data;
          constexpr bool operator<(Foo& other) const
          {
            return data < other.data;
          }
        };

        struct Qux
        {
          int data;
          constexpr bool operator<(Foo const& other)
          {
            return data < other.data;
          }
        };
        REQUIRE_FALSE(is_less_than_comparable_v<Foo>);
        REQUIRE_FALSE(is_less_than_comparable_v<NonConstLessComparableBar>);
        REQUIRE_FALSE(is_less_than_comparable_v<Qux>);
      }
    }

    WHEN("the parameters do not have the same type")
    {
      THEN("the trait value shall be false")
      {
        struct Foo
        {
          int data;
          constexpr bool operator<(int const& other) const
          {
            return data < other;
          }
        };
        REQUIRE_FALSE(is_less_than_comparable_v<Foo>);
        REQUIRE_FALSE(is_less_than_comparable_v<NonMatchedLessComparableBar>);
      }
    }
  }
}