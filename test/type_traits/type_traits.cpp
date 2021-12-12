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

#include <once/cpputils/type_traits/type_traits.hpp>

using namespace once;

SCENARIO("is_variant trait")
{
  GIVEN("a variant type")
  {
    using MyVariant = std::variant<int>;
    THEN("the trait value shall be true") { REQUIRE(is_variant_v<MyVariant>); }
  }

  GIVEN("a non-variant type")
  {
    THEN("the trait value shall be false") { REQUIRE_FALSE(is_variant_v<int>); }
  }
}