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

#include <vaneins/util/tree/tree.hpp>
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

SCENARIO("Tree<int>::Node")
{
    GIVEN("a tree")
    {
        Tree<int> tree(11);
        REQUIRE(11 == *tree.get_root());

        THEN("its parent shall be itself")
        {
            REQUIRE(&tree.get_root() == &tree.get_root().get_parent());
        }

        WHEN("a child and a grandchild are added")
        {
            Tree<int>::Node& child = tree.get_root().add_child(1);
            Tree<int>::Node& grandchild = child.add_child(2);
            REQUIRE(1 == *child);
            REQUIRE(2 == *grandchild);

            THEN("the family hierarchy shall be satisfied")
            {
                REQUIRE(&child.get_parent() == &tree.get_root());
                REQUIRE(&grandchild.get_parent() == &child);
                REQUIRE(&grandchild.get_parent().get_parent() == &tree.get_root());
            }
        }
    }
}

SCENARIO("Tree<Foo>::Node")
{
    GIVEN("a tree")
    {
        Tree<Foo> tree(11, 11.89);
        REQUIRE(11 == tree.get_root()->get_a());
        REQUIRE(11.89 == tree.get_root()->get_b());
    }
}