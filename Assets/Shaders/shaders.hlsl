struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};
#pragma pack_matrix(row_major)//MVP Transpose
Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);
cbuffer SceneConstantBuffer:register(b0)
{
    matrix MVP;
    float offset;
};


PSInput VSMain(float3 position : POSITION, float4 color : COLOR,float2 UV : TEXCOORD)
{
    PSInput result;

    result.position = mul(MVP,float4(position ,1.0f)) ;
    result.color =  color;

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}