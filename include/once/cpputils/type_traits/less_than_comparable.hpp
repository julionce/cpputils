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
#ifndef ONCE__CPPUTILS__TYPE_TRAITS__LESS_THAN_COMPARABLE_HPP_
#define ONCE__CPPUTILS__TYPE_TRAITS__LESS_THAN_COMPARABLE_HPP_

#include <type_traits>

namespace once {

namespace detail {
template<typename Lhs, typename Rhs, typename = void>
struct is_less_than_comparable : std::false_type
{};

template<typename Lhs, typename Rhs>
struct is_less_than_comparable<
  Lhs,
  Rhs,
  std::void_t<decltype(std::declval<Lhs const&>() <
                       std::declval<Rhs const&>())>>
  : std::bool_constant<std::is_same_v<decltype(std::declval<Lhs const&>() <
                                               std::declval<Rhs const&>()),
                                      bool>>
{};

} // namespace detail

template<typename Lhs, typename Rhs = Lhs>
struct is_less_than_comparable : detail::is_less_than_comparable<Lhs, Rhs>
{
  constexpr bool operator()() { return this->value; }
};

template<typename Lhs, typename Rhs = Lhs>
inline constexpr bool is_less_than_comparable_v =
  is_less_than_comparable<Lhs, Rhs>::value;

template<typename Lhs, typename Rhs = Lhs>
using require_less_than_comparable =
  std::enable_if_t<is_less_than_comparable_v<Lhs, Rhs>>;

} // namespace once

#endif // ONCE__CPPUTILS__TYPE_TRAITS__LESS_THAN_COMPARABLE_HPP_