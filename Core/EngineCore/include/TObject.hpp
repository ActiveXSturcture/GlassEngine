#pragma once

#include "GlassEngineTemplate.hpp"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
namespace EngineCore
{
    class ENGINECORE_DLL TObject
    {
    protected:
        std::string Name;
        bool bIsTick{true};
        glm::mat4x4 ModelToWorld;
        glm::vec3 WorldPosition;
        glm::vec3 WorldRotation;
        glm::vec3 WorldScale;
    public:
        TObject(const char* name);
        virtual ~TObject();
        virtual void Tick(float deltaTime)=0;
        virtual void Start()=0;
        inline bool IsTick(){return bIsTick;}
        void SetWorldPosition(float x,float y,float z);
        void SetWorldRotation(float AngleX,float AngleY,float AngleZ);
        void SetWorldScale(float ScaleX,float ScaleY,float ScaleZ);
        inline void SetTickable(bool bTick){bIsTick = bTick;}
        void ComputeModelToWorldMatrix();
    };
    

} // namespace EngineCore
