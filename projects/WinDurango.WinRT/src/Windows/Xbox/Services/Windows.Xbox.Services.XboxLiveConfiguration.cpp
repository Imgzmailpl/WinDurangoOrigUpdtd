#include "Windows.Xbox.Services.XboxLiveConfiguration.h"
#include "WinDurangoWinRT.h"
#include <sstream>
#include <windows.h>

std::wstring ExtractAttribute(const std::wstring &xml, const std::wstring &attr)
{
    std::wstring key = attr + L"=\"";

    size_t start = xml.find(key);
    if (start == std::wstring::npos)
        return L"";

    start += key.length();
    size_t end = xml.find(L"\"", start);
    if (end == std::wstring::npos)
        return L"";

    return xml.substr(start, end - start);
}

std::wstring ReadFile(const std::wstring &path)
{
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring::size_type pos = std::wstring(exePath).find_last_of(L"\\/");
    std::wstring fullPath = std::wstring(exePath).substr(0, pos) + L"\\" + path;

    FILE *f = _wfopen(fullPath.c_str(), L"rb");
    if (!f)
    {
        f = _wfopen(path.c_str(), L"rb");
        if (!f)
            return L"";
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    rewind(f);

    std::string buffer(size, '\0');
    fread(buffer.data(), 1, size, f);
    fclose(f);

    int len = MultiByteToWideChar(CP_UTF8, 0, buffer.data(), (int)buffer.size(), NULL, 0);
    std::wstring wstr(len, 0);
    MultiByteToWideChar(CP_UTF8, 0, buffer.data(), (int)buffer.size(), &wstr[0], len);

    return wstr;
}

namespace winrt::Windows::Xbox::Services::implementation
{
    hstring XboxLiveConfiguration::TitleId()
    {
        auto xml = ReadFile(L"AppxManifest.xml");
        auto titleId = ExtractAttribute(xml, L"TitleId");
        if (titleId.empty() || titleId.find(L"746302C4") != std::wstring::npos ||
            titleId.find(L"746302c4") != std::wstring::npos)
        {
            OutputDebugStringW(
                L"[WinDurango TRACE] XboxLiveConfiguration::TitleId() called (Using Fallback: 1952645828)\n");
            return L"1952645828";
        }
        std::wstringstream wss;
        wss << L"[WinDurango TRACE] XboxLiveConfiguration::TitleId() called: " << titleId << L"\n";
        OutputDebugStringW(wss.str().c_str());
        return titleId.c_str();
    }
    hstring XboxLiveConfiguration::PrimaryServiceConfigId()
    {
        auto xml = ReadFile(L"AppxManifest.xml");
        auto primaryServiceConfigId = ExtractAttribute(xml, L"PrimaryServiceConfigId");
        if (primaryServiceConfigId.empty())
        {
            OutputDebugStringW(L"[WinDurango TRACE] XboxLiveConfiguration::PrimaryServiceConfigId() called (Using "
                               L"Fallback: 00000000-0000-0000-0000-00007900C3C7)\n");
            return L"00000000-0000-0000-0000-00007900C3C7";
        }
        std::wstringstream wss;
        wss << L"[WinDurango TRACE] XboxLiveConfiguration::PrimaryServiceConfigId() called: " << primaryServiceConfigId
            << L"\n";
        OutputDebugStringW(wss.str().c_str());
        return primaryServiceConfigId.c_str();
    }
    hstring XboxLiveConfiguration::SandboxId()
    {
        auto xml = ReadFile(L"AppxManifest.xml");
        auto sandboxId = ExtractAttribute(xml, L"SandboxId");
        if (sandboxId.empty())
        {
            OutputDebugStringW(
                L"[WinDurango TRACE] XboxLiveConfiguration::SandboxId() called (Using Fallback: RETAIL)\n");
            return L"RETAIL";
        }
        std::wstringstream wss;
        wss << L"[WinDurango TRACE] XboxLiveConfiguration::SandboxId() called: " << sandboxId << L"\n";
        OutputDebugStringW(wss.str().c_str());
        return sandboxId.c_str();
    }
} // namespace winrt::Windows::Xbox::Services::implementation