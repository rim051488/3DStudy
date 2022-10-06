struct PSInput {
	float4 svpos:SV_POSITION;
	float3 pos:POSITION;
	float3 norm:NORMAL;
	float2 uv:TECOORD;
	float2 toon:TECOORD;
	float3 col:COLOR;
	float3 tan:TANGENT;
	float3 bin:BINORMAL;
};

SamplerState sam:register(s0);
Texture2D<float4> tex:register(t0);

// �f�B���N�V�������C�g�p�̒萔�o�b�t�@
cbuffer DirectionLightCb : register(b0)
{
	float3 ligDirection;	//���C�g�̕���
	float3 ligColor;		//���C�g�̃J���[
	float3 eyePos;			//���_�̈ʒu
}

float4 main(PSInput input) : SV_TARGET
{
	//�s�N�Z���̖@���ƃ��C�g�̕����̓��ς��v�Z����
	float t = dot(input.norm,ligDirection);
	t *= -1.0f;

	//���ς̌��ʂ��O�ȉ��Ȃ�O�ɂ���
	if (t < 0.0f)
	{
		t = 0.0f;
	}
	//�g�U���ˌ������߂�
	float3 diffuseLig = ligColor * t;
	//���˃x�N�g�������߂�
	float3 refVec = reflect(ligDirection, input.norm);
	//�������������T�[�t�F�C�X���王�_�ɐL�т�x�N�g�������߂�
	float3 toEye = eyePos - input.pos;
	//���K������
	toEye = normalize(toEye);
	//���ʔ��˂̋��������߂�
	//dot�֐��𗘗p����refVec��toEye�̓��ς����߂�
	t = dot(refVec, toEye);
	//���ς̌��ʂ̓}�C�i�X�ɂȂ�̂ŁA�}�C�i�X�̏ꍇ��0�ɂ���
	if (t < 0.0f)
	{
		t = 0.0f;
	}
	//���ʔ��˂̋������i��
	t = pow(t, 20.0f);
	//���ʔ��ˌ������߂�
	float3 specularLig = ligColor * t;
	//�g�U���ˌ��Ƌ��ʔ��ˌ��𑫂��Z���āA�ŏI�I�Ȍ������߂�
	float3 lig = diffuseLig + specularLig;
	float4 color = tex.Sample(sam, input.uv);
	color.xyz *= lig;
	return color;
}