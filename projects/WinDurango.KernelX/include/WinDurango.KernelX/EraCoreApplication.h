#pragma once
#include "CoreWindow.h"
#include <windows.ui.core.h>
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <wrl.h>
#include "EraCoreWindow.h"

using namespace ABI::Windows::ApplicationModel;
using namespace ABI::Windows::ApplicationModel::Activation;
using namespace ABI::Windows::ApplicationModel::Core;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::System;
using namespace Microsoft::WRL;

MIDL_INTERFACE("0aacf7a4-5e1d-49df-8034-fb6a68bc5ed1")
ICoreApplicationEra : public IInspectable
{
  public:
    virtual HRESULT QueryInterface(const IID &riid, void **ppvObject) override = 0;
    virtual ULONG AddRef() override = 0;
    virtual ULONG Release() override = 0;

    virtual INT32 _abi_get_Id(HSTRING * value) = 0;
    virtual INT32 _abi_add_Suspending(__FIEventHandler_1_Windows__CApplicationModel__CSuspendingEventArgs * handler,
                                      EventRegistrationToken * token) = 0;
    virtual INT32 _abi_remove_Suspending(EventRegistrationToken token) = 0;
    virtual INT32 _abi_add_Resuming(__FIEventHandler_1_IInspectable * handler, EventRegistrationToken * token) = 0;
    virtual INT32 _abi_remove_Resuming(EventRegistrationToken token) = 0;
    virtual INT32 _abi_get_Properties(ABI::Windows::Foundation::Collections::IPropertySet * *value) = 0;
    virtual INT32 _abi_GetCurrentView(ABI::Windows::ApplicationModel::Core::ICoreApplicationView * *value) = 0;
    virtual INT32 _abi_Run(ABI::Windows::ApplicationModel::Core::IFrameworkViewSource * viewSource) = 0;
    virtual INT32 _abi_RunWithActivationFactories(ABI::Windows::Foundation::IGetActivationFactory *
                                                  activationFactoryCallback) = 0;
};

MIDL_INTERFACE("42A6A529-3CC6-4867-B5B7-6D1FAF5325F9")
ICoreApplicationGpuPolicyEra : public IInspectable
{
  public:
    virtual HRESULT get_DisableKinectGpuReservation(bool *) = 0;
    virtual HRESULT set_DisableKinectGpuReservation(bool) = 0;
};

enum ResourceAvailability
{
    ResourceAvailability_Full,
    ResourceAvailability_Constrained,
    ResourceAvailability_FullWithExtendedSystemReserve,
};

MIDL_INTERFACE("9FF3799E-6E83-468C-A5B0-501CE6BB0FA1")
ICoreApplicationResourceAvailabilityEra : public IInspectable
{
  public:
    STDMETHOD(_abi_get_ResourceAvailability)(ResourceAvailability *) PURE;
    STDMETHOD(_abi_add_ResourceAvailabilityChanged)(winrt::Windows::Foundation::EventHandler<IInspectable> * handler,
                                                    EventRegistrationToken * token) PURE;
    STDMETHOD(_abi_remove_ResourceAvailabilityChanged)(EventRegistrationToken) PURE;
};

// Main Wrapper
class CoreApplicationEra : public RuntimeClass<IActivationFactory, ICoreApplicationResourceAvailabilityEra,
                                               ICoreApplicationGpuPolicyEra, ICoreApplicationEra>
{
  public:
    CoreApplicationEra(ComPtr<IActivationFactory> realFactory)
    {
        m_realFactory = realFactory;
        HRESULT hr = m_realFactory.As(&m_realCoreApplication);
        if (FAILED(hr))
            throw E_FAIL;
        AddRef();
    }

    // for IActivationFactory
    HRESULT STDMETHODCALLTYPE ActivateInstance(__RPC__deref_out_opt IInspectable **instance) override
    {
        return m_realFactory->ActivateInstance(instance);
    }

    // ICoreApplication
    INT32 _abi_get_Id(HSTRING *value) override;
    INT32 _abi_add_Suspending(__FIEventHandler_1_Windows__CApplicationModel__CSuspendingEventArgs *handler,
                              EventRegistrationToken *token) override;
    INT32 _abi_remove_Suspending(EventRegistrationToken token) override;
    INT32 _abi_add_Resuming(__FIEventHandler_1_IInspectable *handler, EventRegistrationToken *token) override;
    INT32 _abi_remove_Resuming(EventRegistrationToken token) override;
    INT32 _abi_get_Properties(ABI::Windows::Foundation::Collections::IPropertySet **value) override;
    INT32 _abi_GetCurrentView(ABI::Windows::ApplicationModel::Core::ICoreApplicationView **value) override;
    INT32 _abi_Run(ABI::Windows::ApplicationModel::Core::IFrameworkViewSource *viewSource) override;
    INT32 _abi_RunWithActivationFactories(
        ABI::Windows::Foundation::IGetActivationFactory *activationFactoryCallback) override;

    // ICoreApplicationResourceAvailability
    HRESULT _abi_get_ResourceAvailability(ResourceAvailability *resourceAvailability) override;
    HRESULT _abi_add_ResourceAvailabilityChanged(winrt::Windows::Foundation::EventHandler<IInspectable> *handler,
                                                 EventRegistrationToken *token) override;
    HRESULT _abi_remove_ResourceAvailabilityChanged(EventRegistrationToken token) override;

    // ICoreApplicationGpuPolicy
    HRESULT get_DisableKinectGpuReservation(bool *) override;
    HRESULT set_DisableKinectGpuReservation(bool) override;

    // IActivationFactory (IInspectable + IUnknown)
    HRESULT QueryInterface(const IID &riid, void **ppvObject) override;
    ULONG AddRef() override;
    ULONG Release() override;

    HRESULT GetIids(ULONG *iidCount, IID **iids) override;
    HRESULT GetRuntimeClassName(HSTRING *className) override;
    HRESULT GetTrustLevel(TrustLevel *trustLevel) override;

  private:
    ULONG m_RefCount = 0;
    ComPtr<ICoreApplication> m_realCoreApplication;
    ComPtr<IActivationFactory> m_realFactory;
    bool m_KinectGpuReservation = false;
};

