#include "StaticMeshRenderProxyBase.hpp"

namespace RenderCore
{
    StaticMeshRenderProxyBase::StaticMeshRenderProxyBase(const std::wstring& Name,const std::wstring& FilePath,const std::vector<GUI::INPUT_LAYOUT_OFFSET>& InputLayout):
    Name(Name),FilePath(FilePath)
    {
        pImporter = std::make_unique<GUI::FileImporterBase>("D://GlassEngine//Assets//Geometry//HeightFiled.fbx",InputLayout);
        if(pImporter->LoadFile())
        {
            bIsLoaded = true;
        }
    }
    StaticMeshRenderProxyBase::StaticMeshRenderProxyBase(const StaticMeshRenderProxyBase& Mesh)
    {
        
    }
} // namespace RenderCore
