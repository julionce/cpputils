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

#include <algorithm>
#include <catch2/catch.hpp>
#include <julibert/cpputils/tree/tree.hpp>

class Foo
{
public:
  Foo(int a, double b)
    : a_{ a }
    , b_{ b }
  {}

  int get_a() const { return a_; }
  double get_b() const { return b_; }

private:
  int a_;
  double b_;
};

using namespace julibert;

SCENARIO("tree::Node")
{
  using Node = tree::Node<char>;
  GIVEN("a Node")
  {
    Node root{ 'F' };

    THEN("it shall not have parent") { REQUIRE_FALSE(root.parent()); }

    WHEN("children are added")
    {
      //      auto b_node_ref = root.add_child('B');
      //      auto a_node_ref = b_node_ref.get().add_child('A');
      //      auto d_node_ref = b_node_ref.get().add_child('D');
      //      auto c_node_ref = d_node_ref.get().add_child('C');
      //      auto e_node_ref = d_node_ref.get().add_child('E');
      //      auto g_node_ref = root.add_child('G');
      //      auto i_node_ref = g_node_ref.get().add_child('I');
      //      auto h_node_ref = i_node_ref.get().add_child('H');
      //
      //      THEN("the parent of its children shall be itseft")
      //      {
      //        REQUIRE(b_node_ref.get().parent().value().get() == root);
      //        REQUIRE(a_node_ref.get().parent().value().get() ==
      //        b_node_ref.get());
      //      }

      auto&& b_node = root.add_child('B');
      auto&& a_node = b_node.add_child('A');
      auto&& d_node = b_node.add_child('D');
      auto&& c_node = d_node.add_child('C');
      auto&& e_node = d_node.add_child('E');
      auto&& g_node = root.add_child('G');
      auto&& i_node = g_node.add_child('I');
      auto&& h_node = i_node.add_child('H');

      THEN("the parent of its children shall be itseft")
      {
        REQUIRE(b_node.parent().value().get() == root);
        REQUIRE(a_node.parent().value().get() == b_node);
      }

      THEN("we shall be able to visit in pre-order and post-order")
      {
        std::vector<char> result;
        std::vector<char> expected_preorder{ 'F', 'B', 'A', 'D', 'C',
                                             'E', 'G', 'I', 'H' };
        std::vector<char> expected_postorder{ 'A', 'C', 'E', 'D', 'B',
                                              'H', 'I', 'G', 'F' };

        tree::for_each_in_preorder(
          root, [&result](auto&& item) { result.push_back(item.data()); });
        REQUIRE(result == expected_preorder);

        result.clear();
        tree::for_each_in_postorder(
          root, [&result](auto&& item) { result.push_back(item.data()); });
        REQUIRE(result == expected_postorder);
      }
    }
  }
}