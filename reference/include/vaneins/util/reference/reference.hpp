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

#ifndef VANEINS_UTIL_REFERENCE_REFERENCE_HPP_
#define VANEINS_UTIL_REFERENCE_REFERENCE_HPP_

#include <vaneins/util/reference/reference_base.hpp>

namespace vaneins {
namespace utils {

template<typename T>
class Reference : public ReferenceBase<T>
{
    using ReferenceBase<T>::ReferenceBase;
public:
    virtual ~Reference() = default;

    Reference(Reference&& other) = default;
    Reference(const Reference& other) = default;
    Reference& operator=(Reference&& other) = default;
    Reference& operator=(const Reference& other) = default;

    T* operator->();
};

template<typename T>
T* Reference<T>::operator->()
{
    return this->impl_.get();
}

} // namespace utils
} // namespace vaneins

#endif // VANEINS_UTIL_REFERENCE_REFERENCE_HPP_