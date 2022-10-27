// �s�N�Z���V�F�[�_�[�̓���
struct PSInput
{
	float4 diff         : COLOR0 ;       // �f�B�t���[�Y�J���[
	float4 spec        : COLOR1 ;       // �X�y�L�����J���[
	float4 uv0      : TEXCOORD0 ;    // �e�N�X�`�����W
	float4 lpos: POSITION;    // ���C�g����݂����W( x��y�̓��C�g�̎ˉe���W�Az�̓r���[���W )
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PSOutput
{
	float4 Color0          : SV_TARGET0 ;	// �F
} ;

// �萔�o�b�t�@�s�N�Z���V�F�[�_�[��{�p�����[�^
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
	float4		FactorColor ;			// �A���t�@�l��

	float		MulAlphaColor ;			// �J���[�ɃA���t�@�l����Z���邩�ǂ���( 0.0f:��Z���Ȃ�  1.0f:��Z���� )
	float		AlphaTestRef ;			// �A���t�@�e�X�g�Ŏg�p�����r�l
	float2		Padding1 ;

	int			AlphaTestCmpMode ;		// �A���t�@�e�X�g��r���[�h( DX_CMP_NEVER �Ȃ� )
	int3		Padding2 ;

	float4		IgnoreTextureColor ;	// �e�N�X�`���J���[���������p�J���[
} ;

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register( b1 )
{
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base ;
} ;

cbuffer DEPTH_CONST:register(b5)
{
	float4 test;
};

SamplerState sam            : register( s0 ) ;		// �f�B�t���[�Y�}�b�v�e�N�X�`��
Texture2D    tex            : register( t0 ) ;		// �f�B�t���[�Y�}�b�v�e�N�X�`��

SamplerState depth              : register( s1 ) ;		// �[�x�o�b�t�@�e�N�X�`��
Texture2D    depthtex              : register( t1 ) ;		// �[�x�o�b�t�@�e�N�X�`��


// main�֐�
PSOutput main( PSInput input )
{
	PSOutput output ;
	float4 TextureDiffuseColor ;
	float TextureDepth ;
	float2 DepthTexCoord ;
	float4 DefaultOutput ;


	// �e�N�X�`���J���[�̓ǂݍ���
	TextureDiffuseColor = tex.Sample(sam, input.uv0.xy ) ;

	// �o�̓J���[ = �f�B�t���[�Y�J���[ * �e�N�X�`���J���[ + �X�y�L�����J���[
	DefaultOutput = input.diff * TextureDiffuseColor + input.spec ;

	// �o�̓A���t�@ = �f�B�t���[�Y�A���t�@ * �e�N�X�`���A���t�@ * �s�����x
	DefaultOutput.a = input.diff.a * TextureDiffuseColor.a * g_Base.FactorColor.a ;
	output.Color0 = DefaultOutput;
	//return output;

	// //�[�x�e�N�X�`���̍��W���Z�o
	// //PSInput.LPPosition.xy �� -1.0f �` 1.0f �̒l�Ȃ̂ŁA����� 0.0f �` 1.0f �̒l�ɂ���
	//DepthTexCoord.x = (input.lpos.x + 1.0f ) / 2.0f;

	// //y�͍X�ɏ㉺���]
	//DepthTexCoord.y = 1.0f - (input.lpos.y + 1.0f ) / 2.0f;

	// //�[�x�o�b�t�@�e�N�X�`������[�x���擾
	//TextureDepth = depthtex.Sample(depth, DepthTexCoord );

	// //�e�N�X�`���ɋL�^����Ă���[�x( +�␳�l )���y�l���傫�������牜�ɂ���Ƃ������ƂŋP�x�𔼕��ɂ���
	//if(input.lpos.z > TextureDepth + 60.0f )
	//{
	//	DefaultOutput.rgb *= 0.5f;
	//}

	// //�o�̓J���[���Z�b�g
	//output.Color0 = DefaultOutput;

	// //�o�̓p�����[�^��Ԃ�
	//return output;

	//���C�g�r���[�X�N���[����Ԃ���UV���W��Ԃɕϊ����Ă���
	float2 shadowMapUV = input.lpos.xy / input.lpos.w;
	shadowMapUV *= float2(0.5f, -0.5f);
	shadowMapUV += 0.5f;
	// UV���W���g���ăV���h�E�}�b�v����e�����T���v�����O����
	float3 shadowMap = 1.0f;
	if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f &&
		shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
	{
		shadowMap = depthtex.Sample(depth, shadowMapUV);
	}
	// �e�N�X�`���ɃV���h�E�}�b�v����T���v�����O���������|���Z����
	output.Color0.xyz = shadowMap.xyz * DefaultOutput.xyz;
	return output;
}


