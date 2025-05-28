

struct VS_BASIC_INPUT
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_BASIC_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

cbuffer cbCameraData : register(b0)
{
    matrix gmtxViewProjection;
};

cbuffer cbWorldTransformData : register(b1)
{
    matrix gmtxModel;
    matrix gmtxWorld;
}

VS_BASIC_OUTPUT VSBasic(VS_BASIC_INPUT input)
{
    VS_BASIC_OUTPUT output;
    
    output.pos = mul(mul(mul(float4(input.pos, 1), gmtxModel), gmtxWorld), gmtxViewProjection);
    output.color = input.color;
    
    return output;
}

float4 PSBasic(VS_BASIC_OUTPUT input) : SV_TARGET
{
    return input.color;
}