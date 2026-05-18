#pragma once
#include "Windows.Xbox.Achievements.AchievementNotifier.g.h"

namespace winrt::Windows::Xbox::Achievements::implementation
{
    struct AchievementNotifier
    {
        AchievementNotifier() = default;

        static winrt::Windows::Xbox::Achievements::AchievementSource GetTitleIdFilteredSource();
        static winrt::Windows::Xbox::Achievements::AchievementSource GetUnfilteredSource();
    };
}

namespace winrt::Windows::Xbox::Achievements::factory_implementation
{
    struct AchievementNotifier : AchievementNotifierT<AchievementNotifier, implementation::AchievementNotifier>
    {
    };
}
