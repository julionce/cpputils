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
#ifndef VANEINS_UTIL_REFERENCE_Reference_HPP_
#define VANEINS_UTIL_REFERENCE_Reference_HPP_

#include <memory>
#include <utility>
#include <type_traits>

namespace vaneins {
namespace util {

template<typename T>
class Reference
{
public:
    template<class... Args>
    explicit Reference(Args&&... args);

    virtual ~Reference() = default;

    Reference(Reference&& other) = default;
    Reference(Reference& other) = default;
    Reference(const Reference& other) = default;
    Reference& operator=(Reference&& other) = default;
    Reference& operator=(Reference& other) = default;
    Reference& operator=(const Reference& other) = default;

    void copy_from(
        const Reference<T>& other);

    bool is_null() const;
    
    operator bool() const;

    T* operator->();

    const T* operator->() const;

    template<typename R>
    friend bool operator==(const Reference<R>& lhs, const Reference<R>& rhs);

    template<typename R>
    friend bool operator!=(const Reference<R>& lhs, const Reference<R>& rhs);

    template<typename R>
    operator R() const;

    static Reference<T> null();

    static Reference<T> copy(
        const Reference<T>& other);

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
inline void Reference<T>::copy_from(
        const Reference<T>& other)
{
    if (other.impl_)
    {
        impl_ = std::shared_ptr<T>(new T(*other.impl_.get()));
    }
    else
    {
        impl_.reset();
    }
}

template<typename T>
inline bool Reference<T>::is_null() const
{
    return !bool(impl_);
}

template<typename T>
inline Reference<T>::operator bool() const
{
    return bool(impl_);
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
    if (lhs.is_null() || rhs.is_null())
    {
        rv = lhs.is_null() && rhs.is_null();
    }
    else
    {
        if (lhs.impl_.get() == rhs.impl_.get())
        {
            rv = true;
        }
        else
        {
            rv = (*lhs.impl_ == *rhs.impl_);
        }
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
inline Reference<T> Reference<T>::copy(
        const Reference<T>& other)
{
    Reference<T> rv = Reference<T>::null();
    rv.copy_from(other);
    return rv;
}

} // namespace util
} // namespace vaneins

#endif // VANEINS_UTIL_REFERENCE_Reference_HPP_