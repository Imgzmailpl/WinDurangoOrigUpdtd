#include "Windows.Xbox.Input.NavigationController.h"
#include "WinDurangoWinRT.h"
namespace winrt::Windows::Xbox::Input::implementation
{
    uint64_t NavigationController::Id()
    {
        return id;
    }

    hstring NavigationController::Type()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Input", "Unimplemented: Type");
        throw hresult_not_implemented();
    }

    winrt::Windows::Xbox::System::User NavigationController::User()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Input", "Unimplemented: User");
        throw hresult_not_implemented();
    }

    winrt::Windows::Xbox::Input::INavigationReading NavigationController::GetNavigationReading()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Input", "Unimplemented: GetNavigationReading");
        throw hresult_not_implemented();
    }

    winrt::Windows::Xbox::Input::RawNavigationReading NavigationController::GetRawNavigationReading()
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::Input", "Unimplemented: GetRawNavigationReading");
        throw hresult_not_implemented();
    }
}
