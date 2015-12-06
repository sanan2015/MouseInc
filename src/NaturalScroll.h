#include <mutex>
#include <chrono>
#include <thread>
#include <condition_variable>

class NaturalScroll
{
public:
    void Init()
    {
        abort_ = false;
        ignore_wheel_event = false;
        wheel_thread_ = new std::thread(&NaturalScroll::Thread, this);
    }

    void Exit()
    {
        abort_ = true;
        wheel_event_.notify_one();
        wheel_thread_->join();
        delete wheel_thread_;
    }

    bool Wheel(bool direction)
    {
        if (ignore_wheel_event)
        {
            ignore_wheel_event = false;
            return false;
        }

        ignore_wheel_event = true;
        direction_ = direction;
        wheel_event_.notify_one();
        return true;
    }
private:
    void Thread()
    {
        while (!abort_)
        {
            std::unique_lock<std::mutex> event_lock(wheel_mutex_);

            wheel_event_.wait(event_lock);
            if (abort_) break;

            SendWheel(direction_);
        }
    }
private:
    bool abort_;
    bool ignore_wheel_event;

    bool direction_;
    std::thread *wheel_thread_;

    std::mutex wheel_mutex_;
    std::condition_variable wheel_event_;
} natural_scroll;