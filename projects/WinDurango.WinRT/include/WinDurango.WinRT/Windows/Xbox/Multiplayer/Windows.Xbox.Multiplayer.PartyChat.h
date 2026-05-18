#pragma once
#include <winrt/Windows.Foundation.h>
#include "Windows.Xbox.Multiplayer.PartyChat.g.h"

namespace winrt::Windows::Xbox::Multiplayer::implementation
{
    struct PartyChat : PartyChatT<PartyChat>
    {
        PartyChat() = default;

        winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::Multiplayer::PartyChatView> GetPartyChatViewAsync();
        winrt::event_token PartyChatViewChanged(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler);
        void PartyChatViewChanged(winrt::event_token const& token) noexcept;
        bool IsPartyChatActive();
        bool IsPartyChatSuppressed();
        void IsPartyChatSuppressed(bool value);
        winrt::event_token IsPartyChatActiveChanged(winrt::Windows::Foundation::EventHandler<bool> const& handler);
        void IsPartyChatActiveChanged(winrt::event_token const& token) noexcept;
        winrt::event_token IsPartyChatSuppressedChanged(winrt::Windows::Foundation::EventHandler<bool> const& handler);
        void IsPartyChatSuppressedChanged(winrt::event_token const& token) noexcept;
    private:
        winrt::event<winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable>> e_PartyChatViewChanged;
        winrt::event<winrt::Windows::Foundation::EventHandler<bool>> e_IsPartyChatActiveChanged;
        winrt::event<winrt::Windows::Foundation::EventHandler<bool>> e_IsPartyChatSuppressedChanged;
        bool isPartyChatSuppressed = false;
    };
}
namespace winrt::Windows::Xbox::Multiplayer::factory_implementation
{
    struct PartyChat : PartyChatT<PartyChat, implementation::PartyChat>
    {
    };
}
