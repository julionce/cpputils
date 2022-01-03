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
#ifndef ONCE__CPPUTILS__TYPE_TRAITS__AGGREGATE_INITIALIZABLE_HPP_
#define ONCE__CPPUTILS__TYPE_TRAITS__AGGREGATE_INITIALIZABLE_HPP_

#include <type_traits>

namespace once {
namespace detail {

template<typename Enable, typename T, typename... Args>
struct is_aggregate_initializable : std::false_type
{};

template<typename T, typename... Args>
struct is_aggregate_initializable<
  std::void_t<decltype(T{ std::declval<Args>()... })>,
  T,
  Args...> : std::is_aggregate<T>
{};

} // namespace detail

template<typename T, typename... Args>
struct is_aggregate_initializable
  : detail::is_aggregate_initializable<void, T, Args...>
{};

template<typename T, typename... Args>
inline constexpr bool is_aggregate_initializable_v =
  is_aggregate_initializable<T, Args...>::value;

} // namespace once

#endif // ONCE__CPPUTILS__TYPE_TRAITS__AGGREGATE_INITIALIZABLE_HPP_