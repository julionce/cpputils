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
#ifndef VANEINS_UTIL_REFERENCE_REFERENCEBASE_HPP_
#define VANEINS_UTIL_REFERENCE_REFERENCEBASE_HPP_

#include <memory>
#include <utility>
#include <type_traits>

namespace vaneins {
namespace util {

template<typename T>
class ReferenceBase
{
public:
    ReferenceBase() = default;

    template<class... Args,
             typename =
                typename std::enable_if<(sizeof...(Args) > 1)>::type>
    ReferenceBase(Args&&... args);

    template<typename R,
             typename =
                typename std::enable_if<
                    !std::is_same<ReferenceBase<T>, typename std::decay<R>::type>::value &&
                    !std::is_base_of<ReferenceBase<T>, typename std::decay<R>::type>::value>
                ::type>
    ReferenceBase(R&& arg);

    virtual ~ReferenceBase() = default;

    ReferenceBase(ReferenceBase&& other) = default;
    ReferenceBase(const ReferenceBase& other) = default;
    ReferenceBase& operator=(ReferenceBase&& other) = default;
    ReferenceBase& operator=(const ReferenceBase& other) = default;

    void copy(
        const ReferenceBase& other);

    bool is_null() const;
    
    operator bool() const;

protected:
    std::shared_ptr<T> impl_;
};

template<typename T>
template<class... Args, typename>
inline ReferenceBase<T>::ReferenceBase(Args&&... args)
    : impl_{std::make_shared<T>(args...)}
{}

template<typename T>
template<typename R, typename>
inline ReferenceBase<T>::ReferenceBase(R&& arg)
    : impl_{std::make_shared<T>(arg)}
{}

template<typename T>
inline void ReferenceBase<T>::copy(
        const ReferenceBase& other)
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
inline bool ReferenceBase<T>::is_null() const
{
    return !bool(impl_);
}

template<typename T>
ReferenceBase<T>::operator bool() const
{
    return bool(impl_);
}

} // namespace util
} // namespace vaneins

#endif // VANEINS_UTIL_REFERENCE_REFERENCEBASE_HPP_