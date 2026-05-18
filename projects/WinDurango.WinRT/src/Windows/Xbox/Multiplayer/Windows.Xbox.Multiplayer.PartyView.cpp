#include "Windows.Xbox.Multiplayer.PartyView.h"
#include "Windows.Xbox.Multiplayer.MultiplayerSessionReference.h"
#include <winrt/Windows.Foundation.Collections.h>
#include "WinDurangoWinRT.h"

namespace winrt::Windows::Xbox::Multiplayer::implementation
{
    hstring PartyMember::XboxUserId()
    {
        return L"1";
    }
    bool PartyMember::IsLocal()
    {
        return true;
    }
    winrt::Windows::Foundation::DateTime PartyMember::JoinTime()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: JoinTime");
        throw hresult_not_implemented();
    }
    winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Multiplayer::PartyMember> PartyMemberDeviceGroup::Members()
    {
        return winrt::single_threaded_vector<winrt::Windows::Xbox::Multiplayer::PartyMember>().GetView();
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
        return winrt::single_threaded_vector<winrt::Windows::Xbox::Multiplayer::PartyMember>().GetView();
    }
    winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Multiplayer::PartyMemberDeviceGroup> PartyView::MembersGroupedByDevice()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: MembersGroupedByDevice");
        throw hresult_not_implemented();
    }
    winrt::Windows::Xbox::Multiplayer::MultiplayerSessionReference PartyView::MatchSession()
    {
        return winrt::make<implementation::MultiplayerSessionReference>();
    }
    winrt::Windows::Xbox::Multiplayer::MultiplayerSessionReference PartyView::GameSession()
    {
        return winrt::make<implementation::MultiplayerSessionReference>();
    }
    bool PartyView::IsPartyInAnotherTitle()
    {
        return false;
    }
    winrt::Windows::Xbox::Multiplayer::SuppressGameSessionReadyToastMode PartyView::suppressGameSessionReadyToast{};
}
