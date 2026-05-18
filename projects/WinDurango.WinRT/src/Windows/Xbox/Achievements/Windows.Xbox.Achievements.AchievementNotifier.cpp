#include "Windows.Xbox.Achievements.AchievementNotifier.h"
#include "Windows.Xbox.Achievements.AchievementSource.h"

namespace winrt::Windows::Xbox::Achievements::implementation
{
    winrt::Windows::Xbox::Achievements::AchievementSource AchievementNotifier::GetTitleIdFilteredSource()
    {
        return winrt::make<implementation::AchievementSource>();
    }
    winrt::Windows::Xbox::Achievements::AchievementSource AchievementNotifier::GetUnfilteredSource()
    {
        return winrt::make<implementation::AchievementSource>();
    }
}
