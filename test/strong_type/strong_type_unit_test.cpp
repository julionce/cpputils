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
#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <once/cpputils/strong_type/strong_type.hpp>

using namespace once;

using MyStrongTypeFoo = strong_type<int, struct MyStrongTypeFooTag>;
using MyStrongTypeBar = strong_type<int, struct MyStrongTypeBarTag>;

SCENARIO("simple strong_type")
{
  REQUIRE_FALSE(std::is_same_v<MyStrongTypeFoo, MyStrongTypeBar>);
}

struct Qux
{
  explicit Qux(bool) {}
};

using MyStrongTypeQux = strong_type<Qux, struct MyStrongTypeQuxTag>;

SCENARIO("strong_type constructor")
{
  REQUIRE(std::is_constructible_v<MyStrongTypeQux, bool>);
  REQUIRE_FALSE(std::is_constructible_v<MyStrongTypeQux>);

  REQUIRE(std::is_constructible_v<MyStrongTypeQux, Qux>);
  REQUIRE(std::is_constructible_v<MyStrongTypeQux, Qux&&>);

  REQUIRE(std::is_assignable_v<MyStrongTypeQux, Qux>);
  REQUIRE(std::is_assignable_v<MyStrongTypeQux, Qux&&>);
  REQUIRE(std::is_assignable_v<MyStrongTypeQux, MyStrongTypeQux>);
}

using MyEquallyComparableStrongType =
  strong_type<int,
              struct MyEquallyComparableStrongTypeTag,
              st::equality_comparable>;

SCENARIO("equally comparable strong_type")
{
  REQUIRE(is_equality_comparable_v<MyEquallyComparableStrongType>);
  REQUIRE_FALSE(is_subtractable_v<MyEquallyComparableStrongType>);

  MyEquallyComparableStrongType foo{ 11 };
  MyEquallyComparableStrongType bar{ 11 };
  REQUIRE(foo == bar);
  REQUIRE_FALSE(foo != bar);

  foo = 89;
  REQUIRE_FALSE(foo == bar);
  REQUIRE(foo != bar);
}

using MyLessThanComparableStrongType =
  strong_type<int,
              struct MyLessThanComparableStrongTypeTag,
              st::less_than_comparable>;

SCENARIO("less than comparable strong_type")
{
  REQUIRE(is_less_than_comparable_v<MyLessThanComparableStrongType>);
  REQUIRE_FALSE(is_subtractable_v<MyLessThanComparableStrongType>);

  MyLessThanComparableStrongType::value_type raw_value = 11;
  MyLessThanComparableStrongType foo{ raw_value };
  MyLessThanComparableStrongType bar{ raw_value };

  REQUIRE_FALSE(foo < bar);
  REQUIRE_FALSE(foo < raw_value);
  REQUIRE_FALSE(foo > bar);
  REQUIRE_FALSE(foo > raw_value);
  REQUIRE(foo <= bar);
  REQUIRE(foo <= raw_value);
  REQUIRE(foo >= bar);
  REQUIRE(foo >= raw_value);

  foo = 12;
  REQUIRE(foo > bar);
  REQUIRE(foo > raw_value);
  REQUIRE_FALSE(foo < bar);
  REQUIRE_FALSE(foo < raw_value);
  REQUIRE(foo >= bar);
  REQUIRE(foo >= raw_value);
  REQUIRE_FALSE(foo <= bar);
  REQUIRE_FALSE(foo <= raw_value);
}

using MyAddableStrongType =
  strong_type<int, struct MyAddableStrongTypeTag, st::addable>;

SCENARIO("addable strong_type")
{
  REQUIRE(is_addable_v<MyAddableStrongType>);
  REQUIRE_FALSE(is_subtractable_v<MyAddableStrongType>);

  MyAddableStrongType foo{ 11 };
  MyAddableStrongType bar{ 11 };
  decltype(foo + bar) addition;

  REQUIRE(std::is_same_v<decltype(addition), MyAddableStrongType>);

  addition = foo + bar;
  REQUIRE(*addition == *foo + *bar);

  addition = foo + 89;
  REQUIRE(*addition == *foo + 89);

  addition = 11 + foo;
  REQUIRE(*addition == 11 + *foo);
}

using MySubtractableStrongType =
  strong_type<int, struct MySubtractableStrongTypeTag, st::subtractable>;

