#pragma once

#include "WindowBase.hpp"
#include "DirectXRenderer.hpp"


namespace RenderCore
{

    class RENDERCORE_DLL DirectXWindow : public windowBase
    {
    protected:
        /* data */
        static HWND hWnd;
        DirectXRenderer* renderer;
        static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    public:
        DirectXWindow(DirectXRenderer* renderer, HINSTANCE hInstance, int nCmdShow);
        DirectXWindow() = delete;
        virtual ~DirectXWindow();

        virtual void Resize(uint32_t width,uint32_t height) override;
        virtual bool ShouldClose() override;

        virtual void Run() override;

        static inline HWND GetHwnd(){return hWnd;}
    };
}