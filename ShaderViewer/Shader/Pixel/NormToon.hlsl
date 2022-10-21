struct PSInput {
	float4 svpos:SV_POSITION;
	float3 pos:POSITION;
	float3 norm:NORMAL;
	float2 uv:TECOORD;
	float4 diff:COLOR0;
	float4 spec:COLOR1;
	float3 tan:TANGENT;
	float3 bin:BINORMAL;
};

SamplerState sam:register(s0);
sampler Toon:register(s1);
Texture2D<float4> tex:register(t0);
Texture2D<float4> toon:register(t1);

// �f�B���N�V�������C�g�p�̒萔�o�b�t�@
cbuffer DirectionLightCb : register(b0)
{
	float3 ligDirection;	//���C�g�̕���
	float3 ligColor;		//���C�g�̃J���[
}

float4 main(PSInput input) : SV_TARGET
{
	// �g�D�[���V�F�[�_���g�����`��

	//float4 color = tex.Sample(sam,input.uv);
	//// �n�[�t�����o�[�g�g�U�Ɩ��ɂ�郉�C�e�B���O�v�Z
	//float p = saturate(dot(input.norm * -1.0f, ligDirection));
	//p = p * 0.5f + 0.5f;
	//p = saturate(p * p);
	//p = clamp(p, 0.0f, 0.99f);
	////return toon.Sample(sam, float2(p, 0.0f));
	//// �v�Z���ʂ��g�D�[���V�F�[�_�̃e�N�X�`������t�F�b�`
	//float4 Col = toon.Sample(sam, float2(p, 0.0f));
	//return color *= Col;

	float4 color;
	float p = dot(input.norm * -1.0f, ligDirection);
	p = p * 0.5f + 0.5f;
	p = p * p;

	float4 Col = toon.Sample(Toon, float2(p, 0.0f));
	color = Col * tex.Sample(sam, input.uv);
	return color;
}