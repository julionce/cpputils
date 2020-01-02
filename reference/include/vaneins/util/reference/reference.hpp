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
    template<class... Args,
             typename =
                typename std::enable_if<(sizeof...(Args) != 1)>::type>
    Reference(Args&&... args);

    template<typename R,
             typename =
                typename std::enable_if<
                    !std::is_same<Reference<T>, typename std::decay<R>::type>::value &&
                    !std::is_base_of<Reference<T>, typename std::decay<R>::type>::value>
                ::type>
    Reference(R&& arg);

    virtual ~Reference() = default;

    Reference(Reference&& other) = default;
    Reference(const Reference& other) = default;
    Reference& operator=(Reference&& other) = default;
    Reference& operator=(const Reference& other) = default;

    T* operator->();
    const T* operator->() const;

    static Reference<T> copy(
        const Reference<T>& other);

    void copy_from(
        const Reference<T>& other);

    bool is_null() const;
    
    operator bool() const;

protected:
    std::shared_ptr<T> impl_;
};

template<typename T>
template<class... Args, typename>
inline Reference<T>::Reference(Args&&... args)
    : impl_{std::make_shared<T>(args...)}
{}

template<typename T>
template<typename R, typename>
inline Reference<T>::Reference(R&& arg)
    : impl_{std::make_shared<T>(arg)}
{}

template<typename T>
T* Reference<T>::operator->()
{
    return this->impl_.get();
}

template<typename T>
const T* Reference<T>::operator->() const
{
    return this->impl_.get();
}

template<typename T>
inline Reference<T> Reference<T>::copy(
        const Reference<T>& other)
{
    Reference<T> rv;
    rv.copy_from(other);
    return rv;
}

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
Reference<T>::operator bool() const
{
    return bool(impl_);
}

} // namespace util
} // namespace vaneins

#endif // VANEINS_UTIL_REFERENCE_Reference_HPP_