SCENARIO("subtractable strong_type")
{
  REQUIRE(is_subtractable_v<MySubtractableStrongType>);
  REQUIRE_FALSE(is_addable_v<MySubtractableStrongType>);

  MySubtractableStrongType foo{ 11 };
  MySubtractableStrongType bar{ 11 };
  decltype(foo - bar) subtraction;

  REQUIRE(std::is_same_v<decltype(subtraction), MySubtractableStrongType>);

  subtraction = foo - bar;
  REQUIRE(*subtraction == *foo - *bar);

  subtraction = foo - 89;
  REQUIRE(*subtraction == *foo - 89);

  subtraction = 11 - foo;
  REQUIRE(*subtraction == 11 - *foo);
}

using MyMultiplicableStrongType =
  strong_type<int, struct MyMultiplicableStrongTypeTag, st::multiplicable>;

SCENARIO("multiplicable strong_type")
{
  REQUIRE(is_multiplicable_v<MyMultiplicableStrongType>);
  REQUIRE_FALSE(is_subtractable_v<MyMultiplicableStrongType>);

  MyMultiplicableStrongType foo{ 11 };
  MyMultiplicableStrongType bar{ 11 };
  decltype(foo * bar) multiplication;

  REQUIRE(std::is_same_v<decltype(multiplication), MyMultiplicableStrongType>);

  multiplication = foo * bar;
  REQUIRE(*multiplication == *foo * *bar);

  multiplication = foo * 89;
  REQUIRE(*multiplication == *foo * 89);

  multiplication = 11 * foo;
  REQUIRE(*multiplication == 11 * *foo);
}

using MyDivisibleStrongType =
  strong_type<int, struct MyDivisibleStrongTypeTag, st::divisible>;

SCENARIO("divisible strong_type")
{
  REQUIRE(is_divisible_v<MyDivisibleStrongType>);
  REQUIRE_FALSE(is_subtractable_v<MyDivisibleStrongType>);

  MyDivisibleStrongType foo{ 11 };
  MyDivisibleStrongType bar{ 11 };
  decltype(foo / bar) division;

  REQUIRE(std::is_same_v<decltype(division), MyDivisibleStrongType>);

  division = foo / bar;
  REQUIRE(*division == *foo / *bar);

  division = foo / 89;
  REQUIRE(*division == *foo / 89);

  division = 11 / foo;
  REQUIRE(*division == 11 / *foo);
}

using MyModulableStrongType =
  strong_type<int, struct MyModulableStrongTypeTag, st::modulable>;

SCENARIO("modulable strong_type")
{
  REQUIRE(is_modulable_v<MyModulableStrongType>);
  REQUIRE_FALSE(is_subtractable_v<MyModulableStrongType>);

  MyModulableStrongType foo{ 11 };
  MyModulableStrongType bar{ 11 };
  decltype(foo % bar) modulo;

  REQUIRE(std::is_same_v<decltype(modulo), MyModulableStrongType>);

  modulo = foo % bar;
  REQUIRE(*modulo == *foo % *bar);

  modulo = foo % 89;
  REQUIRE(*modulo == *foo % 89);

  modulo = 11 % foo;
  REQUIRE(*modulo == 11 % *foo);
}

using MyLeftShiftableStrongType =
  strong_type<int, struct MyLeftShiftableStrongTypeTag, st::left_shiftable>;

SCENARIO("left_shiftable strong_type")
{
  REQUIRE(is_left_shiftable_v<MyLeftShiftableStrongType>);
  REQUIRE_FALSE(is_subtractable_v<MyLeftShiftableStrongType>);

  MyLeftShiftableStrongType foo{ 1 };
  MyLeftShiftableStrongType bar{ 2 };
  decltype(foo << bar) result;

  REQUIRE(std::is_same_v<decltype(result), MyLeftShiftableStrongType>);

  result = foo << bar;
  REQUIRE(*result == *foo << *bar);

  result = foo << 3;
  REQUIRE(*result == *foo << 3);

  result = 4 << foo;
  REQUIRE(*result == 4 << *foo);
}

using MyRightShiftableStrongType =
  strong_type<int, struct MyRightShiftableStrongTypeTag, st::right_shiftable>;

SCENARIO("right_shiftable strong_type")
{
  REQUIRE(is_right_shiftable_v<MyRightShiftableStrongType>);
  REQUIRE_FALSE(is_subtractable_v<MyRightShiftableStrongType>);

  MyRightShiftableStrongType foo{ 1 };
  MyRightShiftableStrongType bar{ 2 };
  decltype(foo >> bar) result;

  REQUIRE(std::is_same_v<decltype(result), MyRightShiftableStrongType>);

  result = foo >> bar;
  REQUIRE(*result == *foo >> *bar);

  result = foo >> 3;
  REQUIRE(*result == *foo >> 3);

  result = 4 >> foo;
  REQUIRE(*result == 4 >> *foo);
}