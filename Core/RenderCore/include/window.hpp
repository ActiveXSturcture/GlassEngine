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

    class RENDERCORE_DLL window
    {
    protected:
        uint32_t width;uint32_t height;
        const char* Name;
    public:
        window(uint32_t width,uint32_t height,const char* Name);
        window() = delete;
        virtual ~window() = default;

        uint32_t GetWidth();
        uint32_t GetHeight();

        virtual void Resize(uint32_t width,uint32_t height)=0;
        virtual bool ShouldClose()=0;
        virtual void Tick()=0;
    };
    
   
    

}