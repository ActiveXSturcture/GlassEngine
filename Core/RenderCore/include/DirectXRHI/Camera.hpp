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
            FMatrix4X4 MVP;
            float offset;
        };

        ComPtr<ID3D12Resource1> m_ConstBuffer;
        UINT8 *m_pCbvDataBegin;
        CameraConstBuffer buffer;
    public:
        Camera(float WorldPosX, float WorldPosY, float WorldPosZ, float Roll, float Pitch, float Yaw);
        virtual ~Camera()=default;
        virtual void* UploadBuffer() override;
        virtual void Tick() override;
        void BuildConstBuffer(ComPtr<ID3D12Device> m_device);
    };

};