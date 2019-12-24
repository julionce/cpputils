#define CATCH_CONFIG_MAIN

#include <vaneins/utils/shared_mutex.hpp>

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
