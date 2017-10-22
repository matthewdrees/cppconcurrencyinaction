#include "ch2.hpp"

#include <thread>
#include <iostream>

namespace {

    class ThreadTask {
    public:
        ThreadTask(int i) : _i(i) {}

        void operator()() {
            _i += 20;
        }

        int getI() const { return _i; }

    private:
        int _i;
    };
        
    int thread_function(int a, int b) {
        return a+b;
    }

    class ThreadGuard
    {
    public:
        explicit ThreadGuard(std::thread& t_): t(t_) {}

        ~ThreadGuard() {
            if (t.joinable()) {
                t.join();
            }
        }
        ThreadGuard(const ThreadGuard&) = delete;
        ThreadGuard& operator=(const ThreadGuard&) = delete;

    private:
        std::thread& t;
    };
}

namespace cppconcurrency {

    bool ch2_thread_stuff() {

        bool ret = true;
        {
            // Detached thread. Note that if this was the last thing this
            // program does, valgrind complains about "possibly lost" memory.
            std::thread my_thread(thread_function, 3, 5);
            my_thread.detach();
            ret = ret && my_thread.joinable();
        }

        {
            // Nonsensical, return value is ignored, just playing around here.
            std::thread my_thread(thread_function, 2, 4);
            my_thread.join();
        }

        {
            // Prefer this way so far.
            ThreadTask threadTask(1);
            std::thread my_thread(threadTask);
            my_thread.join();
            ret = ret && (threadTask.getI() == 21);
        }

        {
            // The thread guard will call join even if an exception is thrown,
            // or not if it was joined somewhere else. Otherwise I don't really
            // like this approach. Looking forward to futures and std::promise.
            std::thread my_thread(thread_function, 1, 3);
            ThreadGuard threadGuard(my_thread);
        }

        return ret;
    }
}
