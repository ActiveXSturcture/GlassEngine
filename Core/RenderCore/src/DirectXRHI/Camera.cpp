#include "DirectXRHI/Camera.hpp"

namespace RenderCore
{
    Camera::Camera(float WorldPosX, float WorldPosY, float WorldPosZ, float Roll, float Pitch, float Yaw, float FOV, float aspectRadio, float nearPlane, float farPlane)
        : CameraBase(WorldPosX, WorldPosY, WorldPosZ, Roll, Pitch, Yaw, FOV, aspectRadio, nearPlane, farPlane), m_pCbvDataBegin(nullptr)
    {

        XMStoreFloat4x4(&Project, XMMatrixPerspectiveFovLH(FOV, aspectRadio, nearPlane, farPlane));
        InitLookAt({0.0f,0.0f,1.0f});
        m_pCbvDataBegin = nullptr;
    }
    void Camera::InitLookAt(XMVECTOR L)
    {
        XMVECTOR R = XMVector3Normalize(XMVector3Cross(WorldUp, L));
        XMVECTOR U = XMVector3Normalize(XMVector3Cross(L, R));

        WorldPos = XMFLOAT3(WorldPosX,WorldPosY,WorldPosZ);
        XMStoreFloat3(&Up, U);
        XMStoreFloat3(&Right, R);
        XMStoreFloat3(&Look, L);
        mViewDirty = true;
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
        UpdateViewMatrix();
        XMStoreFloat4x4(&buffer.MVP,XMMatrixMultiply(XMLoadFloat4x4(&View),XMLoadFloat4x4(&Project)));
        

        const float translationSpeed = 0.005f;
        const float offsetBounds = 1.25f;

        buffer.offset += translationSpeed;
        if (buffer.offset > offsetBounds)
        {
            buffer.offset = -offsetBounds;
        }
        memcpy(m_pCbvDataBegin, &buffer, sizeof(buffer));
    }
    void Camera::UpdateViewMatrix()
    {
        if (mViewDirty)
        {
            XMVECTOR R = XMLoadFloat3(&Right);
            XMVECTOR L = XMLoadFloat3(&Look);
            XMVECTOR U = XMLoadFloat3(&Up);
            XMVECTOR P = XMLoadFloat3(&WorldPos);

            L = XMVector3Normalize(L);
            R = XMVector3Normalize(XMVector3Cross(WorldUp,L));
            U = XMVector3Normalize(XMVector3Cross(L, R));

            //R = XMVector3Cross(U, L);

            float x = -XMVectorGetX(XMVector3Dot(P, R));
            float y = -XMVectorGetY(XMVector3Dot(P, U));
            float z = -XMVectorGetZ(XMVector3Dot(P, L));

            XMStoreFloat3(&Up, U);
            XMStoreFloat3(&Right, R);
            XMStoreFloat3(&Look, L);
            View(0, 0) = Right.x;
            View(1, 0) = Right.y;
            View(2, 0) = Right.z;
            View(3, 0) = x;

            View(0, 1) = Up.x;
            View(1, 1) = Up.y;
            View(2, 1) = Up.z;
            View(3, 1) = y;

            View(0, 2) = Look.x;
            View(1, 2) = Look.y;
            View(2, 2) = Look.z;
            View(3, 2) = z;

            View(0, 3) = 0.0f;
            View(1, 3) = 0.0f;
            View(2, 3) = 0.0f;
            View(3, 3) = 1.0f;

            mViewDirty = false;
        }
    }
    void Camera::MoveForward(float distance)
    {
        XMVECTOR s = XMVectorReplicate(distance);
        XMVECTOR r = XMLoadFloat3(&Look);
        XMVECTOR p = XMLoadFloat3(&WorldPos);
        XMStoreFloat3(&WorldPos,XMVectorMultiplyAdd(s,r,p));
        mViewDirty = true;
    }
    void Camera::MoveSide(float distance)
    {
        XMVECTOR s = XMVectorReplicate(distance);
        XMVECTOR r = XMLoadFloat3(&Right);
        XMVECTOR p = XMLoadFloat3(&WorldPos);
        XMStoreFloat3(&WorldPos,XMVectorMultiplyAdd(s,r,p));
        mViewDirty = true;
    }
    void Camera::MoveUp(float distance)
    {
        XMVECTOR s = XMVectorReplicate(distance);
        XMVECTOR r = XMLoadFloat3(&Up);
        XMVECTOR p = XMLoadFloat3(&WorldPos);
        XMStoreFloat3(&WorldPos,XMVectorMultiplyAdd(s,r,p));
        mViewDirty = true;
    }

    void Camera::RotateYaw(float degree)
    {
        float radians = XMConvertToRadians(degree);
        XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&Up),radians);
        XMStoreFloat3(&Right,XMVector3TransformNormal(XMLoadFloat3(&Right),R));
        XMStoreFloat3(&Look,XMVector3TransformNormal(XMLoadFloat3(&Look),R));

        mViewDirty = true;
    }
    void Camera::RotatePitch(float degree)
    {
        float radians = XMConvertToRadians(degree);
        XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&Right),radians);
        XMStoreFloat3(&Up,XMVector3TransformNormal(XMLoadFloat3(&Up),R));
        XMStoreFloat3(&Look,XMVector3TransformNormal(XMLoadFloat3(&Look),R));

        mViewDirty = true;
    }
    void Camera::RotateRoll(float degree)
    {
        float radians = XMConvertToRadians(degree);
        XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&Look),radians);
        XMStoreFloat3(&Up,XMVector3TransformNormal(XMLoadFloat3(&Up),R));
        XMStoreFloat3(&Right,XMVector3TransformNormal(XMLoadFloat3(&Right),R));

        mViewDirty = true;
    }

};