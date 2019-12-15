#include "FiberScheduler.h"
#include "Fiber.h"
#include <algorithm>
#include <cassert>

namespace fibers
{
    FiberScheduler::FiberScheduler()
        : m_fiberToDelete(nullptr)
    {
    }

    Fiber* FiberScheduler::getNext()
    {
        Fiber* thisFiber = this_fiber::getThisFiber();
        auto it = std::find_if(m_fibers.begin(), m_fibers.end(), [=](Fiber* f) 
            {
                return f != m_fiberToDelete && f != thisFiber; 
            });
        if (it == m_fibers.end())
        {
            return thisFiber;
        }
        return *it;
    }

    void FiberScheduler::markForDelete(Fiber* fiber)
    {
        assert(std::find(m_fibers.begin(), m_fibers.end(), fiber) != m_fibers.end());
        m_fiberToDelete = fiber;
    }

}