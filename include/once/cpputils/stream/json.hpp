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
#ifndef ONCE__CPPUTILS__STREAM__JSON_HPP_
#define ONCE__CPPUTILS__STREAM__JSON_HPP_

#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace once::cpputils::stream {

class Json
{
  enum class Token : uint8_t
  {
    none,
    left_brace,
    right_brace,
    identifier,
    colon,
    value,
    comma
  };

public:
  enum class Object : bool
  {
    begin,
    end
  };

  class Identifier
  {
  public:
    template<class T>
    Identifier(T&& data)
      : data_{ std::forward<T>(data) }
    {
    }

    explicit operator std::string() { return data_; }

  private:
    std::string data_;
  };

  template<class T>
  using Member = std::pair<Identifier, T>;

public:
  Json(std::iostream& os);

  Json& operator<<(Object object);
  template<typename T>
  Json& operator<<(const Member<T>& member);

private:
  template<typename T>
  Json& operator<<(std::decay_t<T*> pointer);
  template<typename T>
  Json& operator<<(const std::vector<T>& vector);
  template<typename T, size_t N>
  Json& operator<<(const std::array<T, N>& array);
  template<typename T>
  Json& operator<<(const std::optional<T>& optional);

private:
  std::iostream stream_;
  Token last_token_;
};

inline Json::Json(std::iostream& stream)
  : stream_{ stream }
  , last_token_{ Token::none }
{
  stream_ << std::boolalpha;
}

inline Json&
Json::operator<<(Object object)
{
  switch (object) {
    case Object::begin:
      stream_ << '{';
      last_token_ = Token::left_brace;
      break;
    case Object::end:
      stream_ << '}';
      last_token_ = Token::right_brace;
      break;
  }
  return *this;
}

template<typename T>
inline Json&
Json::operator<<(const Member<T>& member)
{
  if (Token::left_brace == last_token_ || Token::value == last_token_) {
    if (Token::value == last_token_) {
      stream_ << ',';
    }
    stream_ << '"' << member.first << '"' << ":" << member.second;
    last_token_ = Token::value;
  }
  return *this;
}

template<typename T>
inline Json&
Json::operator<<(std::decay_t<T*> pointer)
{
  stream_ << (nullptr != pointer) ? pointer : "null";
  return *this;
}

template<typename T>
inline Json&
Json::operator<<(const std::vector<T>& vector)
{
  stream_ << '[';
  if (!vector.empty()) {
    stream_ << vector.front();
    std::for_each(++vector.begin(), vector.end(), [&vector](auto&& item) {
      stream_ << ',' << item;
    });
  }
  stream_ << ']';
  return *this;
}

template<typename T, size_t N>
inline Json&
Json::operator<<(const std::array<T, N>& array)
{
  stream_ << '[';
  if constexpr (N > 0) {
    stream_ << array.front();
    std::for_each(++array.begin(), array.end(), [&array](auto&& item) {
      stream_ << ',' << item;
    });
  }
  stream_ << ']';
  return *this;
}

template<typename T>
inline Json&
Json::operator<<(const std::optional<T>& optional)
{
  stream_ << (optional) ? *optional << "null";
  return *this;
}

} // namespace once::cpputils::stream

#endif // ONCE__CPPUTILS__STREAM__JSON_HPP_