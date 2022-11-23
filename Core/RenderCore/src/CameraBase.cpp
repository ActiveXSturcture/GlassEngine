#include "CameraBase.hpp"

namespace RenderCore
{
    CameraBase::CameraBase(float WorldPosX, float WorldPosY, float WorldPosZ, float Roll, float Pitch, float Yaw,float FOV,float aspectRadio,float nearPlane,float farPlane) 
    :Roll(Roll), Pitch(Pitch), Yaw(Yaw),FOV(FOV),aspectRadio(aspectRadio),nearPlane(nearPlane),farPlane(farPlane),WorldPosX(WorldPosX),WorldPosY(WorldPosY),WorldPosZ(WorldPosZ)
    {
    }
};
