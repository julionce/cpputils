/*
 * Copyright 2021-present Julián Bermúdez Ortega
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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