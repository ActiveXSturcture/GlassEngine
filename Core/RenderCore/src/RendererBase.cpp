#include "RendererBase.hpp"

namespace RenderCore
{
    RendererBase::RendererBase(uint32_t width,uint32_t height,std::wstring name)
    :bufferWidth(width),bufferHeight(height),name(name),useWarpRatio(false)
    {
        aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    }
}