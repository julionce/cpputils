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

#include <once/cpputils/type_traits/aggregate_initializable.hpp>

#include <catch2/catch.hpp>

using namespace once;

struct Bar
{
  int foo;
};

struct Qux
{
  int foo;
  int bar;
};

SCENARIO("is_aggregate_initializable trait")
{
  GIVEN("the `T` and `Args...` template parameters")
  {
    WHEN("T is an aggregate type")
    {
      struct Foo
      {
        int foo;
        int bar;
      };

      REQUIRE(std::is_aggregate_v<Foo>);

      WHEN("and the variable definition `T obj{std::declval<Args>()...)};` is "
           "well-formed")
      {
        THEN("the trait provided the member constant value equal to `true`")
        {
          REQUIRE(is_aggregate_initializable<Foo>::value);
          REQUIRE(is_aggregate_initializable_v<Foo>);
          REQUIRE(is_aggregate_initializable_v<Foo, Foo&>);
          REQUIRE(is_aggregate_initializable_v<Foo, Foo const&>);
          REQUIRE(is_aggregate_initializable_v<Foo, Foo&&>);
          REQUIRE(is_aggregate_initializable_v<Foo, Foo const&&>);
          REQUIRE(is_aggregate_initializable_v<Foo, int>);
          REQUIRE(is_aggregate_initializable_v<Foo, int, int>);
        }
      }

      AND_WHEN(
        "and the variable definition `T obj{std::declval<Args>()...)};` is "
        "not well-formed")
      {
        THEN("the trait provided the member constant value equal to `false`")
        {
          REQUIRE_FALSE(is_aggregate_initializable<Foo, void>::value);
          REQUIRE_FALSE(is_aggregate_initializable_v<Foo, void>);
          REQUIRE_FALSE(is_aggregate_initializable_v<Foo, int, int, int>);
        }
      }
    }

    WHEN("T is a non-aggregate type")
    {
      struct Foo
      {
        Foo() {}
        Foo(int) {}

        int foo;
        int bar;
      };

      REQUIRE_FALSE(std::is_aggregate_v<Foo>);

      WHEN("and the variable definition `T obj{std::declval<Args>()...)};` is "
           "well-formed")
      {
        THEN("the trait provided the member constant value equal to `false`")
        {
          REQUIRE_FALSE(is_aggregate_initializable<Foo>::value);
          REQUIRE_FALSE(is_aggregate_initializable_v<Foo>);
          REQUIRE_FALSE(is_aggregate_initializable_v<Foo, int>);
        }
      }

      AND_WHEN(
        "and the variable definition `T obj{std::declval<Args>()...)};` is not"
        "well-formed")
      {
        THEN("the trait provided the member constant value equal to `false`")
        {
          REQUIRE_FALSE(is_aggregate_initializable<Foo, int, int, int>::value);
          REQUIRE_FALSE(is_aggregate_initializable_v<Foo, int, int, int>);
        }
      }
    }
  }
}