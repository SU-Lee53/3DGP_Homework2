

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

cbuffer CameraData : register(b0)
{
    matrix gmtxViewProjection;
}

struct InstancingData
{
    matrix gmtxWorld;
    float4 color;
};

StructuredBuffer<InstancingData> gInstancingDatas : register(t0);

VS_BASIC_OUTPUT VSBasic(VS_BASIC_INPUT input, int instancedID : SV_InstanceID)
{
    VS_BASIC_OUTPUT output;
    
    output.pos = mul(mul(float4(input.pos, 1), gInstancingDatas[instancedID].gmtxWorld), gmtxViewProjection);
    output.color = input.color;
    
    return output;
}

float PSBasic(VS_BASIC_OUTPUT input) : SV_Target
{
    return input.color;
}