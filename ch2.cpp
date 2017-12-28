#include "ch2.hpp"

#include <thread>
#include <iostream>


namespace cppconcurrency {
namespace ch2 {

    bool run_sample_code() {

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

        {
            std::vector<int> v = {1,2,3,4,5,6,7,8,9,10,11,12};
            int init = 0;
            
            ret = ret && (parallel_accumulate(v.begin(), v.end(), init) == 78);
        }
        return ret;
    }
}
}
