#include "Utils/stdafx.h"


namespace DirectX12
{
    extern ComPtr<ID3D12Device> g_Device;
    extern ComPtr<IDXGISwapChain3> g_swapChain;
    extern ComPtr<ID3D12CommandAllocator> g_commandAllocator;
    extern ComPtr<ID3D12CommandQueue> g_commandQueue;
    class DirectXRenderSystem
    {
    private:
        UINT m_frameIndex;
        static const uint16_t BufferCount{2};
    public:
        DirectXRenderSystem(/* args */)=default;
        ~DirectXRenderSystem()=default;

        void Initialize(HWND window,bool bUseWarpDevice,uint16_t width,uint16_t height);

    protected:
        void GetHardwareAdapter(IDXGIFactory4* pFactory,IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter = false);
        void LoadPipeline(HWND window,bool bUseWarpDevice,uint16_t width,uint16_t height);
        bool CheckTearingSupport();
    };
    
} // namespace DirectX12
