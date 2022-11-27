#include "StaticMeshRenderProxyBase.hpp"

namespace RenderCore
{
    StaticMeshRenderProxyBase::StaticMeshRenderProxyBase(const std::wstring& Name,const std::wstring& FilePath,const std::vector<GUI::INPUT_LAYOUT_OFFSET>& InputLayout):
    Name(Name),FilePath(FilePath)
    {
        pImporter = std::make_unique<GUI::FileImporterBase>("C://Users//18044//Desktop//GlassEngine//Assets//Geometry//viking_room.obj",InputLayout);
        if(pImporter->LoadFile())
        {
            bIsLoaded = true;
        }
    }
    StaticMeshRenderProxyBase::StaticMeshRenderProxyBase(const StaticMeshRenderProxyBase& Mesh)
    {
        
    }
} // namespace RenderCore
