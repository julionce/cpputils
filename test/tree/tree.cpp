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

using namespace julibert::cpputils;

SCENARIO("tree::Node")
{
  using Node = tree::Node<char>;
  GIVEN("a Node")
  {
    Node root{ 'F' };

    WHEN("children are added")
    {
      Node b_node = root->add_child('B');
      Node a_node = b_node->add_child('A');
      Node d_node = b_node->add_child('D');
      Node c_node = d_node->add_child('C');
      REQUIRE(d_node->import_node_as_child(Node{ 'E' }));
      Node g_node = root->add_child('G');
      Node i_node = g_node->add_child('I');
      Node h_node{ 'H' };
      REQUIRE(i_node->import_node_as_child(h_node));

      THEN("the parent of its children shall be itseft")
      {
        REQUIRE(b_node->get_parent() == root);
        REQUIRE(a_node->get_parent() == b_node);
      }

      THEN("we shall be able to visit in pre-order and post-order")
      {
        std::vector<char> result;
        std::vector<char> expected_preorder{ 'F', 'B', 'A', 'D', 'C',
                                             'E', 'G', 'I', 'H' };
        std::vector<char> expected_postorder{ 'A', 'C', 'E', 'D', 'B',
                                              'H', 'I', 'G', 'F' };

        auto push_result = [&result](tree::Node<char> const& node) {
          result.push_back(node->data());
        };

        std::list<Node> preorder_list = tree::preorder_list(root);
        std::list<Node> postorder_list = tree::postorder_list(root);

        std::for_each(preorder_list.begin(), preorder_list.end(), push_result);
        REQUIRE(result == expected_preorder);

        result.clear();
        std::for_each(
          postorder_list.begin(), postorder_list.end(), push_result);
        REQUIRE(result == expected_postorder);
      }
    }
  }
}