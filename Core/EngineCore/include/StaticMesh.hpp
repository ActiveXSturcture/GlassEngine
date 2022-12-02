#pragma once

#include "GlassEngineTemplate.hpp"
#include "TObject.hpp"
namespace EngineCore
{
    struct AABB
    {
        float XAxisMin,YAxisMin,ZAxisMin,XAxisMax,YAxisMax,ZAxisMax;
    };
    
    class StaticMesh : public TObject
    {
    private:
        float* VertexBufferData;
        uint32_t* IndexBufferData;
    public:
        StaticMesh(/* args */);
        ~StaticMesh();
        virtual void Tick(float deltaTime) override;
    };
    

    
} // namespace EngineCore
