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
  template<class... Args>
  explicit Reference(Args&&... args)
    : impl_{ std::make_shared<T>(args...) }
  {}

  virtual ~Reference() = default;

  Reference(Reference&& other) = delete;
  Reference(Reference& other) noexcept
    : impl_{ other.impl_ }
  {}
  Reference(Reference const& other) noexcept
    : impl_{ other.impl_ }
  {}
  Reference& operator=(Reference&& other) = delete;
  Reference& operator=(Reference& other) noexcept
  {
    impl_ = other.impl_;
    return *this;
  };
  Reference& operator=(Reference const& other) noexcept
  {
    impl_ = other.impl_;
    return *this;
  };

  T& get() { return *impl_.get(); }

  T const& get() const { return *impl_.get(); }

  T* operator->();

  const T* operator->() const;

  template<typename R>
  friend bool operator==(Reference<R> const& lhs, Reference<R> const& rhs);

  template<typename R>
  friend bool operator!=(Reference<R> const& lhs, Reference<R> const& rhs);

  template<typename R>
  explicit operator R() const;

  static Reference<T> clone(Reference<T> const& other);

private:
  Reference(void*)
    : impl_{ nullptr }
  {}

protected:
  std::shared_ptr<T> impl_;
};

template<typename T>
inline T*
Reference<T>::operator->()
{
  return this->impl_.get();
}

template<typename T>
inline const T*
Reference<T>::operator->() const
{
  return this->impl_.get();
}

template<typename T>
template<typename R>
inline Reference<T>::operator R() const
{
  return impl_->operator R();
}

template<typename R>
inline bool
operator==(Reference<R> const& lhs, Reference<R> const& rhs)
{
  return (lhs.impl_.get() == rhs.impl_.get()) ? true : *lhs.impl_ == *rhs.impl_;
}

template<typename R>
inline bool
operator!=(Reference<R> const& lhs, Reference<R> const& rhs)
{
  return !(lhs == rhs);
}

template<typename T>
inline Reference<T>
Reference<T>::clone(Reference<T> const& other)
{
  Reference<T> rv = Reference<T>(static_cast<void*>(nullptr));
  if (other.impl_) {
    rv.impl_ = std::make_shared<T>(*other.impl_.get());
  } else {
    rv.impl_.reset();
  }
  return rv;
}

} // namespace julibert

#endif // JULIBERT_CPPUTILS_REFERENCE_REFERENCE_HPP_