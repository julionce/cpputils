/*
 * Copyright 2021-present Julián Bermúdez Ortega
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef ONCE__CPPUTILS__TYPE_TRAITS__HASHABLE_HPP_
#define ONCE__CPPUTILS__TYPE_TRAITS__HASHABLE_HPP_

#include <functional>
#include <type_traits>

namespace once {

template<typename T>
struct is_hashable : std::is_invocable_r<std::size_t, std::hash<T>, const T&>
{
};

template<typename T>
inline constexpr bool is_hashable_v = is_hashable<T>::value;

template<typename T>
using require_hashable = std::enable_if_t<is_hashable_v<T>>;

} // namespace once

#endif // ONCE__CPPUTILS__TYPE_TRAITS__HASHABLE_HPP_