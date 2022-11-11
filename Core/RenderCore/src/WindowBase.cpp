#include "WindowBase.hpp"

namespace RenderCore
{
    uint32_t windowBase::GetWidth()
    {
        return width;
    }
    uint32_t windowBase::GetHeight()
    {
        return height;
    }
    windowBase::windowBase(RendererBase* renderer)
    {
        this->width = renderer->GetWidth();
        this->height = renderer->GetHeight();
        this->Name = renderer->GetName();
    }
}