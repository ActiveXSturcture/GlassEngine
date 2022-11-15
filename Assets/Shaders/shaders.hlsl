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


PSInput VSMain(float4 position : POSITION, float4 color : COLOR)
{
    PSInput result;

    result.position = position + offset;
    result.color = color;

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}