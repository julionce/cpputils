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
#ifndef ONCE__CPPUTILS__TYPE_TRAITS__TYPE_TRAITS_HPP_
#define ONCE__CPPUTILS__TYPE_TRAITS__TYPE_TRAITS_HPP_

#include <type_traits>
#include <variant>

namespace once {
template<typename T>
struct is_variant : std::false_type
{};

template<typename... Types>
struct is_variant<std::variant<Types...>> : std::true_type
{};

template<typename T>
inline constexpr bool is_variant_v = is_variant<T>::value;

template<typename T, typename... Ts>
struct is_any_of
{
  template<typename U>
  struct same_as : std::disjunction<std::is_same<T, U>, std::is_same<Ts, U>...>
  {};

  template<typename U>
  static constexpr bool same_as_v = same_as<U>::value;

  template<typename U, typename... Us>
  struct same_as_any_of : std::disjunction<same_as<U>, same_as<Us>...>
  {};

  template<typename U, typename... Us>
  static constexpr bool same_as_any_of_v = same_as_any_of<U, Us...>::value;
};

template<typename T>
struct is
{
  template<typename... Us>
  struct in : std::disjunction<std::is_same<T, Us>...>
  {};

  template<typename... Us>
  struct in<std::variant<Us...>> : in<Us...>
  {};

  template<typename... Us>
  struct in<std::tuple<Us...>> : in<Us...>
  {};

  template<typename... Us>
  static constexpr bool in_v = in<Us...>::value;
};

} // namespace once

#include "./aggregate_initializable.hpp"
#include "./equality_comparable.hpp"
#include "./less_than_comparable.hpp"

#endif // ONCE__CPPUTILS__TYPE_TRAITS__TYPE_TRAITS_HPP_