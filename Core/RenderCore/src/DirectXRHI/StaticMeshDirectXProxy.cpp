
#include "DirectXRHI/StaticMeshDirectXProxy.hpp"

namespace RenderCore
{
    StaticMeshDirectXProxy::StaticMeshDirectXProxy(const std::wstring &Name, const std::wstring &FilePath, ComPtr<ID3D12Device> m_device, ComPtr<ID3D12GraphicsCommandList> m_CommandList)
        : StaticMeshRenderProxyBase(Name, FilePath)
    {
        this->m_device = m_device;
        this->m_CommandList = m_CommandList;
    }

    void StaticMeshDirectXProxy::BuildDrawCommand()
    {
    }

    void StaticMeshDirectXProxy::BuildVertexAndPixelShader()
    {
    }

    void StaticMeshDirectXProxy::PopulateDrawCommand()
    {
        ID3D12DescriptorHeap *ppHeaps[] = {m_srvHeap.Get()};
        m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
        m_CommandList->SetGraphicsRootDescriptorTable(0, m_srvHeap->GetGPUDescriptorHandleForHeapStart());
    }

    void StaticMeshDirectXProxy::BuildShaderResourceViewsHeap()
    {
        D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
        srvHeapDesc.NumDescriptors = 1;
        srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        ThrowIfFailed(m_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap)));
    }
} // namespace RenderCore
