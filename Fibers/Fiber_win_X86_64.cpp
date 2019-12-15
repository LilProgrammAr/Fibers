#include "Fiber.h"
#include "FiberScheduler.h"
#include <cassert>
#include <algorithm>

namespace fibers
{
    void Fiber::FiberEntry(void*)
    {
        Fiber* thisFiber = this_fiber::getThisFiber();
        thisFiber->deleteFiberToDelete();
        thisFiber->m_proc();
        this_fiber::getScheduler()->markForDelete(thisFiber);
        this_fiber::yield();
        assert(this_fiber::getScheduler()->m_fibers.size() == 1);
        ::ConvertFiberToThread();
    }

    void Fiber::deleteFiberToDelete()
    {
        FiberScheduler* scheduler = this_fiber::getScheduler();
        if (Fiber* toDelete = scheduler->m_fiberToDelete)
        {
            auto it = std::find(scheduler->m_fibers.begin(), scheduler->m_fibers.end(), toDelete);
            assert(it != scheduler->m_fibers.end());
            scheduler->m_fibers.erase(it);
            delete toDelete;
            scheduler->m_fiberToDelete = nullptr;
        }
    }

    fibers::details::NativeHandle Fiber::createHandle(size_t stackSize)
    {
        return ::CreateFiber(stackSize, &Fiber::FiberEntry, this);
    }

    void Fiber::schedule()
    {
        this_fiber::getScheduler()->m_fibers.push_back(this);
    }

    Fiber::Fiber()
        : m_handle(nullptr)
        , m_proc()
    {
    }

    Fiber::Fiber(std::function<void()> proc, size_t stackSize /* = 0*/)
        : m_proc(proc)
        , m_handle(nullptr)
    {
        m_handle = ::CreateFiber(stackSize, &Fiber::FiberEntry, this);
        this_fiber::getScheduler()->m_fibers.push_back(this);
    }

    Fiber::Fiber(fibers::details::NativeHandle fiber)
        : m_handle(fiber)
        , m_proc([]() {})
    {
        this_fiber::getScheduler()->m_fibers.push_back(this);
    }

    Fiber::~Fiber()
    {
        assert(this != this_fiber::getThisFiber());
        ::DeleteFiber(m_handle);
    }

    void Fiber::run()
    {
        resume();
    }

    void Fiber::resume()
    {
        ::SwitchToFiber(m_handle);
        deleteFiberToDelete();
    }


    Fiber* Fiber::ConvetrThreadToFiber()
    {
        Fiber* res = new Fiber;
        res->m_handle = ::ConvertThreadToFiber(res);
        res->m_proc = []() {};
        res->schedule();
        return res;
    }
	Fiber* Fiber::GetThis()
	{
        return static_cast<Fiber*>(::GetFiberData());
	}
}