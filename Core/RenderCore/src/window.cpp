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
    window::window(Renderer* renderer)
    {
        this->width = renderer->GetWidth();
        this->height = renderer->GetHeight();
        this->Name = renderer->GetName();
    }
}