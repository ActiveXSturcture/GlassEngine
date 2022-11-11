#include "StaticMeshRenderProxyBase.hpp"

namespace RenderCore
{
    StaticMeshRenderProxyBase::StaticMeshRenderProxyBase(const std::wstring& Name,const std::wstring& FilePath):
    Name(Name),FilePath(FilePath)
    {
        
    }
    StaticMeshRenderProxyBase::StaticMeshRenderProxyBase(const StaticMeshRenderProxyBase& Mesh)
    {
        //this->Name = Mesh.GetName();
        //this->FilePath = Mesh.GetFilePath();
    }
} // namespace RenderCore
