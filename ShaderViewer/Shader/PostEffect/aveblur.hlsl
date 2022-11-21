struct PSInput
{
    float4 diff : COLOR0; // �f�B�t���[�Y
    float4 spec : COLOR1; // �X�y�L�����J���[
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
};

struct PSOutput
{
    float4 color : SV_TARGET0; // �F
};

SamplerState texsam : register(s0);
Texture2D<float4> tex : register(t0);
PSOutput main(PSInput input)
{
    PSOutput output;
    float4 color;
    color = float4(tex.Sample(texsam, input.uv));
    // ���σu���[
    // ��e�N�Z��+�ߖT8�e�N�Z���̕��ς��v�Z����
    // 2.5�e�N�Z�����炷���߂�UV�l�����߂�
    float offsetU = 2.5f / 1280;
    float offsetV = 2.5f / 720;
    
    // ��e�N�Z������E�̃e�N�Z���̃J���[���T���v�����O����
    color += float4(tex.Sample(texsam, input.uv + float2(offsetU, 0.0f)));
    
    // ��e�N�Z�����獶�̃e�N�Z���̃J���[���T���v�����O����
    color += float4(tex.Sample(texsam, input.uv + float2(-offsetU, 0.0f)));
    
    // ��e�N�Z�����牺�̃e�N�Z���̃J���[���T���v�����O����
    color += float4(tex.Sample(texsam, input.uv + float2(0.0f, offsetV)));
    
    // ��e�N�Z�������̃e�N�Z���̃J���[���T���v�����O����
    color += float4(tex.Sample(texsam, input.uv + float2(0.0f, -offsetV)));
    
    // ��e�N�Z������E���̃e�N�Z���̃J���[���T���v�����O����
    color += float4(tex.Sample(texsam, input.uv + float2(offsetU, offsetV)));
    
    // ��e�N�Z������E��̃e�N�Z���̃J���[���T���v�����O����
    color += float4(tex.Sample(texsam, input.uv + float2(offsetU, -offsetV)));
    
    // ��e�N�Z�����獶���̃e�N�Z���̃J���[���T���v�����O����
    color += float4(tex.Sample(texsam, input.uv + float2(-offsetU, offsetV)));
    
    // ��e�N�Z�����獶��̃e�N�Z���̃J���[���T���v�����O����
    color += float4(tex.Sample(texsam, input.uv + float2(-offsetU, -offsetV)));
    
    // ��e�N�Z���ƋߖT8�e�N�Z���̕��ςȂ̂�9�ŏ��Z����
    color /= 9.0f;
    
    output.color = color;
    return output;
}