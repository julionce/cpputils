/*
 * Copyright 2019-present Julián Bermúdez Ortega.
 *
 * This file is part of julibert::cpputils.
 *
 * julibert::cpputils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * julibert::cpputils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with julibert::cpputils.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef JULIBERT_CPPUTILS_REFERENCE_REFERENCE_HPP_
#define JULIBERT_CPPUTILS_REFERENCE_REFERENCE_HPP_

#include <memory>
#include <type_traits>
#include <utility>

namespace julibert {

template<typename T>
class Reference
{
public:
  template<typename... Args,
           std::enable_if_t<std::is_constructible_v<T, Args...>, bool> = true>
  explicit Reference(std::in_place_t, Args&&... args)
    : impl_{ std::make_shared<T>(args...) }
  {}

  Reference(Reference&&) = delete;
  Reference(Reference const&) = default;
  Reference& operator=(Reference&&) = delete;
  Reference& operator=(Reference const&) = default;

  T& get() { return *impl_.get(); }

  T const& get() const { return *impl_.get(); }

  T* operator->() { return impl_.get(); }

  T const* operator->() const { return impl_.get(); }

  template<typename R>
  friend bool operator==(Reference<R> const& lhs, Reference<R> const& rhs)
  {
    return (lhs.impl_.get() == rhs.impl_.get()) ? true
                                                : *lhs.impl_ == *rhs.impl_;
  }

  template<typename R>
  friend bool operator!=(Reference<R> const& lhs, Reference<R> const& rhs)
  {
    return !(lhs == rhs);
  }

  template<typename R>
  explicit operator R() const
  {
    return impl_->operator R();
  }

  static Reference<T> clone(Reference<T> const& other)
  {
    return Reference<T>(std::make_shared<T>(*other.impl_.get()));
  }

private:
  Reference(std::shared_ptr<T>&& impl)
    : impl_{ impl }
  {}

protected:
  std::shared_ptr<T> impl_;
};

} // namespace julibert

#endif // JULIBERT_CPPUTILS_REFERENCE_REFERENCE_HPP_