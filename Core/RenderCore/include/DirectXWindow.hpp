#pragma once

#include "window.hpp"
#include "Utils/d3dx12.h"
#include "Utils/stdafx.h"


namespace RenderCore
{

    class RENDERCORE_DLL DirectXWindow : public window
    {
    protected:
        /* data */
        HWND hWnd;
        HWND createWin32Window();
    public:
        DirectXWindow(uint32_t width,uint32_t height,const char* Name);
        virtual ~DirectXWindow();

        virtual void Resize(uint32_t width,uint32_t height) override;
        virtual bool ShouldClose() override;

        virtual void Tick() override;
    };
}