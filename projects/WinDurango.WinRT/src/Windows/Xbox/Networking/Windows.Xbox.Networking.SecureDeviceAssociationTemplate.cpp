#include "Windows.Xbox.Networking.SecureDeviceAssociationTemplate.h"
#include "WinDurangoWinRT.h"
#include "Windows.Xbox.Networking.Development.h"
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::Windows::Xbox::Networking::implementation
{
    // CRITICAL FIX: Single immortal handle used by all network async calls
    // This stops the handle leak that was fragmenting memory.
    static HANDLE GetDeadlockEvent()
    {
        static HANDLE h = CreateEvent(nullptr, true, false, nullptr);
        return h;
    }

    winrt::Windows::Xbox::Networking::SecureDeviceAssociation SecureDeviceAssociationIncomingEventArgs::Association()
    {
        return nullptr;
    }

    winrt::Windows::Xbox::Networking::SecureDeviceAssociationState SecureDeviceAssociationStateChangedEventArgs::
        OldState()
    {
        return static_cast<winrt::Windows::Xbox::Networking::SecureDeviceAssociationState>(0);
    }

    winrt::Windows::Xbox::Networking::SecureDeviceAssociationState SecureDeviceAssociationStateChangedEventArgs::
        NewState()
    {
        return static_cast<winrt::Windows::Xbox::Networking::SecureDeviceAssociationState>(0);
    }

    winrt::Windows::Xbox::Networking::SecureDeviceAssociationTemplate SecureDeviceAssociationTemplate::
        GetTemplateByName(hstring const &name)
    {
        return winrt::make<SecureDeviceAssociationTemplate>();
    }

    // SAFE ARRAY INITIALIZATION
    winrt::Windows::Foundation::Collections::IVectorView<
        winrt::Windows::Xbox::Networking::SecureDeviceAssociationTemplate>
    SecureDeviceAssociationTemplate::Templates()
    {
        static auto g_templates =
            winrt::single_threaded_vector<winrt::Windows::Xbox::Networking::SecureDeviceAssociationTemplate>();
        return g_templates.GetView();
    }

    static winrt::event<winrt::Windows::Foundation::TypedEventHandler<
        winrt::Windows::Xbox::Networking::SecureDeviceAssociationTemplate,
        winrt::Windows::Xbox::Networking::SecureDeviceAssociationIncomingEventArgs>> &
    GetAssociationIncomingEvent()
    {
        static winrt::event<winrt::Windows::Foundation::TypedEventHandler<
            winrt::Windows::Xbox::Networking::SecureDeviceAssociationTemplate,
            winrt::Windows::Xbox::Networking::SecureDeviceAssociationIncomingEventArgs>>
            e;
        return e;
    }

    winrt::event_token SecureDeviceAssociationTemplate::AssociationIncoming(
        winrt::Windows::Foundation::TypedEventHandler<
            winrt::Windows::Xbox::Networking::SecureDeviceAssociationTemplate,
            winrt::Windows::Xbox::Networking::SecureDeviceAssociationIncomingEventArgs> const &handler)
    {
        return GetAssociationIncomingEvent().add(handler);
    }

    void SecureDeviceAssociationTemplate::AssociationIncoming(winrt::event_token const &token) noexcept
    {
        GetAssociationIncomingEvent().remove(token);
    }

    // ABI-COMPATIBLE INFINITE HANGS
    // We co_await the immortal event so the game assumes the network is just "slow"
    // and doesn't crash on a null pointer.
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::Networking::SecureDeviceAssociation>
    SecureDeviceAssociationTemplate::CreateAssociationAsync(
        winrt::Windows::Xbox::Networking::SecureDeviceAddress secureDeviceAddress,
        winrt::Windows::Xbox::Networking::CreateSecureDeviceAssociationBehavior behavior)
    {
        co_await winrt::resume_on_signal(GetDeadlockEvent());
        co_return nullptr;
    }

    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::Networking::SecureDeviceAssociation>
    SecureDeviceAssociationTemplate::CreateAssociationForPortsAsync(
        winrt::Windows::Xbox::Networking::SecureDeviceAddress secureDeviceAddress,
        winrt::Windows::Xbox::Networking::CreateSecureDeviceAssociationBehavior behavior, hstring initiatorPort,
        hstring acceptorPort)
    {
        co_await winrt::resume_on_signal(GetDeadlockEvent());
        co_return nullptr;
    }

    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::Streams::IBuffer>
    SecureDeviceAssociationTemplate::CreateCertificateRequestAsync(hstring subjectName)
    {
        co_await winrt::resume_on_signal(GetDeadlockEvent());
        co_return nullptr;
    }

    winrt::Windows::Foundation::IAsyncAction SecureDeviceAssociationTemplate::InstallCertificateAsync(
        winrt::Windows::Storage::Streams::IBuffer certificateBuffer)
    {
        co_await winrt::resume_on_signal(GetDeadlockEvent());
        co_return;
    }

    hstring SecureDeviceAssociationTemplate::Name()
    {
        return L"DummyTemplate";
    }

    winrt::Windows::Xbox::Networking::SecureDeviceSocketDescription SecureDeviceAssociationTemplate::
        InitiatorSocketDescription()
    {
        return winrt::make<winrt::Windows::Xbox::Networking::implementation::SecureDeviceSocketDescription>();
    }

    winrt::Windows::Xbox::Networking::SecureDeviceSocketDescription SecureDeviceAssociationTemplate::
        AcceptorSocketDescription()
    {
        return winrt::make<winrt::Windows::Xbox::Networking::implementation::SecureDeviceSocketDescription>();
    }

    winrt::Windows::Xbox::Networking::SecureDeviceAssociationUsage SecureDeviceAssociationTemplate::AllowedUsages()
    {
        return static_cast<winrt::Windows::Xbox::Networking::SecureDeviceAssociationUsage>(0);
    }

    hstring SecureDeviceAssociationTemplate::RelyingParty()
    {
        return L"DummyParty";
    }

    winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Networking::ProtocolParserDescription>
    SecureDeviceAssociationTemplate::ProtocolParserDescriptions()
    {
        static auto g_parsers =
            winrt::single_threaded_vector<winrt::Windows::Xbox::Networking::ProtocolParserDescription>();
        return g_parsers.GetView();
    }

    winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Networking::QualityOfServiceMetric>
    SecureDeviceAssociationTemplate::QualityOfServiceMetricPathPriorities()
    {
        static auto g_qos = winrt::single_threaded_vector<winrt::Windows::Xbox::Networking::QualityOfServiceMetric>();
        return g_qos.GetView();
    }

    winrt::Windows::Xbox::Networking::MultiplayerSessionRequirement SecureDeviceAssociationTemplate::
        MultiplayerSessionRequirement()
    {
        return {};
    }

    winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Networking::SecureDeviceAssociation>
    SecureDeviceAssociationTemplate::Associations()
    {
        static auto g_assocs =
            winrt::single_threaded_vector<winrt::Windows::Xbox::Networking::SecureDeviceAssociation>();
        return g_assocs.GetView();
    }
} // namespace winrt::Windows::Xbox::Networking::implementation

// CRITICAL FIX: Un-squash this so it actually exports to the linker
void *GetSecureDeviceAssociationTemplateFactory()
{
    return reinterpret_cast<void *>(winrt::detach_abi(
        winrt::make<winrt::Windows::Xbox::Networking::factory_implementation::SecureDeviceAssociationTemplate>()));
}