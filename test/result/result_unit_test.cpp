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

#include <catch2/catch.hpp>

#include <once/cpputils/result/result.hpp>
#include <type_traits>

using namespace once;

SCENARIO("result constructors")
{
  GIVEN("a result<int, std::string> type")
  {
    using Result = result<int, std::string>;
    THEN("the ok result alternative shall be constructible from: int")
    {
      REQUIRE(std::is_constructible_v<Result, ok_result_t, int>);
    }

    THEN("the ok result alternative shall not be constructible from: int, int")
    {
      REQUIRE_FALSE(std::is_constructible_v<Result, ok_result_t, int, int>);
    }

    THEN(
      "the error result alternative shall be constructible from: const char*")
    {
      REQUIRE(std::is_constructible_v<Result, error_result_t, const char*>);
    }

    THEN("the error result alternative shall be constructible from: "
         "std::initializer_list<char>")
    {
      REQUIRE(std::is_constructible_v<Result,
                                      error_result_t,
                                      std::initializer_list<char>>);
    }

    THEN("the error result alternative shall not be constructible from: int")
    {
      REQUIRE_FALSE(std::is_constructible_v<Result, error_result_t, int>);
    }
  }

  GIVEN("a result<std::vector<int>, bool> type")
  {
    using Result = result<std::vector<int>, bool>;
    THEN("the ok result alternative shall be constructible from: "
         "std::initializer_list<int>")
    {
      REQUIRE(std::is_constructible_v<Result,
                                      ok_result_t,
                                      std::initializer_list<int>>);
    }
  }

  GIVEN("a result<std::tuple<int, float>, const char*> type")
  {
    using Result = result<std::tuple<int, float>, const char*>;
    THEN("the ok result alternative shall be constructible from: int, float")
    {
      REQUIRE(std::is_constructible_v<Result, ok_result_t, int, float>);
    }

    THEN("the ok result alternative shall not be constructible from: int")
    {
      REQUIRE_FALSE(std::is_constructible_v<Result, ok_result_t, int>);
    }
  }
}

SCENARIO("result::is_ok and result::is_error methods")
{
  using Result = result<int, std::string>;
  GIVEN("a result object")
  {
    WHEN("it has an ok_result value")
    {
      const auto ok = Result{ ok_result, 11 };
      THEN("the is_ok method shall return true")
      {
        REQUIRE(ok.is_ok());
      }

      THEN("the is_error method shall return false")
      {
        REQUIRE_FALSE(ok.is_error());
      }
    }

    WHEN("it has an error_result value")
    {
      const auto error = Result{ error_result, "error" };
      THEN("the is_ok method shall return false")
      {
        REQUIRE_FALSE(error.is_ok());
      }

      THEN("the is_error method shall return true")
      {
        REQUIRE(error.is_error());
      }
    }
  }
}

SCENARIO("result::is_ok_and and result::is_error_and methods")
{
  using ResultFoo = result<int, bool>;
  using ResultBar = result<bool, int>;
  GIVEN("a result object")
  {
    WHEN("it has an ok_result value")
    {
      const auto ok = ResultFoo{ ok_result, 11 };
      THEN("the is_ok_and method shall return true if the values match")
      {
        REQUIRE(ok.is_ok_and(11));
      }

      THEN("the is_ok_and method shall return false if the values do not "
           "match")
      {
        REQUIRE_FALSE(ok.is_ok_and(12));
      }

      THEN("the is_error method shall return false for any value")
      {
        REQUIRE_FALSE(ok.is_error_and(true));
        REQUIRE_FALSE(ok.is_error_and(false));
      }
    }

    WHEN("it has an error_result value")
    {
      const auto error = ResultBar{ error_result, 11 };
      THEN("the is_ok_and method shall return true for any value")
      {
        REQUIRE_FALSE(error.is_ok_and(true));
        REQUIRE_FALSE(error.is_ok_and(false));
      }

      THEN("the is_error_and method shall return true if the values match")
      {
        REQUIRE(error.is_error_and(11));
      }

      THEN(
        "the is_error_and method shall return false if the values do not match")
      {
        REQUIRE_FALSE(error.is_error_and(12));
      }
    }
  }
}