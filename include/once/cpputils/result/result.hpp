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
#ifndef ONCE__CPPUTILS__RESULT__RESULT_HPP_
#define ONCE__CPPUTILS__RESULT__RESULT_HPP_

#include <once/cpputils/strong_type/strong_type.hpp>
#include <variant>

namespace once {

struct ok_result_t
{};

inline constexpr ok_result_t ok_result{};

struct error_result_t
{};

inline constexpr error_result_t error_result{};

template<typename O, typename E>
class result
{
public:
  using ok_type = O;
  using error_type = E;

private:
  using ok_strong_type = strong_type<ok_type, struct ok_type_tag>;
  using error_strong_type = strong_type<error_type, struct error_type_tag>;

public:
  template<
    typename... Args,
    std::enable_if_t<std::is_constructible_v<ok_type, Args...>, bool> = true>
  constexpr result(ok_result_t, Args&&... args)
    : data_{ std::in_place_type<ok_strong_type>, std::forward<Args>(args)... }
  {
  }

  template<
    typename U,
    typename... Args,
    std::enable_if_t<
      std::is_constructible_v<ok_type, std::initializer_list<U>, Args...>,
      bool> = true>
  constexpr result(ok_result_t, std::initializer_list<U> li, Args&&... args)
    : data_{ std::in_place_type<ok_strong_type>,
             li,
             std::forward<Args>(args)... }
  {
  }

  template<
    typename... Args,
    std::enable_if_t<std::is_constructible_v<error_type, Args...>, bool> = true>
  constexpr result(error_result_t, Args&&... args)
    : data_{ std::in_place_type<error_strong_type>,
             std::forward<Args>(args)... }
  {
  }

  template<
    typename U,
    typename... Args,
    std::enable_if_t<
      std::is_constructible_v<error_type, std::initializer_list<U>, Args...>,
      bool> = true>
  constexpr result(error_result_t, std::initializer_list<U> li, Args&&... args)
    : data_{ std::in_place_type<error_strong_type>,
             li,
             std::forward<Args>(args)... }
  {
  }

  bool is_ok() const { return std::holds_alternative<ok_strong_type>(data_); }

  bool is_ok_and(ok_type const& value) const
  {
    return is_ok() && (value == ok());
  }

  bool is_error() const
  {
    return std::holds_alternative<error_strong_type>(data_);
  }

  bool is_error_and(error_type const& value) const
  {
    return is_error() && (value == error());
  }

  ok_type& ok() & { return *std::get<ok_strong_type>(data_); }

  ok_type const& ok() const& { return *std::get<ok_strong_type>(data_); }

  ok_type&& ok() && { return std::move(*std::get<ok_strong_type>(data_)); }

  ok_type const&& ok() const&&
  {
    return std::move(*std::get<ok_strong_type>(data_));
  }

  error_type& error() & { return *std::get<error_strong_type>(data_); }

  error_type const& error() const&
  {
    return *std::get<error_strong_type>(data_);
  }

  error_type&& error() &&
  {
    return std::move(*std::get<error_strong_type>(data_));
  }

  error_type const&& error() const&&
  {
    return std::move(*std::get<error_strong_type>(data_));
  }

private:
  std::variant<ok_strong_type, error_strong_type> data_;
};

} // namespace once

#endif // ONCE__CPPUTILS__RESULT__RESULT_HPP_