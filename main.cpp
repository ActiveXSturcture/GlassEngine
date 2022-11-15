#include <iostream>
#include <memory>
#include "DirectXRHI/DirectXWindow.hpp"
#include "DirectXRHI/DirectXRenderer.hpp"


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
     //必须先初始化该库，然后才能使用大多数GLFW函数。成功初始化后，GLFW_TRUE将返回。如果发生错误，GLFW_FALSE则返回。
    if (!glfwInit())
        return -1;

    //创建窗口
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    //循环直到用户关闭窗口
    while (!glfwWindowShouldClose(window))
    {
        //轮询并处理事件
        glfwPollEvents();
    }

    //使用GLFW完成操作后，通常是在应用程序退出之前，需要终止GLFW
    glfwTerminate();
    system("pause");
    return 0;
}
*/