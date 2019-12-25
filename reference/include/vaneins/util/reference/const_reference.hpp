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

#ifndef VANEINS_UTIL_REFERENCE_CONSTREFERENCE_HPP_
#define VANEINS_UTIL_REFERENCE_CONSTREFERENCE_HPP_

#include <vaneins/util/reference/reference.hpp>

namespace vaneins {
namespace util {

template<typename T>
class ConstReference : public ReferenceBase<T>
{
    using ReferenceBase<T>::ReferenceBase;
public:
    template<typename R>
    ConstReference(R&& reference);

    virtual ~ConstReference() = default;

    ConstReference(ConstReference&& other) = default;
    ConstReference(const ConstReference& other) = default;
    ConstReference& operator=(ConstReference&& other) = default;
    ConstReference& operator=(const ConstReference& other) = default;

    const T* operator->() const;
};

template<typename T>
template<typename R>
inline ConstReference<T>::ConstReference(R&& reference)
    : ReferenceBase<T>{std::forward<R>(reference)}
{}

template<typename T>
const T* ConstReference<T>::operator->() const
{
    return this->impl_.get();
}

} // namespace util
} // namespace vaneins

#endif // VANEINS_UTIL_REFERENCE_CONSTREFERENCE_HPP_