class FrameworkViewEra : public IFrameworkView
{
  public:
    FrameworkViewEra(IFrameworkView *windowView)
    {
        m_realView = windowView;
        AddRef();
    }

    HRESULT Initialize(ABI::Windows::ApplicationModel::Core::ICoreApplicationView *applicationView) override
    {
        return m_realView->Initialize(applicationView);
    }

    HRESULT SetWindow(ABI::Windows::UI::Core::ICoreWindow *window) override
    {
        window = reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow *>(
            new CoreWindowEra((ABI::Windows::UI::Core::CoreWindow *)window));
        m_realView->SetWindow(window);

        //Resizes game window to 1280x720 so it's not a square by default.
        auto view = winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView();
        winrt::Windows::Foundation::Size desiredSize{1280.0f, 720.0f};
        view.TryResizeView(desiredSize);

        return S_OK;
    }

    HRESULT Load(HSTRING entryPoint) override
    {
        return m_realView->Load(entryPoint);
    }

    HRESULT Run() override
    {
        return m_realView->Run();
    }

    HRESULT Uninitialize() override
    {
        return m_realView->Uninitialize();
    }

    // IActivationFactory (IInspectable + IUnknown)
    HRESULT QueryInterface(const IID &riid, void **ppvObject) override
    {
        if (riid == __uuidof(IFrameworkView) || riid == __uuidof(IUnknown) || riid == __uuidof(IInspectable))
        {
            *ppvObject = this;
            AddRef();
            return S_OK;
        }

        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    ULONG AddRef() override
    {
        return InterlockedIncrement(&m_RefCount);
    }

    ULONG Release() override
    {
        ULONG refCount = InterlockedDecrement(&m_RefCount);
        if (refCount == 0)
            delete this;
        return refCount;
    }

    HRESULT GetIids(ULONG *iidCount, IID **iids) override
    {
        return m_realView->GetIids(iidCount, iids);
    }

    HRESULT GetRuntimeClassName(HSTRING *className) override
    {
        return m_realView->GetRuntimeClassName(className);
    }

    HRESULT GetTrustLevel(TrustLevel *trustLevel) override
    {
        return m_realView->GetTrustLevel(trustLevel);
    }

  private:
    ULONG m_RefCount = 0;
    IFrameworkView *m_realView;
};

class FrameworkViewSourceEra : public IFrameworkViewSource
{
  public:
    FrameworkViewSourceEra(IFrameworkViewSource *windowViewSource)
    {
        m_realViewSource = windowViewSource;
        AddRef();
    }

    HRESULT STDMETHODCALLTYPE CreateView(ABI::Windows::ApplicationModel::Core::IFrameworkView **viewProvider) override
    {
        auto hr = m_realViewSource->CreateView(viewProvider);

        if (SUCCEEDED(hr))
            *viewProvider = new FrameworkViewEra(*viewProvider);

        return hr;
    }

    // IActivationFactory (IInspectable + IUnknown)
    HRESULT QueryInterface(const IID &riid, void **ppvObject) override
    {
        if (riid == __uuidof(IFrameworkViewSource))
        {
            *ppvObject = this;
            AddRef();
            return S_OK;
        }

        char iidstr[sizeof("{AAAAAAAA-BBBB-CCCC-DDEE-FFGGHHIIJJKK}")];
        OLECHAR iidwstr[sizeof(iidstr)];
        StringFromGUID2(riid, iidwstr, ARRAYSIZE(iidwstr));
        WideCharToMultiByte(CP_UTF8, 0, iidwstr, -1, iidstr, sizeof(iidstr), nullptr, nullptr);
        MessageBoxA(nullptr, iidstr, typeid(*this).name(), MB_OK);
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    ULONG AddRef() override
    {
        return InterlockedIncrement(&m_RefCount);
    }

    ULONG Release() override
    {
        ULONG refCount = InterlockedDecrement(&m_RefCount);
        if (refCount == 0)
            delete this;
        return refCount;
    }

    HRESULT GetIids(ULONG *iidCount, IID **iids) override
    {
        return m_realViewSource->GetIids(iidCount, iids);
    }

    HRESULT GetRuntimeClassName(HSTRING *className) override
    {
        return m_realViewSource->GetRuntimeClassName(className);
    }

    HRESULT GetTrustLevel(TrustLevel *trustLevel) override
    {
        return m_realViewSource->GetTrustLevel(trustLevel);
    }

  private:
    ULONG m_RefCount = 0;
    IFrameworkViewSource *m_realViewSource;
};