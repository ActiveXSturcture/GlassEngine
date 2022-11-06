#include <iostream>
#include <memory>
#include "DirectXWindow.hpp"
#include "DirectXRenderer.hpp"
int main()
{
    std::cout<<"Hello World!"<<std::endl;
   
    
    std::shared_ptr<RenderCore::DirectXRenderer> renderer = std::make_shared<RenderCore::DirectXRenderer>(800,600,"Hello Window");
    RenderCore::DirectXWindow window(renderer.get());
    system("pause");
    return 0;
}