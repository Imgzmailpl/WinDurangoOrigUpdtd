#include "Windows.Xbox.Management.Deployment.PackageTransferManager.h"
#include "WinDurangoWinRT.h"
#include "Windows.Xbox.Management.Deployment.PackageTransferWatcher.h"
#include <winrt/Windows.Foundation.h>

extern "C" void *CreateDummyChunkSpecifiers();

namespace winrt::Windows::Xbox::Management::Deployment::implementation
{
    // One single immortal handle for the whole process
    static HANDLE GetDeadlockEvent()
    {
        static HANDLE h = CreateEvent(nullptr, true, false, nullptr);
        return h;
    }

    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::Management::Deployment::CheckForUpdateResult>
    PackageTransferManager::CheckForUpdateAsync(
        winrt::Windows::Xbox::Management::Deployment::DownloadableContentPackage unk)
    {
        co_await winrt::resume_on_signal(GetDeadlockEvent());
        co_return nullptr;
    }

    winrt::Windows::Foundation::IAsyncOperation<
        winrt::Windows::Xbox::Management::Deployment::RequestUpdatePackageResult>
    PackageTransferManager::RequestUpdatePackageAsync(
        winrt::Windows::Xbox::Management::Deployment::DownloadableContentPackage unk)
    {
        co_await winrt::resume_on_signal(GetDeadlockEvent());
        co_return nullptr;
    }

    winrt::Windows::Foundation::IAsyncOperation<
        winrt::Windows::Xbox::Management::Deployment::RequestUpdatePackageResult>
    PackageTransferManager::RequestUpdateCurrentPackageAsync()
    {
        co_await winrt::resume_on_signal(GetDeadlockEvent());
        co_return nullptr;
    }

    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::Management::Deployment::PackageTransferWatcher>
    PackageTransferManager::AddChunkSpecifiersAsync(winrt::Windows::Xbox::Management::Deployment::ChunkSpecifiers unk)
    {
        co_await winrt::resume_on_signal(GetDeadlockEvent());
        co_return nullptr;
    }

    winrt::Windows::Foundation::IAsyncAction PackageTransferManager::RemoveChunkSpecifiersAsync(
        winrt::Windows::Xbox::Management::Deployment::ChunkSpecifiers unk)
    {
        co_await winrt::resume_on_signal(GetDeadlockEvent());
        co_return;
    }

    winrt::Windows::Xbox::Management::Deployment::PackageTransferManager PackageTransferManager::Current()
    {
        static auto instance = winrt::make<PackageTransferManager>();
        return instance;
    }

    winrt::Windows::Xbox::Management::Deployment::PackageTransferManager PackageTransferManager::Create(
        winrt::Windows::ApplicationModel::Package const &package)
    {
        return PackageTransferManager::Current();
    }

    winrt::Windows::Xbox::Management::Deployment::ChunkSpecifiers PackageTransferManager::AvailableChunkSpecifiers()
    {
        winrt::Windows::Xbox::Management::Deployment::ChunkSpecifiers obj{nullptr};
        winrt::attach_abi(obj, CreateDummyChunkSpecifiers());
        return obj;
    }

    winrt::Windows::Xbox::Management::Deployment::InstallationState PackageTransferManager::GetInstallationState(
        winrt::Windows::Foundation::Collections::IIterable<uint32_t> const &unk)
    {
        return static_cast<winrt::Windows::Xbox::Management::Deployment::InstallationState>(0);
    }
    winrt::Windows::Xbox::Management::Deployment::InstallationState PackageTransferManager::GetInstallationState(
        winrt::Windows::Xbox::Management::Deployment::ChunkSpecifiers const &unk)
    {
        return static_cast<winrt::Windows::Xbox::Management::Deployment::InstallationState>(0);
    }
    winrt::Windows::Xbox::Management::Deployment::PackageTransferType PackageTransferManager::TransferType()
    {
        return static_cast<winrt::Windows::Xbox::Management::Deployment::PackageTransferType>(0);
    }
    void PackageTransferManager::UpdateInstallOrder(
        winrt::Windows::Foundation::Collections::IIterable<uint32_t> const &unk,
        winrt::Windows::Xbox::Management::Deployment::UpdateInstallOrderBehavior const &unka)
    {
    }
    bool PackageTransferManager::IsChunkInstalled(uint32_t chunkId)
    {
        return true;
    }
    bool PackageTransferManager::AreChunksInstalled(
        winrt::Windows::Foundation::Collections::IIterable<uint32_t> const &chunkIds)
    {
        return true;
    }
    uint32_t PackageTransferManager::FindChunkFromFile(hstring const &unk)
    {
        return 0;
    }
    winrt::Windows::Xbox::Management::Deployment::PackageTransferStatus PackageTransferManager::TransferStatus()
    {
        return {};
    }
} // namespace winrt::Windows::Xbox::Management::Deployment::implementation

void *GetPackageTransferManagerFactory()
{
    return reinterpret_cast<void *>(winrt::detach_abi(
        winrt::make<winrt::Windows::Xbox::Management::Deployment::factory_implementation::PackageTransferManager>()));
}