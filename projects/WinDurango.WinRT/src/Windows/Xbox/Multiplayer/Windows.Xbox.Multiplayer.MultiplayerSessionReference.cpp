#include "Windows.Xbox.Multiplayer.MultiplayerSessionReference.h"
#include "WinDurangoWinRT.h"

namespace winrt::Windows::Xbox::Multiplayer::implementation
{
    winrt::Windows::Xbox::Multiplayer::MultiplayerSessionReference MultiplayerSessionReferenceFactory::CreateMultiplayerSessionReference(hstring const& unk, hstring const& unka, hstring const& unkb)
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: CreateMultiplayerSessionReference");
        throw hresult_not_implemented();
    }
    hstring MultiplayerSessionReference::SessionName()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: SessionName");
        throw hresult_not_implemented();
    }
    hstring MultiplayerSessionReference::ServiceConfigurationId()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: ServiceConfigurationId");
        throw hresult_not_implemented();
    }
    hstring MultiplayerSessionReference::SessionTemplateName()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Multiplayer", "Unimplemented: SessionTemplateName");
        throw hresult_not_implemented();
    }
}
