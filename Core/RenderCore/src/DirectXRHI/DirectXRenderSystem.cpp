#include "DirectXRHI/DirectXRenderSystem.hpp"

namespace DirectX12
{
    ComPtr<ID3D12Device> g_Device = nullptr;
    ComPtr<IDXGISwapChain3> g_swapChain = nullptr;
    ComPtr<ID3D12CommandAllocator> g_commandAllocator = nullptr;
    ComPtr<ID3D12CommandQueue> g_commandQueue = nullptr;
    void DirectXRenderSystem::Initialize(HWND window,bool bUseWarpDevice,uint16_t width,uint16_t height)
    {
        LoadPipeline(window,bUseWarpDevice,width,height);
    }
    void DirectXRenderSystem::LoadPipeline(HWND window,bool bUseWarpDevice,uint16_t width,uint16_t height)
    {
        UINT dxgiFactoryFlags = 0;
#ifdef _DEBUG
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
#endif

        ComPtr<IDXGIFactory4> factory;
        ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

        if (bUseWarpDevice)
        {
            ComPtr<IDXGIAdapter> warpAdapter;
            ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

            ThrowIfFailed(D3D12CreateDevice(
                warpAdapter.Get(),
                D3D_FEATURE_LEVEL_12_1,
                IID_PPV_ARGS(&g_Device)));
        }
        else
        {
            ComPtr<IDXGIAdapter1> hardwareAdapter;
            GetHardwareAdapter(factory.Get(), &hardwareAdapter, true);

            ThrowIfFailed(D3D12CreateDevice(
                hardwareAdapter.Get(),
                D3D_FEATURE_LEVEL_12_1,
                IID_PPV_ARGS(&g_Device)));
        }

        // Describe and create the command queue.
        D3D12_COMMAND_QUEUE_DESC queueDesc{};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        ThrowIfFailed(g_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&g_commandQueue)));

        // Describe and create the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.BufferCount = BufferCount;
        swapChainDesc.Width = width;
        swapChainDesc.Height = height;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

        ComPtr<IDXGISwapChain1> swapChain;

        ThrowIfFailed(factory->CreateSwapChainForHwnd(
            g_commandQueue.Get(),
            window,
            &swapChainDesc,
            nullptr,
            nullptr,
            &swapChain));

        // This sample does not support fullscreen transitions.
        ThrowIfFailed(factory->MakeWindowAssociation(window, DXGI_MWA_NO_ALT_ENTER));

        ThrowIfFailed(swapChain.As(&g_swapChain));
        m_frameIndex = g_swapChain->GetCurrentBackBufferIndex();

        ThrowIfFailed(g_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_commandAllocator)));
    }
    void DirectXRenderSystem::GetHardwareAdapter(IDXGIFactory4 *pFactory, IDXGIAdapter1 **ppAdapter, bool requestHighPerformanceAdapter)
    {
         *ppAdapter = nullptr;
        ComPtr<IDXGIAdapter1> adapter;
        ComPtr<IDXGIFactory6> factory6;
        ComPtr<ID3D12Device5> pD3DDevice;
        if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
        {
            for (UINT adapterIndex{0}; SUCCEEDED(factory6->EnumAdapterByGpuPreference(
                     adapterIndex,
                     requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
                     IID_PPV_ARGS(&adapter)));
                 ++adapterIndex)
            {
                DXGI_ADAPTER_DESC1 desc;
                // D3D12_FEATURE_ARCHITECTURE stArchitecture{};
                adapter->GetDesc1(&desc);
                if (desc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)
                {
                    continue;
                }
                if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&pD3DDevice))))
                {
                    // ThrowIfFailed(pD3DDevice->CheckFeatureSupport(D3D12_FEATURE_ARCHITECTURE,&stArchitecture,sizeof(D3D12_FEATURE_ARCHITECTURE1)))
                    break;
                }
            }
        }

        if (adapter.Get() == nullptr)
        {
            for (UINT adapterIndex{0}; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
            {
                DXGI_ADAPTER_DESC1 desc;
                adapter->GetDesc1(&desc);
                if (desc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)
                {
                    continue;
                }

                if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, _uuidof(ID3D12Device), nullptr)))
                {
                    break;
                }
            }
        }

        *ppAdapter = adapter.Detach();
    }
     bool DirectXRenderSystem::CheckTearingSupport()
    {
        bool allowTearing{false};

        ComPtr<IDXGIFactory4> factory4;
        if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
        {
            ComPtr<IDXGIFactory5> factory5;
            if (SUCCEEDED(factory4.As(&factory5)))
            {
                if (FAILED(factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing))))
                {
                    allowTearing = false;
                }
            }
        }
        return allowTearing;
    }
} // namespace DirectX12
