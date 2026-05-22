#include "WinDurangoWinRT.h"
#include "Windows.Xbox.Input.Gamepad.h"
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Networking.Connectivity.h>
#include <winrt/Windows.Xbox.Achievements.h>
#include <winrt/Windows.Xbox.Input.h>
#include <winrt/Windows.Xbox.System.h>
#include <Windows/Xbox/UI/Windows.Xbox.UI.SystemUI.h>
#include <Xinput.h>
#include <activation.h>
#include <chrono>
#include <iomanip>
#include <set>
#include <sstream>
#include <string>
#include <tlhelp32.h>
#include <windows.h>
#pragma comment(lib, "Xinput.lib")

extern "C" int32_t __stdcall WINRT_GetActivationFactory(void *classId, void **factory) noexcept;
extern "C" void *GetApplicationPackageFactory();
std::shared_ptr<wd::common::WinDurango> p_wd = nullptr;

inline HRESULT CreateHString(const wchar_t *str, HSTRING *out)
{
    if (!out)
        return E_POINTER;
    winrt::hstring temp(str);
    *out = reinterpret_cast<HSTRING>(winrt::detach_abi(temp));
    return S_OK;
}

// ============================================================================
// AUDIO & CRT DETOURS
// ============================================================================
extern "C" void *__cdecl Safe_memset(void *dest, int c, size_t count)
{
    if (!dest)
        return nullptr;
    __stosb((BYTE *)dest, (BYTE)c, count);
    return dest;
}
static void *g_FakeAudioHeapBase = nullptr;
static UINT64 g_HeapOffset = 0;
extern "C" long __stdcall UniversalDummyMethod()
{
    return 0;
}

UINT64 GetSinkholeChunk()
{
    if (!g_FakeAudioHeapBase)
        g_FakeAudioHeapBase = VirtualAlloc(nullptr, 1024 * 1024 * 256, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!g_FakeAudioHeapBase)
        return 0;
    if (g_HeapOffset + (1024 * 1024) >= 1024 * 1024 * 256)
        g_HeapOffset = 0;
    UINT64 *chunk = (UINT64 *)((UINT64)g_FakeAudioHeapBase + g_HeapOffset);
    g_HeapOffset += 1024 * 1024;
    chunk[0] = (UINT64)&chunk[1];
    for (int k = 1; k < 1024; k++)
        chunk[k] = (UINT64)UniversalDummyMethod;
    return (UINT64)chunk;
}

static bool IsSafeOutPtr(void **ptr)
{
    if ((uintptr_t)ptr < 0x10000ULL || (uintptr_t)ptr > 0x00007FFFFFFFFFFFULL || ((uintptr_t)ptr % 8) != 0)
        return false;
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(ptr, &mbi, sizeof(mbi)) != 0)
    {
        if (mbi.State == MEM_COMMIT && (mbi.Protect & (PAGE_READWRITE | PAGE_WRITECOPY)))
            return true;
    }
    return false;
}
inline bool IsValidCOMOutPtr(UINT64 ptr)
{
    return IsSafeOutPtr((void **)ptr);
}

