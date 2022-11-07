#include <iostream>
#include <memory>
#include "DirectXWindow.hpp"
#include "DirectXRenderer.hpp"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    std::shared_ptr<RenderCore::DirectXRenderer> renderer = std::make_shared<RenderCore::DirectXRenderer>(1280,720,L"Hello Window");
    RenderCore::DirectXWindow window(renderer.get(),hInstance,nCmdShow);
    //return MessageBox(NULL, L"Hello, OneFLOW CFD!", L"Computation", 0);
}
/*
int main()
{
    std::cout<<"Hello World!"<<std::endl;
   
    
    std::shared_ptr<RenderCore::DirectXRenderer> renderer = std::make_shared<RenderCore::DirectXRenderer>(1280,720,L"Hello Window");
    RenderCore::DirectXWindow window(renderer.get());
    system("pause");
    return 0;
}*/