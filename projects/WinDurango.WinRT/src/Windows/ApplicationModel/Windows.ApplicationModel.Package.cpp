#include "WinDurango.WinRT/Windows/ApplicationModel/Windows.ApplicationModel.Package.h"
#include <windows.applicationmodel.h>
#include <windows.h>
#include <windows.storage.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.h>
#include <wrl.h>
#include <wrl/implements.h>

using namespace Microsoft::WRL;

namespace wd_wrl_stubs
{
    // Build a fully spoofed Package ID to stop the 0xC0000005 crash!
    class PackageIdStub : public RuntimeClass<RuntimeClassFlags<WinRt>, ABI::Windows::ApplicationModel::IPackageId>
    {
      public:
        HRESULT STDMETHODCALLTYPE get_Name(HSTRING *value) override
        {
            WindowsCreateString(L"Anthem", 6, value);
            return S_OK;
        }
        HRESULT STDMETHODCALLTYPE get_Version(ABI::Windows::ApplicationModel::PackageVersion *value) override
        {
            value->Major = 1;
            value->Minor = 0;
            value->Build = 0;
            value->Revision = 64;
            return S_OK;
        }
        HRESULT STDMETHODCALLTYPE get_Architecture(ABI::Windows::System::ProcessorArchitecture *value) override
        {
            *value = ABI::Windows::System::ProcessorArchitecture_X64;
            return S_OK;
        }
        HRESULT STDMETHODCALLTYPE get_ResourceId(HSTRING *value) override
        {
            WindowsCreateString(L"", 0, value);
            return S_OK;
        }
        HRESULT STDMETHODCALLTYPE get_Publisher(HSTRING *value) override
        {
            WindowsCreateString(L"CN=Microsoft Corporation, O=Microsoft Corporation, L=Redmond, S=Washington, C=US", 76,
                                value);
            return S_OK;
        }
        HRESULT STDMETHODCALLTYPE get_PublisherId(HSTRING *value) override
        {
            WindowsCreateString(L"8wekyb3d8bbwe", 13, value);
            return S_OK;
        }
        HRESULT STDMETHODCALLTYPE get_FullName(HSTRING *value) override
        {
            WindowsCreateString(L"Anthem_1.0.0.64_x64__8wekyb3d8bbwe", 34, value);
            return S_OK;
        }
        HRESULT STDMETHODCALLTYPE get_FamilyName(HSTRING *value) override
        {
            WindowsCreateString(L"Anthem_8wekyb3d8bbwe", 20, value);
            return S_OK;
        }
    };

    // Build the raw ABI object
    class PackageStub : public RuntimeClass<RuntimeClassFlags<WinRt>, ABI::Windows::ApplicationModel::IPackage>
    {
      public:
        HRESULT STDMETHODCALLTYPE get_Id(ABI::Windows::ApplicationModel::IPackageId **value) override
        {
            auto id = Make<PackageIdStub>();
            return id.CopyTo(value); // FIX: Return actual ID data, not nullptr!
        }

        HRESULT STDMETHODCALLTYPE get_InstalledLocation(ABI::Windows::Storage::IStorageFolder **value) override
        {
            wchar_t path[MAX_PATH];
            GetModuleFileNameW(NULL, path, MAX_PATH);
            std::wstring ws(path);
            size_t pos = ws.find_last_of(L"\\/");
            std::wstring dir = (pos != std::wstring::npos) ? ws.substr(0, pos) : L".";

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

extern "C" void *GetApplicationPackageFactory()
{
    auto factory = Make<wd_wrl_stubs::PackageFactory>();
    IActivationFactory *rawFactory = nullptr;
    factory.CopyTo(&rawFactory);
    return rawFactory;
}