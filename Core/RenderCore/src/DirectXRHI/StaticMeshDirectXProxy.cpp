
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
        BuildVertexAndPixelShader();
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        D3D12_RASTERIZER_DESC rasterizerDesc{};
        rasterizerDesc = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
        //rasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
        psoDesc.InputLayout = {inputLayoutDesc.data(), inputLayoutSize};
        psoDesc.pRootSignature = m_RootSignature;
        psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
        psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
        psoDesc.RasterizerState = rasterizerDesc;
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
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
    void StaticMeshDirectXProxy::BuildVertexAndIndexBufferStatic(ComPtr<ID3D12Device>& m_device,ComPtr<ID3D12GraphicsCommandList>& Cmd)
    {
        const GUI::MeshData &mesh = pImporter->GetMeshWidthIndex(0);
        {
            /*float *cube_vertices = new float[24]{
                // front
                -1.0, -1.0, 1.0,
                1.0, -1.0, 1.0,
                1.0, 1.0, 1.0,
                -1.0, 1.0, 1.0,
                // back
                -1.0, -1.0, -1.0,
                1.0, -1.0, -1.0,
                1.0, 1.0, -1.0,
                -1.0, 1.0, -1.0};*/
            
            NumVertices = mesh.NumVertices;
            const UINT vertexBufferSize = NumVertices* mesh.VerticesDataOffset * sizeof(float);
            CreateStaticCommitResource(vertexBufferSize,m_device,Cmd,mesh.VertexBuffer.data(),m_vertexBuffer,L"Vertex Buffer");
            // Initialize the vertex buffer view.
            m_vertexBufferView.StrideInBytes =  mesh.VerticesDataOffset * sizeof(float);
            m_vertexBufferView.SizeInBytes = vertexBufferSize;
        }
        {
            /*uint32_t* cube_indexs = new uint32_t[36]{
                // front
                0, 1, 2,
                2, 3, 0,
                // right
                1, 5, 6,
                6, 2, 1,
                // back
                7, 6, 5,
                5, 4, 7,
                // left
                4, 0, 3,
                3, 7, 4,
                // bottom
                4, 5, 1,
                1, 0, 4,
                // top
                3, 2, 6,
                6, 7, 3};*/
            const UINT indexBufferSize = sizeof(uint32_t) * mesh.NumIndices;
            NumIndices = mesh.NumIndices;
            CreateStaticCommitResource(indexBufferSize,m_device,Cmd,mesh.IndexBuffer.data(),m_indexBuffer,L"Index Buffer");
            m_indexBufferView.SizeInBytes = indexBufferSize;
            m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
        }
    }
    void StaticMeshDirectXProxy::BindStaticData()
    {
        m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
        m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
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
        // m_CommandList->DrawInstanced(NumVertices,1,0,0);
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
