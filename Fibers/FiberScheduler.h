#pragma once
#include "framework.h"

#include <vector>

namespace fibers
{
    class FIBERS_API FiberScheduler
    {
    private:
        friend class Fiber;
    private:
        std::vector<Fiber*> m_fibers;
        Fiber* m_fiberToDelete;
    public:
        FiberScheduler();

        Fiber* getNext();

        void markForDelete(Fiber* fiber);
    };
}