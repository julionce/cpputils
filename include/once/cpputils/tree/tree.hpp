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
#ifndef ONCE__CPPUTILS__TREE__TREE_HPP_
#define ONCE__CPPUTILS__TREE__TREE_HPP_

#include <once/cpputils/reference/reference.hpp>

#include <cstdint>
#include <functional>
#include <list>
#include <optional>
#include <vector>

namespace once {
namespace tree {

template<typename T>
class node
{
  using const_parent_type =
    std::optional<std::reference_wrapper<const node<T>>>;
  using parent_type = std::optional<std::reference_wrapper<node<T>>>;
  using children_type = std::list<node<T>>;
  using reference = T&;
  using const_reference = const T&;

public:
  template<typename... Args,
           std::enable_if_t<std::is_constructible_v<T, Args...>, bool> = true>
  explicit node(Args&&... args)
    : data_{ std::forward<Args>(args)... }
  {}

  parent_type parent() { return parent_; }
  const_parent_type parent() const { return parent_; }
  children_type const& children() const { return children_; }

  template<typename... Args>
  node<T>& add_child(Args&&... args)
  {
    children_.emplace_back(std::forward<Args>(args)...);
    children_.back().parent_ = *this;
    return children_.back();
  }

  const_reference data() const { return data_; }
  reference data() { return data_; }

  template<typename F>
  void walk_in_preorder(F function) const
  {
    walk_in_preorder(*this, function);
  }

  template<typename F>
  void walk_in_preorder(F function)
  {
    walk_in_preorder(*this, function);
  }

  template<typename F>
  void walk_in_postorder(F function) const
  {
    walk_in_postorder(*this, function);
  }

  template<typename F>
  void walk_in_postorder(F function)
  {
    walk_in_postorder(*this, function);
  }

  bool operator==(node const& other) const { return &data_ == &other.data_; }
  bool operator!=(node const& other) const { return !(*this == other); }

private:
  template<typename N, typename F>
  static void walk_in_preorder(N&& node, F function)
  {
    function(node.data_);
    for (auto&& child : node.children_) {
      walk_in_preorder(child, function);
    }
  }

  template<typename N, typename F>
  static void walk_in_postorder(N&& node, F function)
  {
    for (auto&& child : node.children_) {
      walk_in_postorder(child, function);
    }
    function(node.data_);
  }

private:
  T data_;
  parent_type parent_;
  children_type children_;
};

} // namespace tree
} // namespace once

#endif // ONCE__CPPUTILS__TREE__TREE_HPP_