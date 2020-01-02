/*
 * Copyright 2019-present Julián Bermúdez Ortega.
 *
 * This file is part of vaneins::utils.
 *
 * vaneins::utils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * vaneins::utils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with vaneins::utils.  If not, see <https://www.gnu.org/licenses/>.
 */
#define CATCH_CONFIG_MAIN

#include <vaneins/util/reference/reference.hpp>
#include <vaneins/util/reference/const_reference.hpp>

#include <catch2/catch.hpp>

struct MyReferenceImpl 
{
public:
    MyReferenceImpl() = default;

    MyReferenceImpl(uint8_t foo, uint8_t bar)
        : foo_{foo}
        , bar_{bar}
    {}

    void set_foo(uint8_t foo) { foo_ = foo; }
    uint8_t get_foo() const { return foo_; }
    uint8_t get_bar() const { return bar_; }

private:
    uint8_t foo_ = 0;
    uint8_t bar_ = 0;
};

using MyReferenceBase = vaneins::util::ReferenceBase<MyReferenceImpl>;
using MyReference = vaneins::util::Reference<MyReferenceImpl>;
using MyConstReference = vaneins::util::ConstReference<MyReferenceImpl>;

SCENARIO("reference")
{
    GIVEN("a null Reference")
    {
        MyReference foo;
        REQUIRE(foo.is_null());
        REQUIRE_FALSE(foo);

        THEN("it can be assigned from a prvalue")
        {
            MyReference foo = MyReference(11, 89);
            REQUIRE_FALSE(foo.is_null());
            REQUIRE(11 == foo->get_foo());
            REQUIRE(89 == foo->get_bar());
        }

        THEN("it can be assigned from a lvalue")
        {
            MyReference bar(11, 89);
            REQUIRE_FALSE(bar.is_null());
            REQUIRE(11 == bar->get_foo());
            REQUIRE(89 == bar->get_bar());

            foo = bar;
            REQUIRE_FALSE(bar.is_null());
            REQUIRE_FALSE(foo.is_null());
            REQUIRE(11 == foo->get_foo());
            REQUIRE(89 == foo->get_bar());
        }

        THEN("it can be assigned from an xvalue")
        {
            MyReference bar(11, 89);

            foo = std::move(bar);
            REQUIRE(bar.is_null());
        }
    }

    GIVEN("a non-null Reference")
    {
        MyReference foo(11, 89);
        THEN("it can by copied from")
        {
            MyReference bar(foo);
            REQUIRE_FALSE(foo.is_null());
            REQUIRE(11 == bar->get_foo());
            REQUIRE(89 == bar->get_bar());
        }

        THEN("we can generate a ConstRerence copying this one")
        {
            MyConstReference bar(foo);
            REQUIRE_FALSE(bar.is_null());
            REQUIRE(11 == bar->get_foo());
            REQUIRE(89 == bar->get_bar());

            foo->set_foo(12);
            REQUIRE(12 == bar->get_foo());
        }

        THEN("we can generate a ConstReference moving this one")
        {
            MyConstReference bar(std::move(foo));
            REQUIRE(foo.is_null());
            REQUIRE_FALSE(bar.is_null());
        }
    }

    GIVEN("a ConstReference, its constant implementation methods are available")
    {
        MyConstReference foo(11, 89);
        // my_reference->set_value(11); It would give an error.
        REQUIRE(11 == foo->get_foo());
    }

    GIVEN("a Reference, its implementation methods are available")
    {
        MyReference foo(0, 0);
        foo->set_foo(11);
        REQUIRE(11 == foo->get_foo());
    }
}