extern "C" UINT64 __stdcall Hook_AcpHalCreate(UINT64 a1, UINT64 a2)
{
    if (IsValidCOMOutPtr(a1))
        *(UINT64 *)a1 = GetSinkholeChunk();
    else if (IsValidCOMOutPtr(a2))
        *(UINT64 *)a2 = GetSinkholeChunk();
    return 0;
}
extern "C" UINT64 __stdcall Hook_ApuCreateHeap(UINT64 a1, UINT64 a2, UINT64 a3)
{
    if (IsValidCOMOutPtr(a2))
        *(UINT64 *)a2 = GetSinkholeChunk();
    else if (IsValidCOMOutPtr(a3))
        *(UINT64 *)a3 = GetSinkholeChunk();
    return 0;
}
extern "C" UINT64 __stdcall Hook_ApuAlloc(UINT64 a1, UINT64 a2, UINT64 a3, UINT64 a4)
{
    if (IsValidCOMOutPtr(a1))
        *(UINT64 *)a1 = GetSinkholeChunk();
    return 0;
}
extern "C" UINT64 __stdcall Hook_AcpHalAllocateShapeContexts(UINT64 a1, UINT64 a2, UINT64 a3)
{
    if (IsValidCOMOutPtr(a2))
        *(UINT64 *)a2 = GetSinkholeChunk();
    else if (IsValidCOMOutPtr(a3))
        *(UINT64 *)a3 = GetSinkholeChunk();
    return 0;
}
extern "C" UINT64 __stdcall Hook_ApuMapApuAddress(UINT64 a1, UINT64 a2, UINT64 a3)
{
    return 0;
}
extern "C" UINT64 __stdcall Hook_ApuIsVirtualAddressValid(UINT64 a1)
{
    return 1;
}
extern "C" UINT64 __stdcall Hook_AcpHalReleaseShapeContexts(UINT64 a1, UINT64 a2)
{
    return 0;
}
extern "C" UINT64 __stdcall Hook_ApuMapVirtualAddress(UINT64 a1, UINT64 a2, UINT64 a3)
{
    return 0;
}
extern "C" UINT64 __stdcall Hook_ApuFree(UINT64 a1)
{
    return 0;
}

void PatchIAT_All_AcpHal(HMODULE hMod)
{
    if (!hMod)
        return;
    PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)hMod;
    if (pDos->e_magic != IMAGE_DOS_SIGNATURE)
        return;
    PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)((PBYTE)hMod + pDos->e_lfanew);
    if (pNt->Signature != IMAGE_NT_SIGNATURE)
        return;
    DWORD importDirRVA = pNt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    if (!importDirRVA)
        return;

    PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)((PBYTE)hMod + importDirRVA);
    while (pImport->Name)
    {
        LPCSTR szModName = (LPCSTR)((PBYTE)hMod + pImport->Name);
        if (_stricmp(szModName, "AcpHal.dll") == 0)
        {
            PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)((PBYTE)hMod + pImport->FirstThunk);
            PIMAGE_THUNK_DATA pOrigThunk = nullptr;
            if (pImport->OriginalFirstThunk)
                pOrigThunk = (PIMAGE_THUNK_DATA)((PBYTE)hMod + pImport->OriginalFirstThunk);
            while (pThunk->u1.Function)
            {
                FARPROC targetHook = (FARPROC)UniversalDummyMethod;
                if (pOrigThunk && !IMAGE_SNAP_BY_ORDINAL(pOrigThunk->u1.Ordinal))
                {
                    PIMAGE_IMPORT_BY_NAME pImportName =
                        (PIMAGE_IMPORT_BY_NAME)((PBYTE)hMod + pOrigThunk->u1.AddressOfData);
                    LPCSTR funcName = (LPCSTR)pImportName->Name;
                    if (strcmp(funcName, "ApuMapApuAddress") == 0)
                        targetHook = (FARPROC)Hook_ApuMapApuAddress;
                    else if (strcmp(funcName, "ApuIsVirtualAddressValid") == 0)
                        targetHook = (FARPROC)Hook_ApuIsVirtualAddressValid;
                    else if (strcmp(funcName, "AcpHalReleaseShapeContexts") == 0)
                        targetHook = (FARPROC)Hook_AcpHalReleaseShapeContexts;
                    else if (strcmp(funcName, "AcpHalAllocateShapeContexts") == 0)
                        targetHook = (FARPROC)Hook_AcpHalAllocateShapeContexts;
                    else if (strcmp(funcName, "AcpHalCreate") == 0)
                        targetHook = (FARPROC)Hook_AcpHalCreate;
                    else if (strcmp(funcName, "ApuMapVirtualAddress") == 0)
                        targetHook = (FARPROC)Hook_ApuMapVirtualAddress;
                    else if (strcmp(funcName, "ApuFree") == 0)
                        targetHook = (FARPROC)Hook_ApuFree;
                    else if (strcmp(funcName, "ApuAlloc") == 0)
                        targetHook = (FARPROC)Hook_ApuAlloc;
                    else if (strcmp(funcName, "ApuCreateHeap") == 0)
                        targetHook = (FARPROC)Hook_ApuCreateHeap;
                }
                DWORD oldProtect;
                if (VirtualProtect(&pThunk->u1.Function, sizeof(ULONGLONG), PAGE_READWRITE, &oldProtect))
                {
                    pThunk->u1.Function = (ULONGLONG)targetHook;
                    VirtualProtect(&pThunk->u1.Function, sizeof(ULONGLONG), oldProtect, &oldProtect);
                }
                pThunk++;
                if (pOrigThunk)
                    pOrigThunk++;
            }
        }
        else if (_stricmp(szModName, "msvcr110.dll") == 0 || _stricmp(szModName, "msvcrt.dll") == 0)
        {
            PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)((PBYTE)hMod + pImport->FirstThunk);
            PIMAGE_THUNK_DATA pOrigThunk = nullptr;
            if (pImport->OriginalFirstThunk)
                pOrigThunk = (PIMAGE_THUNK_DATA)((PBYTE)hMod + pImport->OriginalFirstThunk);
            while (pThunk->u1.Function)
            {
                if (pOrigThunk && !IMAGE_SNAP_BY_ORDINAL(pOrigThunk->u1.Ordinal))
                {
                    PIMAGE_IMPORT_BY_NAME pImportName =
                        (PIMAGE_IMPORT_BY_NAME)((PBYTE)hMod + pOrigThunk->u1.AddressOfData);
                    if (strcmp((LPCSTR)pImportName->Name, "memset") == 0)
                    {
                        DWORD oldProtect;
                        if (VirtualProtect(&pThunk->u1.Function, sizeof(ULONGLONG), PAGE_READWRITE, &oldProtect))
                        {
                            pThunk->u1.Function = (ULONGLONG)Safe_memset;
                            VirtualProtect(&pThunk->u1.Function, sizeof(ULONGLONG), oldProtect, &oldProtect);
                        }
                    }
                }
                pThunk++;
                if (pOrigThunk)
                    pOrigThunk++;
            }
        }
        pImport++;
    }
}

