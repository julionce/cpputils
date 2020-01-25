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
#ifndef VANEINS_UTIL_TREE_TREE_HPP_
#define VANEINS_UTIL_TREE_TREE_HPP_

#include <vector>

namespace vaneins {
namespace util {

template<typename T>
class Node
{
    template<class... Args>
    Node(Args&&... args);

public:
    bool is_root() const;

    const Node& get_parent() const;
    Node& get_parent();

    template<class... Args>
    Node& add_child(Args&&... args);

    const T& operator->() const

private:
    const T data_;
    const Node* parent_;
    std::vector<Node> children_;
};

template<typename T>
template<class... Args>
Node<T>::Node(Args&&... args)
    : T{std::forward<Args>(args)...}
    , parent_{nullptr}
    , children_
{}

template<typename T>
bool Node<T>::is_root() const
{
    return (nullptr == parent_);
}

template<typename T>
const Node<T>& Node<T>::get_parent() const
{
    return is_root() ? *this : *parent_;
}

template<typename T>
Node<T>& Node<T>::get_parent()
{
    return is_root() ? *this : *parent_;
}

template<typename T>
class Tree 
{
public:
    template<typename R>
    Tree(R&& arg);

    const Node<T>& get_root() const { return root_; }
    Node<T>& get_root() { return root_; }

private:
    Node<T> root_;
};

template<typename T>
template<typename R>
inline Tree<T>::Tree(R&& arg)
    : root_{}
{}

} // namespace util
} // namespace vaneins

#endif // VANEINS_UTIL_TREE_TREE_HPP_