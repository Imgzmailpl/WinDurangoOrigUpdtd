#include "Windows.Xbox.System.User.h"
#include <windows.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Xbox.Input.h>
#include <Windows/Xbox/Input/Windows.Xbox.Input.Gamepad.h>
namespace winrt::Windows::Xbox::System::implementation
{
    winrt::Windows::Xbox::System::User UserAddedEventArgs::User()
    {
        return m_user;
    }
    winrt::Windows::Xbox::System::User UserRemovedEventArgs::User()
    {
        return m_user;
    }
    winrt::Windows::Xbox::System::UserOnlineState User::OnlineState()
    {
        return winrt::Windows::Xbox::System::UserOnlineState::Online;
    }

    winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::System::User> User::Users()
    {
        static auto g_users = []() {
            auto vec = winrt::single_threaded_vector<winrt::Windows::Xbox::System::User>();
            vec.Append(winrt::make<User>(1));
            return vec;
        }();
        return g_users.GetView();
    }

    winrt::Windows::Xbox::System::User User::GetUserById(uint32_t Id)
    {
        return Users().GetAt(0);
    }

    winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Input::IController> User::Controllers()
    {
        auto vec = winrt::single_threaded_vector<winrt::Windows::Xbox::Input::IController>();
        auto gamepads = winrt::Windows::Xbox::Input::implementation::Gamepad::Gamepads();

        if (gamepads.Size() > 0)
        {
            // FIX: Explicitly cast to IController to prevent VTable misalignment
            vec.Append(gamepads.GetAt(0).as<winrt::Windows::Xbox::Input::IController>());
        }

        return vec.GetView();
    }

    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::System::GetTokenAndSignatureResult> User::
        GetTokenAndSignatureForAllUsersAsync(hstring method, hstring url, hstring headers)
    {
        co_return winrt::make<winrt::Windows::Xbox::System::implementation::GetTokenAndSignatureResult>();
    }
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::System::GetTokenAndSignatureResult> User::
        GetTokenAndSignatureForAllUsersAsync(hstring method, hstring url, hstring headers,
                                             winrt::Windows::Foundation::Collections::IVector<uint8_t> const &body)
    {
        co_return winrt::make<winrt::Windows::Xbox::System::implementation::GetTokenAndSignatureResult>();
    }
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::System::GetTokenAndSignatureResult> User::
        GetTokenAndSignatureForAllUsersAsync(hstring method, hstring url, hstring headers, hstring body)
    {
        co_return winrt::make<winrt::Windows::Xbox::System::implementation::GetTokenAndSignatureResult>();
    }

    winrt::guid User::GetNetworkCacheIdForUsers(
        winrt::Windows::Foundation::Collections::IVectorView<uint32_t> const &unk)
    {
        return {};
    }
    winrt::event_token User::OnlineStateChanged(
        winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::OnlineStateChangedEventArgs> const
            &handler)
    {
        return {};
    }
    void User::OnlineStateChanged(winrt::event_token const &token) noexcept
    {
    }

    winrt::event_token User::UserAdded(
        winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::UserAddedEventArgs> const &handler)
    {
        // FIX: Do not fire synchronously with uninitialized args. Let Forza read User::Users() natively.
        return {};
    }

    void User::UserAdded(winrt::event_token const &token) noexcept
    {
    }
    winrt::event_token User::UserRemoved(
        winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::UserRemovedEventArgs> const &handler)
    {
        return {};
    }
    void User::UserRemoved(winrt::event_token const &token) noexcept
    {
    }
    winrt::event_token User::AudioDeviceAdded(
        winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::AudioDeviceAddedEventArgs> const
            &handler)
    {
        return {};
    }
    void User::AudioDeviceAdded(winrt::event_token const &token) noexcept
    {
    }
    winrt::event_token User::AudioDeviceRemoved(
        winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::AudioDeviceRemovedEventArgs> const
            &handler)
    {
        return {};
    }
    void User::AudioDeviceRemoved(winrt::event_token const &token) noexcept
    {
    }
    winrt::event_token User::AudioDeviceChanged(
        winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::AudioDeviceChangedEventArgs> const
            &handler)
    {
        return {};
    }
    void User::AudioDeviceChanged(winrt::event_token const &token) noexcept
    {
    }

    winrt::event_token User::SignInCompleted(
        winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::SignInCompletedEventArgs> const &handler)
    {
        // FIX: Silence this to prevent sending nullptr payloads to Forza's event listener.
        return {};
    }
    void User::SignInCompleted(winrt::event_token const &token) noexcept
    {
    }
    winrt::event_token User::SignOutStarted(
        winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::SignOutStartedEventArgs> const &handler)
    {
        return {};
    }
    void User::SignOutStarted(winrt::event_token const &token) noexcept
    {
    }
    winrt::event_token User::SignOutCompleted(
        winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::SignOutCompletedEventArgs> const
            &handler)
    {
        return {};
    }
    void User::SignOutCompleted(winrt::event_token const &token) noexcept
    {
    }
    winrt::event_token User::UserDisplayInfoChanged(
        winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::UserDisplayInfoChangedEventArgs> const
            &handler)
    {
        return {};
    }
    void User::UserDisplayInfoChanged(winrt::event_token const &token) noexcept
    {
    }

    uint32_t User::Id()
    {
        return id;
    }
    winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::System::AudioDeviceInfo> User::
        AudioDevices()
    {
        return winrt::single_threaded_vector<winrt::Windows::Xbox::System::AudioDeviceInfo>().GetView();
    }
    winrt::Windows::Xbox::System::UserDisplayInfo User::DisplayInfo()
    {
        return winrt::make<winrt::Windows::Xbox::System::implementation::UserDisplayInfo>(to_hstring(id));
    }
    bool User::IsGuest()
    {
        return false;
    }
    bool User::IsSignedIn()
    {
        return true;
    }
    winrt::Windows::Xbox::System::UserLocation User::Location()
    {
        return winrt::Windows::Xbox::System::UserLocation::Local;
    }
    winrt::Windows::Xbox::System::User User::Sponsor()
    {
        return nullptr;
    }
    hstring User::XboxUserHash()
    {
        return L"12345678";
    }
    hstring User::XboxUserId()
    {
        return L"2533274791381234";
    }
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::System::GetTokenAndSignatureResult> User::
        GetTokenAndSignatureAsync(hstring method, hstring url, hstring headers)
    {
        co_return winrt::make<GetTokenAndSignatureResult>();
    }
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::System::GetTokenAndSignatureResult> User::
        GetTokenAndSignatureAsync(hstring method, hstring url, hstring headers, uint32_t __bodySize, uint8_t body)
    {
        co_return winrt::make<GetTokenAndSignatureResult>();
    }
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::System::GetTokenAndSignatureResult> User::
        GetTokenAndSignatureAsync(hstring method, hstring url, hstring headers, hstring body)
    {
        co_return winrt::make<GetTokenAndSignatureResult>();
    }
} // namespace winrt::Windows::Xbox::System::implementation