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
#include <list>

namespace julibert {
namespace cpputils {

template<typename T>
class Tree
{
public:
    class NodeImpl;

    using Node = julibert::cpputils::Reference<NodeImpl>;
    class NodeImpl
    {
        friend Tree;

    public:
        template<class... Args>
        explicit NodeImpl(
                Node parent,
                Node preorder_prev,
                Node preorder_next,
                Args&&... args)
            : data_{std::forward<Args>(args)...}
            , parent_{parent}
            , youngest_child_{Node::null()}
            , preorder_prev_{preorder_prev}
            , preorder_next_{preorder_next}
        {}

        const T& data() const { return data_; }
        bool operator==(const NodeImpl& other) const { this == &other; }

        NodeImpl(const NodeImpl&) = delete;
        NodeImpl& operator=(const NodeImpl&) = delete;

    private:
        const T data_;
        Node const parent_;
        Node youngest_child_;
        Node preorder_prev_;
        Node preorder_next_;
    };

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

public:
    class PreOrderIterator
    {
        friend Tree;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = int64_t;
        using pointer = T*;
        using reference = T&;

        explicit PreOrderIterator(Node node = Node::null())
            : node_{node}
        {}

        PreOrderIterator& operator++()
        {
            if (node_->preorder_next_)
            {
                node_ = node_->preorder_next_;
            }
            return *this;
        }

        PreOrderIterator& operator--()
        {
            if (node_->preorder_prev_)
            {
                node_ = node_->preorder_prev_;
            }
            return *this;
        }

        bool operator==(const PreOrderIterator& other) const { return (node_.operator->() == other.node_.operator->()); }
        bool operator!=(const PreOrderIterator& other) const { return !(*this == other); }
        const T& operator*() const { return node_->data_; }

    private:
        Node node_;
    };

    using RevPreOrderIterator = ReverseIterator<PreOrderIterator>;

    template<class... Args>
    explicit Tree(Args&&... args)
        : root_{Node::null(), Node::null(), Node::null(), std::forward<Args>(args)...}
        , first_{Node::null(), Node::null(), Node::null(), std::forward<Args>(args)...}
        , last_{Node::null(), Node::null(), Node::null(), std::forward<Args>(args)...}
    {
        root_->preorder_prev_ = first_;
        root_->preorder_next_ = last_;
        first_->preorder_next_ = root_;
        last_->preorder_prev_ = root_;
    }

    ~Tree() = default;

    Node get_root()
    {
        return root_;
    }

    Node get_parent(Node node)
    {
        return (!node->parent_.is_null()) ? node->parent_ : node;
    }

    template<class... Args>
    Node add_child(Node node, Args&&... args)
    {
        Node prev_node = node;
        while (prev_node->youngest_child_)
        {
            prev_node = prev_node->youngest_child_;
        }
        Node next_node = prev_node->preorder_next_;
        node->youngest_child_ = Node(node, prev_node, next_node, std::forward<Args>(args)...);
        prev_node->preorder_next_ = node->youngest_child_;
        next_node->preorder_prev_ = node->youngest_child_;
        return node->youngest_child_;
    }

    PreOrderIterator begin_preorder() { return PreOrderIterator(root_); }
    PreOrderIterator end_preorder() { return PreOrderIterator(last_); }
    RevPreOrderIterator rbegin_preorder() { return RevPreOrderIterator(PreOrderIterator(last_->preorder_prev_)); }
    RevPreOrderIterator rend_preorder() { return RevPreOrderIterator(PreOrderIterator(first_)); }

private:
    Node root_;
    Node first_;
    Node last_;
};

} // namespace cpputils
} // namespace julibert

#endif // JULIBERT_CPPUTILS_TREE_TREE_HPP_