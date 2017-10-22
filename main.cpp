#include <iostream>

#include "ch2.hpp"

#define TEST_ASSERT_TRUE(COND,text,ret) if ((COND)) { \
    std::cerr << __FILE__ << ':' << __LINE__ << "(text)" << std::endl; \
    (ret) = 1; }


int main() 
{
    int ret = 0;
    TEST_ASSERT_TRUE(cppconcurrency::ch2_thread_stuff(), "ch2 thread stuff", ret);
    return ret;
}
