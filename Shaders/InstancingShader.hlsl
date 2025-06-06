

struct VS_DIFFUSED_INPUT
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_DIFFUSED_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

cbuffer CameraData : register(b0)
{
    matrix gmtxViewProjection;
}

struct InstancingData
{
    matrix gmtxModel;
    matrix gmtxWorld;
    float4 color;
};

StructuredBuffer<InstancingData> gInstancingDatas : register(t0);

VS_DIFFUSED_OUTPUT VSInstancing(VS_DIFFUSED_INPUT input, uint nInstanceID : SV_InstanceID)
{
    VS_DIFFUSED_OUTPUT output;
    
    output.pos = mul(mul(mul(float4(input.pos, 1), gInstancingDatas[nInstanceID].gmtxModel), gInstancingDatas[nInstanceID].gmtxWorld), gmtxViewProjection);
    output.color = gInstancingDatas[nInstanceID].color;
    
    return output;
}

float4 PSInstancing(VS_DIFFUSED_OUTPUT input) : SV_TARGET
{
    return input.color;
}