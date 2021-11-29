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

#define CATCH_CONFIG_MAIN

#include <once/cpputils/stream/xcdr2.hpp>

#include <catch2/catch.hpp>

#include <limits>
#include <utility>

using namespace once::cpputils;

#define TEST_SERIALIZE(TYPE)                                                   \
  SECTION("serializing a TYPE")                                                \
  {                                                                            \
    TYPE data(0x11);                                                           \
    stream << data;                                                            \
    REQUIRE(sizeof(TYPE) == stream.ser_length());                              \
    REQUIRE(0 == stream.deser_length());                                       \
  }

#define TEST_DESERIALIZE(TYPE)                                                 \
  SECTION("deserializing a TYPE")                                              \
  {                                                                            \
    TYPE ser_data(0x11);                                                       \
    stream << ser_data;                                                        \
    TYPE deser_data{};                                                         \
    stream >> deser_data;                                                      \
    REQUIRE(stream.deser_state() == xcdr2::StreamState::ok);                   \
    REQUIRE(stream.deser_length() == sizeof(TYPE));                            \
    REQUIRE(ser_data == deser_data);                                           \
    stream >> deser_data;                                                      \
    REQUIRE(stream.deser_state() == xcdr2::StreamState::error);                \
    REQUIRE(stream.deser_length() == sizeof(TYPE));                            \
  }

TEMPLATE_TEST_CASE_SIG("xcdr2::Stream",
                       "",
                       ((Endian E), E),
                       (Endian::little),
                       (Endian::big))
{
  xcdr2::VectorStreamEndian<E> stream{};
  SECTION("checking initial state")
  {
    REQUIRE(0 == stream.ser_length());
    REQUIRE(0 == stream.deser_length());
    REQUIRE(stream.ser_state() == xcdr2::StreamState::ok);
    REQUIRE(stream.deser_state() == xcdr2::StreamState::ok);

    TEST_SERIALIZE(bool)
    TEST_SERIALIZE(int8_t)
    TEST_SERIALIZE(uint8_t)
    TEST_SERIALIZE(int16_t)
    TEST_SERIALIZE(uint16_t)
    TEST_SERIALIZE(int32_t)
    TEST_SERIALIZE(uint32_t)
    TEST_SERIALIZE(int64_t)
    TEST_SERIALIZE(uint64_t)
    TEST_SERIALIZE(float)
    TEST_SERIALIZE(double)

    SECTION("serializing a std::string")
    {
      std::string ser_data{ "helo" };
      stream << ser_data;
      REQUIRE(stream.ser_state() == xcdr2::StreamState::ok);
      REQUIRE(sizeof(uint32_t) + ser_data.length() == stream.ser_length());

      SECTION("deserializing a std::string")
      {
        std::string deser_data{ "world" };
        stream >> deser_data;
        REQUIRE(stream.deser_state() == xcdr2::StreamState::ok);
        REQUIRE(deser_data == ser_data);
        REQUIRE(sizeof(uint32_t) + deser_data.length() ==
                stream.deser_length());
      }
    }

    SECTION("serializeing a std::vector<int>")
    {
      std::vector<int> ser_data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      stream << ser_data;
      REQUIRE(stream.ser_state() == xcdr2::StreamState::ok);
      REQUIRE(sizeof(uint32_t) + sizeof(int) * ser_data.size() ==
              stream.ser_length());
      SECTION("deserializing a std::vector<int>")
      {
        std::vector<int> deser_data{ 10, 11 };
        stream >> deser_data;
        REQUIRE(stream.deser_state() == xcdr2::StreamState::ok);
        REQUIRE(deser_data == ser_data);
        REQUIRE(sizeof(uint32_t) + sizeof(int) * ser_data.size() ==
                stream.deser_length());
      }
    }

    SECTION("serializeing a std::array<int, 10>")
    {
      std::array<int, 10> ser_data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      stream << ser_data;
      REQUIRE(stream.ser_state() == xcdr2::StreamState::ok);
      REQUIRE(sizeof(int) * ser_data.size() == stream.ser_length());
      SECTION("deserializing a std::array<int, 10>")
      {
        std::array<int, 10> deser_data{ 10, 11 };
        stream >> deser_data;
        REQUIRE(stream.deser_state() == xcdr2::StreamState::ok);
        REQUIRE(deser_data == ser_data);
        REQUIRE(sizeof(int) * ser_data.size() == stream.deser_length());
      }
    }

    TEST_DESERIALIZE(bool)
    TEST_DESERIALIZE(int8_t)
    TEST_DESERIALIZE(uint8_t)
    TEST_DESERIALIZE(int16_t)
    TEST_DESERIALIZE(uint16_t)
    TEST_DESERIALIZE(int32_t)
    TEST_DESERIALIZE(uint32_t)
    TEST_DESERIALIZE(int64_t)
    TEST_DESERIALIZE(uint64_t)
    TEST_DESERIALIZE(float)
    TEST_DESERIALIZE(double)
  }
}
