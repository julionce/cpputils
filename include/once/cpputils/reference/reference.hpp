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
#ifndef ONCE__CPPUTILS__REFERENCE__REFERENCE_HPP_
#define ONCE__CPPUTILS__REFERENCE__REFERENCE_HPP_

#include <memory>
#include <type_traits>
#include <utility>

#include <once/cpputils/type_traits/type_traits.hpp>

namespace once {

template<typename T>
class reference
{
  template<typename>
  friend class reference;

public:
  template<typename... Args,
           std::enable_if_t<std::is_constructible_v<T, Args...>, bool> = true>
  explicit reference(Args&&... args)
    : impl_{ std::make_shared<T>(args...) }
  {}

  template<
    typename U,
    std::enable_if_t<std::is_constructible_v<T, std::initializer_list<U>&>,
                     bool> = true>
  explicit reference(std::initializer_list<U> ilist)
    : impl_{ std::make_shared<T>(T(ilist)) }
  {}

  reference(reference const&) = default;
  reference& operator=(reference const&) = default;

  T& get() const { return *impl_.get(); }

  T* operator->() const { return impl_.get(); }

  friend bool operator<(reference<T> const& lhs, reference<T> const& rhs)
  {
    static_assert(
      is_less_than_comparable_v<T>,
      "Error in reference<T>: the type T is not less than comparable");

    return (lhs.impl_.get() != rhs.impl_.get()) && *lhs.impl_ < *rhs.impl_;
  }

  friend bool operator>(reference<T> const& lhs, reference<T> const& rhs)
  {
    return (rhs < lhs);
  }

  friend bool operator<=(reference<T> const& lhs, reference<T> const& rhs)
  {
    return !(lhs > rhs);
  }

  friend bool operator>=(reference<T> const& lhs, reference<T> const& rhs)
  {
    return !(lhs < rhs);
  }

  friend bool operator==(reference<T> const& lhs, reference<T> const& rhs)
  {
    static_assert(
      is_equality_comparable_v<T>,
      "Error in reference<T>: the type T is not equality comparable");

    return (lhs.impl_.get() == rhs.impl_.get()) || (*lhs.impl_ == *rhs.impl_);
  }

  friend bool operator!=(reference<T> const& lhs, reference<T> const& rhs)
  {
    return !(lhs == rhs);
  }

  template<typename R, typename = std::enable_if_t<std::is_convertible_v<T, R>>>
  explicit operator reference<R>() const
  {
    return std::static_pointer_cast<R>(impl_);
  }

  static reference<T> clone(reference<T> const& other)
  {
    return reference<T>(std::make_shared<T>(*other.impl_.get()));
  }

private:
  reference(std::shared_ptr<T>&& impl)
    : impl_{ impl }
  {}

protected:
  std::shared_ptr<T> impl_;
};

} // namespace once

#endif // ONCE__CPPUTILS__REFERENCE__REFERENCE_HPP_