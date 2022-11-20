struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);
cbuffer SceneConstantBuffer:register(b0)
{
    float4 offset;
    float4 padding[15];
};


PSInput VSMain(float3 position : POSITION, float4 color : COLOR,float2 UV : TEXCOORD)
{
    PSInput result;

    result.position = float4(position ,2.5f) + offset;
    result.color =  color;

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}