#pragma once
#include "Windows.Xbox.System.GetTokenAndSignatureResult.g.h"
#include <winrt/Windows.Foundation.h>

namespace winrt::Windows::Xbox::System::implementation
{
    struct GetTokenAndSignatureResult : GetTokenAndSignatureResultT<GetTokenAndSignatureResult>
    {
        GetTokenAndSignatureResult() = default;

        hstring Token();
        hstring Signature();
    };
} // namespace winrt::Windows::Xbox::System::implementation
// The factory_implementation block has been intentionally removed.
// Result structs do not have activation factories.