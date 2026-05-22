#include "Windows.Xbox.Storage.ConnectedStorageSpace.h"
#include "Windows.Xbox.Storage.ConnectedStorageContainer.h"
#include "Windows.Xbox.Storage.ContainerInfoQueryResult.h"
#include "Windows.Xbox.System.User.h"

namespace winrt::Windows::Xbox::Storage::implementation
{
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::Storage::ConnectedStorageSpace>
    ConnectedStorageSpace::GetForUserAsync(winrt::Windows::Xbox::System::User user)
    {
        // AUTO-INITIALIZE USER STORAGE PATHS ON THE FLY
        if (wd::WinRT::g_UserStorage == nullptr)
        {
            auto storage = new wd::WinRT::ConnectedStorage();
            co_await storage->InitializeStorage(L"User");
            wd::WinRT::g_UserStorage = storage;
        }

        if (userStorageSpace == Storage::ConnectedStorageSpace(nullptr))
        {
            userStorageSpace = winrt::make<implementation::ConnectedStorageSpace>(wd::WinRT::g_UserStorage);
        }

        co_return userStorageSpace;
    }
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::Storage::ConnectedStorageSpace>
    ConnectedStorageSpace::GetForUserAsync(winrt::Windows::Xbox::System::User user, hstring serviceConfigurationId)
    {
        // AUTO-INITIALIZE USER STORAGE PATHS ON THE FLY
        if (wd::WinRT::g_UserStorage == nullptr)
        {
            auto storage = new wd::WinRT::ConnectedStorage();
            co_await storage->InitializeStorage(L"User");
            wd::WinRT::g_UserStorage = storage;
        }

        if (userStorageSpace == Storage::ConnectedStorageSpace(nullptr))
        {
            userStorageSpace = winrt::make<implementation::ConnectedStorageSpace>(wd::WinRT::g_UserStorage);
        }

        co_return userStorageSpace;
    }
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::Storage::ConnectedStorageSpace>
    ConnectedStorageSpace::GetForMachineAsync()
    {
        // AUTO-INITIALIZE MACHINE STORAGE PATHS ON THE FLY
        if (wd::WinRT::g_MachineStorage == nullptr)
        {
            auto storage = new wd::WinRT::ConnectedStorage();
            co_await storage->InitializeStorage(L"Machine");
            wd::WinRT::g_MachineStorage = storage;
        }

        if (machineStorageSpace == Storage::ConnectedStorageSpace(nullptr))
        {
            machineStorageSpace = winrt::make<implementation::ConnectedStorageSpace>(wd::WinRT::g_MachineStorage);
        }

        co_return machineStorageSpace;
    }
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::Storage::ConnectedStorageSpace>
    ConnectedStorageSpace::GetForMachineAsync(hstring serviceConfigurationId)
    {
        // AUTO-INITIALIZE MACHINE STORAGE PATHS ON THE FLY
        if (wd::WinRT::g_MachineStorage == nullptr)
        {
            auto storage = new wd::WinRT::ConnectedStorage();
            co_await storage->InitializeStorage(L"Machine");
            wd::WinRT::g_MachineStorage = storage;
        }

        if (machineStorageSpace == Storage::ConnectedStorageSpace(nullptr))
        {
            machineStorageSpace = winrt::make<implementation::ConnectedStorageSpace>(wd::WinRT::g_MachineStorage);
        }

        co_return machineStorageSpace;
    }
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::Storage::ConnectedStorageSpace>
    ConnectedStorageSpace::GetSyncOnDemandForUserAsync(winrt::Windows::Xbox::System::User user)
    {
        // AUTO-INITIALIZE USER STORAGE PATHS ON THE FLY
        if (wd::WinRT::g_UserStorage == nullptr)
        {
            auto storage = new wd::WinRT::ConnectedStorage();
            co_await storage->InitializeStorage(L"User");
            wd::WinRT::g_UserStorage = storage;
        }

        if (userStorageSpace == Storage::ConnectedStorageSpace(nullptr))
        {
            userStorageSpace = winrt::make<implementation::ConnectedStorageSpace>(wd::WinRT::g_UserStorage);
        }

        co_return userStorageSpace;
    }
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::Storage::ConnectedStorageSpace>
    ConnectedStorageSpace::GetSyncOnDemandForUserAsync(winrt::Windows::Xbox::System::User user,
                                                       hstring serviceConfigurationId)
    {
        // AUTO-INITIALIZE USER STORAGE PATHS ON THE FLY
        if (wd::WinRT::g_UserStorage == nullptr)
        {
            auto storage = new wd::WinRT::ConnectedStorage();
            co_await storage->InitializeStorage(L"User");
            wd::WinRT::g_UserStorage = storage;
        }

        if (userStorageSpace == Storage::ConnectedStorageSpace(nullptr))
        {
            userStorageSpace = winrt::make<implementation::ConnectedStorageSpace>(wd::WinRT::g_UserStorage);
        }

        co_return userStorageSpace;
    }
    winrt::Windows::Xbox::System::User ConnectedStorageSpace::User()
    {
        return winrt::Windows::Xbox::System::User::Users().GetAt(0);
    }
    hstring ConnectedStorageSpace::ServiceConfigurationId()
    {
        // FIX: Match the active GDK store configuration exactly with the retail SCID [B2.2.1]
        return L"00000000-0000-0000-0000-00007900C3C7";
    }
    bool ConnectedStorageSpace::IsReadOnly()
    {
        return false;
    }
    winrt::Windows::Xbox::Storage::ConnectedStorageContainer ConnectedStorageSpace::CreateContainer(
        hstring const &containerName)
    {
        if (wd::WinRT::g_UserStorage == nullptr || wd::WinRT::g_MachineStorage == nullptr)
            assert("g_UserStorage is null");

        if (!containers)
        {
            containers =
                winrt::single_threaded_map<hstring, winrt::Windows::Xbox::Storage::ConnectedStorageContainer>();
        }

        if (!containers.HasKey(containerName))
        {
            winrt::Windows::Xbox::Storage::ConnectedStorageContainer container =
                winrt::make<ConnectedStorageContainer>(containerName, m_connectedStorage);

            containers.Insert(containerName, container);
            return container;
        }

        return containers.Lookup(containerName);
    }
    winrt::Windows::Foundation::IAsyncAction ConnectedStorageSpace::DeleteContainerAsync(hstring containerName)
    {
        co_await m_connectedStorage->DeleteContainer(containerName);
        containers.Remove(containerName);
    }
    winrt::Windows::Xbox::Storage::ContainerInfoQueryResult ConnectedStorageSpace::CreateContainerInfoQuery(
        hstring const &unk)
    {
        return winrt::make<winrt::Windows::Xbox::Storage::implementation::ContainerInfoQueryResult>(unk,
                                                                                                    m_connectedStorage);
    }
    winrt::Windows::Foundation::IAsyncOperation<int32_t> ConnectedStorageSpace::GetRemainingBytesInQuotaAsync()
    {
        co_return 0xFFFFFFFFFFFFFFFF;
    }
    winrt::Windows::Foundation::IAsyncOperation<int64_t> ConnectedStorageSpace::GetRemainingBytesInQuota64Async()
    {
        co_return 0xFFFFFFFFFFFFFFFF;
    }
} // namespace winrt::Windows::Xbox::Storage::implementation