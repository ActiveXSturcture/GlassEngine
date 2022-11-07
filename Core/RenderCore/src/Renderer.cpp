#include "Renderer.hpp"

namespace RenderCore
{
    Renderer::Renderer(uint32_t width,uint32_t height,std::wstring name)
    :bufferWidth(width),bufferHeight(height),name(name),useWarpRatio(false)
    {
        aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    }
}