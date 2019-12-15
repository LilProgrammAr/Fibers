#pragma once

#include "framework.h"

namespace fibers
{
    namespace this_fiber
    {
        FIBERS_API void yield();
        FIBERS_API Fiber* getThisFiber();
        FIBERS_API FiberScheduler* getScheduler();
        FIBERS_API void markForDelete();
    }
}