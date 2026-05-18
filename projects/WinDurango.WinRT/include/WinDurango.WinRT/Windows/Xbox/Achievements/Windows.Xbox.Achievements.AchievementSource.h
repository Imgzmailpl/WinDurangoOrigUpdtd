#pragma once
#include "Windows.Xbox.Achievements.AchievementSource.g.h"

namespace winrt::Windows::Xbox::Achievements::implementation
{
    struct AchievementSource : AchievementSourceT<AchievementSource>
    {
        AchievementSource() = default;

        winrt::event_token AchievementUnlocked(winrt::Windows::Xbox::Achievements::AchievementUnlockedHandler const& handler);
        void AchievementUnlocked(winrt::event_token const& token) noexcept;
    };
}
