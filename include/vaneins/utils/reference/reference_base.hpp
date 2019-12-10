/*
 * Copyright 2019-present Julián Bermúdez Ortega.
 *
 * This file is part of VaneinsCpputils.
 *
 * VaneinsCpputils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * VaneinsCpputils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with VaneinsCpputils.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef VANEINS_UTILS_REFERENCE_REFERENCEBASE_HPP_
#define VANEINS_UTILS_REFERENCE_REFERENCEBASE_HPP_

#include <memory>
#include <utility>

namespace vaneins {
namespace utils {

template<typename T>
class ReferenceBase
{
public:
    ReferenceBase() = default;

    template<typename R>
    ReferenceBase(R&& impl);

    virtual ~ReferenceBase() = default;

    ReferenceBase(ReferenceBase&& other) = default;
    ReferenceBase(const ReferenceBase& other) = default;
    ReferenceBase& operator=(ReferenceBase&& other) = default;
    ReferenceBase& operator=(const ReferenceBase& other) = default;

    bool is_null() const;
    
    operator bool() const;

protected:
    std::shared_ptr<T> impl_;
};

template<typename T>
template<typename R>
inline ReferenceBase<T>::ReferenceBase(R&& impl)
    : impl_{std::forward<R>(impl)}
{}

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

} // namespace utils
} // namespace vaneins

#endif // VANEINS_UTILS_REFERENCE_REFERENCEBASE_HPP_