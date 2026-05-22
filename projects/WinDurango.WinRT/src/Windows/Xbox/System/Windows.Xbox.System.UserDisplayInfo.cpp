#include "Windows.Xbox.System.UserDisplayInfo.h"
#include "WinDurangoWinRT.h"
#include "Windows.Xbox.System.h" // CRITICAL FIX: Include the header that defines GetPictureResult!
#include <windows.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>

namespace winrt::Windows::Xbox::System::implementation
{
    winrt::Windows::Xbox::System::User UserDisplayInfoChangedEventArgs::User()
    {
        OutputDebugStringW(L"[WinDurango TRACE] UserDisplayInfoChangedEventArgs::User() called\n");
        return user;
    }

    hstring UserDisplayInfo::Gamertag()
    {
        OutputDebugStringW(L"[WinDurango TRACE] UserDisplayInfo::Gamertag() called\n");
        return L"WinDurango Player"; // Safely allocated static string
    }

    uint32_t UserDisplayInfo::GamerScore()
    {
        OutputDebugStringW(L"[WinDurango TRACE] UserDisplayInfo::GamerScore() called\n");
        return 125000;
    }

    hstring UserDisplayInfo::ApplicationDisplayName()
    {
        OutputDebugStringW(L"[WinDurango TRACE] UserDisplayInfo::ApplicationDisplayName() called\n");
        return L"WinDurango Player"; // Safely allocated static string
    }

    hstring UserDisplayInfo::GameDisplayName()
    {
        OutputDebugStringW(L"[WinDurango TRACE] UserDisplayInfo::GameDisplayName() called\n");
        return L"WinDurango Player"; // Safely allocated static string
    }

    uint32_t UserDisplayInfo::Reputation()
    {
        OutputDebugStringW(L"[WinDurango TRACE] UserDisplayInfo::Reputation() called\n");
        return 100;
    }

    winrt::Windows::Xbox::System::UserAgeGroup UserDisplayInfo::AgeGroup()
    {
        OutputDebugStringW(L"[WinDurango TRACE] UserDisplayInfo::AgeGroup() called\n");
        return winrt::Windows::Xbox::System::UserAgeGroup::Adult;
    }

    winrt::Windows::Foundation::Collections::IVectorView<uint32_t> UserDisplayInfo::Privileges()
    {
        OutputDebugStringW(L"[WinDurango TRACE] UserDisplayInfo::Privileges() called.\n");
        static auto g_privileges = []() {
            auto vec = winrt::single_threaded_vector<uint32_t>();
            for (uint32_t i = 1; i <= 255; ++i)
            {
                vec.Append(i);
            }
            return vec;
        }();
        return g_privileges.GetView();
    }

    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::System::GetPictureResult> UserDisplayInfo::
        GetApplicationPictureAsync(winrt::Windows::Xbox::System::UserPictureSize unk,
                                   winrt::Windows::Storage::Streams::IBuffer unka)
    {
        OutputDebugStringW(L"[WinDurango TRACE] UserDisplayInfo::GetApplicationPictureAsync() called\n");
        co_return winrt::make<winrt::Windows::Xbox::System::implementation::GetPictureResult>(0, 0);
    }

    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::System::GetPictureResult> UserDisplayInfo::
        GetGamerPictureAsync(winrt::Windows::Xbox::System::UserPictureSize unk,
                             winrt::Windows::Storage::Streams::IBuffer unka)
    {
        OutputDebugStringW(L"[WinDurango TRACE] UserDisplayInfo::GetGamerPictureAsync() called\n");
        co_return winrt::make<winrt::Windows::Xbox::System::implementation::GetPictureResult>(0, 0);
    }
} // namespace winrt::Windows::Xbox::System::implementation