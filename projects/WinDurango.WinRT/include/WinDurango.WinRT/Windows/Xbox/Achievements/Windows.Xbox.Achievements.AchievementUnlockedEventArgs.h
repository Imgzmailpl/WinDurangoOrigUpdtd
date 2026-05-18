#pragma once
#include "Windows.Xbox.Achievements.AchievementUnlockedEventArgs.g.h"

namespace winrt::Windows::Xbox::Achievements::implementation
{
    struct AchievementUnlockedEventArgs : AchievementUnlockedEventArgsT<AchievementUnlockedEventArgs>
    {
        AchievementUnlockedEventArgs() = default;

        winrt::Windows::System::User User();
        hstring TitleId();
        hstring ServiceConfigurationId();
        hstring AchievementId();
        winrt::Windows::Foundation::DateTime UnlockTime();
        winrt::Windows::Foundation::Uri ExtendedInfoUrl();
        winrt::Windows::Foundation::Collections::IVectorView<hstring> InAppRewardValue();
    };
}
