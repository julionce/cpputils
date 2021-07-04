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
#ifndef JULIBERT_CPPUTILS_TREE_TREE_HPP_
#define JULIBERT_CPPUTILS_TREE_TREE_HPP_

#include <julibert/cpputils/reference/reference.hpp>

#include <cstdint>
#include <functional>
#include <list>
#include <optional>
#include <vector>

namespace julibert {
namespace tree {

template<typename T>
class Node
{
  using ReferenceType = std::reference_wrapper<Node<T>>;
  using ChildrenType = std::list<Node<T>>;

public:
  template<typename... Args>
  explicit Node(Args&&... args)
    : data_{ std::forward<Args>(args)... }
  {}

  std::optional<ReferenceType> parent() const { return parent_; }
  ChildrenType const& children() const { return children_; }

  template<typename... Args>
  Node<T>& add_child(Args&&... args)
  {
    children_.emplace_back(std::forward<Args>(args)...);
    children_.back().parent_ = *this;
    return children_.back();
  }

  T const& data() const { return data_; }
  T& data() { return data_; }

  bool operator==(Node const& other) const { return &data_ == &other.data_; }

private:
  T data_;
  std::optional<ReferenceType> parent_;
  ChildrenType children_;
};

template<typename T, typename F>
void
for_each_in_preorder(Node<T>& node, F function)
{
  function(node);
  for (auto&& n : node.children()) {
    for_each_in_preorder(n, function);
  }
}

template<typename T, typename F>
void
for_each_in_preorder(Node<T> const& node, F function)
{
  function(node);
  for (auto&& n : node.children()) {
    for_each_in_preorder(n, function);
  }
}

template<typename T, typename F>
void
for_each_in_postorder(Node<T>& node, F function)
{
  for (auto&& n : node.children()) {
    for_each_in_postorder(n, function);
  }
  function(node);
}

template<typename T, typename F>
void
for_each_in_postorder(Node<T> const& node, F function)
{
  for (auto&& n : node.children()) {
    for_each_in_postorder(n, function);
  }
  function(node);
}

} // namespace tree
} // namespace julibert

#endif // JULIBERT_CPPUTILS_TREE_TREE_HPP_