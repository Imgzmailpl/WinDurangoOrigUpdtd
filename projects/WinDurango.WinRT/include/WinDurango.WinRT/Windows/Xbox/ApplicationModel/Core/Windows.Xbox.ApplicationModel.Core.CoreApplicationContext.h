#pragma once
#include "Windows.Xbox.ApplicationModel.Core.CoreApplicationContext.g.h"
#include <winrt/Windows.Foundation.h>

namespace winrt::Windows::Xbox::ApplicationModel::Core::implementation
{
    struct CoreApplicationContext : CoreApplicationContextT<CoreApplicationContext>
    {
        CoreApplicationContext() = default;

        void SetCurrentUser(winrt::Windows::Xbox::System::User const& value);
        winrt::Windows::Xbox::System::User CurrentSystemUser();
        winrt::event_token CurrentSystemUserChanged(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler);
        void CurrentSystemUserChanged(winrt::event_token const& token) noexcept;
        winrt::Windows::Xbox::System::User CurrentUser();
        winrt::Windows::Xbox::System::User PreviousUser();
        winrt::event_token CurrentUserChanged(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler);
        void CurrentUserChanged(winrt::event_token const& token) noexcept;
    private:
        winrt::event<winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable>> e_CurrentUserChanged;
    };
}
namespace winrt::Windows::Xbox::ApplicationModel::Core::factory_implementation
{
    struct CoreApplicationContext : CoreApplicationContextT<CoreApplicationContext, implementation::CoreApplicationContext>
    {
    };
}
