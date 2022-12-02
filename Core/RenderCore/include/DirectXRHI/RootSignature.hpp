#pragma once
#include "Utils/stdafx.h"

namespace DirectX12
{
    class RootSignature
    {
    public:
        RootSignature(uint16_t NumRootParams=0,uint16_t NumStaticSamplers=0);
        ~RootSignature();

    protected:
        bool m_Finalized;
        uint16_t m_NumParameters;
        uint16_t m_NumSamplers;
        uint16_t m_NumInitializedStaticSamplers;
        uint32_t m_DescriptorTableBitMap;
        uint32_t m_SamplerTableBitMap;
        uint32_t m_DescriptorTableSize[16];
        std::unique_ptr<D3D12_STATIC_SAMPLER_DESC[]> m_SamplerArray;
        ID3D12RootSignature* m_Signature;
    };
    
} // namespace DirectX12
