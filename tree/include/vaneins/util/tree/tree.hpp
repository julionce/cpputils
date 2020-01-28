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
        explicit Node(Node* parent, Args&&... args)
            : data_{std::forward<Args>(args)...}
            , parent_{parent}
            , younger_sibling_{nullptr}
            , children_{}
        {}

    public:
        const Node& get_parent() const { return (nullptr == parent_) ? *this : *parent_; }
        Node& get_parent() { return (nullptr == parent_) ? *this : *parent_; }

        template<class... Args>
        Node& add_child(Args&&... args)
        {
            Node* older_sibling = children_.empty() ? nullptr : &children_.back();
            children_.emplace_back(Node(this, std::forward<Args>(args)...));
            if (nullptr != older_sibling)
            {
                older_sibling->younger_sibling_ = &children_.back();
            }
            return children_.back();
        }

        const T& operator*() const { return data_; }
        const T* operator->() const { return &data_; }

    private:
        const T data_;
        Node* const parent_;
        Node* younger_sibling_;
        std::list<Node> children_;
    };

    class PreOrderIterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = int64_t;
        using pointer = T*;
        using reference = T&;

        explicit PreOrderIterator(const Node* node = nullptr)
            : node_{node}
        {}

        PreOrderIterator& operator++()
        {
            if (nullptr != node_)
            {
                node_ = (node_->children_.empty())
                    ? get_next_preorder(*node_)
                    : &node_->children_.front();
            }
            return *this;
        }

        PreOrderIterator& operator--()
        {
            PreOrderIterator rv;
            return rv;
        }

        bool operator==(const PreOrderIterator& other) const { return (node_ == other.node_); }
        bool operator!=(const PreOrderIterator& other) const { return !(*this == other); }
        const T& operator*() const { return node_->data_; }

    private:
        static Node* get_next_preorder(const Node& node)
        {
            return (nullptr != node.younger_sibling_)
                ? node.younger_sibling_
                : (nullptr != node.parent_) ? get_next_preorder(*node.parent_) : nullptr;
        }

    private:
        const Node* node_;
    };

    template<class... Args>
    explicit Tree(Args&&... args)
        : root_{nullptr, std::forward<Args>(args)...}
        , preorder_it_begin_{&root_}
        , preorder_it_end_{}
    {}

    Node& get_root()
    {
        return root_;
    }

    const PreOrderIterator& preorder_begin() { return preorder_it_begin_; }
    const PreOrderIterator& preorder_end() { return preorder_it_end_; }

private:
    Node root_;
    PreOrderIterator preorder_it_begin_;
    PreOrderIterator preorder_it_end_;
};

}
}

#endif // VANEINS_UTIL_TREE_TREE_HPP_