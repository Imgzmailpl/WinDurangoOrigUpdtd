#include "Windows.Xbox.Multiplayer.PartyView.h"
#include "WinDurangoWinRT.h"

namespace winrt::Windows::Xbox::Multiplayer::implementation
{
    hstring PartyMember::XboxUserId()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: XboxUserId");
        throw hresult_not_implemented();
    }
    bool PartyMember::IsLocal()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: IsLocal");
        throw hresult_not_implemented();
    }
    winrt::Windows::Foundation::DateTime PartyMember::JoinTime()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: JoinTime");
        throw hresult_not_implemented();
    }
    winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Multiplayer::PartyMember> PartyMemberDeviceGroup::Members()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: Members");
        throw hresult_not_implemented();
    }
    winrt::Windows::Xbox::Multiplayer::SuppressGameSessionReadyToastMode PartyView::SuppressGameSessionReadyToastMode()
    {
        return suppressGameSessionReadyToast;
    }
    void PartyView::SuppressGameSessionReadyToastMode(winrt::Windows::Xbox::Multiplayer::SuppressGameSessionReadyToastMode const& value)
    {
        suppressGameSessionReadyToast = value;
    }
    winrt::Windows::Foundation::Collections::IVectorView<hstring> PartyView::ReservedMembers()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: ReservedMembers");
        throw hresult_not_implemented();
    }
    winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Multiplayer::PartyMember> PartyView::Members()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: Members");
        throw hresult_not_implemented();
    }
    winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Multiplayer::PartyMemberDeviceGroup> PartyView::MembersGroupedByDevice()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: MembersGroupedByDevice");
        throw hresult_not_implemented();
    }
    winrt::Windows::Xbox::Multiplayer::MultiplayerSessionReference PartyView::MatchSession()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: MatchSession");
        throw hresult_not_implemented();
    }
    winrt::Windows::Xbox::Multiplayer::MultiplayerSessionReference PartyView::GameSession()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: GameSession");
        throw hresult_not_implemented();
    }
    bool PartyView::IsPartyInAnotherTitle()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: IsPartyInAnotherTitle");
        throw hresult_not_implemented();
    }
    winrt::Windows::Xbox::Multiplayer::SuppressGameSessionReadyToastMode PartyView::suppressGameSessionReadyToast{};
}
