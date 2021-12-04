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
#ifndef JULIBERT__CPPUTILS__TYPE_TRAITS__TYPE_TRAITS_HPP_
#define JULIBERT__CPPUTILS__TYPE_TRAITS__TYPE_TRAITS_HPP_

#include <type_traits>

namespace julibert {

namespace detail {
template<typename T>
using less_than_operator =
  decltype(std::declval<const T&>().operator<(std::declval<const T&>()));

template<typename T>
using less_than_operator_standalone =
  decltype(operator<(std::declval<const T&>(), std::declval<const T&>()));

template<typename T, typename = void>
struct is_less_than_comparable : std::false_type
{};

template<typename T>
struct is_less_than_comparable<T, std::void_t<less_than_operator<T>>>
  : std::bool_constant<std::is_same_v<less_than_operator<T>, bool>>
{};

template<typename T, typename = void>
struct is_less_than_comparable_standalone : std::false_type
{};

template<typename T>
struct is_less_than_comparable_standalone<
  T,
  std::void_t<less_than_operator_standalone<T>>>
  : std::bool_constant<std::is_same_v<less_than_operator_standalone<T>, bool>>
{};

} // namespace detail

template<typename T>
struct is_less_than_comparable
  : std::disjunction<std::is_arithmetic<T>,
                     std::is_enum<T>,
                     std::is_pointer<T>,
                     detail::is_less_than_comparable<T>,
                     detail::is_less_than_comparable_standalone<T>>
{
  constexpr bool operator()() { return this->value; }
};

template<typename T>
inline constexpr bool is_less_than_comparable_v =
  is_less_than_comparable<T>::value;

template<typename T>
using require_less_than_comparable =
  std::enable_if_t<is_less_than_comparable_v<T>>;

} // namespace julibert

#endif // JULIBERT__CPPUTILS__TYPE_TRAITS__TYPE_TRAITS_HPP_