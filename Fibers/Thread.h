#pragma once 
#include "framework.h"

#include <thread>

namespace fibers
{
    class FIBERS_API Thread
    {
    private:
        static void threadProc(std::function<void()> proc);
    private:
        std::thread m_thread;
    public:
        Thread(std::function<void()> proc);
        void join();
        void detach();

        bool joinable();

        std::thread::id get_id();
        std::thread::native_handle_type native_handle();
    };
}