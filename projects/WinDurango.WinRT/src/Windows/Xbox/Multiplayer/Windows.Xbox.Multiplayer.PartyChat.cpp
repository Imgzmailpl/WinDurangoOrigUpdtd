#include "Windows.Xbox.Multiplayer.PartyChat.h"
#include "WinDurangoWinRT.h"

namespace winrt::Windows::Xbox::Multiplayer::implementation
{
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::Multiplayer::PartyChatView> PartyChat::GetPartyChatViewAsync()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: GetPartyChatViewAsync");
        throw hresult_not_implemented();
    }
    winrt::event_token PartyChat::PartyChatViewChanged(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler)
    {
        return e_PartyChatViewChanged.add(handler);
    }
    void PartyChat::PartyChatViewChanged(winrt::event_token const& token) noexcept
    {
        return e_PartyChatViewChanged.remove(token);
    }
    bool PartyChat::IsPartyChatActive()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: IsPartyChatActive");
        throw hresult_not_implemented();
    }
    bool PartyChat::IsPartyChatSuppressed()
    {
        return isPartyChatSuppressed;
    }
    void PartyChat::IsPartyChatSuppressed(bool value)
    {
        isPartyChatSuppressed = value;
    }
    winrt::event_token PartyChat::IsPartyChatActiveChanged(winrt::Windows::Foundation::EventHandler<bool> const& handler)
    {
        return e_IsPartyChatActiveChanged.add(handler);
    }
    void PartyChat::IsPartyChatActiveChanged(winrt::event_token const& token) noexcept
    {
        e_IsPartyChatActiveChanged.remove(token);
    }
    winrt::event_token PartyChat::IsPartyChatSuppressedChanged(winrt::Windows::Foundation::EventHandler<bool> const& handler)
    {
        return e_IsPartyChatSuppressedChanged.add(handler);
    }
    void PartyChat::IsPartyChatSuppressedChanged(winrt::event_token const& token) noexcept
    {
        e_IsPartyChatSuppressedChanged.remove(token);
    }
}
