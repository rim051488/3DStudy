// �s�N�Z���V�F�[�_�[�̓���
struct PSInput
{
	float4 pos       : TEXCOORD0 ;        // ���W( �r���[��� )
	float4 svpos        : SV_POSITION;	// ���W( �v���W�F�N�V������� )
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PSOutput
{
	float4 Color0          : SV_TARGET0 ;	// �F
} ;

// main�֐�
PSOutput main(PSInput input )
{
	PSOutput output ;

	// �y�l��F�Ƃ��ďo��
	output.Color0 = input.pos.z;

	// �����ɂȂ�Ȃ��悤�ɃA���t�@�͕K���P
	output.Color0.a = 1.0f;

	// �o�̓p�����[�^��Ԃ�
	return output;
}


