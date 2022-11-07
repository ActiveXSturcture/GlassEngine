#pragma once
#include <iostream>
#ifndef RENDERCORE_DLL
    #ifdef BUILD_DLL
        #define RENDERCORE_DLL __declspec(dllexport)
    #else
        #define RENDERCORE_DLL
    #endif
#endif
namespace RenderCore
{
    class RENDERCORE_DLL Renderer
    {
    protected:
        /* data */
        uint32_t bufferWidth;
        uint32_t bufferHeight;
        std::wstring name;
        float aspectRatio;
        bool useWarpRatio;
    public:
        Renderer(uint32_t width,uint32_t height,std::wstring name);
        Renderer() = delete;
        virtual ~Renderer()=default;

        virtual void OnInit() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnRender() = 0;
        virtual void OnDestroy() = 0;

        // Samples override the event handlers to handle specific messages.
        virtual void OnKeyDown(uint8_t /*key*/)   {}
        virtual void OnKeyUp(uint8_t /*key*/)     {}

        inline uint32_t GetWidth(){return bufferWidth;}
        inline uint32_t GetHeight(){return bufferHeight;}
        inline std::wstring GetName(){return name;}

    };
    
    
}