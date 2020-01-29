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

#include <cstdint>
#include <list>

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
        explicit Node(
                Node* parent,
                Node* preorder_prev,
                Node* preorder_next,
                Args&&... args)
            : data_{std::forward<Args>(args)...}
            , parent_{parent}
            , youngest_child_{nullptr}
            , preorder_prev_{preorder_prev}
            , preorder_next_{preorder_next}
        {}

    public:
        const T& operator*() const { return data_; }
        const T* operator->() const { return &data_; }

        Node(const Node&) = delete;
        Node& operator==(const Node&) = delete;

    private:
        const T data_;
        Node* const parent_;
        Node* youngest_child_;
        Node* preorder_prev_;
        Node* preorder_next_;
    };

    class PreOrderIterator
    {
        friend Tree;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = int64_t;
        using pointer = T*;
        using reference = T&;

        explicit PreOrderIterator(Node* node = nullptr)
            : node_{node}
        {}

        PreOrderIterator& operator++()
        {
            if (nullptr != node_->preorder_next_)
            {
                node_ = node_->preorder_next_;
            }
            return *this;
        }

        PreOrderIterator& operator--()
        {
            if (nullptr != node_->preorder_prev_)
            {
                node_ = node_->preorder_prev_;
            }
            return *this;
        }

        bool operator==(const PreOrderIterator& other) const { return (node_ == other.node_); }
        bool operator!=(const PreOrderIterator& other) const { return !(*this == other); }
        const T& operator*() const { return node_->data_; }

    private:
        Node* node_;
    };

    template<class... Args>
    explicit Tree(Args&&... args)
        : root_{nullptr, nullptr, &last_, std::forward<Args>(args)...}
        , last_{nullptr, &root_, nullptr, std::forward<Args>(args)...}
    {}

    ~Tree()
    {
        // TODO
    }

    Node& get_root()
    {
        return root_;
    }

    Node& get_parent(Node& node)
    {
        return (nullptr != node.parent_) ? *node.parent_ : node;
    }

    template<class... Args>
    Node& add_child(Node& node, Args&&... args)
    {
        Node* prev_node = &node;
        while (nullptr != prev_node->youngest_child_)
        {
            prev_node = prev_node->youngest_child_;
        }
        Node* next_node = prev_node->preorder_next_;
        node.youngest_child_ = new Node(&node, prev_node, next_node, std::forward<Args>(args)...);
        prev_node->preorder_next_ = node.youngest_child_;
        next_node->preorder_prev_ = node.youngest_child_;
        return *node.youngest_child_;
    }

    PreOrderIterator begin_preorder() { return PreOrderIterator(&root_); }
    PreOrderIterator end_preorder() { return PreOrderIterator(&last_); }

private:
    Node root_;
    Node last_;
};

} // namespace util
} // namespace vaneins

#endif // VANEINS_UTIL_TREE_TREE_HPP_