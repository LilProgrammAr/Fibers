#pragma once

#include "framework.h"

#include "utils.h"
#include <functional>

namespace fibers
{
    class FIBERS_API Fiber
    {
    private:
        static void FiberEntry(void*);
        void deleteFiberToDelete();
        fibers::details::NativeHandle createHandle(size_t stackSize);
        void schedule();

        Fiber();
    private:
        static std::uint32_t ms_fibersCount;

        details::NativeHandle m_handle;
        std::function<void()> m_proc;
        std::uint32_t m_id;

        bool m_isJoinedTo;

        std::vector<Fiber*> m_joinedFibers;
    public:
        Fiber(std::function<void()> proc, size_t stackSize = 0);
        Fiber(details::NativeHandle fiber);

        template<class Fn>
        Fiber(Fn proc);

        ~Fiber();

        void run();
        void resume();

        void join();

        static Fiber* ConvetrThreadToFiber();
        static Fiber* GetThis();
    };

    template<class Fn>
    inline Fiber::Fiber(Fn proc)
        : m_handle(nullptr)
        , m_proc([&]() { proc(); })
        , m_id(ms_fibersCount++)
        , m_isJoinedTo(false)
    {
        m_handle = createHandle(0);
        schedule();
    }

}