#include "Windows.Xbox.Input.Gamepad.h"
#include <windows.h>
#include <winrt/Windows.Foundation.Collections.h>
#include "../System/Windows.Xbox.System.User.h"
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winrt::Windows::Xbox::Input::implementation
{
    event<EventHandler<::winrt::Windows::Xbox::Input::GamepadAddedEventArgs>> Gamepad::e_GamepadAdded;
    event<EventHandler<::winrt::Windows::Xbox::Input::GamepadRemovedEventArgs>> Gamepad::e_GamepadRemoved;

    winrt::Windows::Xbox::Input::Gamepad GamepadAddedEventArgs::Gamepad()
    {
        return gamepad;
    }
    winrt::Windows::Xbox::Input::Gamepad GamepadRemovedEventArgs::Gamepad()
    {
        return gamepad;
    }

    DateTime GamepadReading::Timestamp()
    {
        return time;
    }
    winrt::Windows::Xbox::Input::GamepadButtons GamepadReading::Buttons()
    {
        uint32_t b = 0;
        if (x_gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
            b |= 0x1;
        if (x_gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
            b |= 0x2;
        if (x_gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
            b |= 0x4;
        if (x_gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
            b |= 0x8;
        if (x_gamepad.wButtons & XINPUT_GAMEPAD_START)
            b |= 0x10;
        if (x_gamepad.wButtons & XINPUT_GAMEPAD_BACK)
            b |= 0x20;
        if (x_gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
            b |= 0x40;
        if (x_gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
            b |= 0x80;
        if (x_gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
            b |= 0x100;
        if (x_gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
            b |= 0x200;
        if (x_gamepad.wButtons & XINPUT_GAMEPAD_A)
            b |= 0x1000;
        if (x_gamepad.wButtons & XINPUT_GAMEPAD_B)
            b |= 0x2000;
        if (x_gamepad.wButtons & XINPUT_GAMEPAD_X)
            b |= 0x4000;
        if (x_gamepad.wButtons & XINPUT_GAMEPAD_Y)
            b |= 0x8000;
        return static_cast<winrt::Windows::Xbox::Input::GamepadButtons>(b);
    }

    bool GamepadReading::IsDPadUpPressed()
    {
        return (x_gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
    }
    bool GamepadReading::IsDPadDownPressed()
    {
        return (x_gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
    }
    bool GamepadReading::IsDPadLeftPressed()
    {
        return (x_gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
    }
    bool GamepadReading::IsDPadRightPressed()
    {
        return (x_gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
    }
    bool GamepadReading::IsMenuPressed()
    {
        return (x_gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
    }
    bool GamepadReading::IsViewPressed()
    {
        return (x_gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
    }
    bool GamepadReading::IsLeftThumbstickPressed()
    {
        return (x_gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
    }
    bool GamepadReading::IsRightThumbstickPressed()
    {
        return (x_gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
    }
    bool GamepadReading::IsLeftShoulderPressed()
    {
        return (x_gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
    }
    bool GamepadReading::IsRightShoulderPressed()
    {
        return (x_gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
    }
    bool GamepadReading::IsAPressed()
    {
        return (x_gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
    }
    bool GamepadReading::IsBPressed()
    {
        return (x_gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
    }
    bool GamepadReading::IsXPressed()
    {
        return (x_gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
    }
    bool GamepadReading::IsYPressed()
    {
        return (x_gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
    }
    float GamepadReading::LeftTrigger()
    {
        return static_cast<float>(x_gamepad.bLeftTrigger) / 255.0f;
    }
    float GamepadReading::RightTrigger()
    {
        return static_cast<float>(x_gamepad.bRightTrigger) / 255.0f;
    }
    float GamepadReading::LeftThumbstickX()
    {
        return static_cast<float>(x_gamepad.sThumbLX) / 32768.0f;
    }
    float GamepadReading::LeftThumbstickY()
    {
        return static_cast<float>(x_gamepad.sThumbLY) / 32768.0f;
    }
    float GamepadReading::RightThumbstickX()
    {
        return static_cast<float>(x_gamepad.sThumbRX) / 32768.0f;
    }
    float GamepadReading::RightThumbstickY()
    {
        return static_cast<float>(x_gamepad.sThumbRY) / 32768.0f;
    }

    IVectorView<winrt::Windows::Xbox::Input::IGamepad> Gamepad::Gamepads()
    {
        static auto g_gamepads = []() {
            auto vec = single_threaded_vector<winrt::Windows::Xbox::Input::IGamepad>();
            // FIX: Always append the Gamepad. We must spoof a permanently connected
            // controller to the game engine to satisfy the User 1 identity.
            vec.Append(winrt::make<Gamepad>(1, true));
            return vec;
        }();
        return g_gamepads.GetView();
    }

    winrt::event_token Gamepad::GamepadAdded(
        EventHandler<::winrt::Windows::Xbox::Input::GamepadAddedEventArgs> const &handler)
    {
        // FIX: Prevent synchronous ghost gamepad injection into Forza's state machine.
        // Forza naturally calls Gamepads() to get the connected controllers.
        return e_GamepadAdded.add(handler);
    }

    winrt::event_token Gamepad::GamepadRemoved(
        EventHandler<::winrt::Windows::Xbox::Input::GamepadRemovedEventArgs> const &handler)
    {
        return e_GamepadRemoved.add(handler);
    }
    void Gamepad::GamepadAdded(winrt::event_token const &token) noexcept
    {
        e_GamepadAdded.remove(token);
    }
   
    void Gamepad::GamepadRemoved(winrt::event_token const &token) noexcept
    {
        e_GamepadRemoved.remove(token);
    }
    uint64_t Gamepad::Id()
    {
        return id;
    }
    hstring Gamepad::Type()
    {
        return L"Gamepad";
    }

    winrt::Windows::Xbox::System::User Gamepad::User()
    {
        // FIX: The Absolute Singleton Rule. The controller is always held by User 1.
        return winrt::Windows::Xbox::System::implementation::User::Users().GetAt(0);
    }

    winrt::Windows::Xbox::Input::INavigationReading Gamepad::GetNavigationReading()
    {
        return nullptr;
    }
    winrt::Windows::Xbox::Input::RawNavigationReading Gamepad::GetRawNavigationReading()
    {
        return {};
    }

    void Gamepad::SetVibration(winrt::Windows::Xbox::Input::GamepadVibration const &value)
    {
        XINPUT_VIBRATION vib = {};
        if (sizeof(value) == 32)
        {
            const double *motors = reinterpret_cast<const double *>(&value);
            vib.wLeftMotorSpeed = static_cast<WORD>(motors[0] * 65535.0);
            vib.wRightMotorSpeed = static_cast<WORD>(motors[1] * 65535.0);
        }
        else
        {
            const float *motors = reinterpret_cast<const float *>(&value);
            vib.wLeftMotorSpeed = static_cast<WORD>(motors[0] * 65535.0f);
            vib.wRightMotorSpeed = static_cast<WORD>(motors[1] * 65535.0f);
        }
        XInputSetState(0, &vib);
    }

    winrt::Windows::Xbox::Input::GamepadReading Gamepad::GetCurrentReading()
    {
        XINPUT_STATE state = {};
        XInputGetState(0, &state);
        return winrt::make<GamepadReading>(state.Gamepad);
    }

    winrt::Windows::Xbox::Input::RawGamepadReading Gamepad::GetRawCurrentReading()
    {
        return {};
    }
} // namespace winrt::Windows::Xbox::Input::implementation