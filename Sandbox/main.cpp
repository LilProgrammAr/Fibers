#include <Fibers/main.h>
#include <iostream>

void FiberMain(void*)
{
    auto mainFiber = fibers::this_fiber::getThisFiber();
    fibers::Fiber* f = new fibers::Fiber([mainFiber]()
        {
            for (int i = 0; i < 10; ++i)
            {
                std::cout << i << ' ';
                fibers::this_fiber::yield();
            }
            mainFiber->join();
            std::cout << '\n';
        });
    f->join();
    std::cout << fibers::this_fiber::getScheduler();
}