void InjectContinuousIATHooks()
{
    static std::set<HMODULE> s_patchedModules;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32W me = {sizeof(me)};
        if (Module32FirstW(hSnap, &me))
        {
            do
            {
                if (s_patchedModules.find(me.hModule) == s_patchedModules.end())
                {
                    PatchIAT_All_AcpHal(me.hModule);
                    s_patchedModules.insert(me.hModule);
                }
            } while (Module32NextW(hSnap, &me));
        }
        CloseHandle(hSnap);
    }
}

namespace wd_quick_stubs
{
    class BaseInspectable : public ::IInspectable
    {
      protected:
        long m_ref = 1;
        std::wstring m_className;

      public:
        BaseInspectable(const wchar_t *className) : m_className(className)
        {
        }
        virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppv) noexcept override
        {
            if (!ppv)
                return E_POINTER;
            if (riid.Data1 == 0x00000000 || riid.Data1 == 0xAF86E2E0 || riid.Data1 == 0x94ea2b94)
            {
                *ppv = static_cast<::IInspectable *>(this);
                AddRef();
                return S_OK;
            }
            *ppv = nullptr;
            return E_NOINTERFACE;
        }
        virtual ULONG __stdcall AddRef() noexcept override
        {
            return InterlockedIncrement(&m_ref);
        }
        virtual ULONG __stdcall Release() noexcept override
        {
            long ref = InterlockedDecrement(&m_ref);
            if (ref == 0)
                delete this;
            return ref;
        }
        virtual HRESULT __stdcall GetIids(ULONG *c, GUID **i) noexcept override
        {
            if (c)
                *c = 0;
            if (i)
                *i = nullptr;
            return S_OK;
        }
        virtual HRESULT __stdcall GetRuntimeClassName(HSTRING *s) noexcept override
        {
            return CreateHString(m_className.c_str(), s);
        }
        virtual HRESULT __stdcall GetTrustLevel(TrustLevel *t) noexcept override
        {
            if (t)
                *t = BaseTrust;
            return S_OK;
        }
    };

    class UniversalMockObject : public ::IInspectable
    {
        long m_ref = 1;
        std::wstring m_name;

      public:
        UniversalMockObject(std::wstring name = L"") : m_name(name)
        {
        }
        virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppv) noexcept override
        {
            if (!ppv)
                return E_POINTER;
            if (riid.Data1 == 0x00000003 || riid.Data1 == 0x00000019 || riid.Data1 == 0x00000038)
            {
                *ppv = nullptr;
                return E_NOINTERFACE;
            }
            *ppv = this;
            AddRef();
            return S_OK;
        }
        virtual ULONG __stdcall AddRef() noexcept override
        {
            return InterlockedIncrement(&m_ref);
        }
        virtual ULONG __stdcall Release() noexcept override
        {
            return 1;
        }
        virtual HRESULT __stdcall GetIids(ULONG *c, GUID **i) noexcept override
        {
            if (c)
                *c = 0;
            return S_OK;
        }
        virtual HRESULT __stdcall GetRuntimeClassName(HSTRING *s) noexcept override
        {
            return CreateHString(m_name.c_str(), s);
        }
        virtual HRESULT __stdcall GetTrustLevel(TrustLevel *t) noexcept override
        {
            if (t)
                *t = BaseTrust;
            return S_OK;
        }

