#include "Windows.Xbox.Management.Deployment.PackageTransferWatcher.h"
#include "WinDurangoWinRT.h"
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::Windows::Xbox::Management::Deployment::implementation
{
    uint32_t ProgressChangedEventArgs::PercentComplete()
    {
        return percent;
    }
    void TransferStatusChangedEventArgs::UnknownOne()
    {
    }
    void TransferStatusChangedEventArgs::UnknownTwo()
    {
    }

    winrt::Windows::Xbox::Management::Deployment::PackageTransferWatcher PackageTransferWatcher::Create(
        winrt::Windows::ApplicationModel::Package const &unk,
        winrt::Windows::Foundation::Collections::IIterable<uint32_t> const &unka)
    {
        return winrt::make<PackageTransferWatcher>();
    }

    winrt::Windows::Xbox::Management::Deployment::PackageTransferWatcher PackageTransferWatcher::Create(
        winrt::Windows::ApplicationModel::Package const &unk)
    {
        return winrt::make<PackageTransferWatcher>();
    }

    winrt::Windows::Xbox::Management::Deployment::PackageTransferStatus PackageTransferWatcher::TransferStatus()
    {
        return {};
    }
    winrt::Windows::Xbox::Management::Deployment::PackageTransferType PackageTransferWatcher::TransferType()
    {
        return static_cast<winrt::Windows::Xbox::Management::Deployment::PackageTransferType>(0);
    }

    // CRITICAL FIX: Safe Meyer's Singletons for the event delegates
    static winrt::event<winrt::Windows::Foundation::TypedEventHandler<
        winrt::Windows::Xbox::Management::Deployment::PackageTransferWatcher,
        winrt::Windows::Xbox::Management::Deployment::TransferStatusChangedEventArgs>> &
    GetTransferStatusChangedEvent()
    {
        static winrt::event<winrt::Windows::Foundation::TypedEventHandler<
            winrt::Windows::Xbox::Management::Deployment::PackageTransferWatcher,
            winrt::Windows::Xbox::Management::Deployment::TransferStatusChangedEventArgs>>
            e;
        return e;
    }
    static winrt::event<winrt::Windows::Foundation::TypedEventHandler<
        winrt::Windows::Xbox::Management::Deployment::PackageTransferWatcher,
        winrt::Windows::Xbox::Management::Deployment::ChunkCompletedEventArgs>> &
    GetChunkCompletedEvent()
    {
        static winrt::event<winrt::Windows::Foundation::TypedEventHandler<
            winrt::Windows::Xbox::Management::Deployment::PackageTransferWatcher,
            winrt::Windows::Xbox::Management::Deployment::ChunkCompletedEventArgs>>
            e;
        return e;
    }
    static winrt::event<winrt::Windows::Foundation::TypedEventHandler<
        winrt::Windows::Xbox::Management::Deployment::PackageTransferWatcher,
        winrt::Windows::Xbox::Management::Deployment::ProgressChangedEventArgs>> &
    GetProgressChangedEvent()
    {
        static winrt::event<winrt::Windows::Foundation::TypedEventHandler<
            winrt::Windows::Xbox::Management::Deployment::PackageTransferWatcher,
            winrt::Windows::Xbox::Management::Deployment::ProgressChangedEventArgs>>
            e;
        return e;
    }
    static winrt::event<winrt::Windows::Foundation::TypedEventHandler<
        winrt::Windows::Xbox::Management::Deployment::PackageTransferWatcher,
        winrt::Windows::Xbox::Management::Deployment::TransferCompletedEventArgs>> &
    GetTransferCompletedEvent()
    {
        static winrt::event<winrt::Windows::Foundation::TypedEventHandler<
            winrt::Windows::Xbox::Management::Deployment::PackageTransferWatcher,
            winrt::Windows::Xbox::Management::Deployment::TransferCompletedEventArgs>>
            e;
        return e;
    }

    winrt::event_token PackageTransferWatcher::TransferStatusChanged(
        winrt::Windows::Foundation::TypedEventHandler<
            winrt::Windows::Xbox::Management::Deployment::PackageTransferWatcher,
            winrt::Windows::Xbox::Management::Deployment::TransferStatusChangedEventArgs> const &handler)
    {
        return GetTransferStatusChangedEvent().add(handler);
    }
    void PackageTransferWatcher::TransferStatusChanged(winrt::event_token const &token) noexcept
    {
        GetTransferStatusChangedEvent().remove(token);
    }

    winrt::Windows::Foundation::Collections::IVectorView<uint32_t> PackageTransferWatcher::ChunkIds()
    {
        static auto ids = winrt::single_threaded_vector<uint32_t>();
        return ids.GetView();
    }

    winrt::event_token PackageTransferWatcher::ChunkCompleted(
        winrt::Windows::Foundation::TypedEventHandler<
            winrt::Windows::Xbox::Management::Deployment::PackageTransferWatcher,
            winrt::Windows::Xbox::Management::Deployment::ChunkCompletedEventArgs> const &handler)
    {
        return GetChunkCompletedEvent().add(handler);
    }
    void PackageTransferWatcher::ChunkCompleted(winrt::event_token const &token) noexcept
    {
        GetChunkCompletedEvent().remove(token);
    }

    winrt::event_token PackageTransferWatcher::ProgressChanged(
        winrt::Windows::Foundation::TypedEventHandler<
            winrt::Windows::Xbox::Management::Deployment::PackageTransferWatcher,
            winrt::Windows::Xbox::Management::Deployment::ProgressChangedEventArgs> const &handler)
    {
        return GetProgressChangedEvent().add(handler);
    }
    void PackageTransferWatcher::ProgressChanged(winrt::event_token const &token) noexcept
    {
        GetProgressChangedEvent().remove(token);
    }

    winrt::event_token PackageTransferWatcher::TransferCompleted(
        winrt::Windows::Foundation::TypedEventHandler<
            winrt::Windows::Xbox::Management::Deployment::PackageTransferWatcher,
            winrt::Windows::Xbox::Management::Deployment::TransferCompletedEventArgs> const &handler)
    {
        return GetTransferCompletedEvent().add(handler);
    }
    void PackageTransferWatcher::TransferCompleted(winrt::event_token const &token) noexcept
    {
        GetTransferCompletedEvent().remove(token);
    }
} // namespace winrt::Windows::Xbox::Management::Deployment::implementation

void *GetPackageTransferWatcherFactory()
{
    return reinterpret_cast<void *>(winrt::detach_abi(
        winrt::make<winrt::Windows::Xbox::Management::Deployment::factory_implementation::PackageTransferWatcher>()));
}

extern "C" void *CreateDummyPackageTransferWatcher()
{
    return winrt::detach_abi(
        winrt::make<winrt::Windows::Xbox::Management::Deployment::implementation::PackageTransferWatcher>());
}