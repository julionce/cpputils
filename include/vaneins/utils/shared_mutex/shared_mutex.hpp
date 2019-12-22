#ifndef VANEINS_UTILS_SHAREDMUTEX_HPP_
#define VANEINS_UTILS_SHAREDMUTEX_HPP_

#include <mutex>
#include <condition_variable>

namespace vaneins {
namespace utils {
    
class SharedMutex
{
public:
    friend class SharedLock;
    friend class ExclusiveLock;

    SharedMutex()
        : mtx_{}
        , cv_{}
        , shared_counter_{0}
        , exclusively_locked_{false}
    {}

private:
    std::mutex mtx_;
    std::condition_variable cv_;
    size_t shared_counter_;
    bool exclusively_locked_;
};

class SharedLock 
{
public:
    SharedLock(
            SharedMutex& shared_mtx)
        : shared_mtx_{shared_mtx}
    {
        std::unique_lock lock{shared_mtx_.mtx_};
        shared_mtx_.cv_.wait(
            lock,
            [&]{ return !shared_mtx_.exclusively_locked_; });
        ++shared_mtx_.shared_counter_;
    }

    ~SharedLock()
    {
        std::unique_lock lock{shared_mtx_.mtx_};
        --shared_mtx_.shared_counter_;
        if (0 == shared_mtx_.shared_counter_)
        {
            shared_mtx_.cv_.notify_one();
        }
    }

private:
    SharedMutex& shared_mtx_;
};

class ExclusiveLock 
{
public:
    ExclusiveLock(
            SharedMutex& shared_mtx)
        : shared_mtx_{shared_mtx}
    {
        std::unique_lock lock{shared_mtx_.mtx_};
        shared_mtx_.cv_.wait(
            lock,
            [&]{ return (0 != shared_mtx_.shared_counter_) &&  !shared_mtx_.exclusively_locked_; });
        shared_mtx_.exclusively_locked_ = true;
    }

    ~ExclusiveLock()
    {
        std::unique_lock lock{shared_mtx_.mtx_};
        shared_mtx_.exclusively_locked_ = false;
        shared_mtx_.cv_.notify_all();
    }

private:
    SharedMutex& shared_mtx_;
};


} // utils
} // vaneins

#endif // VANEINS_UTILS_SHAREDMUTEX_HPP_
