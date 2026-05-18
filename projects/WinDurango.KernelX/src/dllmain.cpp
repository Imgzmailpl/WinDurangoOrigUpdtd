#include "Hooks.h"
#include "kernelx.h"
#include "Logan.h"
#include "ForzaThreadHook.h"

static DWORD ReasonForCall = 0;
#define RETURN_IF_FAILED(hr) if (FAILED(hr)) return hr

void KernelxInitialize(HINSTANCE hinstDLL)
{
    if (!GetConsoleWindow())
    {
        AllocConsole();
        FILE *f;
        freopen_s(&f, "CONOUT$", "w", stdout);
        freopen_s(&f, "CONOUT$", "w", stderr);
        freopen_s(&f, "CONIN$", "r", stdin);

        SetConsoleTitleW(L"WinDurango");
    }

    if (ReasonForCall == DLL_PROCESS_ATTACH || ReasonForCall == DLL_THREAD_ATTACH)
    {
        DetourRestoreAfterWith();
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        XWinePatchImport(GetModuleHandleW(nullptr), GetRuntimeModule(),
                         "?GetActivationFactoryByPCWSTR@@YAJPEAXAEAVGuid@Platform@@PEAPEAX@Z",
                         GetActivationFactoryRedirect);

        //This is needed for DXVK.
        if (!User32) User32 = LoadLibraryA("user32.dll");
        if (User32)
        {
            FARPROC CreateWindowInBandEx = GetProcAddress(User32, "CreateWindowInBandEx");
            TrueCreateWindowInBandEx = reinterpret_cast<PCreateWindowInBandEx>(CreateWindowInBandEx);
            DetourAttach(&reinterpret_cast<PVOID &>(TrueCreateWindowInBandEx), EraCreateWindowInBandEx);
        }

        //Forza Horizon 2 Demo
        if (winrt::Windows::ApplicationModel::Package::Current().Id().FamilyName() == L"265E1020-Anthem_8wekyb3d8bbwe")
        {
            *(void**)&P_StartForzaThread = (char*)GetModuleHandleW(nullptr) + 0xFE6920;
            DetourAttach((void**)&P_StartForzaThread, &D_StartForzaThread);
        }
        //Forza Horizon 2
        if (winrt::Windows::ApplicationModel::Package::Current().Id().FamilyName() == L"Anthem_8wekyb3d8bbwe")
        {
            *(void**)&P_StartForzaThread = (char*)GetModuleHandleW(nullptr) + 0x1081A90;
            DetourAttach((void**)&P_StartForzaThread, &D_StartForzaThread);
            *(void**)&P_FmodThreadProc = (char*)GetModuleHandleW(nullptr) + 0x19D3F80;
            DetourAttach((void**)&P_FmodThreadProc, &D_FmodThreadProc);
        }
         //Forza Horizon 2 Presents Fast & Furious
        if (winrt::Windows::ApplicationModel::Package::Current().Id().FamilyName() == L"Spire_8wekyb3d8bbwe")
        {
            *(void**)&P_StartForzaThread = (char*)GetModuleHandleW(nullptr) + 0x10A7C00;
            DetourAttach((void**)&P_StartForzaThread, &D_StartForzaThread);
        }

        DetourAttach(&reinterpret_cast<PVOID &>(TrueCoCreateInstance), EraCoCreateInstance);
        DetourAttach(&reinterpret_cast<PVOID &>(TrueDeviceIoControl), EraDeviceIoControl);
        DetourTransactionCommit();
    }
    else if (ReasonForCall == DLL_PROCESS_DETACH || ReasonForCall == DLL_THREAD_DETACH)
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&reinterpret_cast<PVOID &>(TrueCreateWindowInBandEx), EraCreateWindowInBandEx);
        DetourDetach(&reinterpret_cast<PVOID &>(TrueCoCreateInstance), EraCoCreateInstance);
        DetourDetach(&reinterpret_cast<PVOID &>(TrueDeviceIoControl), EraDeviceIoControl);
        DetourTransactionCommit();
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    InitializeCriticalSection(&XmpAllocationHookLock);
    if (DetourIsHelperProcess()) return TRUE;

    ReasonForCall = ul_reason_for_call;

    if (ul_reason_for_call != DLL_PROCESS_ATTACH)
    {
        ReasonForCall = ul_reason_for_call;
        return TRUE;
    }

    return QueueUserAPC([](ULONG_PTR dwParam) { KernelxInitialize(reinterpret_cast<HINSTANCE>(dwParam)); },
                        GetCurrentThread(), reinterpret_cast<ULONG_PTR>(hModule)) != 0;
}