#pragma once

#include "RendererBase.hpp"
#include "Utils/stdafx.h"
#include "Utils/DXSampleHelper.h"
#include "Camera.hpp"
using Microsoft::WRL::ComPtr;
using namespace DirectX;
namespace RenderCore
{    
    struct Vertex
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
        XMFLOAT2 UV;
    };

    struct SceneConstantBuffer
    {
        XMFLOAT4 offset;
        float padding[60];
        //XMFLOAT4X4 gWorldViewProj;
    };
    static_assert((sizeof(SceneConstantBuffer) % 256) == 0, "Constant Buffer size must be 256-byte aligned");
    class RENDERCORE_DLL DirectXRenderer : public RendererBase
    {
    protected:
        void LoadPipeline();
        void LoadAssets();
        void PopulateCommandList();
        void WaitForPreviousFrame();
        void GetHardwareAdapter(IDXGIFactory4* pFactory,IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter = false);
    public:
        DirectXRenderer(uint32_t width,uint32_t height,std::wstring name);
        virtual ~DirectXRenderer();

        virtual void OnInit() override;
        virtual void OnUpdate(float deltaTime) override;
        virtual void OnRender() override;
        virtual void OnDestroy() override;

        friend class DirectXWindow;

        virtual void OnKeyDown(uint8_t key) override;
        virtual void OnKeyUp(uint8_t key) override;
        virtual void RightKeyDraw()override;
        virtual void LeftKeyDraw()override;
    private:
        static const UINT FrameCount = 2;
        static const UINT TextureWidth = 512;
        static const UINT TextureHeight = 512;
        static const UINT TexturePixelSize = 4;    // The number of bytes used to represent a pixel in the texture.
        std::wstring AssetsPath;
        // Pipeline objects.
        CD3DX12_VIEWPORT m_viewport;
        CD3DX12_RECT m_scissorRect;
        ComPtr<IDXGISwapChain3> m_swapChain;
        ComPtr<ID3D12Device> m_device;
        ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
        ComPtr<ID3D12CommandAllocator> m_commandAllocator;
        ComPtr<ID3D12CommandQueue> m_commandQueue;
        ComPtr<ID3D12RootSignature> m_rootSignature;
        ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
        //Const Buffer View
        ComPtr<ID3D12DescriptorHeap> m_cbvHeap;
        //Shader Resource View
        ComPtr<ID3D12DescriptorHeap> m_srvHeap;
        ComPtr<ID3D12PipelineState> m_pipelineState;
        ComPtr<ID3D12GraphicsCommandList> m_CommandList;
        UINT m_rtvDescriptorSize;

        //App resources
        ComPtr<ID3D12Resource> m_vertexBuffer;
        D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
        ComPtr<ID3D12Resource> m_indexBuffer;
        D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
        //Const Buffer View
        ComPtr<ID3D12Resource> m_constantBuffer;
        SceneConstantBuffer m_constantBufferData;
        UINT8* m_pCbvDataBegin;
        //Shader Resource View
        ComPtr<ID3D12Resource> m_texture;

        //Sychornization objects
        UINT m_frameIndex;
        HANDLE m_fenceEvent;
        ComPtr<ID3D12Fence> m_fence;
        UINT64 m_fenceValue;

        // Adapter info.
        bool m_useWarpDevice;

        std::vector<UINT8> GenerateTextureData();

        bool CheckTearingSupport();

        Camera cam;
    };
}