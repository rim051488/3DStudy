struct PSInput
{
    float4 diff : COLOR0;
    float4 spec : COLOR1;
    float2 uv : TEXCOORD0;
};
struct PSOutput
{
    float4 color : SV_Target0;
};

SamplerState texsam : register(s0);
Texture2D<float4> tex : register(t0);

PSOutput main(PSInput input)
{
    PSOutput output;
    output.color = float4(1.0f, 0.0f, 0.0f, 1.0f);
    return output;
}