#include "FileImporter.hpp"

namespace GUI
{
    FileImporterBase::FileImporterBase(const std::string &pFile, const std::vector<INPUT_LAYOUT_OFFSET> &Layout)
        : pFile(pFile), NumIndices(0), NumVertices(0)
    {
        InputLayout = Layout;
    }
    bool FileImporterBase::LoadFile()
    {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(pFile,
                                                    aiProcess_CalcTangentSpace |
                                                   aiProcess_Triangulate |
                                                   aiProcess_JoinIdenticalVertices |
                                                   aiProcess_SortByPType |
                                                   aiProcess_MakeLeftHanded);

        if (nullptr == scene)
        {
            std::runtime_error(import.GetErrorString());
            return false;
        }
        BuildMeshData(scene, NumVertices, NumIndices);
        return true;
    }
    void FileImporterBase::BuildMeshData(const aiScene *scene, uint32_t &NumVertices, uint32_t &NumIndices)
    {
        Meshs.resize(scene->mNumMeshes);
        for (size_t index{0}; index < scene->mNumMeshes; index++)
        {
            const aiMesh *mesh = scene->mMeshes[index];
            Meshs[index].NumIndices = mesh->mNumFaces * 3;
            Meshs[index].NumVertices = mesh->mNumVertices;
            uint16_t offset{0};
            for (size_t i = 0; i < InputLayout.size(); i++)
            {
                if (InputLayout[i] == INPUT_LAYOUT_POSITION)
                {
                    Meshs[index].InputLayout.push_back(INPUT_LAYOUT_POSITION);
                    offset += MeshData::GetInputLayoutOffset(INPUT_LAYOUT_POSITION);
                }
                else if (InputLayout[i] == INPUT_LAYOUT_NORMAL && mesh->HasNormals())
                {
                    Meshs[index].InputLayout.push_back(INPUT_LAYOUT_NORMAL);
                    offset += MeshData::GetInputLayoutOffset(INPUT_LAYOUT_NORMAL);
                }
                else if (InputLayout[i] == INPUT_LAYOUT_TANGENT && mesh->HasTangentsAndBitangents())
                {
                    Meshs[index].InputLayout.push_back(INPUT_LAYOUT_TANGENT);
                    offset += MeshData::GetInputLayoutOffset(INPUT_LAYOUT_TANGENT);
                }
                else if (InputLayout[i] == INPUT_LAYOUT_BITANGENT && mesh->HasTangentsAndBitangents())
                {
                    Meshs[index].InputLayout.push_back(INPUT_LAYOUT_BITANGENT);
                    offset += MeshData::GetInputLayoutOffset(INPUT_LAYOUT_BITANGENT);
                }
                else if (InputLayout[i] == INPUT_LAYOUT_COLOR)
                {
                    Meshs[index].InputLayout.push_back(INPUT_LAYOUT_COLOR);
                    offset += MeshData::GetInputLayoutOffset(INPUT_LAYOUT_COLOR);
                }
            }

            offset /= sizeof(float);
            Meshs[index].VerticesDataOffset = offset;
            Meshs[index].IndexBuffer = new uint32_t[Meshs[index].NumIndices]{0};
            Meshs[index].VertexBuffer = new float[Meshs[index].NumVertices * offset]{0.0f};
            //memcpy(Meshs[index].IndexBuffer, mesh->mFaces->mIndices, Meshs[index].NumIndices * sizeof(unsigned int));
            for(uint32_t Index{0};Index < mesh->mNumFaces ;Index++)
            {
                const aiFace& face = mesh->mFaces[Index];
                uint32_t realIndex = Index * 3;
                Meshs[index].IndexBuffer[realIndex] = face.mIndices[0];
                Meshs[index].IndexBuffer[realIndex+1] = face.mIndices[1];
                Meshs[index].IndexBuffer[realIndex+2] = face.mIndices[2];
            }
            for (uint32_t VerticesIndex{0}; VerticesIndex < Meshs[index].NumVertices; VerticesIndex++)
            {
                uint16_t VerticesOffset = VerticesIndex * offset;
                uint16_t LayoutOffset{0};
                for (uint16_t orderIndex{0}; orderIndex < Meshs[index].InputLayout.size(); orderIndex++)
                {

                    switch (Meshs[index].InputLayout[orderIndex])
                    {
                    case 1:
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset] = mesh->mVertices[VerticesIndex].x;
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset + 1] = mesh->mVertices[VerticesIndex].y;
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset + 2] = mesh->mVertices[VerticesIndex].z;
                        LayoutOffset += 3;
                        break;
                    case 2:
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset] = mesh->mNormals[VerticesIndex].x;
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset + 1] = mesh->mNormals[VerticesIndex].y;
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset + 2] = mesh->mNormals[VerticesIndex].z;
                        LayoutOffset += 3;
                        break;
                    case 3:
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset] = mesh->mColors[0][VerticesIndex].r;
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset + 1] = mesh->mColors[0][VerticesIndex].g;
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset + 2] = mesh->mColors[0][VerticesIndex].b;
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset + 3] = mesh->mColors[0][VerticesIndex].a;
                        LayoutOffset += 4;
                        break;
                    case 4:
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset] = mesh->mTextureCoords[0][VerticesIndex].x;
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset + 1] = mesh->mTextureCoords[0][VerticesIndex].y;
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset + 2] = mesh->mTextureCoords[0][VerticesIndex].z;
                        LayoutOffset += 3;
                        break;
                    case 5:
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset] = mesh->mTangents[VerticesIndex].x;
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset + 1] = mesh->mTangents[VerticesIndex].y;
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset + 2] = mesh->mTangents[VerticesIndex].z;
                        LayoutOffset += 3;
                        break;
                    case 6:
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset] = mesh->mBitangents[VerticesIndex].x;
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset + 1] = mesh->mBitangents[VerticesIndex].y;
                        Meshs[index].VertexBuffer[VerticesOffset + LayoutOffset + 2] = mesh->mBitangents[VerticesIndex].z;
                        LayoutOffset += 3;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
    FileImporterBase::~FileImporterBase()
    {
        // delete[] m_Position, m_Normal, m_Texcoords, m_Tangent, m_Bitangent, m_TexCoord, m_Indices;
    }
}