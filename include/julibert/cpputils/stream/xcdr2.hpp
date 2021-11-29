/*
 * Copyright 2019-present Julián Bermúdez Ortega.
 *
 * This file is part of julibert::idlcom.
 *
 * julibert::idlcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * julibert::idlcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with julibert::idlcom.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef JULIBERT__CPPUTILS__STREAM__XCDR2_HPP_
#define JULIBERT__CPPUTILS__STREAM__XCDR2_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace once {
namespace cpputils {

// Possible endian implementation according with
// https://en.cppreference.com/w/cpp/types/endian
enum class Endian
{
#ifdef _WIN32
  little = 0,
  big = 1,
  native = little
#else
  little = __ORDER_LITTLE_ENDIAN__,
  big = __ORDER_LITTLE_ENDIAN__,
  native = __BYTE_ORDER__
#endif
};

namespace xcdr2 {

enum class StreamState : uint8_t
{
  ok,
  error
};

struct StreamBase
{
  size_t ser_length() { return ser_length_; }
  size_t deser_length() { return deser_length_; }

  StreamState ser_state() { return ser_state_; }
  StreamState deser_state() { return deser_state_; }

protected:
  template<typename U>
  static const uint8_t* constant_cast(U* ptr)
  {
    return static_cast<const uint8_t*>(static_cast<const void*>(ptr));
  }

  template<typename U>
  static uint8_t* cast(U* ptr)
  {
    return static_cast<uint8_t*>(static_cast<void*>(ptr));
  }

  template<typename U, size_t N>
  struct Aligner
  {
    static size_t padding(size_t current_size)
    {
      const size_t remainder = current_size & (N - 1);
      return (remainder) ? (N - remainder) : 0;
    }
  };

  template<typename U>
  struct Aligner<U, 8>
  {
    static size_t padding(size_t current_size)
    {
      return Aligner<U, 4>::padding(current_size);
    }
  };

  template<typename U>
  size_t padding(size_t current_size)
  {
    return Aligner<U, sizeof(U)>::padding(current_size);
  }

  size_t ser_length_ = 0;
  size_t deser_length_ = 0;
  StreamState ser_state_ = StreamState::ok;
  StreamState deser_state_ = StreamState::ok;
};

template<typename T>
struct StreamBuffer
{
  using buffer_type = T;
  T const& buffer() { return buffer_; }

protected:
  T buffer_;
};

template<Endian E, typename T>
struct Stream
{};

template<Endian E>
struct Stream<E, std::vector<uint8_t>>
  : public StreamBase
  , public StreamBuffer<std::vector<uint8_t>>
{
  template<typename T,
           typename = std::enable_if_t<std::is_arithmetic<T>::value>>
  Stream& operator<<(T const& data)
  {
    ser_length_ += padding<T>(ser_length_);
    buffer_.resize(ser_length_);
    auto ptr = constant_cast(&data);
    if constexpr (E == Endian::native) {
      buffer_.insert(buffer_.begin() + ser_length_, ptr, ptr + sizeof(T));
    } else {
      buffer_.insert(buffer_.begin() + ser_length_,
                     std::reverse_iterator(ptr + sizeof(T)),
                     std::reverse_iterator(ptr));
    }
    ser_length_ += sizeof(T);
    return *this;
  }

  template<typename T,
           typename = std::enable_if_t<std::is_arithmetic<T>::value>>
  Stream& operator>>(T& data)
  {
    size_t padding{ this->padding<T>(deser_length_) };
    size_t future_size = deser_length_ + padding + sizeof(T);
    if (buffer_.size() >= future_size) {
      auto ptr = cast(&data);
      auto it = buffer_.begin() + deser_length_ + padding;
      if constexpr (E == Endian::native) {
        std::copy(it, it + sizeof(T), ptr);
      } else {
        std::reverse_copy(it, it + sizeof(T), ptr);
      }
      deser_length_ = future_size;
    } else {
      deser_state_ = StreamState::error;
    }

    return *this;
  }

  Stream& operator<<(std::string const& data)
  {
    uint32_t length = data.length();
    *this << length;
    buffer_.resize(ser_length_ + length);
    std::copy(data.begin(), data.end(), buffer_.begin() + ser_length_);
    ser_length_ += length;
    return *this;
  }

  Stream& operator>>(std::string& data)
  {
    uint32_t length;
    *this >> length;
    data.resize(length);
    std::copy(buffer_.begin() + deser_length_,
              buffer_.begin() + deser_length_ + length,
              data.data());
    deser_length_ += length;
    return *this;
  }

  template<typename T>
  Stream& operator<<(std::vector<T> const& data)
  {
    uint32_t length = data.size();
    *this << length;
    std::for_each(
      data.begin(), data.end(), [this](auto&& item) { *this << item; });
    return *this;
  }

  template<typename T>
  Stream& operator>>(std::vector<T>& data)
  {
    uint32_t length;
    *this >> length;
    data.resize(length);
    std::for_each(
      data.begin(), data.end(), [this](auto&& item) { *this >> item; });
    return *this;
  }

  template<typename T, size_t N>
  Stream& operator<<(std::array<T, N> const& data)
  {
    std::for_each(
      data.begin(), data.end(), [this](auto&& item) { *this << item; });
    return *this;
  }

  template<typename T, size_t N>
  Stream& operator>>(std::array<T, N>& data)
  {
    std::for_each(
      data.begin(), data.end(), [this](auto&& item) { *this >> item; });
    return *this;
  }
};

template<Endian E>
using VectorStreamEndian = Stream<E, std::vector<uint8_t>>;
using VectorStream = VectorStreamEndian<Endian::native>;

} // namespace xcdr2
} // namespace utils
} // namespace once

#endif // JULIBERT__CPPUTILS__STREAM__XCDR2_HPP_