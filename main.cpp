#include <iostream>
#include <chrono>
#include "commandline.h"
#include "crashdump.hpp"
#include "clock.hpp"
#include "ctimer.hpp"

void test_crashdump()
{
    CrashDump crashdump;
    struct Test {
        int *p;
    };
    Test t;
    delete t.p;
    return;
}

void test_clock()
{
    LARGE_INTEGER t1 = Clock::now();
    Sleep(1000);
    LARGE_INTEGER t2 = Clock::now();

    printf("Time in s:%f\n", Clock::duration(t2, t1));
    std::cout<<"timestamp:"<<Clock::getSecondTimestamp()<<std::endl;
    return;
}

int onTimeout()
{
    std::cout<<"time out"<<std::endl;
    return 0;
}
void test_ctimer()
{
    CTimer timer;
    timer.startAfter(5, onTimeout);
    Sleep(6000);
    return;
}


int main()
{
    test_ctimer();
    return 0;
}
