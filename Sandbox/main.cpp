#include <Fibers/main.h>
#include <Fibers/Thread.h>
#include <iostream>

void FiberMain(void*)
{
    fibers::Thread t1([]() {
        fibers::Fiber* f1 = new fibers::Fiber([]()
        {
            for (int i = 0; i < 11; i += 2)
            {
                std::cout << i << ' ';
                fibers::this_fiber::yield();
            }
        });

        fibers::Fiber* f2 = new fibers::Fiber([]()
        {
            for (int i = 1; i < 10; i += 2)
            {
                std::cout << i << ' ';
                fibers::this_fiber::yield();
            }
        });
        std::cout << std::endl << fibers::this_fiber::getScheduler() << std::endl;
    });
    
    t1.join();
    std::cout << fibers::this_fiber::getScheduler();
}