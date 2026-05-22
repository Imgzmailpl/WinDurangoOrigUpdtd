#include "Windows.Xbox.System.GetTokenAndSignatureResult.h"

namespace winrt::Windows::Xbox::System::implementation
{
    hstring GetTokenAndSignatureResult::Token()
    {
        // Fix: Provide exactly what the GDK offline cache requires
        return L"XBL3.0 x=12345678;dummy_token";
    }

    hstring GetTokenAndSignatureResult::Signature()
    {
        // Fix: Provide exact bypass signature
        return L"AAAAA_Mock_Signature_AAAAA";
    }
} // namespace winrt::Windows::Xbox::System::implementation