// �s�N�Z���V�F�[�_�[�̓���
struct PSInput
{
	float4 pos       : POSITION0 ;        // ���W( �r���[��� )
	float4 vpos : POSITION1;
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



	// Z�l��F�Ƃ��ďo��
	output.Color0 = pow(input.vpos.z/ input.vpos.w,10);
	//output.Color0 = input.pos.r;

	// �����ɂȂ�Ȃ��悤�ɃA���t�@�͕K���P
	output.Color0.a = 1.0f;

	// �����ɊD�F�ŏo���Ă݂�
	//output.Color0 = float4(0.5f, 0.5f, 0.5f, 1.0f);
	// �o�̓p�����[�^��Ԃ�
	return output;
}


