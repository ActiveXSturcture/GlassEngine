
#include "DirectXRHI/StaticMeshDirectXProxy.hpp"

namespace RenderCore
{
    StaticMeshDirectXProxy::StaticMeshDirectXProxy(const std::wstring &Name, const std::wstring &FilePath, const std::vector<GUI::INPUT_LAYOUT_OFFSET> &InputLayout, ComPtr<ID3D12Device> m_device, ComPtr<ID3D12GraphicsCommandList> m_CommandList)
        : StaticMeshRenderProxyBase(Name, FilePath, InputLayout)
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

    void StaticMeshDirectXProxy::InitInputLayoutDesc()
    {
        if(bIsLoaded)
        {
            const GUI::MeshData& mesh = pImporter->GetMeshWidthIndex(0);
            inputLayoutDesc = new D3D12_INPUT_ELEMENT_DESC[mesh.InputLayout.size()]{};
            uint16_t offset{0};
            for (size_t i = 0; i < mesh.InputLayout.size(); i++)
            {
                if(i>0)
                {
                    offset += GUI::MeshData::GetInputLayoutOffset(mesh.InputLayout[i-1]);
                }
                const char* name = GUI::MeshData::GetInputSemanticName(mesh.InputLayout[i]);
                inputLayoutDesc[i]={ name, offset, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0};
            }
            
        }
    }
} // namespace RenderCore
