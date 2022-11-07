#pragma once
#include <iostream>
#include "Renderer.hpp"



namespace RenderCore
{

    class RENDERCORE_DLL window
    {
    protected:
        uint32_t width;uint32_t height;
        std::wstring Name;
    public:
        window(Renderer* renderer);
        window() = delete;
        virtual ~window() = default;

        uint32_t GetWidth();
        uint32_t GetHeight();

        virtual void Resize(uint32_t width,uint32_t height)=0;
        virtual bool ShouldClose()=0;
        virtual void Run()=0;
    };
    
   
    

}