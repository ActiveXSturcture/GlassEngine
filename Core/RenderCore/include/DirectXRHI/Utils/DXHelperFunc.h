#include "stdafx.h"

namespace RenderCore
{
    template<typename T>
    static void CreateStaticCommitResource(uint32_t BufferSize,
    ComPtr<ID3D12Device>& device,
    ComPtr<ID3D12GraphicsCommandList>& Cmd,
    T* pData,ComPtr<ID3D12Resource>& pBuffer,
    std::wstring DebugName)
    {
        device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(BufferSize),
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(&pBuffer)
        );

        pBuffer->SetName(DebugName.c_str());

        ID3D12Resource* BufferUploadHeap;
        device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(BufferSize),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&BufferUploadHeap)
        );
        //DebugName += " Upload";
        BufferUploadHeap->SetName(DebugName.c_str());

        D3D12_SUBRESOURCE_DATA data{};
        data.pData = pData;
        data.RowPitch = BufferSize;
        data.SlicePitch = BufferSize;

        UpdateSubresources(Cmd.Get(),pBuffer.Get(),BufferUploadHeap,0,0,1,&data);

        Cmd->ResourceBarrier(1,&CD3DX12_RESOURCE_BARRIER::Transition(pBuffer.Get(),D3D12_RESOURCE_STATE_COPY_DEST,D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

    }
};