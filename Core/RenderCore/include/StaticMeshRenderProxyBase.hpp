#pragma once 
#include "RendererBase.hpp"
#include "FileImporter.hpp"

namespace RenderCore
{
    class RENDERCORE_DLL StaticMeshRenderProxyBase
    {
    protected:
        std::wstring Name;
        std::wstring FilePath;
        const float* VertexPosition;
        std::unique_ptr<GUI::FileImporterBase> pImporter;
        bool bIsLoaded{false};
    public:
        StaticMeshRenderProxyBase(const std::wstring& Name,const std::wstring& FilePath,const std::vector<GUI::INPUT_LAYOUT_OFFSET>& InputLayout);
        StaticMeshRenderProxyBase(const StaticMeshRenderProxyBase& Mesh);
        StaticMeshRenderProxyBase() = delete;
        ~StaticMeshRenderProxyBase() = default;
        inline const std::wstring& GetName(){return Name;}
        inline const std::wstring& GetFilePath(){return FilePath;}
        virtual bool LoadFromFile()=0;
        virtual void BuildDrawCommand() = 0;
        virtual void PopulateDrawCommand()=0;
        virtual void Tick(float deltaTime)=0;
        inline bool IsLoadedSucceed(){return bIsLoaded;}
    };
    
}