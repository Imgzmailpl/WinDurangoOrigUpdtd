#pragma once
#include "Windows.Xbox.Input.Controller.g.h"
#include "Windows.Xbox.Input.ControllerAddedEventArgs.g.h"
#include "Windows.Xbox.Input.ControllerOrderChangedEventArgs.g.h"
#include "Windows.Xbox.Input.ControllerPairingChangedEventArgs.g.h"
#include "Windows.Xbox.Input.ControllerRemovedEventArgs.g.h"

namespace winrt::Windows::Xbox::Input::implementation
{
    struct ControllerAddedEventArgs : ControllerAddedEventArgsT<ControllerAddedEventArgs>
    {
        ControllerAddedEventArgs() = default;
        winrt::Windows::Xbox::Input::Controller Controller();
    };

    struct ControllerRemovedEventArgs : ControllerRemovedEventArgsT<ControllerRemovedEventArgs>
    {
        ControllerRemovedEventArgs() = default;
        winrt::Windows::Xbox::Input::Controller Controller();
    };

    struct ControllerPairingChangedEventArgs : ControllerPairingChangedEventArgsT<ControllerPairingChangedEventArgs>
    {
        ControllerPairingChangedEventArgs() = default;
        winrt::Windows::Xbox::Input::Controller Controller();
        winrt::Windows::Xbox::System::User User();
        winrt::Windows::Xbox::System::User PreviousUser();
    };

    struct ControllerOrderChangedEventArgs : ControllerOrderChangedEventArgsT<ControllerOrderChangedEventArgs>
    {
        ControllerOrderChangedEventArgs() = default;
        void Unknown();
    };

    struct Controller : ControllerT<Controller>
    {
        Controller() = default;

        static winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Input::IController>
        Controllers();
        static winrt::event_token ControllerAdded(
            winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::Input::ControllerAddedEventArgs> const
                &handler);
        static void ControllerAdded(winrt::event_token const &token) noexcept;
        static winrt::event_token ControllerRemoved(
            winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::Input::ControllerRemovedEventArgs> const
                &handler);
        static void ControllerRemoved(winrt::event_token const &token) noexcept;
        static winrt::event_token ControllerPairingChanged(
            winrt::Windows::Foundation::EventHandler<
                winrt::Windows::Xbox::Input::ControllerPairingChangedEventArgs> const &handler);
        static void ControllerPairingChanged(winrt::event_token const &token) noexcept;
        static winrt::event_token ControllerOrderChanged(
            winrt::Windows::Foundation::EventHandler<winrt::Windows::Xbox::Input::ControllerOrderChangedEventArgs> const
                &handler);
        static void ControllerOrderChanged(winrt::event_token const &token) noexcept;
        static winrt::Windows::Xbox::Input::Controller GetControllerById(uint64_t unk);
        static void GetControllerMetadata(uint64_t unk, winrt::Windows::Xbox::Input::MetadataVersion const &version,
                                          winrt::Windows::Foundation::Collections::IVector<uint8_t> const &unka);
        static winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Input::IController>
        GetControllersOrderedLeftToRight(winrt::Windows::Xbox::Input::ControllerOrderFilter const &filter);

        uint64_t Id();
        hstring Type();
        winrt::Windows::Xbox::System::User User();
    };
} // namespace winrt::Windows::Xbox::Input::implementation

namespace winrt::Windows::Xbox::Input::factory_implementation
{
    struct Controller : ControllerT<Controller, implementation::Controller>
    {
    };
} // namespace winrt::Windows::Xbox::Input::factory_implementation