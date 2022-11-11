#pragma once 
#include "RendererBase.hpp"

namespace RenderCore
{
    class RENDERCORE_DLL StaticMeshRenderProxyBase
    {
    private:
        std::wstring Name;
        std::wstring FilePath;
    public:
        StaticMeshRenderProxyBase(const std::wstring& Name,const std::wstring& FilePath);
        StaticMeshRenderProxyBase(const StaticMeshRenderProxyBase& Mesh);
        StaticMeshRenderProxyBase() = delete;
        ~StaticMeshRenderProxyBase() = default;
        inline const std::wstring& GetName(){return Name;}
        inline const std::wstring& GetFilePath(){return FilePath;}
        virtual void BuildDrawCommand() = 0;
        virtual void PopulateDrawCommand()=0;
    };
    
}