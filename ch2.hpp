/**
 * Code from C++ Concurrency In Action, chapter 2.
 *
 * 2. Managing threads.
 * 2.1. Basic thread management.
 * 2.2. Passing arguments to a thread function.
 * 2.3. Transferring ownership of a thread.
 * 2.4. Choosing number of threads at runtime.
 * 2.5. Identifying threads.
 */
#pragma once

#include <algorithm>
#include <functional>
#include <numeric>
#include <thread>
#include <vector>

namespace cppconcurrency {
namespace ch2 {

    /**
     * 2.1.1. Launching a thread.
     * 
     * std::thread my_thread((ThreadTask()));
     *
     * Could be a lambda as well:
     *
     *    std::thread my_thread([] {...});
     */
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

    /**
     * 2.1.3. Waiting in exceptional circumstances.
     */
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

    /**
     * 2.2. Passing arguments to a Thread function.
     */
    inline int thread_function(int a, int b) {
        return a+b;
    }

    /**
     * 2.4 Choosing the number of threads at runtime
     */
    template<typename Iterator, typename T>
    struct accumulate_block
    {
        void operator() (Iterator first, Iterator last, T& result)
        {
            result=std::accumulate(first,last,result);
        }
    };

    template<typename Iterator, typename T>
    T parallel_accumulate(Iterator first, Iterator last, T init)
    {
        unsigned long const length=std::distance(first,last);
        if (!length) {
            return init;
        }

        unsigned long const min_per_thread=25;
        unsigned long const max_threads=(length+min_per_thread-1)/min_per_thread;
        unsigned long const hardware_threads=std::thread::hardware_concurrency();
        unsigned long const num_threads=
            std::min(hardware_threads != 0 ? hardware_threads:2, max_threads);
        unsigned long const block_size=length/num_threads;
        std::vector<T> results(num_threads);
        std::vector<std::thread> threads(num_threads-1);
        Iterator block_start = first;
        for (unsigned long i=0;i<(num_threads-1);++i) {
            Iterator block_end=block_start;
            std::advance(block_end, block_size);
            threads[i]=std::thread(accumulate_block<Iterator,T>(),
                    block_start,block_end,std::ref(results[i]));
            block_start=block_end;
        }
        accumulate_block<Iterator,T>()(block_start,last,results[num_threads-1]);
        std::for_each(threads.begin(),threads.end(),std::mem_fn(&std::thread::join));

        return std::accumulate(results.begin(),results.end(),init);
    }

    /**
     * Run code from chapter 2.
     *
     * @return true if everything worked as expected.
     */
    bool run_sample_code();
}
}

