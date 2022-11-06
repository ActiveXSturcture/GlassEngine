#include "window.hpp"

namespace RenderCore
{
    uint32_t window::GetWidth()
    {
        return width;
    }
    uint32_t window::GetHeight()
    {
        return height;
    }
    window::window(uint32_t width,uint32_t height,const char* Name)
    {
        this->width = width;
        this->height = height;
        this->Name = Name;
    }
}