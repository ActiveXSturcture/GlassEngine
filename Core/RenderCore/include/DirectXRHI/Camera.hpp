#pragma once

#include "CameraBase.hpp"
#include "Utils/stdafx.h"
using Microsoft::WRL::ComPtr;
using namespace DirectX;
namespace RenderCore
{
    class Camera : public CameraBase
    {
    private:
        struct CameraConstBuffer
        {
            XMFLOAT4X4 MVP;
            float offset;
        };

        ComPtr<ID3D12Resource1> m_ConstBuffer;
        UINT8 *m_pCbvDataBegin;
        CameraConstBuffer buffer;
        XMFLOAT4X4  Project,View;
        XMFLOAT3 WorldPos,Look,Right,Up;
        const XMVECTOR WorldUp = XMVectorSet(0.0f,1.0f,0.0f,0.0f);
        bool mViewDirty{false};
        void InitLookAt(XMVECTOR L);
    public:
        Camera(float WorldPosX, float WorldPosY, float WorldPosZ, float Roll, float Pitch, float Yaw,float FOV,float aspectRadio,float nearPlane,float farPlane);
        virtual ~Camera()=default;
        virtual void* UploadBuffer() override;
        virtual void Tick() override;
        virtual void MoveForward(float distance)override;
        virtual void MoveSide(float distance)override;
        virtual void MoveUp(float distance)override;

        virtual void RotateYaw(float degree)override;
        virtual void RotatePitch(float degree)override;
        virtual void RotateRoll(float degree)override;
        virtual void UpdateViewMatrix()override;
        void BuildConstBuffer(ComPtr<ID3D12Device> m_device);
    };

};