#include "Windows.Xbox.Achievements.AchievementSource.h"

namespace winrt::Windows::Xbox::Achievements::implementation
{
    winrt::event_token AchievementSource::AchievementUnlocked(winrt::Windows::Xbox::Achievements::AchievementUnlockedHandler const& handler)
    {
        return {};
    }
    void AchievementSource::AchievementUnlocked(winrt::event_token const& token) noexcept
    {

    }
}
