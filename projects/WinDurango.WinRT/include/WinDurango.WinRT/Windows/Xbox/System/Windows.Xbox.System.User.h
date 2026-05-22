#pragma once
#include "Windows.Xbox.System.User.g.h"
#include "Windows.Xbox.System.UserAddedEventArgs.g.h"
#include "Windows.Xbox.System.UserDisplayInfo.h"
#include "Windows.Xbox.System.UserRemovedEventArgs.g.h"
#include "Windows.Xbox.System.h"

namespace winrt::Windows::Xbox::System::implementation
{
    struct UserAddedEventArgs : UserAddedEventArgsT<UserAddedEventArgs>
    {
        UserAddedEventArgs() = default;
        UserAddedEventArgs(winrt::Windows::Xbox::System::User user) : m_user(user)
        {
        }
        winrt::Windows::Xbox::System::User User();

      private:
        winrt::Windows::Xbox::System::User m_user;
    };

    struct UserRemovedEventArgs : UserRemovedEventArgsT<UserRemovedEventArgs>
    {
        UserRemovedEventArgs() = default;
        UserRemovedEventArgs(winrt::Windows::Xbox::System::User user) : m_user(user)
        {
        }
        winrt::Windows::Xbox::System::User User();

      private:
        winrt::Windows::Xbox::System::User m_user;
    };

    struct User : UserT<User>
    {
        User() = default;
        User(uint32_t id) : id(id)
        {
        }

        static winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::System::User> Users();
        static winrt::Windows::Xbox::System::User GetUserById(uint32_t Id);
        static winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::System::GetTokenAndSignatureResult>
        GetTokenAndSignatureForAllUsersAsync(hstring method, hstring url, hstring headers);
        static winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::System::GetTokenAndSignatureResult>
        GetTokenAndSignatureForAllUsersAsync(hstring method, hstring url, hstring headers,
                                             winrt::Windows::Foundation::Collections::IVector<uint8_t> const &body);
        static winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::System::GetTokenAndSignatureResult>
        GetTokenAndSignatureForAllUsersAsync(hstring method, hstring url, hstring headers, hstring body);
        static winrt::guid GetNetworkCacheIdForUsers(
            winrt::Windows::Foundation::Collections::IVectorView<uint32_t> const &unk);
        static winrt::Windows::Xbox::System::UserOnlineState OnlineState();

        static winrt::event_token OnlineStateChanged(
            winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::OnlineStateChangedEventArgs> const
                &handler);
        static void OnlineStateChanged(winrt::event_token const &token) noexcept;
        static winrt::event_token UserAdded(
            winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::UserAddedEventArgs> const &handler);
        static void UserAdded(winrt::event_token const &token) noexcept;
        static winrt::event_token UserRemoved(
            winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::UserRemovedEventArgs> const
                &handler);
        static void UserRemoved(winrt::event_token const &token) noexcept;
        static winrt::event_token AudioDeviceAdded(
            winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::AudioDeviceAddedEventArgs> const
                &handler);
        static void AudioDeviceAdded(winrt::event_token const &token) noexcept;
        static winrt::event_token AudioDeviceRemoved(
            winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::AudioDeviceRemovedEventArgs> const
                &handler);
        static void AudioDeviceRemoved(winrt::event_token const &token) noexcept;
        static winrt::event_token AudioDeviceChanged(
            winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::AudioDeviceChangedEventArgs> const
                &handler);
        static void AudioDeviceChanged(winrt::event_token const &token) noexcept;
        static winrt::event_token SignInCompleted(
            winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::SignInCompletedEventArgs> const
                &handler);
        static void SignInCompleted(winrt::event_token const &token) noexcept;
        static winrt::event_token SignOutStarted(
            winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::SignOutStartedEventArgs> const
                &handler);
        static void SignOutStarted(winrt::event_token const &token) noexcept;
        static winrt::event_token SignOutCompleted(
            winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::System::SignOutCompletedEventArgs> const
                &handler);
        static void SignOutCompleted(winrt::event_token const &token) noexcept;
        static winrt::event_token UserDisplayInfoChanged(
            winrt::Windows::Foundation::EventHandler<
                winrt::Windows::Xbox::System::UserDisplayInfoChangedEventArgs> const &handler);
        static void UserDisplayInfoChanged(winrt::event_token const &token) noexcept;

        uint32_t Id();
        winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::System::AudioDeviceInfo>
        AudioDevices();
        winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Input::IController> Controllers();
        winrt::Windows::Xbox::System::UserDisplayInfo DisplayInfo();
        bool IsGuest();
        bool IsSignedIn();
        winrt::Windows::Xbox::System::UserLocation Location();
        winrt::Windows::Xbox::System::User Sponsor();
        hstring XboxUserHash();
        hstring XboxUserId();

        winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::System::GetTokenAndSignatureResult>
        GetTokenAndSignatureAsync(hstring method, hstring url, hstring headers);
        winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::System::GetTokenAndSignatureResult>
        GetTokenAndSignatureAsync(hstring method, hstring url, hstring headers, uint32_t __bodySize, uint8_t body);
        winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::System::GetTokenAndSignatureResult>
        GetTokenAndSignatureAsync(hstring method, hstring url, hstring headers, hstring body);

      private:
        uint32_t id = 0;
    };
} // namespace winrt::Windows::Xbox::System::implementation

namespace winrt::Windows::Xbox::System::factory_implementation
{
    struct User : UserT<User, implementation::User>
    {
    };
} // namespace winrt::Windows::Xbox::System::factory_implementation