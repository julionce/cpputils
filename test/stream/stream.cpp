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

#define CATCH_CONFIG_MAIN

#include <julibert/cpputils/stream/xcdr2.hpp>

#include <catch2/catch.hpp>

#include <limits>
#include <utility>

using namespace julibert::cpputils;

#define TEST_SERIALIZE(TYPE)                                                   \
  SECTION("serializing a TYPE")                                                \
  {                                                                            \
    TYPE data{ 0x11 };                                                         \
    stream << data;                                                            \
    REQUIRE(sizeof(TYPE) == stream.ser_length());                              \
    REQUIRE(0 == stream.deser_length());                                       \
  }

#define TEST_DESERIALIZE(TYPE)                                                 \
  SECTION("deserializing a TYPE")                                              \
  {                                                                            \
    TYPE ser_data{ 0x11 };                                                     \
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
