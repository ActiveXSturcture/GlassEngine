#include "DirectXWindow.hpp"

namespace RenderCore
{
LRESULT CALLBACK WindowProcedure(HWND window, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
        case WM_DESTROY:
            std::cout << "destroying window\n";
            PostQuitMessage(0);
            return 0L;
        case WM_LBUTTONDOWN:
            std::cout << "mouse left button down at (" << LOWORD(lp) << ',' << HIWORD(lp) << ")\n";
        default:
            return DefWindowProc(window, msg, wp, lp);
    }
}
    DirectXWindow::DirectXWindow(uint32_t width,uint32_t height,const char* Name) : window(width,height,Name)
    {
        std::cout<<"Hello Windows"<<std::endl;
        createWin32Window();
        Tick();
    }
    DirectXWindow::~DirectXWindow()
    {
        DestroyWindow(hWnd);
    }
    void DirectXWindow::Resize(uint32_t width,uint32_t height)
    {

    }
    bool DirectXWindow::ShouldClose()
    {
        return false;
    }
    HWND DirectXWindow::createWin32Window()
    {
        // Register the window class.
    auto myclass = TEXT("myclass");
    WNDCLASSEX wndclass = {
        sizeof(WNDCLASSEX), CS_DBLCLKS,
        WindowProcedure,
        0, 0, GetModuleHandle(0), LoadIcon(0, IDI_APPLICATION),
        LoadCursor(0, IDC_ARROW), HBRUSH(COLOR_WINDOW + 1),
        0, myclass, LoadIcon(0, IDI_APPLICATION)
    };
    static bool bRegistered = RegisterClassEx(&wndclass);
    if (bRegistered)
    {
        HWND window = CreateWindowEx(0, myclass, Name,
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            width, height, 0, 0, GetModuleHandle(0), 0);
        if (window)
        {
            ShowWindow(window, SW_SHOWDEFAULT);
        }
        return window;
    }
    return nullptr;
    }

    void DirectXWindow::Tick()
    {
        int32_t second{0};
        while (second <= 10)
        {
            std::cout<<"Tick: "<<second<<std::endl;
            second++;
        }
        
    }
}