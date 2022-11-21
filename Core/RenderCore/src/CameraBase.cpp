#include "CameraBase.hpp"

namespace RenderCore
{
    void CameraBase::BuildWorldViewProjectMatrix()
    {
        Transform = FMatrix4X4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            WorldPosX,WorldPosY, WorldPosZ, 1.0f);
    }

    CameraBase::CameraBase(float WorldPosX, float WorldPosY, float WorldPosZ, float Roll, float Pitch, float Yaw) : WorldPosX(WorldPosX), WorldPosY(WorldPosY), WorldPosZ(WorldPosZ), Roll(Roll), Pitch(Pitch), Yaw(Yaw)
    {
        Transform = FMatrix4X4(
            1.0f, 0.0f, 0.0f, WorldPosX,
            0.0f, 1.0f, 0.0f, WorldPosY,
            0.0f, 0.0f, 1.0f, WorldPosZ,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    void CameraBase::MoveForward(float distance)
    {
        WorldPosZ += distance;
    }

    void CameraBase::MoveSide(float distance)
    {
        WorldPosX -= distance;
    }

    void CameraBase::MoveUp(float distance)
    {
        WorldPosY += distance;
    }
};
