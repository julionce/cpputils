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

SCENARIO("Tree<int>::Node")
{
  GIVEN("a tree")
  {
    Tree<int> tree(11);
    Tree<int>::Node root = tree.get_root();
    REQUIRE(11 == root->data());

    THEN("its parent shall be null") { REQUIRE_FALSE(root->get_parent()); }

    WHEN("a child and a grandchild are added")
    {
      Tree<int>::Node child = root->add_child(1);
      Tree<int>::Node grandchild = child->add_child(2);
      Tree<int>::Node grandchild_parent = grandchild->get_parent();
      REQUIRE(1 == child->data());
      REQUIRE(2 == grandchild->data());

      THEN("the family hierarchy shall be satisfied")
      {
        REQUIRE(child->get_parent() == tree.get_root());
        REQUIRE(grandchild->get_parent() == child);
        REQUIRE(grandchild_parent == child);
        REQUIRE(grandchild->get_parent()->get_parent() == root);
      }
    }
  }

  GIVEN("a node")
  {
    Tree<int> tree(11);
    Tree<int>::Node node = tree.get_root();

    THEN("it can be reasigned") { node = node->add_child(89); }
  }
}

SCENARIO("Tree<Foo>::Node")
{
  GIVEN("a tree")
  {
    Tree<Foo> tree(11, 11.89);
    REQUIRE(11 == tree.get_root()->data().get_a());
    REQUIRE(11.89 == tree.get_root()->data().get_b());
  }
}

SCENARIO("Pre-order iteration")
{
  GIVEN("a tree")
  {
    Tree<char> tree('A');
    Tree<char>::Node root = tree.get_root();
    Tree<char>::Node b_node = root->add_child('B');
    Tree<char>::Node c_node = root->add_child('C');
    Tree<char>::Node d_node = root->add_child('D');
    Tree<char>::Node e_node = b_node->add_child('E');
    Tree<char>::Node f_node = d_node->add_child('F');
    Tree<char>::Node g_node = d_node->add_child('G');
    Tree<char>::Node h_node = d_node->add_child('H');
    Tree<char>::Node j_node = d_node->add_child('J');

    std::vector<char> expected{ 'A', 'B', 'E', 'C', 'D', 'F', 'G', 'H', 'J' };
    std::vector<char> result{};
    auto append = [&result](const char& n) { result.push_back(n); };

    for (Tree<char>::PreOrderIterator it = tree.begin_preorder();
         it != tree.end_preorder();
         ++it) {
      result.push_back(*it);
    }
    REQUIRE(expected == result);

    result.clear();
    std::for_each(tree.begin_preorder(), tree.end_preorder(), append);
    REQUIRE(expected == result);

    std::reverse(expected.begin(), expected.end());
    result.clear();

    for (Tree<char>::RPreOrderIterator it = tree.rbegin_preorder();
         it != tree.rend_preorder();
         ++it) {
      result.push_back(*it);
    }
    REQUIRE(expected == result);

    result.clear();
    std::for_each(tree.rbegin_preorder(), tree.rend_preorder(), append);
    REQUIRE(expected == result);
  }
}

SCENARIO("Tree splicing")
{
  GIVEN("two trees")
  {
    Tree<char> tree_foo('A');
    Tree<char>::Node root_foo = tree_foo.get_root();
    Tree<char>::Node b_node = root_foo->add_child('B');
    Tree<char>::Node c_node = root_foo->add_child('C');
    Tree<char>::Node e_node = b_node->add_child('E');

    Tree<char> tree_bar('D');
    Tree<char>::Node root_bar = tree_bar.get_root();
    Tree<char>::Node f_node = root_bar->add_child('F');
    Tree<char>::Node g_node = root_bar->add_child('G');
    Tree<char>::Node h_node = root_bar->add_child('H');
    Tree<char>::Node j_node = root_bar->add_child('J');

    WHEN("one is spliced into the other")
    {
      tree_foo.splice(c_node, tree_bar);

      std::vector<char> expected{ 'A', 'B', 'E', 'C', 'D', 'F', 'G', 'H', 'J' };

      std::vector<char> result{};
      auto append = [&result](const char& n) { result.push_back(n); };

      for (Tree<char>::PreOrderIterator it = tree_foo.begin_preorder();
           it != tree_foo.end_preorder();
           ++it) {
        result.push_back(*it);
      }
      REQUIRE(expected == result);

      THEN("the spliced one shall have a clean state")
      {
        REQUIRE_FALSE(tree_bar.get_root());
      }
    }
  }
}

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