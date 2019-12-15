#include "Fiber.h"
#include "FiberScheduler.h"
#include <cassert>
#include <algorithm>

namespace fibers
{
    std::uint32_t Fiber::ms_fibersCount = 0;

    void Fiber::FiberEntry(void*)
    {
        Fiber* thisFiber = this_fiber::getThisFiber();
        thisFiber->deleteFiberToDelete();
        thisFiber->m_proc();
        for (auto joinedFiber : thisFiber->m_joinedFibers)
        {
            joinedFiber->m_isJoinedTo = false;
        }
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
        , m_id(ms_fibersCount++)
        , m_isJoinedTo(false)
    {
    }

    Fiber::Fiber(std::function<void()> proc, size_t stackSize /* = 0*/)
        : m_proc(proc)
        , m_handle(nullptr)
        , m_id(ms_fibersCount++)
        , m_isJoinedTo(false)
    {
        m_handle = ::CreateFiber(stackSize, &Fiber::FiberEntry, this);
        this_fiber::getScheduler()->m_fibers.push_back(this);
    }

    Fiber::Fiber(fibers::details::NativeHandle fiber)
        : m_handle(fiber)
        , m_proc([]() {})
        , m_id(ms_fibersCount++)
        , m_isJoinedTo(false)
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

    void Fiber::join()
    {
        Fiber* thisFiber = this_fiber::getThisFiber();
        assert(thisFiber != this);
        thisFiber->m_isJoinedTo = true;
        m_joinedFibers.push_back(thisFiber);
        while (thisFiber->m_isJoinedTo)
        {
            this_fiber::yield();
        }
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