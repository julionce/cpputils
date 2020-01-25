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

#include <vaneins/util/tree/node.hpp>
#include <catch2/catch.hpp>

class Foo
{
public:
    Foo(int a, double b)
        : a_{a}
        , b_{b}
    {}

    int get_a() const { return a_; }
    double get_b() const { return b_; }

private:
    int a_;
    double b_;
};

using namespace vaneins::util;

SCENARIO("Node<int>")
{
    GIVEN("a root node")
    {
        Node<int> root(0);
        REQUIRE(0 == *root);

        THEN("its parent shall be itself")
        {
            REQUIRE(&root.get_parent() == &root);
        }

        WHEN("a child and a grandchild are added")
        {
            Node<int>& child = root.add_child(1);
            REQUIRE(1 == *child);
            Node<int>& grandchild = child.add_child(2);
            REQUIRE(2 == *grandchild);

            THEN("the family hierarchy shall be satisfied")
            {
                REQUIRE(&child.get_parent() == &root);
                REQUIRE(&grandchild.get_parent() == &child);
                REQUIRE(&grandchild.get_parent().get_parent() == &root);
            }
        }
    }
}

SCENARIO("Node<Foo>")
{
    GIVEN("a root node")
    {
        Node<Foo> root(0, 0.1);
        REQUIRE(0 == (*root).get_a());
        REQUIRE(0.1 == (*root).get_b());

        THEN("its parent shall be itself")
        {
            REQUIRE(&root.get_parent() == &root);
        }

        WHEN("a child and a grandchild are added")
        {
            Node<Foo>& child = root.add_child(2, 0.3);
            REQUIRE(2 == (*child).get_a());
            REQUIRE(0.3 == (*child).get_b());
            Node<Foo>& grandchild = child.add_child(4, 0.5);
            REQUIRE(4 == (*grandchild).get_a());
            REQUIRE(0.5 == (*grandchild).get_b());

            THEN("the family hierarchy shall be satisfied")
            {
                REQUIRE(&child.get_parent() == &root);
                REQUIRE(&grandchild.get_parent() == &child);
                REQUIRE(&grandchild.get_parent().get_parent() == &root);
            }
        }
    }
}