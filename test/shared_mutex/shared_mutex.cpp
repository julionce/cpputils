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

#include <julibert/cpputils/shared_mutex/shared_mutex.hpp>

#include <catch2/catch.hpp>

#include <chrono>
#include <thread>

once::cpputils::SharedMutex shared_mutex;

class SharedTask
{
public:
  void run()
  {
    once::cpputils::SharedLock lock(shared_mutex);
    init_time = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(std::chrono::microseconds(500));
    final_time = std::chrono::steady_clock::now();
  }

public:
  std::chrono::steady_clock::time_point init_time;
  std::chrono::steady_clock::time_point final_time;
};

class ExclusiveTask
{
public:
  void run()
  {
    once::cpputils::ExclusiveLock lock(shared_mutex);
    init_time = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(std::chrono::microseconds(500));
    final_time = std::chrono::steady_clock::now();
  }

public:
  std::chrono::steady_clock::time_point init_time;
  std::chrono::steady_clock::time_point final_time;
};

SCENARIO("basic")
{
  GIVEN("a shared task running")
  {
    SharedTask main_task;
    std::thread main_thread(&SharedTask::run, &main_task);

    WHEN("another shared task is launched")
    {
      SharedTask secondary_task;
      std::thread secondary_thread(&SharedTask::run, &secondary_task);

      THEN("this last shall not be blocked")
      {
        main_thread.join();
        secondary_thread.join();
        REQUIRE(secondary_task.init_time < main_task.final_time);
      }
    }

    WHEN("a exclusive task is launched")
    {
      ExclusiveTask secondary_task;
      std::thread secondary_thread(&ExclusiveTask::run, &secondary_task);

      THEN("this last shall be blocked")
      {
        main_thread.join();
        secondary_thread.join();
        REQUIRE(secondary_task.init_time > main_task.final_time);
      }
    }
  }

  GIVEN("a exclusive task running")
  {
    ExclusiveTask main_task;
    std::thread main_thread(&ExclusiveTask::run, &main_task);

    WHEN("a shared task is launched")
    {
      SharedTask secondary_task;
      std::thread secondary_thread(&SharedTask::run, &secondary_task);

      THEN("this last shall be blocked")
      {
        main_thread.join();
        secondary_thread.join();
        REQUIRE(secondary_task.init_time > main_task.final_time);
      }
    }

    WHEN("another exclusive task is launched")
    {
      ExclusiveTask secondary_task;
      std::thread secondary_thread(&ExclusiveTask::run, &secondary_task);

      THEN("this last shall be blocked")
      {
        main_thread.join();
        secondary_thread.join();
        REQUIRE(secondary_task.init_time > main_task.final_time);
      }
    }
  }
}
