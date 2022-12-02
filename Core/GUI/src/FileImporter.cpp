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
                                                   aiProcess_CalcTangentSpace 
                                                    |aiProcess_Triangulate 
                                                     |aiProcess_JoinIdenticalVertices 
                                                    |aiProcess_SortByPType 
                                                   |aiProcess_MakeLeftHanded
                                                   
                                                   //|aiProcess_FlipUVs
                                                  // |aiProcess_ForceGenNormals
                                                   );

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
                    std::cout<<mesh->mName.C_Str()<<": Has Position!"<<std::endl;
                }
                else if (InputLayout[i] == INPUT_LAYOUT_NORMAL && mesh->HasNormals())
                {
                    Meshs[index].InputLayout.push_back(INPUT_LAYOUT_NORMAL);
                    offset += MeshData::GetInputLayoutOffset(INPUT_LAYOUT_NORMAL);
                    std::cout<<mesh->mName.C_Str()<<": Has Normal!"<<std::endl;
                }
                else if (InputLayout[i] == INPUT_LAYOUT_TANGENT && mesh->HasTangentsAndBitangents())
                {
                    Meshs[index].InputLayout.push_back(INPUT_LAYOUT_TANGENT);
                    offset += MeshData::GetInputLayoutOffset(INPUT_LAYOUT_TANGENT);
                    std::cout<<mesh->mName.C_Str()<<": Has Tangent!"<<std::endl;
                }
                else if (InputLayout[i] == INPUT_LAYOUT_BITANGENT && mesh->HasTangentsAndBitangents())
                {
                    Meshs[index].InputLayout.push_back(INPUT_LAYOUT_BITANGENT);
                    offset += MeshData::GetInputLayoutOffset(INPUT_LAYOUT_BITANGENT);
                    std::cout<<mesh->mName.C_Str()<<": Has BiTangent!"<<std::endl;
                }
                else if (InputLayout[i] == INPUT_LAYOUT_COLOR)
                {
                    Meshs[index].InputLayout.push_back(INPUT_LAYOUT_COLOR);
                    offset += MeshData::GetInputLayoutOffset(INPUT_LAYOUT_COLOR);
                    std::cout<<mesh->mName.C_Str()<<": Has Color!"<<std::endl;
                }
            }

            offset /= sizeof(float);
            Meshs[index].VerticesDataOffset = offset;
            //Meshs[index].VertexBuffer.resize(Meshs[index].NumVertices * offset);
            //Meshs[index].IndexBuffer.resize(Meshs[index].NumIndices);
            for(uint32_t Index{0};Index < mesh->mNumFaces ;Index++)
            {
                const aiFace& face = mesh->mFaces[Index];
                Meshs[index].IndexBuffer.push_back(face.mIndices[0]);
                Meshs[index].IndexBuffer.push_back(face.mIndices[1]);
                Meshs[index].IndexBuffer.push_back(face.mIndices[2]);
            }
            for (uint32_t VerticesIndex{0}; VerticesIndex < Meshs[index].NumVertices; VerticesIndex++)
            {
                uint16_t VerticesOffset = VerticesIndex * offset;
                for (uint16_t orderIndex{0}; orderIndex < Meshs[index].InputLayout.size(); orderIndex++)
                {

                    switch (Meshs[index].InputLayout[orderIndex])
                    {
                    case 1:
                        Meshs[index].VertexBuffer.push_back(mesh->mVertices[VerticesIndex].x);
                        Meshs[index].VertexBuffer.push_back(mesh->mVertices[VerticesIndex].y);
                        Meshs[index].VertexBuffer.push_back(mesh->mVertices[VerticesIndex].z);
                        break;
                    case 2:
                        Meshs[index].VertexBuffer.push_back(mesh->mNormals[VerticesIndex].z);
                        Meshs[index].VertexBuffer.push_back(mesh->mNormals[VerticesIndex].y);
                        Meshs[index].VertexBuffer.push_back(mesh->mNormals[VerticesIndex].x);
                        break;
                    case 3:
                        Meshs[index].VertexBuffer.push_back(mesh->mColors[0][VerticesIndex].r);
                        Meshs[index].VertexBuffer.push_back(mesh->mColors[0][VerticesIndex].g);
                        Meshs[index].VertexBuffer.push_back(mesh->mColors[0][VerticesIndex].b);
                        Meshs[index].VertexBuffer.push_back(mesh->mColors[0][VerticesIndex].a);
                        break;
                    case 4:
                        Meshs[index].VertexBuffer.push_back(mesh->mTextureCoords[0][VerticesIndex].x);
                        Meshs[index].VertexBuffer.push_back(mesh->mTextureCoords[0][VerticesIndex].y);
                        Meshs[index].VertexBuffer.push_back(mesh->mTextureCoords[0][VerticesIndex].z);
                        break;
                    case 5:
                        Meshs[index].VertexBuffer.push_back(mesh->mTangents[VerticesIndex].x);
                        Meshs[index].VertexBuffer.push_back(mesh->mTangents[VerticesIndex].y);
                        Meshs[index].VertexBuffer.push_back(mesh->mTangents[VerticesIndex].z);
                        break;
                    case 6:
                        Meshs[index].VertexBuffer.push_back(mesh->mBitangents[VerticesIndex].x);
                        Meshs[index].VertexBuffer.push_back(mesh->mBitangents[VerticesIndex].y);
                        Meshs[index].VertexBuffer.push_back(mesh->mBitangents[VerticesIndex].z);
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