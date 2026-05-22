#include "Windows.Xbox.ApplicationModel.Store.Configuration.h"
#include "WinDurangoWinRT.h"

namespace winrt::Windows::Xbox::ApplicationModel::Store::implementation
{
    hstring Configuration::MarketplaceId()
    {
        // SAFEGUARD: Return a valid GDK dummy Sandbox/Marketplace GUID string [B1]
        // This prevents the task-aborting exception and lets the licensing chain finish successfully! [B1]
        return L"00000000-0000-0000-0000-000000000000";
    }
} // namespace winrt::Windows::Xbox::ApplicationModel::Store::implementation