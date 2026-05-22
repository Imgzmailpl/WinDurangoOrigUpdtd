#include <Windows/Xbox/UI/Windows.Xbox.UI.SystemUI.h>
#include "Windows.Xbox.System.User.h" 
#include <winrt/Windows.Xbox.System.h>
#include <string>
#include <thread>
// [CRITICAL] Standard Windows SDK headers for raw COM / WinRT ABI bridging
#include <inspectable.h>
#include <activation.h>

namespace
{
    // Apex Hunter Out-Pointer Validation
    bool IsValidOutPtr(void **ptr)
    {
        if ((uintptr_t)ptr < 0x10000ULL || (uintptr_t)ptr > 0x00007FFFFFFFFFFFULL || ((uintptr_t)ptr % 8) != 0)
            return false;
        MEMORY_BASIC_INFORMATION mbi;
        if (VirtualQuery(ptr, &mbi, sizeof(mbi)) != 0)
        {
            return (mbi.State == MEM_COMMIT && (mbi.Protect & (PAGE_READWRITE | PAGE_WRITECOPY)));
        }
        return false;
    }

    inline HRESULT CreateHString(const wchar_t *str, HSTRING *out)
    {
        if (!out)
            return E_POINTER;
        winrt::hstring temp(str);
        *out = reinterpret_cast<HSTRING>(winrt::detach_abi(temp));
        return S_OK;
    }
} // namespace

namespace WinDurango::Mocks::UI
{
    class UIBaseInspectable : public ::IInspectable
    {
      protected:
        long m_ref = 1;
        std::wstring m_className;

      public:
        UIBaseInspectable(const wchar_t *className) : m_className(className)
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
            *ppv = static_cast<::IInspectable *>(this);
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
            if (c)
                *c = 0;
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

    // STEP 3: AccountPickerResult
    class MockAccountPickerResult : public UIBaseInspectable
    {
      public:
        MockAccountPickerResult() : UIBaseInspectable(L"Windows.Xbox.UI.AccountPickerResult")
        {
        }

        // VTable Index 6: AccountPickerResult::get_User()
        virtual HRESULT __stdcall M06_getUser(void *a = nullptr, void *b = nullptr, void *c = nullptr,
                                              void *d = nullptr, void *e = nullptr, void *f = nullptr) noexcept
        {
            void **targetOutPtr = nullptr;
            if (a && IsValidOutPtr((void **)a) && *reinterpret_cast<void **>(a) == nullptr)
                targetOutPtr = (void **)a;
            else if (b && IsValidOutPtr((void **)b) && *reinterpret_cast<void **>(b) == nullptr)
                targetOutPtr = (void **)b;
            else if (c && IsValidOutPtr((void **)c) && *reinterpret_cast<void **>(c) == nullptr)
                targetOutPtr = (void **)c;
            else if (d && IsValidOutPtr((void **)d) && *reinterpret_cast<void **>(d) == nullptr)
                targetOutPtr = (void **)d;
            else if (e && IsValidOutPtr((void **)e) && *reinterpret_cast<void **>(e) == nullptr)
                targetOutPtr = (void **)e;
            else if (f && IsValidOutPtr((void **)f) && *reinterpret_cast<void **>(f) == nullptr)
                targetOutPtr = (void **)f;

            if (targetOutPtr)
            {
                // Trap #1: Inject Native C++/WinRT Singleton
                auto userSingleton = winrt::Windows::Xbox::System::implementation::User::Users().GetAt(0);
                *targetOutPtr = winrt::detach_abi(userSingleton);
                return S_OK;
            }
            return 0x80004001; // E_NOTIMPL (False Success Trap)
        }
    };

    // STEP 2: AsyncOperation for the Account Picker
    class MockAsyncOperation : public UIBaseInspectable
    {
      public:
        MockAsyncOperation()
            : UIBaseInspectable(L"Windows.Foundation.IAsyncOperation`1<Windows.Xbox.UI.AccountPickerResult>")
        {
        }

