#pragma once
#include <windows.h>

namespace WinDurango::Mocks::UI
{
    // Exports the mock SystemUI factory to the WinDurangoWinRT switchboard
    void *CreateSystemUIFactory();
} // namespace WinDurango::Mocks::UI