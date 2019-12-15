#include "Thread.h"
#include "Fiber.h"

void fibers::Thread::threadProc(std::function<void()> proc)
{
    fibers::Fiber* startupFiber = fibers::Fiber::ConvetrThreadToFiber();
    proc();
    fibers::this_fiber::markForDelete();
    fibers::this_fiber::yield();
}

fibers::Thread::Thread(std::function<void()> proc)
    : m_thread(threadProc, proc)
{
}

void fibers::Thread::join()
{
    m_thread.join();
}

void fibers::Thread::detach()
{
    m_thread.detach();
}

bool fibers::Thread::joinable()
{
    return m_thread.joinable();
}

std::thread::id fibers::Thread::get_id()
{
    return m_thread.get_id();
}

std::thread::native_handle_type fibers::Thread::native_handle()
{
    return m_thread.native_handle();
}
