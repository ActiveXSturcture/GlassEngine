#include "RendererBase.hpp"

namespace RenderCore
{
    RendererBase::RendererBase(uint32_t width, uint32_t height, std::wstring name)
        : bufferWidth(width), bufferHeight(height), name(name), useWarpRatio(false), IsStatic(false)
    {
        aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    }
    void RendererBase::MouseMove(int NewPosx, int NewPosy)
    {
        DeltaX = MousePosX - NewPosx;
        DeltaY = MousePosY - NewPosy;
        MousePosX = NewPosx;
        MousePosY = NewPosy;
    }
}