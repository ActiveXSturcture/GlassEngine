#pragma once

#include "window.hpp"
#include "DirectXRenderer.hpp"


namespace RenderCore
{

    class RENDERCORE_DLL DirectXWindow : public window
    {
    protected:
        /* data */
        HWND hWnd;
        HWND createWin32Window();
        inline HWND GetHwnd(){return hWnd;}
        //LRESULT CALLBACK WindowProcedure(HWND window, UINT msg, WPARAM wp, LPARAM lp);
    public:
        DirectXWindow(DirectXRenderer* renderer);
        DirectXWindow() = delete;
        virtual ~DirectXWindow();

        virtual void Resize(uint32_t width,uint32_t height) override;
        virtual bool ShouldClose() override;

        virtual void Tick() override;
    };
}