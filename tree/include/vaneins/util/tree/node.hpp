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
#ifndef VANEINS_UTIL_TREE_NODE_HPP_
#define VANEINS_UTIL_TREE_NODE_HPP_

#include <vector>

namespace vaneins {
namespace util {

template<typename T>
class Node
{
    template<class... Args>
    explicit Node(Node* parent, Args&&... args);

public:
    template<class... Args>
    explicit Node(Args&&... args);

    const Node& get_parent() const;
    Node& get_parent();

    template<class... Args>
    Node& add_child(Args&&... args);

    const T& operator*() const;

private:
    const T data_;
    Node* const parent_;
    std::vector<Node> children_;
};

template<typename T>
template<class... Args>
inline Node<T>::Node(Node* parent, Args&&... args)
    : data_{std::forward<Args>(args)...}
    , parent_{parent}
    , children_{}
{}

template<typename T>
template<class... Args>
inline Node<T>::Node(Args&&... args)
    : data_{std::forward<Args>(args)...}
    , parent_{nullptr}
    , children_{}
{}

template<typename T>
inline const Node<T>& Node<T>::get_parent() const
{
    return (nullptr == parent_) ? *this : *parent_;
}

template<typename T>
inline Node<T>& Node<T>::get_parent()
{
    return (nullptr == parent_) ? *this : *parent_;
}

template<typename T>
template<class... Args>
inline Node<T>& Node<T>::add_child(Args&&... args)
{
    children_.emplace_back(Node<T>(this, std::forward<Args>(args)...));
    return children_.back();
}

template<typename T>
inline const T& Node<T>::operator*() const
{
    return data_;
}

} // namespace util
} // namespace vaneins

#endif // VANEINS_UTIL_TREE_NODE_HPP_