#define MOCK_PAD(idx)                                                                                                  \
    virtual HRESULT __stdcall Pad##idx(void *a = nullptr, void *b = nullptr, void *c = nullptr, void *d = nullptr,     \
                                       void **e = nullptr, void **f = nullptr) noexcept                                \
    {                                                                                                                  \
        if (f && IsSafeOutPtr((void **)f))                                                                             \
        {                                                                                                              \
            *reinterpret_cast<void **>(f) = new UniversalMockObject();                                                 \
        }                                                                                                              \
        else if (e && IsSafeOutPtr((void **)e))                                                                        \
        {                                                                                                              \
            *reinterpret_cast<void **>(e) = new UniversalMockObject();                                                 \
        }                                                                                                              \
        else if (d && IsSafeOutPtr((void **)d))                                                                        \
        {                                                                                                              \
            *reinterpret_cast<void **>(d) = new UniversalMockObject();                                                 \
        }                                                                                                              \
        else if (c && IsSafeOutPtr((void **)c))                                                                        \
        {                                                                                                              \
            *reinterpret_cast<void **>(c) = new UniversalMockObject();                                                 \
        }                                                                                                              \
        else if (b && IsSafeOutPtr((void **)b))                                                                        \
        {                                                                                                              \
            *reinterpret_cast<void **>(b) = new UniversalMockObject();                                                 \
        }                                                                                                              \
        else if (a && IsSafeOutPtr((void **)a))                                                                        \
        {                                                                                                              \
            *reinterpret_cast<void **>(a) = new UniversalMockObject();                                                 \
        }                                                                                                              \
        return S_OK;                                                                                                   \
    }
        MOCK_PAD(06)
        MOCK_PAD(07) MOCK_PAD(08) MOCK_PAD(09) MOCK_PAD(10) MOCK_PAD(11) MOCK_PAD(12) MOCK_PAD(13) MOCK_PAD(14)
            MOCK_PAD(15) MOCK_PAD(16) MOCK_PAD(17) MOCK_PAD(18) MOCK_PAD(19) MOCK_PAD(20) MOCK_PAD(21) MOCK_PAD(22)
                MOCK_PAD(23) MOCK_PAD(24) MOCK_PAD(25) MOCK_PAD(26) MOCK_PAD(27) MOCK_PAD(28) MOCK_PAD(29) MOCK_PAD(30)
                    MOCK_PAD(31) MOCK_PAD(32)
    };

