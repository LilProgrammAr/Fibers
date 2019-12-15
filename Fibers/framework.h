#pragma once

#if defined(_MSC_VER)

#define NOMINMAX

#include <windows.h>

#ifdef FIBERS_EXPORTS
#define FIBERS_API __declspec(dllexport)
#define FIBERS_API_EXT_TEMPLATE
#else
#define FIBERS_API __declspec(dllimport)
#define FIBERS_API_EXT_TEMPLATE extern
#endif //FIBERS_EXPORTS

namespace fibers
{
    class FIBERS_API Fiber;
    class FIBERS_API FiberScheduler;
}
#pragma region STD_EXPORTS 

#include <functional>
#include <vector>
#include <thread>

FIBERS_API_EXT_TEMPLATE template class FIBERS_API std::function<void()>;

struct FIBERS_API std::_Container_base12;
FIBERS_API_EXT_TEMPLATE template class FIBERS_API std::_Vector_val<std::_Simple_types<fibers::Fiber*>>;
FIBERS_API_EXT_TEMPLATE template class FIBERS_API std::_Compressed_pair<std::allocator<fibers::Fiber*>,
                                                                        std::_Vector_val<std::_Simple_types<fibers::Fiber*>>, true>;
FIBERS_API_EXT_TEMPLATE template class FIBERS_API std::vector<fibers::Fiber*>;

class FIBERS_API std::thread;


#pragma endregion STD_EXPORTS

namespace fibers
{
    namespace details
    {
        using NativeHandle = void*;
    }
}

#else //_MSC_VER
static_assert(false, "Compiler is not supported");
#endif //_MSC_VER