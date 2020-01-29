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
private:
    template<typename R>
    class ReverseIterator
    {
    public:
        using iterator_category = typename R::iterator_category;
        using value_type = typename R::value_type;
        using difference_type = typename R::difference_type;
        using pointer = typename R::pointer;
        using reference = typename R::reference;

        explicit ReverseIterator(R normal_iterator)
            : normal_iterator_{normal_iterator}
        {}

        ReverseIterator& operator++() { --normal_iterator_; return *this; }
        ReverseIterator& operator--() { ++normal_iterator_; return *this; }
        bool operator==(const ReverseIterator& other) const { return (normal_iterator_ == other.normal_iterator_); }
        bool operator!=(const ReverseIterator& other) const { return (normal_iterator_ != other.normal_iterator_); }
        const value_type& operator*() const { return *normal_iterator_; }

    private:
        R normal_iterator_;
    };

    class NodeImpl
    {
        friend Tree;

        template<class... Args>
        explicit NodeImpl(
                NodeImpl* parent,
                NodeImpl* preorder_prev,
                NodeImpl* preorder_next,
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

        NodeImpl(const NodeImpl&) = delete;
        NodeImpl& operator==(const NodeImpl&) = delete;

    private:
        const T data_;
        NodeImpl* const parent_;
        NodeImpl* youngest_child_;
        NodeImpl* preorder_prev_;
        NodeImpl* preorder_next_;
    };

public:
    using Node = NodeImpl&;

    class PreOrderIterator
    {
        friend Tree;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = int64_t;
        using pointer = T*;
        using reference = T&;

        explicit PreOrderIterator(NodeImpl* node = nullptr)
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
        NodeImpl* node_;
    };

    using RevPreOrderIterator = ReverseIterator<PreOrderIterator>;

    template<class... Args>
    explicit Tree(Args&&... args)
        : root_{nullptr, &first_, &last_, std::forward<Args>(args)...}
        , first_{nullptr, nullptr, &root_, std::forward<Args>(args)...}
        , last_{nullptr, &root_, nullptr, std::forward<Args>(args)...}
    {}

    ~Tree()
    {
        NodeImpl* node = root_.preorder_next_;
        while (node != &last_)
        {
            NodeImpl* next_node = node->preorder_next_;
            delete node;
            node = next_node;
        }
    }

    Node get_root()
    {
        return root_;
    }

    Node get_parent(Node node)
    {
        return (nullptr != node.parent_) ? *node.parent_ : node;
    }

    template<class... Args>
    Node add_child(Node node, Args&&... args)
    {
        NodeImpl* prev_node = &node;
        while (nullptr != prev_node->youngest_child_)
        {
            prev_node = prev_node->youngest_child_;
        }
        NodeImpl* next_node = prev_node->preorder_next_;
        node.youngest_child_ = new NodeImpl(&node, prev_node, next_node, std::forward<Args>(args)...);
        prev_node->preorder_next_ = node.youngest_child_;
        next_node->preorder_prev_ = node.youngest_child_;
        return *node.youngest_child_;
    }

    PreOrderIterator begin_preorder() { return PreOrderIterator(&root_); }
    PreOrderIterator end_preorder() { return PreOrderIterator(&last_); }
    RevPreOrderIterator rbegin_preorder() { return RevPreOrderIterator(PreOrderIterator(last_.preorder_prev_)); }
    RevPreOrderIterator rend_preorder() { return RevPreOrderIterator(PreOrderIterator(&first_)); }

private:
    NodeImpl root_;
    NodeImpl first_;
    NodeImpl last_;
};

} // namespace util
} // namespace vaneins

#endif // VANEINS_UTIL_TREE_TREE_HPP_