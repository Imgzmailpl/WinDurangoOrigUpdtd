#include "Windows.Xbox.Achievements.AchievementUnlockedEventArgs.h"
#include <chrono>

namespace winrt::Windows::Xbox::Achievements::implementation
{
    winrt::Windows::System::User AchievementUnlockedEventArgs::User()
    {
        throw hresult_not_implemented();
    }
    hstring AchievementUnlockedEventArgs::TitleId()
    {
        throw hresult_not_implemented();
    }
    hstring AchievementUnlockedEventArgs::ServiceConfigurationId()
    {
        throw hresult_not_implemented();
    }
    hstring AchievementUnlockedEventArgs::AchievementId()
    {
        throw hresult_not_implemented();
    }
    winrt::Windows::Foundation::DateTime AchievementUnlockedEventArgs::UnlockTime()
    {
        throw hresult_not_implemented();
    }
    winrt::Windows::Foundation::Uri AchievementUnlockedEventArgs::ExtendedInfoUrl()
    {
        throw hresult_not_implemented();
    }
    winrt::Windows::Foundation::Collections::IVectorView<hstring> AchievementUnlockedEventArgs::InAppRewardValue()
    {
        throw hresult_not_implemented();
    }
}
