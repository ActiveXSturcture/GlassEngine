#include "Renderer.hpp"

namespace RenderCore
{
    Renderer::Renderer(uint32_t width,uint32_t height,const char* name)
    :bufferWidth(width),bufferHeight(height),name(name),useWarpRatio(false)
    {
        aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    }
}