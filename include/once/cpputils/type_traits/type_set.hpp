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
#ifndef ONCE__CPPUTILS__TYPE_TRAITS__TYPE_SET_HPP_
#define ONCE__CPPUTILS__TYPE_TRAITS__TYPE_SET_HPP_

#include <once/cpputils/type_traits/type_container.hpp>
#include <variant>

namespace once {

template<typename T, typename U>
struct type_set_constructor
{};

template<typename T, typename... Ts, typename... Us>
struct type_set_constructor<type_container<T, Ts...>, type_container<Us...>>
  : std::conditional_t<
      type_container<Us...>::template includes_v<T>,
      type_set_constructor<type_container<Ts...>, type_container<Us...>>,
      type_set_constructor<type_container<Ts...>, type_container<Us..., T>>>
{};

template<typename... Us>
struct type_set_constructor<type_container<>, type_container<Us...>>
  : type_container<Us...>
{};

template<typename... Ts>
struct type_set;

template<typename T>
struct is_type_set : std::false_type
{};

template<typename... Ts>
struct is_type_set<type_set<Ts...>> : std::true_type
{};

template<typename... Ts>
inline constexpr bool is_type_set_v = is_type_set<Ts...>::value;

template<typename... Ts>
struct type_set : type_set_constructor<type_container<Ts...>, type_container<>>
{
  template<typename U>
  struct equal
  {
    static_assert(!is_type_set_v<U>, "U is not a type_set");
  };

  template<typename... Us>
  struct equal<type_set<Us...>>
    : std::bool_constant<(type_set<Ts...>::size_v == type_set<Us...>::size_v) &&
                         (type_set<Ts...>::template includes_v<Us> && ...)>
  {};

  template<typename... Us>
  static constexpr bool equal_v = equal<Us...>::value;

  template<typename... Us>
  struct insert
  {
    using type = type_set<Ts..., Us...>;
  };

  template<typename... Us>
  using insert_t = typename insert<Us...>::type;
};

template<typename... Ts>
struct underlying_type_set;

template<typename... Ts>
struct underlying_type_set<std::variant<Ts...>>
{
  using type = type_set<Ts...>;
};

template<typename... Ts>
struct underlying_type_set<std::tuple<Ts...>>
{
  using type = type_set<Ts...>;
};

template<typename... Ts>
using underlying_type_set_t = typename underlying_type_set<Ts...>::type;

} // namespace once

#endif // ONCE__CPPUTILS__TYPE_TRAITS__TYPE_SET_HPP_