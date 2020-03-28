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
#include <vector>
#include <list>
#include <functional>

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

    Node<T> const & get_parent() const { return parent_; }
    std::vector<Node<T>> const & get_children() const { return children_; }

    template<class... Args>
    Node<T> const & add_child(Args&&... args);

    T const & data() const { return data_; }

    bool operator==(const NodeImpl& other) const { return this == &other; }

private:
    T data_;
    Node<T> parent_;
    std::vector<Node<T>> children_;
};

template<typename T>
template<class... Args>
inline
NodeImpl<T>::NodeImpl(Args&&... args)
    : data_{std::forward<Args>(args)...}
    , parent_{Node<T>::null()}
    , children_{}
{}

template<typename T>
template<class... Args>
inline
Node<T> const & NodeImpl<T>::add_child(Args&&... args)
{
    children_.emplace_back(std::forward<Args>(args)...);
    children_.back()->parent_ = Node<T>::from_shared(this->shared_from_this());
    return children_.back();
}

template<typename T>
class PreOrder
{
public:
    PreOrder(Node<T> const & node);

    std::list<Node<T>> const & get_nodes() const { return nodes_; }
    std::list<Node<T>> & get_nodes() { return nodes_; }

private:
    void push_node(Node<T> const & node);

private:
    std::list<Node<T>> nodes_;
};

template<typename T, typename F>
inline
void visit_in_preorder(Node<T> const & node, F function)
{
    std::function<void(Node<T> const &)> visit_node;
    visit_node = [&function, &visit_node] (Node<T> const & node) {
        function(node);
        for (auto& n : node->get_children()) { visit_node(n); }
    };
    visit_node(node);
}

template<typename T, typename F>
inline
void visit_in_rpreorder(Node<T> const & node, F function)
{
    std::list<Node<T>> nodes;
    auto create_list = [&nodes] (Node<T> const & node) {
        nodes.push_back(node);
    };
    visit_in_preorder(node, create_list);
    std::for_each(nodes.rbegin(), nodes.rend(), function);
}

template<typename T, typename F>
inline
void visit_in_postorder(Node<T> const & node, F function)
{
    std::function<void(Node<T> const &)> visit_node;
    visit_node = [&function, &visit_node] (Node<T> const & node) {
        for (auto& n : node->get_children()) { visit_node(n); }
        function(node);
    };
    visit_node(node);
}

template<typename T, typename F>
inline
void visit_in_rpostorder(Node<T> const & node, F function)
{
    std::list<Node<T>> nodes;
    auto create_list = [&nodes] (Node<T> const & node) {
        nodes.push_back(node);
    };
    visit_in_postorder(node, create_list);
    std::for_each(nodes.rbegin(), nodes.rend(), function);
}

template<typename T>
class PostOrder
{
public:
    PostOrder(Node<T> const & node);

    std::list<Node<T>> const & get_nodes() const { return nodes_; }
    std::list<Node<T>> & get_nodes() { return nodes_; }

private:
    void push_node(Node<T> const & node);

private:
    std::list<Node<T>> nodes_;
};

template<typename T>
inline
PostOrder<T>::PostOrder(Node<T> const & node)
    : nodes_{}
{
    push_node(node);
}

template<typename T>
inline
void PostOrder<T>::push_node(Node<T> const & node)
{
    for (auto& n : node->get_children())
    {
        push_node(n);
    }
    nodes_.push_back(node);
}

} // namespace tree

template<typename T>
class Tree
{
public:
    class NodeImpl;

    using Node = julibert::cpputils::Reference<NodeImpl>;
    class NodeImpl : public std::enable_shared_from_this<NodeImpl>
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

        NodeImpl(const NodeImpl&) = delete;
        NodeImpl& operator=(const NodeImpl&) = delete;

        const T& data() const { return data_; }
        bool operator==(const NodeImpl& other) const { return this == &other; }

        Node get_parent() const
        {
            return parent_;
        }

        template<class... Args>
        Node add_child(Args&&... args)
        {
            Node this_node = Node::from_shared(this->shared_from_this());
            Node prev_node = this_node;
            while (prev_node->youngest_child_)
            {
                prev_node = prev_node->youngest_child_;
            }
            Node next_node = prev_node->preorder_next_;
            youngest_child_ = Node(this_node, prev_node, next_node, std::forward<Args>(args)...);
            prev_node->preorder_next_ = youngest_child_;
            next_node->preorder_prev_ = youngest_child_;
            return youngest_child_;
        }

        void add_child(Node node)
        {
            if (node)
            {
                Node this_node = Node::from_shared(this->shared_from_this());
                Node prev_node = this_node;
                while (prev_node->youngest_child_)
                {
                    prev_node = prev_node->youngest_child_;
                }
                Node next_node = prev_node->preorder_next_;
                Node last_node = node;
                while (last_node->preorder_next_)
                {
                    last_node = last_node->preorder_next_;
                }
                node->parent_ = this_node;
                youngest_child_ = node;
                node->preorder_prev_ = prev_node;
                last_node->preorder_next_ = next_node;
                prev_node->preorder_next_ = node;
                next_node->preorder_prev_ = last_node;
            }
        }

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

        template<class... Args>
        explicit ReverseIterator(Args... args)
            : normal_it_{std::forward<Args>(args)...}
        {}

        ReverseIterator& operator++() { --normal_it_; return *this; }
        ReverseIterator& operator--() { ++normal_it_; return *this; }
        bool operator==(const ReverseIterator& other) const { return (normal_it_ == other.normal_it_); }
        bool operator!=(const ReverseIterator& other) const { return (normal_it_ != other.normal_it_); }
        const value_type& operator*() const { return *normal_it_; }

    private:
        R normal_it_;
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

        bool operator==(const PreOrderIterator& other) const { return (node_.get() == other.node_.get()); }
        bool operator!=(const PreOrderIterator& other) const { return !(*this == other); }
        const T& operator*() const { return node_->data_; }

    private:
        Node node_;
    };

    using RPreOrderIterator = ReverseIterator<PreOrderIterator>;

    template<class... Args>
    explicit Tree(Args&&... args)
        : root_{Node::null(), Node::null(), Node::null(), std::forward<Args>(args)...}
        , first_{Node::null(), root_, Node::null(), std::forward<Args>(args)...}
        , last_{Node::null(), Node::null(), root_, std::forward<Args>(args)...}
    {
        root_->preorder_prev_ = first_;
        root_->preorder_next_ = last_;
    }

    Node get_root() const
    {
        return root_;
    }

    void splice(Node & node, Tree & other)
    {
        node->preorder_next_->preorder_prev_ = other.last_->preorder_prev_;
        other.last_->preorder_prev_->preorder_next_ = node->preorder_next_;
        node->preorder_next_ = other.root_;
        other.root_->preorder_prev_ = node;
        other.root_ = Node::null();
        other.first_ = Node::null();
        other.last_ = Node::null();
    }

    PreOrderIterator begin_preorder() const { return PreOrderIterator(root_); }
    PreOrderIterator end_preorder() const { return PreOrderIterator(last_); }
    RPreOrderIterator rbegin_preorder() const { return RPreOrderIterator(last_->preorder_prev_); }
    RPreOrderIterator rend_preorder() const { return RPreOrderIterator(first_); }

private:
    Node root_;
    Node first_;
    Node last_;
};

} // namespace cpputils
} // namespace julibert

#endif // JULIBERT_CPPUTILS_TREE_TREE_HPP_