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
#include <vector>

namespace julibert {
namespace cpputils {
namespace tree {

template<typename T>
class NodeImpl;

template<typename T>
using Node = Reference<NodeImpl<T>>;

template<typename T>
class NodeImpl : public std::enable_shared_from_this<NodeImpl<T>>
{
public:
  template<class... Args>
  explicit NodeImpl(Args&&... args);

  Node<T> const& get_parent() const { return parent_; }
  std::vector<Node<T>> const& get_children() const { return children_; }

  template<class... Args>
  Node<T> const& add_child(Args&&... args);

  template<class R>
  bool import_node_as_child(R&& child_node);

  T const& data() const { return data_; }
  T& data() { return data_; }

  bool operator==(const NodeImpl& other) const { return this == &other; }

private:
  T data_;
  Node<T> parent_;
  std::vector<Node<T>> children_;
};

template<typename T>
template<class... Args>
inline NodeImpl<T>::NodeImpl(Args&&... args)
  : data_{ std::forward<Args>(args)... }
  , parent_{ Node<T>::null() }
  , children_{}
{}

template<typename T>
template<class... Args>
inline Node<T> const&
NodeImpl<T>::add_child(Args&&... args)
{
  children_.emplace_back(std::forward<Args>(args)...);
  children_.back()->parent_ = Node<T>::from_shared(this->shared_from_this());
  return children_.back();
}

template<class T>
template<class R>
inline bool
NodeImpl<T>::import_node_as_child(R&& child_node)
{
  bool rv = false;
  NodeImpl* node_ptr = std::forward<Node<T>>(child_node).get();
  if (!node_ptr->parent_) {
    node_ptr->parent_ = Node<T>::from_shared(this->shared_from_this());
    children_.push_back(std::forward<Node<T>>(child_node));
    rv = true;
  }
  return rv;
}

template<typename T>
std::list<Node<T>>
preorder_list(Node<T> const& root)
{
  std::list<Node<T>> nodes;
  std::function<void(Node<T> const&)> push_node;
  push_node = [&nodes, &push_node](Node<T> const& node) {
    nodes.push_back(node);
    for (auto& n : node->get_children()) {
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
    for (auto& n : node->get_children()) {
      push_node(n);
    }
    nodes.push_back(node);
  };
  push_node(root);
  return nodes;
}

} // namespace tree

} // namespace cpputils
} // namespace julibert

#endif // JULIBERT_CPPUTILS_TREE_TREE_HPP_