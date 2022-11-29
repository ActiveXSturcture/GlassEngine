#pragma once
#include "DirectXRenderer.hpp"
#include "StaticMeshRenderProxyBase.hpp"
#include "Utils/DXHelperFunc.h"

namespace RenderCore
{
    class RENDERCORE_DLL StaticMeshDirectXProxy : public StaticMeshRenderProxyBase
    {
    protected:
        ComPtr<ID3D12Resource> m_vertexBuffer;
        D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

        ComPtr<ID3D12Resource> m_indexBuffer;
        D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

        //Shader Resource View
        ComPtr<ID3D12DescriptorHeap> m_srvHeap;

        ComPtr<ID3DBlob> vertexShader;
        ComPtr<ID3DBlob> pixelShader;

        ComPtr<ID3D12PipelineState> m_pipelineState;

        std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayoutDesc;
        uint16_t inputLayoutSize,NumIndices,NumVertices;
        std::wstring AssetsPath;
    public:
        StaticMeshDirectXProxy(const std::wstring& Name,const std::wstring& FilePath,
        const std::vector<GUI::INPUT_LAYOUT_OFFSET>& InputLayout);
        virtual ~StaticMeshDirectXProxy() = default;

        virtual void BuildDrawCommand() override;

        virtual void PopulateDrawCommand(ComPtr<ID3D12GraphicsCommandList> m_CommandList);

        inline ID3D12PipelineState* GetPSO(){return m_pipelineState.Get();}

        void BuildPSO(ID3D12RootSignature* m_RootSignature,ID3D12Device* m_device);

        void BuildVertexAndIndexBufferStatic(ComPtr<ID3D12Device>& m_device,ComPtr<ID3D12GraphicsCommandList>& Cmd);

        void BindStaticData();
    protected:
        void BuildVertexAndPixelShader();

        

        void BuildShaderResourceViewsHeap(ID3D12Device* m_device);

        void InitInputLayoutDesc();
    };
    
} // namespace RenderCore

