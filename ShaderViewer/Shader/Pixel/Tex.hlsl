struct PSInput {
	float4 svpos:SV_POSITION;
	float4 pos:POSITION;
	float3 norm:NORMAL;
	float2 uv:TECOORD;
	float4 diff:COLOR0;
	float4 spec:COLOR1;
};

// �f�B���N�V�������C�g�p�̒萔�o�b�t�@
cbuffer DirectionLightCb : register(b1)
{
	float3 ligDirection;	//���C�g�̕���
	float3 ligColor;		//���C�g�̃J���[
}

cbuffer BaseCBuffer : register(b0) {
	matrix AntiViewportM;//4x4�r���[�|�[�g�t�s��
	matrix ProjectionM;//4x4�v���W�F�N�V�����s��
	float4x3 ViewM;//4x3(�r���[�s��)
	float4x3 LocalM;//4x3(��]�g�k���s�ړ�)
	float4		ToonOutLineSize;						// �g�D�[���̗֊s���̑傫��
	float		DiffuseSource;							// �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
	float		SpecularSource;						// �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
	float		MulSpecularColor;						// �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
	float		Padding;//�l�ߕ�(����)
}

struct MaterialBuffer
{
	float4 diff;			// �f�B�t���[�Y�J���[
	float4 spec;			// �X�y�L�����J���[
	float4 ambi;			// �G�~�b�V�u + �A���r�G���g + �O���[�o���A���r�G���g
	float power;			// �X�y�L�����̋���
	float3 pad;				// �p�f�B���O
};

SamplerState sam:register(s0);
sampler Toon:register(s1);
Texture2D<float4> tex:register(t0);
Texture2D<float4> toon:register(t1);


float4 main(PSInput input) : SV_TARGET
{
	// ���F���ς�邾���Ńe�N�X�`���֌W�Ȃ�
	//return float4(0.0f, 1.0f, 1.0f, 1.0f);
	//return float4(0,0,1,1);
	// ���e�N�X�`����\��t���邾��
	//return float4(input.uv,1,1);
	return tex.Sample(sam,float2(input.uv.xy));

}