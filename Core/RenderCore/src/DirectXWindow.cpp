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
    DirectXWindow::DirectXWindow(DirectXRenderer* renderer) : window(renderer)
    {
        if(hWnd)
        {
            DestroyWindow(hWnd);
        }
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
    auto myclass = TEXT("DXSampleClass");
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
         MSG msg = {};
         while (msg.message != WM_QUIT)
        {
        // Process any messages in the queue.
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
        
    }


}