/*
 * Copyright 2019-present Julián Bermúdez Ortega.
 *
 * This file is part of VaneinsCpputils.
 *
 * VaneinsCpputils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * VaneinsCpputils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with VaneinsCpputils.  If not, see <https://www.gnu.org/licenses/>.
 */
#define CATCH_CONFIG_MAIN

#include <vaneins/util/shared_mutex/shared_mutex.hpp>

#include <catch2/catch.hpp>

#include <thread>
#include <chrono>

vaneins::utils::SharedMutex shared_mutex;

class SharedTask
{
public:
    void run()
    {
        vaneins::utils::SharedLock lock(shared_mutex);
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
        vaneins::utils::ExclusiveLock lock(shared_mutex);
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
