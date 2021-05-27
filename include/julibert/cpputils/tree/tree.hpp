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
  struct Impl
  {
    template<typename... Args>
    explicit Impl(Args&&... args)
      : data{ std::forward<Args>(args)... }
    {}

    T data;
    std::optional<Node> parent = {};
    std::vector<Node> children = {};
  };

public:
  template<typename... Args>
  explicit Node(Args&&... args)
    : impl_{ std::forward<Args>(args)... }
  {}

  std::optional<Node> parent() const { return impl_->parent; }
  std::vector<Node> const& children() const { return impl_->children; }

  template<typename... Args>
  Node add_child(Args&&... args)
  {
    impl_->children.emplace_back(std::forward<Args>(args)...);
    impl_->children.back().impl_->parent = Node(this->impl_);
    return impl_->children.back();
  }

  T const& data() const { return impl_->data; }
  T& data() { return impl_->data; }

  bool operator==(Node const& other) const
  {
    return &impl_->data == &other.impl_->data;
  }

private:
  Node(Reference<Impl> const& impl)
    : impl_{ impl }
  {}

private:
  Reference<Impl> impl_;
};

template<typename T>
std::list<Node<T>>
preorder_list(Node<T> const& root)
{
  std::list<Node<T>> nodes;
  std::function<void(Node<T> const&)> push_node;
  push_node = [&nodes, &push_node](Node<T> const& node) {
    nodes.push_back(node);
    for (auto& n : node.children()) {
      push_node(n);
    }
  };
  push_node(root);
  return nodes;
}

template<typename T>
std::list<Node<T>>
postorder_list(Node<T> const& root)
{
  std::list<Node<T>> nodes;
  std::function<void(Node<T> const&)> push_node;
  push_node = [&nodes, &push_node](Node<T> const& node) {
    for (auto& n : node.children()) {
      push_node(n);
    }
    nodes.push_back(node);
  };
  push_node(root);
  return nodes;
}

} // namespace tree
} // namespace julibert

#endif // JULIBERT_CPPUTILS_TREE_TREE_HPP_