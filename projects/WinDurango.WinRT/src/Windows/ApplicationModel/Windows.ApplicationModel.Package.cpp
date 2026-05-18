#include "WinDurango.WinRT/Windows/ApplicationModel/Windows.ApplicationModel.Package.h"

// 1. WRL Headers (Bypasses C++/WinRT compiler limitations)
#include <windows.applicationmodel.h>
#include <windows.storage.h>
#include <wrl.h>
#include <wrl/implements.h>

// 2. C++/WinRT Headers (Used ONLY for the internal file path helper)
#include <windows.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.h>

using namespace Microsoft::WRL;

namespace wd_wrl_stubs
{
    // Build the raw ABI object
    class PackageStub : public RuntimeClass<RuntimeClassFlags<WinRt>, ABI::Windows::ApplicationModel::IPackage>
    {
      public:
        HRESULT STDMETHODCALLTYPE get_Id(ABI::Windows::ApplicationModel::IPackageId **value) override
        {
            *value = nullptr;
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE get_InstalledLocation(ABI::Windows::Storage::IStorageFolder **value) override
        {
            wchar_t path[MAX_PATH];
            GetModuleFileNameW(NULL, path, MAX_PATH);
            std::wstring ws(path);
            size_t pos = ws.find_last_of(L"\\/");
            std::wstring dir = (pos != std::wstring::npos) ? ws.substr(0, pos) : L".";

            // LOG THE EXACT PATH TO THE DEBUGGER
            OutputDebugStringW((L"[WinDurango] VFS Mounting Directory: " + dir + L"\n").c_str());

            try
            {
                auto folder = winrt::Windows::Storage::StorageFolder::GetFolderFromPathAsync(dir).get();
                *value = reinterpret_cast<ABI::Windows::Storage::IStorageFolder *>(winrt::detach_abi(folder));
                OutputDebugStringA("[WinDurango] VFS Mount SUCCESS!\n");
                return S_OK;
            }
            catch (winrt::hresult_error const &ex)
            {
                OutputDebugStringW((L"[WinDurango] VFS Mount FAILED: " + ex.message() + L"\n").c_str());
                *value = nullptr;
                return E_FAIL;
            }
            catch (...)
            {
                OutputDebugStringA("[WinDurango] VFS Mount FAILED: Unknown WinRT Sandbox exception.\n");
                *value = nullptr;
                return E_FAIL;
            }
        }

        HRESULT STDMETHODCALLTYPE get_IsFramework(boolean *value) override
        {
            *value = false;
            return S_OK;
        }

        // NOTE: get_IsDevelopmentMode removed because it belongs to IPackage2!

        HRESULT STDMETHODCALLTYPE
        get_Dependencies(__FIVectorView_1_Windows__CApplicationModel__CPackage **value) override
        {
            *value = nullptr;
            return S_OK;
        }
    };

    // Build the raw ABI Factory
    class PackageFactory : public RuntimeClass<RuntimeClassFlags<WinRt>, IActivationFactory,
                                               ABI::Windows::ApplicationModel::IPackageStatics>
    {
      public:
        HRESULT STDMETHODCALLTYPE ActivateInstance(IInspectable **instance) override
        {
            auto pkg = Make<PackageStub>();
            return pkg.CopyTo(instance);
        }

        HRESULT STDMETHODCALLTYPE get_Current(ABI::Windows::ApplicationModel::IPackage **value) override
        {
            static auto instance = Make<PackageStub>();
            return instance.CopyTo(value);
        }
    };
} // namespace wd_wrl_stubs

// Export function for WinDurangoWinRT.cpp
extern "C" void *GetApplicationPackageFactory()
{
    auto factory = Make<wd_wrl_stubs::PackageFactory>();
    IActivationFactory *rawFactory = nullptr;
    factory.CopyTo(&rawFactory);
    return rawFactory;
}