#include "DirectXWindow.hpp"

namespace RenderCore
{

    HWND DirectXWindow::hWnd = nullptr;
    LRESULT CALLBACK DirectXWindow::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        DirectXRenderer *pSample = reinterpret_cast<DirectXRenderer *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        switch (message)
        {
        case WM_CREATE:
        {
            // Save the DXSample* passed in to CreateWindow.
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
        }
            return 0;

        case WM_KEYDOWN:
            if (pSample)
            {
                pSample->OnKeyDown(static_cast<UINT8>(wParam));
            }
            return 0;

        case WM_KEYUP:
            if (pSample)
            {
                pSample->OnKeyUp(static_cast<UINT8>(wParam));
            }
            return 0;

        case WM_PAINT:
            if (pSample)
            {
                pSample->OnUpdate();
                pSample->OnRender();
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }

        // Handle any messages the switch statement didn't.
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    DirectXWindow::DirectXWindow(DirectXRenderer *renderer, HINSTANCE hInstance, int nCmdShow) : window(renderer)
    {
        this->renderer = renderer;
        // Parse the command line parameters
        int argc;
        LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);
        LocalFree(argv);

        // Initialize the window class.
        WNDCLASSEX windowClass = {0};
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = WindowProcedure;
        windowClass.hInstance = hInstance;
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClass.lpszClassName = L"DXSampleClass";
        RegisterClassEx(&windowClass);

        RECT windowRect = {0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
        AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

        // Create the window and store a handle to it.
        hWnd = CreateWindow(
            windowClass.lpszClassName,
            Name.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            nullptr, // We have no parent window.
            nullptr, // We aren't using menus.
            hInstance,
            renderer);

        renderer->OnInit();
        ShowWindow(hWnd, nCmdShow);
        Run();
    }
    DirectXWindow::~DirectXWindow()
    {

        DestroyWindow(hWnd);
    }
    void DirectXWindow::Resize(uint32_t width, uint32_t height)
    {
    }
    bool DirectXWindow::ShouldClose()
    {
        return false;
    }

    void DirectXWindow::Run()
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
        renderer->OnDestroy();
    }

}