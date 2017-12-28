#include "ch2.hpp"

#include <iostream>

#define TEST_ASSERT_TRUE(COND,text,ret) if ((COND)) { \
    std::cerr << __FILE__ << ':' << __LINE__ << "(text)" << std::endl; \
    (ret) = 1; }

/**
 * This is essentially a test harness for running sample code.
 */
int main() 
{
    int ret = 0;
    TEST_ASSERT_TRUE(cppconcurrency::ch2::run_sample_code(), "ch2 thread stuff", ret);
    return ret;
}
