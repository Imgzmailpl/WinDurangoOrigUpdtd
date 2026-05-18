#include "d3d11.x.h"

std::shared_ptr<wd::common::WinDurango> p_wd;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, // handle to DLL module
                    DWORD fdwReason,    // reason for calling function
                    LPVOID lpvReserved) // reserved
{
    GetCombaseVersion();
    p_wd = wd::common::WinDurango::GetInstance();
    return TRUE;
}