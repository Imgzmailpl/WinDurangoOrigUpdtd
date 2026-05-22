#include "Windows.Xbox.ApplicationModel.Store.Product.h"
#include "WinDurangoWinRT.h"
#include <sstream>
#include <windows.h>

namespace winrt::Windows::Xbox::ApplicationModel::Store::implementation
{
    hstring ProductPurchasedEventArgs::Receipt()
    {
        OutputDebugStringW(L"[WinDurango TRACE] ProductPurchasedEventArgs::Receipt() called\n");
        return L"1";
    }
    winrt::Windows::Foundation::IAsyncAction Product::ShowPurchaseAsync(winrt::Windows::Xbox::System::User unk,
                                                                        hstring unka)
    {
        OutputDebugStringW(L"[WinDurango TRACE] Product::ShowPurchaseAsync() called\n");
        co_return;
    }
    winrt::Windows::Foundation::IAsyncAction Product::ShowDetailsAsync(winrt::Windows::Xbox::System::User unk,
                                                                       hstring unka)
    {
        OutputDebugStringW(L"[WinDurango TRACE] Product::ShowDetailsAsync() called\n");
        co_return;
    }
    winrt::Windows::Foundation::IAsyncAction Product::ShowMarketplaceAsync(
        winrt::Windows::Xbox::System::User unk, winrt::Windows::Xbox::ApplicationModel::Store::ProductItemTypes unka,
        hstring unkaa, winrt::Windows::Xbox::ApplicationModel::Store::ProductItemTypes unkaaa)
    {
        OutputDebugStringW(L"[WinDurango TRACE] Product::ShowMarketplaceAsync() called\n");
        co_return;
    }
    winrt::Windows::Foundation::IAsyncAction Product::ShowRedeemCodeAsync(winrt::Windows::Xbox::System::User unk,
                                                                          hstring unka)
    {
        OutputDebugStringW(L"[WinDurango TRACE] Product::ShowRedeemCodeAsync() called\n");
        co_return;
    }
    winrt::Windows::Foundation::IAsyncAction Product::ShowSubscriptionAsync(winrt::Windows::Xbox::System::User unk,
                                                                            hstring unka, uint32_t unkaa)
    {
        OutputDebugStringW(L"[WinDurango TRACE] Product::ShowSubscriptionAsync() called\n");
        co_return;
    }
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::ApplicationModel::Store::PrivilegeCheckResult>
    Product::CheckPrivilegeAsync(winrt::Windows::Xbox::System::User unk, uint32_t unka, bool unkaa, hstring unkaaa)
    {
        std::wstringstream wss;
        wss << L"[WinDurango TRACE] Product::CheckPrivilegeAsync(PrivilegeId: " << unka
            << L") called. Returning NoIssue\n";
        OutputDebugStringW(wss.str().c_str());
        co_return PrivilegeCheckResult::NoIssue;
    }
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::ApplicationModel::Store::PrivilegeCheckResult>
    Product::CheckPrivilegesAsync(winrt::Windows::Xbox::System::User unk,
                                  winrt::Windows::Foundation::Collections::IVectorView<uint32_t> unka, bool unkaa,
                                  hstring unkaaa)
    {
        OutputDebugStringW(L"[WinDurango TRACE] Product::CheckPrivilegesAsync() called. Returning NoIssue\n");
        co_return PrivilegeCheckResult::NoIssue;
    }
    winrt::event_token Product::ProductPurchased(
        winrt::Windows::Xbox::ApplicationModel::Store::ProductPurchasedEventHandler const &handler)
    {
        OutputDebugStringW(L"[WinDurango TRACE] Product::ProductPurchased() registered\n");
        return {};
    }
    void Product::ProductPurchased(winrt::event_token const &token) noexcept
    {
        OutputDebugStringW(L"[WinDurango TRACE] Product::ProductPurchased() unregistered\n");
    }
    winrt::Windows::Foundation::IAsyncAction Product::ShowPurchaseForStoreIdAsync(
        winrt::Windows::Xbox::System::User unk, hstring unka)
    {
        OutputDebugStringW(L"[WinDurango TRACE] Product::ShowPurchaseForStoreIdAsync() called\n");
        co_return;
    }
    winrt::Windows::Foundation::IAsyncAction Product::ShowDetailsForStoreIdAsync(winrt::Windows::Xbox::System::User unk,
                                                                                 hstring unka)
    {
        OutputDebugStringW(L"[WinDurango TRACE] Product::ShowDetailsForStoreIdAsync() called\n");
        co_return;
    }

    winrt::event<winrt::Windows::Xbox::ApplicationModel::Store::ProductPurchasedEventHandler>
        Product::e_ProductPurchased{};
} // namespace winrt::Windows::Xbox::ApplicationModel::Store::implementation