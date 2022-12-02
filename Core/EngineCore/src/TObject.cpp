#include "TObject.hpp"

namespace EngineCore
{
    TObject::TObject(const char *name)
    {
        ModelToWorld = glm::identity<glm::mat4x4>();
        WorldPosition = glm::vec3(0.0f,0.0f,0.0f);
        WorldRotation = glm::vec3(0.0f,0.0f,0.0f);
        WorldScale = glm::vec3(1.0f,1.0f,1.0f);
    }

    TObject::~TObject()
    {

    }
    void TObject::SetWorldPosition(float x, float y, float z)
    {
        WorldPosition = glm::vec3(x,y,z);
    }
    void TObject::SetWorldRotation(float AngleX, float AngleY, float AngleZ)
    {
        WorldRotation = glm::vec3(AngleX, AngleY,  AngleZ);
    }
    void TObject::SetWorldScale(float ScaleX, float ScaleY, float ScaleZ)
    {
        WorldScale = glm::vec3(ScaleX, ScaleY, ScaleZ);
    }
    void TObject::ComputeModelToWorldMatrix()
    {
        glm::mat4x4 Scale = {
            WorldScale.x,0.0f,0.0f,0.0f,
            0.0f,WorldScale.y,0.0f,0.0f,
            0.0f,0.0f,WorldScale.z,0.0f,
            0.0f,0.0f,0.0f,1.0f
        };
        float cosX = cos(WorldRotation.x);
        float sinX = sin(WorldRotation.x);
        float cosY = cos(WorldRotation.y);
        float sinY = sin(WorldRotation.y);
        float cosZ = cos(WorldRotation.z);
        float sinZ = sin(WorldRotation.z);
        glm::mat4x4 RotX={
           1.0f,0.0f,0.0f,0.0f,
           0.0f,cosX,-sinX,0.0f,
           0.0f,sinX,cosX,0.0f,
            0.0f,0.0f,0.0f,1.0f
        };
        glm::mat4x4 RotY={
           cosY,0.0f,sinY,0.0f,
           0.0f,1.0f,0.0f,0.0f,
            -sinY,0.0f,cosY,0.0f,
            0.0f,0.0f,0.0f,1.0f
        };
        glm::mat4x4 RotZ={
           1.0f,0.0f,0.0f,0.0f,
           0.0f,cosZ,-sinZ,0.0f,
            0.0f,sinZ,cosZ,0.0f,
            0.0f,0.0f,0.0f,1.0f
        };
        glm::mat4x4 T = {
            1.0f,0.0f,0.0f,0.0f,
            0.0f,1.0f,0.0f,0.0f,
            0.0f,0.0f,1.0f,0.0f,
           WorldPosition.x,WorldPosition.y,WorldPosition.z,1.0f
        };

        ModelToWorld = T * RotZ * RotY * RotX * Scale;
    }
} // namespace EngineCore
