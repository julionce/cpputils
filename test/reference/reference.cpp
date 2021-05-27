/*
 * Copyright 2019-present Julián Bermúdez Ortega.
 *
 * This file is part of julibert::cpputils.
 *
 * julibert::cpputils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * julibert::cpputils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with julibert::cpputils.  If not, see <https://www.gnu.org/licenses/>.
 */
#define CATCH_CONFIG_MAIN

#include <julibert/cpputils/reference/reference.hpp>

#include <catch2/catch.hpp>

struct MyReferenceImpl
{
public:
  MyReferenceImpl() = default;

  MyReferenceImpl(uint8_t foo, uint8_t bar)
    : foo_{ foo }
    , bar_{ bar }
  {}

  friend bool operator==(const MyReferenceImpl& lhs, const MyReferenceImpl& rhs)
  {
    return (lhs.foo_ == rhs.foo_) && (lhs.bar_ == rhs.bar_);
  }

  void set_foo(uint8_t foo) { foo_ = foo; }
  uint8_t get_foo() const { return foo_; }
  uint8_t get_bar() const { return bar_; }

private:
  uint8_t foo_ = 0;
  uint8_t bar_ = 0;
};

using MyReference = julibert::Reference<MyReferenceImpl>;

SCENARIO("Reference")
{
  GIVEN("a Reference")
  {
    MyReference foo(11, 89);

    THEN("it can be assigned from a prvalue")
    {
      MyReference foo = MyReference();
      REQUIRE(0 == foo->get_foo());
      REQUIRE(0 == foo->get_bar());
    }

    THEN("it can be assigned from a lvalue")
    {
      MyReference bar(1, 1);
      REQUIRE(1 == bar->get_foo());
      REQUIRE(1 == bar->get_bar());

      foo = bar;
      REQUIRE(1 == foo->get_foo());
      REQUIRE(1 == foo->get_bar());
    }

    THEN("it can be assigned from an xvalue")
    {
      MyReference bar(11, 89);

      foo = std::move(bar);
    }

    THEN("it can by cloned")
    {
      MyReference bar(MyReference::clone(foo));
      REQUIRE(11 == bar->get_foo());
      REQUIRE(89 == bar->get_bar());

      foo->set_foo(0);
      REQUIRE(11 == bar->get_foo());
    }

    THEN("we can generate another from this one")
    {
      MyReference bar(foo);
      REQUIRE(11 == bar->get_foo());
      REQUIRE(89 == bar->get_bar());

      foo->set_foo(0);
      REQUIRE(0 == bar->get_foo());
    }

    THEN("we can generate a constant Rerence copying this one")
    {
      const MyReference bar(foo);
      REQUIRE(11 == bar->get_foo());
      REQUIRE(89 == bar->get_bar());

      foo->set_foo(12);
      REQUIRE(12 == bar->get_foo());
    }

    THEN("we can generate a constant Reference moving this one")
    {
      const MyReference bar(std::move(foo));
    }
  }

  GIVEN(
    "a constant Reference, its constant implementation methods are available")
  {
    const MyReference foo(11, 89);
    // foo->set_foo(11); // It would give an error.
    REQUIRE(11 == foo->get_foo());
  }

  GIVEN("a Reference, its implementation methods are available")
  {
    MyReference foo(0, 0);
    foo->set_foo(11);
    REQUIRE(11 == foo->get_foo());
  }
}

SCENARIO("Reference operator==")
{
  GIVEN("two References")
  {
    WHEN("one is a copy of the other")
    {
      MyReference foo(11, 89);
      MyReference bar(foo);

      THEN("operator== shall return true")
      {
        REQUIRE(foo == bar);
        REQUIRE_FALSE(foo != bar);
      }
    }

    WHEN("they are contructed independenly")
    {
      THEN("if they are equal the operator== shall return true")
      {
        MyReference foo(11, 89);
        MyReference bar(11, 89);
        REQUIRE(foo == bar);
        REQUIRE_FALSE(foo != bar);
      }

      THEN("if they are different the operator== shall return false")
      {
        MyReference foo(11, 89);
        MyReference bar(11, 79);
        REQUIRE_FALSE(foo == bar);
        REQUIRE(foo != bar);
      }
    }
  }
}