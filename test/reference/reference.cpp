/*
 * Copyright 2019-present Julián Bermúdez Ortega.
 *
 * This file is part of VaneinsCpputils.
 *
 * VaneinsCpputils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * VaneinsCpputils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with VaneinsCpputils.  If not, see <https://www.gnu.org/licenses/>.
 */
#define CATCH_CONFIG_MAIN

#include <vaneins/util/reference/reference.hpp>
#include <vaneins/util/reference/const_reference.hpp>

#include <catch2/catch.hpp>

struct MyReferenceImpl 
{
    void set_value(uint8_t value) { value_ = value; }
    uint8_t get_value() const { return value_; }

private:
    uint8_t value_ = 0;
};

using MyReferenceBase = vaneins::util::ReferenceBase<MyReferenceImpl>;
using MyReference = vaneins::util::Reference<MyReferenceImpl>;
using MyConstReference = vaneins::util::ConstReference<MyReferenceImpl>;

SCENARIO("general")
{
    GIVEN("an empty ReferenceBase")
    {
        std::shared_ptr<int> a;
        std::shared_ptr<int> b(new int());

        a = std::move(b);
        REQUIRE(a);
        REQUIRE(!b);

        MyReference my_reference;
        REQUIRE(my_reference.is_null());
        REQUIRE_FALSE(my_reference);

        WHEN("a ReferenceBase is created by a prvalue")
        {
            MyReference my_second_reference(std::shared_ptr<MyReferenceImpl>(new MyReferenceImpl()));
            REQUIRE_FALSE(my_second_reference.is_null());
            REQUIRE(my_second_reference);

            THEN("it can be copied")
            {
                my_reference = my_second_reference;
                REQUIRE_FALSE(my_reference.is_null());
                REQUIRE(my_reference);
                REQUIRE_FALSE(my_second_reference.is_null());
                REQUIRE(my_second_reference);
            }

            THEN("it can be moved")
            {
                my_reference = std::move(my_second_reference);
                REQUIRE_FALSE(my_reference.is_null());
                REQUIRE(my_reference);
                REQUIRE(my_second_reference.is_null());
                REQUIRE_FALSE(my_second_reference);
            }
        }

        WHEN("a ReferenceBase is created by a lvalue")
        {
            std::shared_ptr<MyReferenceImpl> my_reference_impl(new MyReferenceImpl());
            MyReference my_second_reference(my_reference_impl);
            REQUIRE_FALSE(my_second_reference.is_null());
            REQUIRE(my_second_reference);
        }

        WHEN("a ReferenceBase is created by a xvalue")
        {
            std::shared_ptr<MyReferenceImpl> my_reference_impl(new MyReferenceImpl());
            MyReference my_second_reference(std::move(my_reference_impl));
            REQUIRE_FALSE(my_second_reference.is_null());
            REQUIRE(my_second_reference);
        }
    }

    GIVEN("a ConstReference, its constant implementation methods are available")
    {
        MyConstReference my_reference(std::shared_ptr<MyReferenceImpl>(new MyReferenceImpl()));
        // my_reference->set_value(11); It would give an error.
        REQUIRE(uint8_t(0) == my_reference->get_value());
    }

    GIVEN("a Reference, its implementation methods are available")
    {
        MyReference my_reference(std::shared_ptr<MyReferenceImpl>(new MyReferenceImpl()));
        my_reference->set_value(11);
        REQUIRE(uint8_t(11) == my_reference->get_value());

        MyReference my_second_reference(my_reference);
        REQUIRE(uint8_t(11) == my_second_reference->get_value());

        my_reference->set_value(89);
        REQUIRE(uint8_t(89) == my_second_reference->get_value());

        THEN("we can generate a ConstReference copying this one")
        {
            MyConstReference my_const_reference(my_reference);
            REQUIRE(uint8_t(89) == my_const_reference->get_value());

            my_reference->set_value(11);
            REQUIRE(uint8_t(11) == my_const_reference->get_value());
        }

        THEN("we can generate a ConstReference moving this one")
        {
            MyConstReference my_const_reference(std::move(my_reference));
            REQUIRE(uint8_t(89) == my_const_reference->get_value());

            REQUIRE(my_reference.is_null());
        }
    }
}
