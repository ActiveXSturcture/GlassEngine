#include "DirectXRHI/Camera.hpp"

namespace RenderCore
{
    Camera::Camera(float WorldPosX, float WorldPosY, float WorldPosZ, float Roll, float Pitch, float Yaw) : CameraBase(WorldPosX, WorldPosY, WorldPosZ, Roll, Pitch, Yaw), m_pCbvDataBegin(nullptr)
    {
        buffer.MVP = FMatrix4X4::Identity();
        buffer.offset = 1;
        m_pCbvDataBegin = nullptr;
    }
    void Camera::BuildConstBuffer(ComPtr<ID3D12Device> m_device)
    {
        const UINT constBufferSize = std::ceil((float)sizeof(buffer) / 256.0f) * 256;
        ThrowIfFailed(m_device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(constBufferSize),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&m_ConstBuffer)));
        CD3DX12_RANGE readRange(0, 0);
        ThrowIfFailed(m_ConstBuffer->Map(0, &readRange, reinterpret_cast<void **>(&m_pCbvDataBegin)));
        memcpy(m_pCbvDataBegin, &buffer, sizeof(buffer));
    }
    void *Camera::UploadBuffer()
    {
        if (m_ConstBuffer.Get())
        {
            return (void *)m_ConstBuffer->GetGPUVirtualAddress();
        }
        else
        {
            return nullptr;
        }
    }
    void Camera::Tick()
    {
        BuildWorldViewProjectMatrix();
        buffer.MVP = Transform;
        const float translationSpeed = 0.005f;
        const float offsetBounds = 1.25f;

        /*buffer.offset += translationSpeed;
        if (buffer.offset > offsetBounds)
        {
            buffer.offset = -offsetBounds;
        }*/
        memcpy(m_pCbvDataBegin, &buffer, sizeof(buffer));
    }
};