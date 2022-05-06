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
#ifndef ONCE__CPPUTILS__STRONG_TYPE__STRONG_TYPE_HPP_
#define ONCE__CPPUTILS__STRONG_TYPE__STRONG_TYPE_HPP_

#include <once/cpputils/type_traits/type_traits.hpp>

namespace once {

namespace detail {

template<typename T, typename Tag>
class strong_type
{
public:
  using value_type = T;

public:
  template<typename... Args>
  constexpr explicit strong_type(Args&&... args)
    : data_(std::forward<Args>(args)...)
  {
  }

  strong_type& operator=(T const& data)
  {
    data_ = data;
    return *this;
  }

  T& operator*() & noexcept { return data_; }
  T const& operator*() const& noexcept { return data_; }
  T&& operator*() && noexcept { return std::move(data_); }
  T const&& operator*() const&& noexcept { return std::move(data_); }

  T* operator->() noexcept { return &data_; }
  T const* operator->() const noexcept { return &data_; }

private:
  T data_;
};

}

namespace st {

template<typename U, typename T, typename = require_equality_comparable<U>>
struct equality_comparable
{
  using underlying_type = U;
  using target_type = T;

  bool operator==(underlying_type const& other) const
  {
    return (*static_cast<target_type const&>(*this) == other);
  }

  bool operator==(target_type const& other) const
  {
    return (*static_cast<target_type const&>(*this) == *other);
  }

  bool operator!=(underlying_type const& other) const
  {
    return !(*this == other);
  }

  bool operator!=(target_type const& other) const { return !(*this == other); }
};

template<typename U, typename T, typename = require_less_than_comparable<U>>
struct less_than_comparable
{
  using underlying_type = U;
  using target_type = T;

  bool operator<(underlying_type const& other) const
  {
    return (*static_cast<target_type const&>(*this) < other);
  }

  bool operator<(target_type const& other) const
  {
    return (*static_cast<target_type const&>(*this) < *other);
  }

  bool operator>(underlying_type const& other) const
  {
    return (other < *static_cast<target_type const&>(*this));
  }

  bool operator>(target_type const& other) const
  {
    return (*other < *static_cast<target_type const&>(*this));
  }

  bool operator<=(underlying_type const& other) const
  {
    return !(*this > other);
  }

  bool operator<=(target_type const& other) const { return !(*this > other); }

  bool operator>=(underlying_type const& other) const
  {
    return !(*this < other);
  }

  bool operator>=(target_type const& other) const { return !(*this < other); }
};

template<typename U, typename T, typename = require_addable<U>>
struct addable
{
  using underlying_type = U;
  using target_type = T;

  friend target_type operator+(target_type const& lhs,
                               underlying_type const& rhs)
  {
    return target_type((*lhs) + rhs);
  }

  friend target_type operator+(underlying_type const& lhs,
                               target_type const& rhs)
  {
    return target_type(lhs + (*rhs));
  }

  friend target_type operator+(target_type const& lhs, target_type const& rhs)
  {
    return target_type((*lhs) + (*rhs));
  }
};

template<typename U, typename T, typename = require_subtractable<U>>
struct subtractable
{
  using underlying_type = U;
  using target_type = T;

  friend target_type operator-(target_type const& lhs,
                               underlying_type const& rhs)
  {
    return target_type((*lhs) - rhs);
  }

  friend target_type operator-(underlying_type const& lhs,
                               target_type const& rhs)
  {
    return target_type(lhs - (*rhs));
  }

  friend target_type operator-(target_type const& lhs, target_type const& rhs)
  {
    return target_type((*lhs) - (*rhs));
  }
};

template<typename U, typename T, typename = require_multiplicable<U>>
struct multiplicable
{
  using underlying_type = U;
  using target_type = T;

  friend target_type operator*(target_type const& lhs,
                               underlying_type const& rhs)
  {
    return target_type((*lhs) * rhs);
  }

  friend target_type operator*(underlying_type const& lhs,
                               target_type const& rhs)
  {
    return target_type(lhs * (*rhs));
  }

  friend target_type operator*(target_type const& lhs, target_type const& rhs)
  {
    return target_type((*lhs) * (*rhs));
  }
};

template<typename U, typename T, typename = require_divisible<U>>
struct divisible
{
  using underlying_type = U;
  using target_type = T;

  friend target_type operator/(target_type const& lhs,
                               underlying_type const& rhs)
  {
    return target_type((*lhs) / rhs);
  }

  friend target_type operator/(underlying_type const& lhs,
                               target_type const& rhs)
  {
    return target_type(lhs / (*rhs));
  }

  friend target_type operator/(target_type const& lhs, target_type const& rhs)
  {
    return target_type((*lhs) / (*rhs));
  }
};

template<typename U, typename T, typename = require_modulable<U>>
struct modulable
{
  using underlying_type = U;
  using target_type = T;

  friend target_type operator%(target_type const& lhs,
                               underlying_type const& rhs)
  {
    return target_type((*lhs) % rhs);
  }

  friend target_type operator%(underlying_type const& lhs,
                               target_type const& rhs)
  {
    return target_type(lhs % (*rhs));
  }

  friend target_type operator%(target_type const& lhs, target_type const& rhs)
  {
    return target_type((*lhs) % (*rhs));
  }
};

template<typename U, typename T, typename = require_modulable<U>>
struct left_shiftable
{
  using underlying_type = U;
  using target_type = T;

  friend target_type operator<<(target_type const& lhs,
                                underlying_type const& rhs)
  {
    return target_type((*lhs) << rhs);
  }

  friend target_type operator<<(underlying_type const& lhs,
                                target_type const& rhs)
  {
    return target_type(lhs << (*rhs));
  }

  friend target_type operator<<(target_type const& lhs, target_type const& rhs)
  {
    return target_type((*lhs) << (*rhs));
  }
};

template<typename U, typename T, typename = require_modulable<U>>
struct right_shiftable
{
  using underlying_type = U;
  using target_type = T;

  friend target_type operator>>(target_type const& lhs,
                                underlying_type const& rhs)
  {
    return target_type((*lhs) >> rhs);
  }

  friend target_type operator>>(underlying_type const& lhs,
                                target_type const& rhs)
  {
    return target_type(lhs >> (*rhs));
  }

  friend target_type operator>>(target_type const& lhs, target_type const& rhs)
  {
    return target_type((*lhs) >> (*rhs));
  }
};

} // namespace st

template<typename T,
         typename Tag,
         template<typename, typename>
         typename... Features>
class strong_type
  : public detail::strong_type<T, Tag>
  , public Features<T, strong_type<T, Tag, Features...>>...
{
public:
  using detail::strong_type<int, Tag>::strong_type;
  using detail::strong_type<int, Tag>::operator=;
};

} // namespace once

#endif // ONCE__CPPUTILS__STRONG_TYPE__STRONG_TYPE_HPP_