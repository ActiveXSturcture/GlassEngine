#include "DirectXRHI/DirectXWindow.hpp"

namespace RenderCore
{

    HWND DirectXWindow::hWnd = nullptr;
    LRESULT CALLBACK DirectXWindow::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        DirectXRenderer *pSample = reinterpret_cast<DirectXRenderer *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        wchar_t msg[32];
        switch (message)
        {
        case WM_CREATE:
        {
            // Save the DXSample* passed in to CreateWindow.
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
            break;
        }

        case WM_KEYDOWN:
        {
            if (pSample)
            {
                pSample->OnKeyDown(static_cast<UINT8>(wParam));
                swprintf_s(msg, L"WM_KEYDOWN: 0x%x\n", wParam);
                OutputDebugString(msg);
            }
            break;
        }

        case WM_KEYUP:
        {
            if (pSample)
            {
                pSample->OnKeyUp(static_cast<UINT8>(wParam));
                swprintf_s(msg, L"WM_KEYUP: 0x%x\n", wParam);
                OutputDebugString(msg);
            }
            break;
        }

        case WM_PAINT:
        {
            swprintf_s(msg, L"WM_PAINT: 0x%x\n", wParam);
            OutputDebugString(msg);
            if (pSample)
            {
            }
            break;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }

        case WM_LBUTTONUP:
        {
            swprintf_s(msg, L"WM_LBUTTONUP: 0x%x\n", wParam);
            OutputDebugString(msg);
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            pSample->SetLeftButtonDown(false,xPos,yPos);
            break;
        }

        case WM_LBUTTONDOWN:
        {
            swprintf_s(msg, L"WM_LBUTTONDOWN: 0x%x\n", wParam);
            OutputDebugString(msg);
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            pSample->SetLeftButtonDown(true,xPos,yPos);
            break;
        }

        case WM_RBUTTONUP:
        {
            swprintf_s(msg, L"WM_RBUTTONUP: 0x%x\n", wParam);
            OutputDebugString(msg);
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            pSample->SetRightButtonDown(false,xPos,yPos);
            break;
        }

        case WM_RBUTTONDOWN:
        {
            swprintf_s(msg, L"WM_RBUTTONDOWN: 0x%x\n", wParam);
            OutputDebugString(msg);
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            pSample->SetRightButtonDown(true,xPos,yPos);
            break;
        }

        case WM_MOUSEMOVE:
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            swprintf_s(msg, L"WM_MOUSEMOVE: 0x%x  %i  %i\n", wParam,xPos,yPos);
            OutputDebugString(msg);
            pSample->MouseMove(xPos,yPos);
            break;
        }
        }

        // Handle any messages the switch statement didn't.
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    DirectXWindow::DirectXWindow(DirectXRenderer *renderer, HINSTANCE hInstance, int nCmdShow) : windowBase(renderer)
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
        time_start = time_end = GetTickCount();
        while (msg.message != WM_QUIT)
        {
            time_start = time_end;
            // Process any messages in the queue.
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            renderer->OnUpdate(deltaTime);
            renderer->OnRender();
            time_end = GetTickCount();
            deltaTime = (time_end - time_start) / 1000.0f;
            // std::cout<<"FPS: "<<(1000.0f / (time_end - time_start))<<std::endl;
        }
        renderer->OnDestroy();
    }

}