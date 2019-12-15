#include "utils.h"
#include "framework.h"

#include "Fiber.h"
#include "FiberScheduler.h"

namespace fibers
{
    void this_fiber::yield()
    {
        Fiber* next = getScheduler()->getNext();
        if (next == getThisFiber())
        {
            return;
        }
        next->resume();
    }

    Fiber* this_fiber::getThisFiber()
    {
        return Fiber::GetThis();
    }

    FiberScheduler* this_fiber::getScheduler()
    {
        thread_local FiberScheduler* scheduler = new FiberScheduler;
        return scheduler;
    }

    void this_fiber::markForDelete()
    {
        getScheduler()->markForDelete(getThisFiber());
    }

}