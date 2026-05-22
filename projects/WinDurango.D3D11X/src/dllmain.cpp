#include "d3d11.x.h"

// 1. Define the logger instance
std::shared_ptr<wd::common::WinDurango> p_wd;

// --- FIX: Define the global variables here exactly ONCE ---
abi_t g_ABI{};
BOOL m_Fence = TRUE;
std::multimap<void *, void *> g_ResourceMap;
std::mutex g_ResourceMapMutex;

// Note: ID3D11Device.cpp and ID3D11DeviceContext.cpp both complained about g_Context
// so we define it here as well to clear the linker error.
struct ID3D11DeviceContext2 *g_Context = nullptr;
// --------------------------------------------------------

BOOL WINAPI DllMain(HINSTANCE hinstDLL, // handle to DLL module
                    DWORD fdwReason,    // reason for calling function
                    LPVOID lpvReserved) // reserved
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL); // Good practice for graphics DLLs
        GetCombaseVersion();
        p_wd = wd::common::WinDurango::GetInstance();
    }
    return TRUE;
}