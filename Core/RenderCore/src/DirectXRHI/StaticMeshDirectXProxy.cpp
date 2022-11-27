
#include "DirectXRHI/StaticMeshDirectXProxy.hpp"

namespace RenderCore
{
    StaticMeshDirectXProxy::StaticMeshDirectXProxy(const std::wstring &Name, const std::wstring &FilePath,
                                                   const std::vector<GUI::INPUT_LAYOUT_OFFSET> &InputLayout)
        : StaticMeshRenderProxyBase(Name, FilePath, InputLayout)
    {
        WCHAR assetsPath[512];
        GetAssetsPath(assetsPath, _countof(assetsPath));
        AssetsPath = assetsPath;
    }
    void StaticMeshDirectXProxy::BuildPSO(ID3D12RootSignature *m_RootSignature, ID3D12Device *m_device)
    {
        InitInputLayoutDesc();
        BuildVertexAndIndexBuffer(m_device);
        BuildVertexAndPixelShader();
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        D3D12_RASTERIZER_DESC rasterizerDesc{};
        rasterizerDesc = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        // rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
        psoDesc.InputLayout = {inputLayoutDesc.data(), inputLayoutSize};
        psoDesc.pRootSignature = m_RootSignature;
        psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
        psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
        psoDesc.RasterizerState = rasterizerDesc;
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        psoDesc.DepthStencilState.DepthEnable = FALSE;
        psoDesc.DepthStencilState.StencilEnable = FALSE;
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc.Count = 1;
        ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
    }

    void StaticMeshDirectXProxy::BuildDrawCommand()
    {
    }
    void StaticMeshDirectXProxy::BuildVertexAndIndexBuffer(ID3D12Device *m_device)
    {
        const GUI::MeshData &mesh = pImporter->GetMeshWidthIndex(0);
        {
            const UINT vertexBufferSize = mesh.NumVertices * sizeof(float);
            ThrowIfFailed(m_device->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
                D3D12_HEAP_FLAG_NONE,
                &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&m_vertexBuffer)));

            // Copy the triangle data to the vertex buffer.
            UINT8 *pVertexDataBegin;
            CD3DX12_RANGE readRange(0, 0);
            ThrowIfFailed(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void **>(&pVertexDataBegin)));
            memcpy(pVertexDataBegin, mesh.VertexBuffer, vertexBufferSize);
            m_vertexBuffer->Unmap(0, nullptr);

            // Initialize the vertex buffer view.
            m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
            m_vertexBufferView.StrideInBytes = sizeof(float) * 9;
            m_vertexBufferView.SizeInBytes = vertexBufferSize;
        }
        {
            const UINT indexBufferSize = sizeof(uint32_t) * mesh.NumIndices;
            NumIndices = mesh.NumIndices;
            ThrowIfFailed(m_device->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
                D3D12_HEAP_FLAG_NONE,
                &CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&m_indexBuffer)));
            UINT8 *pIndexDataBegin;
            CD3DX12_RANGE readRange(0, 0);
            ThrowIfFailed(m_indexBuffer->Map(0, &readRange, reinterpret_cast<void **>(&pIndexDataBegin)));
            memcpy(pIndexDataBegin, mesh.IndexBuffer, indexBufferSize);
            m_indexBuffer->Unmap(0, nullptr);
            m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
            m_indexBufferView.SizeInBytes = indexBufferSize;
            m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
        }
    }
    void StaticMeshDirectXProxy::BuildVertexAndPixelShader()
    {

#ifdef _DEBUG
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        UINT compileFlags = 0;
#endif
        AssetsPath += L"shaders.hlsl";
        ThrowIfFailed(D3DCompileFromFile(AssetsPath.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
        ThrowIfFailed(D3DCompileFromFile(AssetsPath.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));
    }

    void StaticMeshDirectXProxy::PopulateDrawCommand(ComPtr<ID3D12GraphicsCommandList> m_CommandList)
    {

        m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_CommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
        m_CommandList->IASetIndexBuffer(&m_indexBufferView);

        m_CommandList->DrawIndexedInstanced(NumIndices, 1, 0, 0, 0);
    }

    void StaticMeshDirectXProxy::BuildShaderResourceViewsHeap(ID3D12Device *m_device)
    {
        D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
        srvHeapDesc.NumDescriptors = 1;
        srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        ThrowIfFailed(m_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap)));
    }

    void StaticMeshDirectXProxy::InitInputLayoutDesc()
    {
        if (bIsLoaded)
        {
            const GUI::MeshData &mesh = pImporter->GetMeshWidthIndex(0);
            inputLayoutSize = mesh.InputLayout.size();
            inputLayoutDesc.resize(inputLayoutSize);
            uint16_t offset{0};
            for (size_t i = 0; i < inputLayoutSize; i++)
            {
                if (i > 0)
                {
                    offset += GUI::MeshData::GetInputLayoutOffset(mesh.InputLayout[i - 1]);
                }
                const char *name = GUI::MeshData::GetInputSemanticName(mesh.InputLayout[i]);
                uint16_t FormatSize = GUI::MeshData::GetInputLayoutOffset(mesh.InputLayout[i]);
                DXGI_FORMAT DXFormat;
                if (FormatSize == 16)
                {
                    DXFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
                }
                else if (FormatSize == 12)
                {
                    DXFormat = DXGI_FORMAT_R32G32B32_FLOAT;
                }
                else if (FormatSize == 8)
                {
                    DXFormat = DXGI_FORMAT_R32G32_FLOAT;
                }
                else if (FormatSize == 4)
                {
                    DXFormat = DXGI_FORMAT_R32G32B32_FLOAT;
                }
                inputLayoutDesc[i] = {name, 0, DXFormat, 0, offset, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0};
            }
        }
    }
} // namespace RenderCore
