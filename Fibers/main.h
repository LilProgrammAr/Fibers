#pragma once

#include "Fiber.h"

#if !defined(FIBERS_EXPORTS) && !defined(FIBERS_MAIN)
#define FIBERS_MAIN

void FiberMain(void*);

int main(int argc, char** argv)
{
    fibers::Fiber* startupFiber = fibers::Fiber::ConvetrThreadToFiber();
    FiberMain(startupFiber);

    fibers::this_fiber::markForDelete();
    fibers::this_fiber::yield();
    return 0;
}
#endif //FIBERS_EXPORTS && FIBERS_MAIN