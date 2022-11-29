#include "DirectXRHI/DirectXRenderer.hpp"
#include "DirectXRHI/DirectXWindow.hpp"
namespace RenderCore
{
    DirectXRenderer::DirectXRenderer(uint32_t width, uint32_t height, std::wstring name) : RendererBase(width, height, name), m_useWarpDevice(false), m_frameIndex(0),
                                                                                           m_viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)),
                                                                                           m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height)),
                                                                                           m_rtvDescriptorSize(0), m_constantBufferData{}, m_pCbvDataBegin(nullptr),
                                                                                           cam(0.0f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f,0.4f*DirectX::XM_PI, (float)width / (float)height, 0.1f, 100.0f)
    {
        WCHAR assetsPath[512];
        GetAssetsPath(assetsPath, _countof(assetsPath));
        AssetsPath = assetsPath;
    }

    void DirectXRenderer::LoadPipeline()
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

        if (m_useWarpDevice)
        {
            ComPtr<IDXGIAdapter> warpAdapter;
            ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

            ThrowIfFailed(D3D12CreateDevice(
                warpAdapter.Get(),
                D3D_FEATURE_LEVEL_12_1,
                IID_PPV_ARGS(&m_device)));
        }
        else
        {
            ComPtr<IDXGIAdapter1> hardwareAdapter;
            GetHardwareAdapter(factory.Get(), &hardwareAdapter, true);

            ThrowIfFailed(D3D12CreateDevice(
                hardwareAdapter.Get(),
                D3D_FEATURE_LEVEL_12_1,
                IID_PPV_ARGS(&m_device)));
        }

        // Describe and create the command queue.
        D3D12_COMMAND_QUEUE_DESC queueDesc{};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

        // Describe and create the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.BufferCount = FrameCount;
        swapChainDesc.Width = bufferWidth;
        swapChainDesc.Height = bufferHeight;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

        ComPtr<IDXGISwapChain1> swapChain;

        ThrowIfFailed(factory->CreateSwapChainForHwnd(
            m_commandQueue.Get(),
            DirectXWindow::GetHwnd(),
            &swapChainDesc,
            nullptr,
            nullptr,
            &swapChain));

        // This sample does not support fullscreen transitions.
        ThrowIfFailed(factory->MakeWindowAssociation(DirectXWindow::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));

        ThrowIfFailed(swapChain.As(&m_swapChain));
        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

        // Create descriptor heaps.
        {
            // Describe and create a render target view (RTV) descriptor heap.
            D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
            rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            rtvHeapDesc.NumDescriptors = FrameCount;
            rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));
            m_rtvHeap->SetName(L"Render Target View Heap");
            m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

            D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
            srvHeapDesc.NumDescriptors = 1;
            srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            ThrowIfFailed(m_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap)));
            m_srvHeap->SetName(L"Shader Resource View Heap");

            D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{};
            cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            cbvHeapDesc.NumDescriptors = 1;
            cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            ThrowIfFailed(m_device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_cbvHeap)));
            m_cbvHeap->SetName(L"Const Buffer Resource Heap");

            D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
            dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            dsvHeapDesc.NumDescriptors = 1;
            dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
            ThrowIfFailed(m_device->CreateDescriptorHeap(&dsvHeapDesc,IID_PPV_ARGS(&m_dsvHeap)));
            m_dsvHeap->SetName(L"Depth/Stencil Resource Heap");
        }

        // Create frame resources.
        {
            CD3DX12_CPU_DESCRIPTOR_HANDLE rtvhandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
            // Create a RTV for each frame.
            for (UINT n = 0; n < FrameCount; n++)
            {
                ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
                m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvhandle);
                rtvhandle.Offset(1, m_rtvDescriptorSize);
            }
            CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
            D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
            dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
            dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
            dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
            m_device->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
                D3D12_HEAP_FLAG_NONE,
                &CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT,bufferWidth,bufferHeight,1,1,1,0,D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
                D3D12_RESOURCE_STATE_DEPTH_WRITE,
                &CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT,1.0f,0),
                IID_PPV_ARGS(&m_depthStencilView)
            );
            m_device->CreateDepthStencilView(m_depthStencilView.Get(),&dsvDesc,dsvHandle);
        }
        ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));
    }
    std::vector<UINT8> DirectXRenderer::GenerateTextureData()
    {
        const UINT rowPitch = TextureWidth * TexturePixelSize;
        const UINT cellPitch = rowPitch >> 3;      // The width of a cell in the checkboard texture.
        const UINT cellHeight = TextureWidth >> 3; // The height of a cell in the checkerboard texture.
        const UINT textureSize = rowPitch * TextureHeight;

        std::vector<UINT8> data(textureSize);
        UINT8 *pData = &data[0];

        for (UINT n = 0; n < textureSize; n += TexturePixelSize)
        {
            UINT x = n % rowPitch;
            UINT y = n / rowPitch;
            UINT i = x / cellPitch;
            UINT j = y / cellHeight;

            if (i % 2 == j % 2)
            {
                pData[n] = 0x00;     // R
                pData[n + 1] = 0x00; // G
                pData[n + 2] = 0x00; // B
                pData[n + 3] = 0xff; // A
            }
            else
            {
                pData[n] = 0xff;     // R
                pData[n + 1] = 0xff; // G
                pData[n + 2] = 0xff; // B
                pData[n + 3] = 0xff; // A
            }
        }

        return data;
    }
    void DirectXRenderer::LoadAssets()
    {
        // Create an  root signature.
        {
            D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

            // This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
            featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

            if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
            {
                featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
            }
            D3D12_STATIC_SAMPLER_DESC sampler = {};
            sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
            sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
            sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
            sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
            sampler.MipLODBias = 0;
            sampler.MaxAnisotropy = 0;
            sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
            sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
            sampler.MinLOD = 0.0f;
            sampler.MaxLOD = D3D12_FLOAT32_MAX;
            sampler.ShaderRegister = 0;
            sampler.RegisterSpace = 0;
            sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

            CD3DX12_ROOT_PARAMETER1 rootParameters[1];
            CD3DX12_DESCRIPTOR_RANGE1 range[1];
            range->Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
            // rootParameters[0].InitAsDescriptorTable(1,&range[0],D3D12_SHADER_VISIBILITY_VERTEX);
            rootParameters[0].InitAsConstantBufferView(0);

            D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
                D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
                D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
                D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
                D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
                D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

            CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
            rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 1, &sampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
            ComPtr<ID3DBlob> signature;
            ComPtr<ID3DBlob> error;
            ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error));
            ThrowIfFailed(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
        }
        std::vector<GUI::INPUT_LAYOUT_OFFSET> layout;
        layout.push_back(GUI::INPUT_LAYOUT_POSITION);
        layout.push_back(GUI::INPUT_LAYOUT_NORMAL);
        //layout.push_back(GUI::INPUT_LAYOUT_TEXCOORD);
        //layout.push_back(GUI::INPUT_LAYOUT_TANGENT);
        //layout.push_back(GUI::INPUT_LAYOUT_BITANGENT);
        mesh = std::make_unique<StaticMeshDirectXProxy>(L"name",L"File",layout);
        if(mesh->IsLoadedSucceed())
        {
            std::cout<<"Load Succeed !"<<std::endl;
        }
        // Create the command list.
        ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_CommandList)));
        mesh->BuildVertexAndIndexBufferStatic(m_device,m_CommandList);
        mesh->BuildPSO(m_rootSignature.Get(),m_device.Get());
        cam.BuildConstBuffer(m_device);

        
        // Create synchronization objects and wait until assets have been uploaded to the GPU.
        {
            ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
            m_fenceValue = 1;
            m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
            if (m_fenceEvent == nullptr)
            {
                ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
            }

            ThrowIfFailed(m_CommandList->Close());
            ID3D12CommandList* ppCommandList[] = {m_CommandList.Get()};
            m_commandQueue->ExecuteCommandLists(_countof(ppCommandList),ppCommandList);
            WaitForPreviousFrame();
        }
        mesh->BindStaticData();
    }
    void DirectXRenderer::PopulateCommandList()
    {
        ThrowIfFailed(m_commandAllocator->Reset());
        ThrowIfFailed(m_CommandList->Reset(m_commandAllocator.Get(), mesh->GetPSO()));
        // Set necessary state.
        m_CommandList->SetGraphicsRootSignature(m_rootSignature.Get());
        m_CommandList->SetGraphicsRootConstantBufferView(0, (D3D12_GPU_VIRTUAL_ADDRESS)cam.UploadBuffer());

        m_CommandList->RSSetViewports(1, &m_viewport);
        m_CommandList->RSSetScissorRects(1, &m_scissorRect);
        // Indicate that the back buffer will be used as a render target.
        m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
        CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
        m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
        
        // Record commands.
        const float clearColor[] = {0.0f, 0.2f, 0.4f, 1.0f};
        m_CommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
        m_CommandList->ClearDepthStencilView(dsvHandle,D3D12_CLEAR_FLAG_DEPTH,1.0f,0,0,nullptr);
        mesh->PopulateDrawCommand(m_CommandList);
        //mesh->PopulateDrawCommand(m_CommandList);
        // Indicate that the back buffer will now be used to present.
        m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

        ThrowIfFailed(m_CommandList->Close());
    }
    void DirectXRenderer::WaitForPreviousFrame()
    {
        const UINT64 fence = m_fenceValue;
        ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), fence));
        m_fenceValue++;
        if (m_fence->GetCompletedValue() < fence)
        {
            ThrowIfFailed(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
            WaitForSingleObject(m_fenceEvent, INFINITE);
        }

        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
    }
    void DirectXRenderer::OnInit()
    {
        LoadPipeline();
        LoadAssets();
    }
    void DirectXRenderer::OnUpdate(float deltaTime)
    {

        if (IsRightButtonDown)
        {
            cam.RotateYaw(-this->DeltaX * deltaTime * 10.0f);
            cam.RotatePitch(-this->DeltaY * deltaTime* 10.0f);
            // cam.RotateRoll(-this->DeltaX * deltaTime);
            // cam.MoveForward(this->DeltaX*deltaTime);
            if (IsMoveForward)
            {
                cam.MoveForward(deltaTime * 5.0f);
            }
            else if (IsMoveBack)
            {
                cam.MoveForward(-deltaTime * 5.0f);
            }
            if (IsMoveLeft)
            {
                cam.MoveSide(-deltaTime * 5.0f);
            }
            else if (IsMoveRight)
            {
                cam.MoveSide(deltaTime * 5.0f);
            }
            this->DeltaX = 0;
            this->DeltaY = 0;
        }
        cam.Tick();
    }
    void DirectXRenderer::OnDestroy()
    {
        WaitForPreviousFrame();
        CloseHandle(m_fenceEvent);
    }
    void DirectXRenderer::OnRender()
    {
        PopulateCommandList();
        ID3D12CommandList *ppCommandLists[] = {m_CommandList.Get()};
        m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        ThrowIfFailed(m_swapChain->Present(1, 0));
        WaitForPreviousFrame();
    }
    DirectXRenderer::~DirectXRenderer()
    {
    }
    void DirectXRenderer::GetHardwareAdapter(IDXGIFactory4 *pFactory, IDXGIAdapter1 **ppAdapter, bool requestHighPerformanceAdapter)
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
    bool DirectXRenderer::CheckTearingSupport()
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

    void DirectXRenderer::OnKeyDown(uint8_t key)
    {
        if ( key == 65)
        {
            IsMoveLeft = true;
        }
        else if (key == 68)
        {
            IsMoveRight = true;
        }
        else if ( key == 83)
        {
            IsMoveBack = true;
        }
        else if (key == 87)
        {
            IsMoveForward = true;
        }
    }
    void DirectXRenderer::OnKeyUp(uint8_t key)
    {
        if (key == 65)
        {
            IsMoveLeft = false;
        }
        else if (key == 68)
        {
            IsMoveRight = false;
        }
        else if (key == 83)
        {
            IsMoveBack = false;
        }
        else if (key == 87)
        {
            IsMoveForward = false;
        }
    }
    void DirectXRenderer::RightKeyDraw()
    {
        if (IsRightButtonDown)
        {
        }
    }
    void DirectXRenderer::LeftKeyDraw()
    {
        if (IsLeftButtonDown)
        {
        }
    }
}