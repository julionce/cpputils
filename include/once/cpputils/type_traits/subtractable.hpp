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
#ifndef ONCE__CPPUTILS__TYPE_TRAITS__SUBTRACTABLE_HPP_
#define ONCE__CPPUTILS__TYPE_TRAITS__SUBTRACTABLE_HPP_

#include <type_traits>

namespace once {

namespace detail {

template<typename T, typename = void>
struct is_subtractable : std::false_type
{
};

template<typename T>
struct is_subtractable<
  T,
  std::void_t<decltype(std::declval<T>() - std::declval<T>())>> : std::true_type
{
};

} // namespace detail

template<typename T>
struct is_subtractable : detail::is_subtractable<T>
{
  constexpr bool operator()() { return this->value; }
};

template<typename T>
inline constexpr bool is_subtractable_v = is_subtractable<T>::value;

template<typename T>
using require_subtractable = std::enable_if_t<is_subtractable_v<T>>;

} // namespace once

#endif // ONCE__CPPUTILS__TYPE_TRAITS__SUBTRACTABLE_HPP_