#pragma once
#include "pch.hpp"

namespace GUI
{
    enum INPUT_LAYOUT_OFFSET
    {
        INPUT_LAYOUT_NONE,
        INPUT_LAYOUT_POSITION,
        INPUT_LAYOUT_NORMAL,
        INPUT_LAYOUT_COLOR,
        INPUT_LAYOUT_TEXCOORD,
        INPUT_LAYOUT_TANGENT,
        INPUT_LAYOUT_BITANGENT
    };
    struct MeshData
    {
        uint32_t NumIndices;
        uint32_t NumVertices;
        uint32_t VerticesDataOffset;
        float* VertexBuffer;
        uint32_t* IndexBuffer;
        bool hasIndex, hasTexture;
        std::vector<INPUT_LAYOUT_OFFSET> InputLayout;
        MeshData()
        {
            NumIndices = 0;
            NumVertices = 0;
            VerticesDataOffset = 0;
            VertexBuffer = nullptr;
            IndexBuffer = nullptr;
            hasIndex = false;
            hasTexture = false;
            InputLayout.clear();
        }
        ~MeshData()
        {
            delete[] VertexBuffer, IndexBuffer;
            std::cout << "Release MeshData" << std::endl;
        }
        static uint8_t GetInputLayoutOffset(const INPUT_LAYOUT_OFFSET &offset)
        {
            uint8_t result{0};
            switch (offset)
            {
            case INPUT_LAYOUT_POSITION:
                result = 12;
                break;
            case INPUT_LAYOUT_NORMAL:
                result = 12;
                break;
            case INPUT_LAYOUT_COLOR:
                result = 16;
                break;
            case INPUT_LAYOUT_TEXCOORD:
                result = 12;
                break;
            case INPUT_LAYOUT_TANGENT:
                result = 12;
                break;
            case INPUT_LAYOUT_BITANGENT:
                result = 12;
                break;
            default:
                break;
            }
            return result;
        }
        static const char *GetInputSemanticName(const INPUT_LAYOUT_OFFSET &offset)
        {
            switch (offset)
            {
            case INPUT_LAYOUT_POSITION:
                return "POSITION";
                break;
            case INPUT_LAYOUT_NORMAL:
                return "NORMAL";
                break;
            case INPUT_LAYOUT_COLOR:
                return "COLOR";
                break;
            case INPUT_LAYOUT_TEXCOORD:
                return "TEXCOORD";
                break;
            case INPUT_LAYOUT_TANGENT:
                return "TANGENT";
                break;
            case INPUT_LAYOUT_BITANGENT:
                return "BITANGENT";
                break;
            default:
                return "NONE";
                break;
            }
        }
    };

    class GUI_DLL FileImporterBase
    {
    private:
        std::string pFile;
        void BuildMeshData(const aiScene *scene, uint32_t &NumVertices, uint32_t &NumIndices);
        std::vector<MeshData> Meshs;
        uint32_t NumVertices, NumIndices;
        // float* m_Position,*m_Normal,*m_Texcoords,*m_Tangent,*m_Bitangent,*m_TexCoord;
        uint32_t *m_Indices;
        std::vector<INPUT_LAYOUT_OFFSET> InputLayout;

    public:
        FileImporterBase(const std::string &pFile, const std::vector<INPUT_LAYOUT_OFFSET> &Layout);
        FileImporterBase() = delete;
        FileImporterBase(const FileImporterBase &) = default;
        ~FileImporterBase();
        bool LoadFile();
        inline uint16_t GetMeshNum() { return Meshs.size(); }
        inline const MeshData &GetMeshWidthIndex(uint16_t Index) const { return Meshs[Index]; }
    };
}
