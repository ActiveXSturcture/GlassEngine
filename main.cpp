#include <iostream>
#include "DirectXWindow.hpp"

int main()
{
    std::cout<<"Hello World!"<<std::endl;
    RenderCore::DirectXWindow window(800,600,"Hello Window");
    system("pause");
    return 0;
}