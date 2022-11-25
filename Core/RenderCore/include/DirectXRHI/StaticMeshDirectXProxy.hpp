#pragma once
#include "DirectXRenderer.hpp"
#include "StaticMeshRenderProxyBase.hpp"



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

        ComPtr<ID3D12Device> m_device;
        ComPtr<ID3D12GraphicsCommandList> m_CommandList;
        ComPtr<ID3D12RootSignature> m_RootSignature;

        D3D12_INPUT_ELEMENT_DESC* inputLayoutDesc;
    public:
        StaticMeshDirectXProxy(const std::wstring& Name,const std::wstring& FilePath,const std::vector<GUI::INPUT_LAYOUT_OFFSET>& InputLayout,ComPtr<ID3D12Device> m_device,ComPtr<ID3D12GraphicsCommandList> m_CommandList);
        virtual ~StaticMeshDirectXProxy() = default;

        virtual void BuildDrawCommand() override;

        virtual void PopulateDrawCommand() override;
    protected:
        void BuildVertexAndPixelShader();

        void BuildShaderResourceViewsHeap();

        void InitInputLayoutDesc();
    };
    
} // namespace RenderCore