#define MARSHAL_SAFE_QI                                                                                                \
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppv) noexcept override                                \
    {                                                                                                                  \
        if (!ppv)                                                                                                      \
            return E_POINTER;                                                                                          \
        if (riid.Data1 == 0x00000003 || riid.Data1 == 0x00000019 || riid.Data1 == 0x00000038)                          \
        {                                                                                                              \
            *ppv = nullptr;                                                                                            \
            return E_NOINTERFACE;                                                                                      \
        }                                                                                                              \
        *ppv = this;                                                                                                   \
        AddRef();                                                                                                      \
        return S_OK;                                                                                                   \
    }

    #define SAFE_NULL_PAD(idx)                                                                                             \
    virtual HRESULT __stdcall Pad##idx(UINT64 a = 0, UINT64 b = 0, UINT64 c = 0, UINT64 d = 0) noexcept                \
    {                                                                                                                  \
        return S_OK;                                                                                                   \
    }

    class DummyControllerList : public BaseInspectable
    {
        //...
      public:
        DummyControllerList()
            : BaseInspectable(L"Windows.Foundation.Collections.IVectorView`1<Windows.Xbox.Input.Controller>")
        {
        }
        MARSHAL_SAFE_QI
        virtual ULONG __stdcall AddRef() noexcept override
        {
            return 2;
        }
        virtual ULONG __stdcall Release() noexcept override
        {
            return 1;
        }
        virtual HRESULT __stdcall GetAt(unsigned int index, void **item) noexcept
        {
            if (index == 0 && item)
            {
                try
                {
                    auto gamepads = winrt::Windows::Xbox::Input::implementation::Gamepad::Gamepads();
                    if (gamepads.Size() > 0)
                    {
                        auto gamepad = gamepads.GetAt(0);

                        // [CRITICAL COM ABI FIX]
                        // We MUST explicitly cast the Gamepad to an IController before detaching it.
                        // This guarantees the VTable layout perfectly matches what Forza expects!
                        auto controller = gamepad.as<winrt::Windows::Xbox::Input::IController>();

                        *item = winrt::detach_abi(controller);
                        return S_OK;
                    }
                }
                catch (...)
                {
                    // Catch errors safely to prevent abort()
                }
            }
            return E_BOUNDS;
        }
        virtual HRESULT __stdcall get_Size(unsigned int *size) noexcept
        {
            if (size)
                *size = 1;
            return S_OK;
        }
        virtual HRESULT __stdcall IndexOf(void *item, unsigned int *index, boolean *found) noexcept
        {
            if (index)
                *index = 0;
            if (found)
                *found = 1;
            return S_OK;
        }
        virtual HRESULT __stdcall GetMany(unsigned int startIndex, unsigned int capacity, void **items,
                                          unsigned int *actual) noexcept
        {
            return E_NOTIMPL;
        }
        SAFE_NULL_PAD(10) SAFE_NULL_PAD(11) SAFE_NULL_PAD(12) SAFE_NULL_PAD(13)
    };

    class DummyControllerStatics : public BaseInspectable
    {
      public:
        DummyControllerStatics() : BaseInspectable(L"Windows.Xbox.Input.Controller")
        {
        }
        MARSHAL_SAFE_QI
        virtual HRESULT __stdcall get_Controllers(void **list) noexcept
        {
            if (list)
                *list = new DummyControllerList();
            return S_OK;
        }
        virtual HRESULT __stdcall add_ControllerAdded(void *handler, UINT64 *token) noexcept
        {
            if (token)
                *token = 1;
            return S_OK;
        }
        virtual HRESULT __stdcall remove_ControllerAdded(UINT64 token) noexcept
        {
            return S_OK;
        }
        virtual HRESULT __stdcall add_ControllerRemoved(void *handler, UINT64 *token) noexcept
        {
            if (token)
                *token = 2;
            return S_OK;
        }
        virtual HRESULT __stdcall remove_ControllerRemoved(UINT64 token) noexcept
        {
            return S_OK;
        }
        virtual HRESULT __stdcall add_ControllerPairingChanged(void *handler, UINT64 *token) noexcept
        {
            if (token)
                *token = 3;
            return S_OK;
        }
        virtual HRESULT __stdcall remove_ControllerPairingChanged(UINT64 token) noexcept
        {
            return S_OK;
        }
        SAFE_NULL_PAD(13) SAFE_NULL_PAD(14) SAFE_NULL_PAD(15) SAFE_NULL_PAD(16)
    };

    class DummyControllerFactory : public ::IActivationFactory
    {
        long m_ref = 1;
        DummyControllerStatics *m_statics;

      public:
        DummyControllerFactory()
        {
            m_statics = new DummyControllerStatics();
        }
        virtual ~DummyControllerFactory()
        {
            m_statics->Release();
        }
        virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppv) noexcept override
        {
            if (!ppv)
                return E_POINTER;

            // Reject standard COM marshalling IIDs so we don't break the C-Runtime
            if (riid.Data1 == 0x00000003 || riid.Data1 == 0x00000019 || riid.Data1 == 0x00000038)
            {
                *ppv = nullptr;
                return E_NOINTERFACE;
            }

            // Accept standard IUnknown, IInspectable, and IActivationFactory
            if (riid.Data1 == 0x00000035 || riid.Data1 == 0x00000000 || riid.Data1 == 0xAF86E2E0 ||
                riid.Data1 == 0x94ea2b94)
            {
                *ppv = static_cast<::IActivationFactory *>(this);
                AddRef();
                return S_OK;
            }

            // [CRITICAL FIX] The Invincible Sandbox:
            // Instead of guessing the exact IID (like 0x7400624f), we assume ANY other
            // requested interface from the game engine is IControllerStatics!
            *ppv = m_statics;
            m_statics->AddRef();
            return S_OK;
        }
        virtual ULONG __stdcall AddRef() noexcept override
        {
            return InterlockedIncrement(&m_ref);
        }
        virtual ULONG __stdcall Release() noexcept override
        {
            long ref = InterlockedDecrement(&m_ref);
            if (ref == 0)
                delete this;
            return ref;
        }
        virtual HRESULT __stdcall GetIids(ULONG *c, GUID **i) noexcept override
        {
            return S_OK;
        }
        virtual HRESULT __stdcall GetRuntimeClassName(HSTRING *s) noexcept override
        {
            return CreateHString(L"Windows.Xbox.Input.Controller", s);
        }
        virtual HRESULT __stdcall GetTrustLevel(TrustLevel *t) noexcept override
        {
            return S_OK;
        }
        virtual HRESULT __stdcall ActivateInstance(::IInspectable **instance) noexcept override
        {
            return E_NOTIMPL;
        }
    };

    class DummyConnectionProfile : public BaseInspectable
    {
      public:
        DummyConnectionProfile() : BaseInspectable(L"Windows.Networking.Connectivity.ConnectionProfile")
        {
        }
        MARSHAL_SAFE_QI
        virtual ULONG __stdcall AddRef() noexcept override
        {
            return 2;
        }
        virtual ULONG __stdcall Release() noexcept override
        {
            return 1;
        }
        virtual HRESULT __stdcall get_ProfileName(HSTRING *value) noexcept
        {
            return CreateHString(L"WinDurangoWiFi", value);
        }
        virtual HRESULT __stdcall GetNetworkConnectivityLevel(uint32_t *value) noexcept
        {
            if (value)
                *value = 1;
            return S_OK;
        }
        SAFE_NULL_PAD(8)
        SAFE_NULL_PAD(9) SAFE_NULL_PAD(10) SAFE_NULL_PAD(11) SAFE_NULL_PAD(12) SAFE_NULL_PAD(13) SAFE_NULL_PAD(14)
    };

    class DummyNetworkInformationStatics : public BaseInspectable
    {
      public:
        DummyNetworkInformationStatics() : BaseInspectable(L"Windows.Networking.Connectivity.NetworkInformation")
        {
        }
        virtual HRESULT __stdcall GetConnectionProfiles(void **value) noexcept
        {
            if (value && IsSafeOutPtr(value))
                *value = nullptr;
            return S_OK;
        }
        virtual HRESULT __stdcall GetInternetConnectionProfile(void **value) noexcept
        {
            if (value && IsSafeOutPtr(value))
            {
                *value = new DummyConnectionProfile();
                return S_OK;
            }
            return S_OK;
        }
        virtual HRESULT __stdcall GetLanIdentifiers(void **value) noexcept
        {
            if (value && IsSafeOutPtr(value))
                *value = nullptr;
            return S_OK;
        }
        virtual HRESULT __stdcall GetHostNames(void **value) noexcept
        {
            if (value && IsSafeOutPtr(value))
                *value = nullptr;
            return S_OK;
        }
        virtual HRESULT __stdcall GetProxyConfigurationAsync(void *uri, void **value) noexcept
        {
            if (value)
                *value = nullptr;
            return S_OK;
        }
        virtual HRESULT __stdcall GetSortedEndpointPairs(void *des, int opts, void **value) noexcept
        {
            if (value)
                *value = nullptr;
            return S_OK;
        }
        virtual HRESULT __stdcall add_NetworkStatusChanged(void *handler, void *token) noexcept
        {
            if (token && IsSafeOutPtr((void **)token))
                *(UINT64 *)token = 1;
            return S_OK;
        }
        virtual HRESULT __stdcall remove_NetworkStatusChanged(UINT64 token) noexcept
        {
            return S_OK;
        }
        SAFE_NULL_PAD(14) SAFE_NULL_PAD(15) SAFE_NULL_PAD(16) SAFE_NULL_PAD(17)
    };

    class DummyNetworkInformationFactory : public ::IActivationFactory
    {
        long m_ref = 1;
        DummyNetworkInformationStatics *m_statics;

      public:
        DummyNetworkInformationFactory()
        {
            m_statics = new DummyNetworkInformationStatics();
        }
        virtual ~DummyNetworkInformationFactory()
        {
            m_statics->Release();
        }
        virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppv) noexcept override
        {
            if (!ppv)
                return E_POINTER;
            if (riid.Data1 == 0x00000035 || riid.Data1 == 0x00000000 || riid.Data1 == 0xAF86E2E0 ||
                riid.Data1 == 0x94ea2b94)
            {
                *ppv = static_cast<::IActivationFactory *>(this);
                AddRef();
                return S_OK;
            }
            *ppv = m_statics;
            m_statics->AddRef();
            return S_OK;
        }
        virtual ULONG __stdcall AddRef() noexcept override
        {
            return InterlockedIncrement(&m_ref);
        }
        virtual ULONG __stdcall Release() noexcept override
        {
            long ref = InterlockedDecrement(&m_ref);
            if (ref == 0)
                delete this;
            return ref;
        }
        virtual HRESULT __stdcall GetIids(ULONG *c, GUID **i) noexcept override
        {
            return S_OK;
        }
        virtual HRESULT __stdcall GetRuntimeClassName(HSTRING *s) noexcept override
        {
            return CreateHString(L"Windows.Networking.Connectivity.NetworkInformation", s);
        }
        virtual HRESULT __stdcall GetTrustLevel(TrustLevel *t) noexcept override
        {
            return S_OK;
        }
        virtual HRESULT __stdcall ActivateInstance(::IInspectable **instance) noexcept override
        {
            if (instance)
            {
                *instance = new UniversalMockObject(L"Windows.Networking.Connectivity.NetworkInformation");
                return S_OK;
            }
            return E_POINTER;
        }
    };

    class UniversalMockFactory : public ::IActivationFactory
    {
        long m_ref = 1;
        std::wstring m_name;

      public:
        UniversalMockFactory(const wchar_t *name) : m_name(name)
        {
        }
        virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppv) noexcept override
        {
            if (!ppv)
                return E_POINTER;
            *ppv = this;
            AddRef();
            return S_OK;
        }
        virtual ULONG __stdcall AddRef() noexcept override
        {
            return InterlockedIncrement(&m_ref);
        }
        virtual ULONG __stdcall Release() noexcept override
        {
            long ref = InterlockedDecrement(&m_ref);
            if (ref == 0)
                delete this;
            return ref;
        }
        virtual HRESULT __stdcall GetIids(ULONG *c, GUID **i) noexcept override
        {
            return S_OK;
        }
        virtual HRESULT __stdcall GetRuntimeClassName(HSTRING *s) noexcept override
        {
            return CreateHString(m_name.c_str(), s);
        }
        virtual HRESULT __stdcall GetTrustLevel(TrustLevel *t) noexcept override
        {
            if (t)
                *t = BaseTrust;
            return S_OK;
        }
        virtual HRESULT __stdcall ActivateInstance(::IInspectable **instance) noexcept override
        {
            if (instance && IsSafeOutPtr((void **)instance))
            {
                *instance = new UniversalMockObject(m_name);
                return S_OK;
            }
            return E_POINTER;
        }
    };
} // namespace wd_quick_stubs

