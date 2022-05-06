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
#ifndef ONCE__CPPUTILS__TYPE_TRAITS__TYPE_CONTAINER_HPP_
#define ONCE__CPPUTILS__TYPE_TRAITS__TYPE_CONTAINER_HPP_

#include <type_traits>

namespace once {

template<typename... Ts>
struct type_container
{
  template<typename... Us>
  struct includes;

  template<typename U>
  struct includes<U> : std::disjunction<std::is_same<U, Ts>...>
  {
  };

  template<typename... Us>
  struct includes : std::conjunction<type_container<Ts...>::includes<Us>...>
  {
  };

  template<typename... Us>
  static constexpr bool includes_v = includes<Us...>::value;

  struct is_empty : std::bool_constant<(sizeof...(Ts) == 0)>
  {};

  static constexpr bool is_empty_v = is_empty::value;

  struct size : std::integral_constant<std::size_t, sizeof...(Ts)>
  {};

  static constexpr std::size_t size_v = (sizeof...(Ts));

  template<typename... Us>
  struct insert
  {
    using type = type_container<Ts..., Us...>;
  };

  template<typename... Us>
  using insert_t = typename insert<Us...>::type;
};

template<typename T>
struct is_type_container : std::false_type
{
};

template<typename... Ts>
struct is_type_container<type_container<Ts...>> : std::true_type
{
};

template<typename... Ts>
inline constexpr bool is_type_container_v = is_type_container<Ts...>::value;

} // namespace once

#endif // ONCE__CPPUTILS__TYPE_TRAITS__TYPE_CONTAINER_HPP_