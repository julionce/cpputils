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
class Tree
{
public:
    class Node
    {
        friend Tree;

        template<class... Args>
        explicit Node(Node* parent, Args&&... args)
            : data_{std::forward<Args>(args)...}
            , parent_{parent}
            , children_{}
        {}

    public:
        const Node& get_parent() const { return (nullptr == parent_) ? *this : *parent_; }
        Node& get_parent() { return (nullptr == parent_) ? *this : *parent_; }

        template<class... Args>
        Node& add_child(Args&&... args)
        {
            children_.emplace_back(Node(this, std::forward<Args>(args)...));
            return children_.back();
        }

        const T& operator*() const { return data_; }
        const T* operator->() const { return &data_; }

    private:
        const T data_;
        Node* const parent_;
        std::vector<Node> children_;
    };

    template<class... Args>
    explicit Tree(Args&&... args)
        : root_{nullptr, std::forward<Args>(args)...}
    {}

    Node& get_root()
    {
        return root_;
    }

//    static const Node& get_node_parent(const Node& node) { return (nullptr == node.parent_) ? node : *node.parent_; }
//    Node& get_parent(const Node& node) { return (nullptr == node.parent_) ? node : *node.parent_; }
//
//    template<class... Args>
//    static Node& add_node_child(Node& parent, Args&&... args)
//    {
//        parent.children_.emplace_back(Node(&parent, std::forward<Args>(args)...));
//    }

private:
    Node root_;
};

}
}

#endif // VANEINS_UTIL_TREE_TREE_HPP_