extern "C" HRESULT WINAPI DllGetActivationFactory(HSTRING classID, void **factory)
{
    InjectContinuousIATHooks();
    if (!factory)
        return E_POINTER;
    *factory = nullptr;

    winrt::hstring req_hstr;
    winrt::copy_from_abi(req_hstr, classID);
    std::wstring req(req_hstr.c_str());
    OutputDebugStringW(
        (L"[WinDurango] Requesting Class (Len " + std::to_wstring(req.length()) + L"): " + req + L"\n").c_str());

    if (req.find(L"Windows.Xbox.UI.SystemUI") != std::wstring::npos)
    {
        *factory = WinDurango::Mocks::UI::CreateSystemUIFactory();
        return S_OK;
    }
    if (req.find(L"Windows.Xbox.Input.Controller") != std::wstring::npos)
    {
        *factory = reinterpret_cast<void *>(new wd_quick_stubs::DummyControllerFactory());
        return S_OK;
    }

    HRESULT hr = WINRT_GetActivationFactory(classID, factory);
    if (SUCCEEDED(hr) && *factory != nullptr)
    {
        OutputDebugStringW((L"[WinDurango] Match: Native GDK Factory for " + req + L"\n").c_str());
        return S_OK;
    }

    if (req.find(L"Windows.Networking.Connectivity.NetworkInformation") != std::wstring::npos)
    {
        *factory = reinterpret_cast<void *>(new wd_quick_stubs::DummyNetworkInformationFactory());
        return S_OK;
    }
    if (req.find(L"Windows.ApplicationModel.Package") != std::wstring::npos)
    {
        *factory = GetApplicationPackageFactory();
        return S_OK;
    }

    // [CRITICAL FIX] The Sandbox Restriction Rule
    // Only fallback to UniversalMock for Xbox-specific classes!
    // Standard Windows classes (Storage, Foundation, etc.) MUST pass through to the real OS.
    if (req.find(L"Windows.Xbox.") != std::wstring::npos || req.find(L"Microsoft.Xbox.") != std::wstring::npos)
    {
        OutputDebugStringW((L"[WinDurango] Falling back to UniversalMockFactory for " + req + L"\n").c_str());
        *factory = reinterpret_cast<void *>(new wd_quick_stubs::UniversalMockFactory(req.c_str()));
        return S_OK;
    }

    // Return CLASS_E_CLASSNOTAVAILABLE. This tells kernelx.dll / OS to handle it natively!
    OutputDebugStringW((L"[WinDurango] Passing through to Host OS for " + req + L"\n").c_str());
    return 0x80040111; // CLASS_E_CLASSNOTAVAILABLE
}

extern "C" HRESULT WINAPI ActivationFactory(HSTRING classID, void **factory)
{
    return DllGetActivationFactory(classID, factory);
}