        // VTable Index 6: IAsyncOperation::put_Completed()
        virtual HRESULT __stdcall M06_put_Completed(void *handler) noexcept
        {
            if (handler)
            {
                // Trap #4: Asynchronous Handler execution via Thread to avoid Double-Free
                std::thread([handler, this]() {
                    Sleep(50);
                    void **vtable = *(void ***)handler;
                    typedef HRESULT(__stdcall * InvokeFn)(void *, void *, int);
                    InvokeFn invoke = (InvokeFn)vtable[3]; // Invoke is typically at VTable 3 of the handler
                    invoke(handler, this, 1);
                }).detach();
            }
            return S_OK;
        }

        // VTable Index 7: get_Completed (Unused but required for padding)
        virtual HRESULT __stdcall M07_get_Completed(void **handler) noexcept
        {
            return S_OK;
        }

        // VTable Index 8: IAsyncOperation::GetResults()
        virtual HRESULT __stdcall M08_GetResults(void **results) noexcept
        {
            if (results && IsValidOutPtr(results))
            {
                *results = new MockAccountPickerResult();
                return S_OK;
            }
            return 0x80004001; // E_NOTIMPL
        }
    };

    // STEP 1: SystemUI Statics Factory Route
    class MockSystemUIStatics : public UIBaseInspectable
    {
      public:
        MockSystemUIStatics() : UIBaseInspectable(L"Windows.Xbox.UI.SystemUI")
        {
        }

// Macro mapping exactly to Cheatsheet indices 6, 7, 8, 9 (ShowAccountPickerAsync overloads)
#define BIND_ACCOUNT_PICKER_ASYNC(idx)                                                                                 \
    virtual HRESULT __stdcall M##idx##_ShowAccountPickerAsync(void *a = nullptr, void *b = nullptr, void *c = nullptr, \
                                                              void *d = nullptr, void *e = nullptr,                    \
                                                              void *f = nullptr) noexcept                              \
    {                                                                                                                  \
        void **out = nullptr;                                                                                          \
        if (a && IsValidOutPtr((void **)a) && *reinterpret_cast<void **>(a) == nullptr)                                \
            out = (void **)a;                                                                                          \
        else if (b && IsValidOutPtr((void **)b) && *reinterpret_cast<void **>(b) == nullptr)                           \
            out = (void **)b;                                                                                          \
        else if (c && IsValidOutPtr((void **)c) && *reinterpret_cast<void **>(c) == nullptr)                           \
            out = (void **)c;                                                                                          \
        else if (d && IsValidOutPtr((void **)d) && *reinterpret_cast<void **>(d) == nullptr)                           \
            out = (void **)d;                                                                                          \
        else if (e && IsValidOutPtr((void **)e) && *reinterpret_cast<void **>(e) == nullptr)                           \
            out = (void **)e;                                                                                          \
        else if (f && IsValidOutPtr((void **)f) && *reinterpret_cast<void **>(f) == nullptr)                           \
            out = (void **)f;                                                                                          \
        if (out)                                                                                                       \
        {                                                                                                              \
            *out = static_cast<void *>(new MockAsyncOperation());                                                      \
            return S_OK;                                                                                               \
        }                                                                                                              \
        return 0x80004001;                                                                                             \
    }

        BIND_ACCOUNT_PICKER_ASYNC(06)
        BIND_ACCOUNT_PICKER_ASYNC(07)
        BIND_ACCOUNT_PICKER_ASYNC(08)
        BIND_ACCOUNT_PICKER_ASYNC(09)
    };

    class MockSystemUIFactory : public ::IActivationFactory
    {
        long m_ref = 1;
        MockSystemUIStatics *m_statics;

      public:
        MockSystemUIFactory()
        {
            m_statics = new MockSystemUIStatics();
        }
        virtual ~MockSystemUIFactory()
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
            if (riid.Data1 == 0x1C13A5C9)
            { // ISystemUIStatics IID
                *ppv = m_statics;
                m_statics->AddRef();
                return S_OK;
            }
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
            return S_OK;
        }
        virtual HRESULT __stdcall GetRuntimeClassName(HSTRING *s) noexcept override
        {
            return CreateHString(L"Windows.Xbox.UI.SystemUI", s);
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

    void *CreateSystemUIFactory()
    {
        return new MockSystemUIFactory();
    }
} // namespace WinDurango::Mocks::UI