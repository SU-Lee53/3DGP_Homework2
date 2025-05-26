

struct VS_BASIC_INPUT
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_BASIC_OUTPUT
{
    float3 pos : SV_POSITION;
    float4 color : COLOR;
};

cbuffer cbCameraData : register(b0)
{
    matrix gmtxViewProjection;
};

cbuffer cbWorldTransformData : register(b1)
{
    matrix gmtxWorld;
}

VS_BASIC_OUTPUT VSBasic(VS_BASIC_INPUT input)
{
    VS_BASIC_OUTPUT output;
    
    output.pos = mul(mul(float4(input.pos, 1), gmtxWorld), gmtxViewProjection);
    output.color = input.color;
    
    return output;
}

float PSBasic(VS_BASIC_OUTPUT input) : SV_Target
{
    return input.color;
}