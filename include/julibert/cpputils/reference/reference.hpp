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
#ifndef JULIBERT_CPPUTILS_REFERENCE_REFERENCE_HPP_
#define JULIBERT_CPPUTILS_REFERENCE_REFERENCE_HPP_

#include <memory>
#include <utility>
#include <type_traits>

namespace julibert {
namespace cpputils {

template<typename T>
class Reference
{
public:
    template<class... Args>
    explicit Reference(Args&&... args);

    virtual ~Reference() = default;

    Reference(Reference && other) = default;
    Reference(Reference & other) noexcept : impl_{other.impl_} {}
    Reference(Reference const & other) noexcept : impl_{other.impl_} {}
    Reference& operator=(Reference && other) = default;
    Reference& operator=(Reference & other) noexcept { impl_ = other.impl_; return *this; };
    Reference& operator=(Reference const & other) noexcept { impl_ = other.impl_; return *this; };

    operator bool() const;

    T* get();

    const T* get() const;

    T* operator->();

    const T* operator->() const;

    template<typename R>
    friend bool operator==(const Reference<R>& lhs, const Reference<R>& rhs);

    template<typename R>
    friend bool operator!=(const Reference<R>& lhs, const Reference<R>& rhs);

    template<typename R>
    operator R() const;

    static Reference<T> null();

    static Reference<T> clone(
        const Reference<T>& other);

    static Reference<T> from_shared(
        const std::shared_ptr<T>& ptr);

private:
    Reference(void*);

protected:
    std::shared_ptr<T> impl_;
};

template<typename T>
template<class... Args>
inline Reference<T>::Reference(Args&&... args)
    : impl_{std::make_shared<T>(args...)}
{}

template<typename T>
inline Reference<T>::Reference(void*)
    : impl_{nullptr}
{}

template<typename T>
inline Reference<T>::operator bool() const
{
    return bool(impl_);
}

template<typename T>
inline T* Reference<T>::get()
{
    return this->impl_.get();
}

template<typename T>
inline const T* Reference<T>::get() const
{
    return this->impl_.get();
}

template<typename T>
inline T* Reference<T>::operator->()
{
    return this->impl_.get();
}

template<typename T>
inline const T* Reference<T>::operator->() const
{
    return this->impl_.get();
}

template<typename T>
template<typename R>
inline Reference<T>::operator R() const
{
    return impl_->operator R();
}

template<typename R>
inline bool operator==(const Reference<R>& lhs, const Reference<R>& rhs)
{
    bool rv = false;
    if (!lhs || !rhs)
    {
        rv = !lhs && !rhs;
    }
    else
    {
        rv = (lhs.impl_.get() == rhs.impl_.get()) ? true : *lhs.impl_ == *rhs.impl_;
    }
    return rv;
}

template<typename R>
inline bool operator!=(const Reference<R>& lhs, const Reference<R>& rhs)
{
    return !(lhs == rhs);
}

template<typename T>
inline Reference<T> Reference<T>::null()
{
    return Reference<T>(static_cast<void*>(nullptr));
}

template<typename T>
inline Reference<T> Reference<T>::clone(
        const Reference<T>& other)
{
    Reference<T> rv = Reference<T>::null();
    if (other.impl_)
    {
        rv.impl_ = std::shared_ptr<T>(new T(*other.impl_.get()));
    }
    else
    {
        rv.impl_.reset();
    }
    return rv;
}

template<typename T>
inline Reference<T> Reference<T>::from_shared(
        const std::shared_ptr<T>& ptr)
{
    Reference<T> rv = Reference<T>::null();
    rv.impl_ = ptr;
    return rv;
}

} // namespace cpputils
} // namespace julibert

#endif // JULIBERT_CPPUTILS_REFERENCE_REFERENCE_HPP_