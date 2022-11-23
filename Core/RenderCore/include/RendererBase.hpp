#pragma once
#include <iostream>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#ifndef RENDERCORE_DLL
    #ifdef BUILD_DLL
        #define RENDERCORE_DLL __declspec(dllexport)
    #else
        #define RENDERCORE_DLL
    #endif
#endif
namespace RenderCore
{
    class RENDERCORE_DLL RendererBase
    {
    protected:
        /* data */
        uint32_t bufferWidth;
        uint32_t bufferHeight;
        std::wstring name;
        float aspectRatio;
        bool useWarpRatio;
        bool IsStatic;
        bool IsLeftButtonDown{false};
        bool IsRightButtonDown{false};
        bool IsMoveForward{false};
        bool IsMoveBack{false};
        bool IsMoveRight{false};
        bool IsMoveLeft{false};
        int MousePosX,MousePosY;
        float DeltaX,DeltaY;
    public:
        RendererBase(uint32_t width,uint32_t height,std::wstring name);
        RendererBase() = delete;
        virtual ~RendererBase()=default;

        virtual void OnInit() = 0;
        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnRender() = 0;
        virtual void OnDestroy() = 0;

        // Samples override the event handlers to handle specific messages.
        virtual void OnKeyDown(uint8_t /*key*/)   {}
        virtual void OnKeyUp(uint8_t /*key*/)     {}
        virtual void SetLeftButtonDown(bool isButtonDown,int posX,int posY){IsLeftButtonDown = isButtonDown;MousePosX = posX;MousePosY = MousePosY;}
        virtual void SetRightButtonDown(bool isButtonDown,int posX,int posY){IsRightButtonDown = isButtonDown;MousePosX = posX;MousePosY = MousePosY;}
        virtual void MouseMove(int NewPosx,int NewPosy);
        virtual void RightKeyDraw()=0;
        virtual void LeftKeyDraw()=0;

        inline uint32_t GetWidth(){return bufferWidth;}
        inline uint32_t GetHeight(){return bufferHeight;}
        inline std::wstring GetName(){return name;}

    };
    
    
}