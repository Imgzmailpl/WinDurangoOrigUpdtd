#include "Windows.Xbox.Input.Controller.h"
#include "Windows.Xbox.Input.Gamepad.h" // CRITICAL: Includes the Gamepad implementation [B1]
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Xbox.System.h>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winrt::Windows::Xbox::Input::implementation
{
    static event<EventHandler<winrt::Windows::Xbox::Input::ControllerAddedEventArgs>> m_added;
    static event<EventHandler<winrt::Windows::Xbox::Input::ControllerRemovedEventArgs>> m_removed;
    static event<EventHandler<winrt::Windows::Xbox::Input::ControllerPairingChangedEventArgs>> m_pairing;
    static event<EventHandler<winrt::Windows::Xbox::Input::ControllerOrderChangedEventArgs>> m_order;

    IVectorView<winrt::Windows::Xbox::Input::IController> Controller::Controllers()
    {
        static auto g_controllers = []() {
            auto vec = single_threaded_vector<winrt::Windows::Xbox::Input::IController>();
            // Return our native Gamepad (which natively inherits from IController and implements IGamepad!) [B1]
            vec.Append(winrt::make<winrt::Windows::Xbox::Input::implementation::Gamepad>(1, true));
            return vec;
        }();
        return g_controllers.GetView();
    }

    winrt::Windows::Xbox::Input::Controller Controller::GetControllerById(uint64_t)
    {
        return nullptr;
    }

    void Controller::GetControllerMetadata(uint64_t, winrt::Windows::Xbox::Input::MetadataVersion const &,
                                           IVector<uint8_t> const &)
    {
    }

    IVectorView<winrt::Windows::Xbox::Input::IController> Controller::GetControllersOrderedLeftToRight(
        winrt::Windows::Xbox::Input::ControllerOrderFilter const &)
    {
        return Controllers();
    }

    event_token Controller::ControllerAdded(
        EventHandler<winrt::Windows::Xbox::Input::ControllerAddedEventArgs> const &handler)
    {
        return m_added.add(handler);
    }
    void Controller::ControllerAdded(event_token const &token) noexcept
    {
        m_added.remove(token);
    }

    event_token Controller::ControllerRemoved(
        EventHandler<winrt::Windows::Xbox::Input::ControllerRemovedEventArgs> const &handler)
    {
        return m_removed.add(handler);
    }
    void Controller::ControllerRemoved(event_token const &token) noexcept
    {
        m_removed.remove(token);
    }

    event_token Controller::ControllerPairingChanged(
        EventHandler<winrt::Windows::Xbox::Input::ControllerPairingChangedEventArgs> const &handler)
    {
        return m_pairing.add(handler);
    }
    void Controller::ControllerPairingChanged(event_token const &token) noexcept
    {
        m_pairing.remove(token);
    }

    event_token Controller::ControllerOrderChanged(
        EventHandler<winrt::Windows::Xbox::Input::ControllerOrderChangedEventArgs> const &handler)
    {
        return m_order.add(handler);
    }
    void Controller::ControllerOrderChanged(event_token const &token) noexcept
    {
        m_order.remove(token);
    }

    uint64_t Controller::Id()
    {
        return 1;
    }

    hstring Controller::Type()
    {
        return L"Gamepad";
    }

    winrt::Windows::Xbox::System::User Controller::User()
    {
        // Link this physical controller directly to our authenticated native User profile! [B1]
        return winrt::Windows::Xbox::System::User::GetUserById(1);
    }
} // namespace winrt::Windows::Xbox::Input::implementation

void *GetXboxInputControllerFactory()
{
    return winrt::detach_abi(winrt::make<winrt::Windows::Xbox::Input::factory_implementation::Controller>());
}