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

static const uint OFFSET_X = 1;
static const uint OFFSET_Y = 1;

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

// �n�[�h�V���h�E
//SamplerState depth              : register( s1 ) ;		// �[�x�o�b�t�@�e�N�X�`��
// �\�t�g�V���h�E
SamplerComparisonState depthSmp          // �[�x�o�b�t�@�e�N�X�`��
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	MaxAnisotropy = 1;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler conmparison state
	ComparisonFunc = GREATER;
};
Texture2D    depthtex              : register(t1) ;		// �[�x�o�b�t�@�e�N�X�`��

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

	//���C�g�r���[�X�N���[����Ԃ���UV���W��Ԃɕϊ����Ă���
	float2 shadowMapUV = input.lpos.xy / input.lpos.w;
	shadowMapUV *= float2(0.5f, -0.5f);
	shadowMapUV += 0.5f;
	// ���C�g�r���[�X�N���[����Ԃł�Z�l���v�Z����
	float zlpos = pow(input.lpos.z/input.lpos.w,10);
	 //UV���W���g���ăV���h�E�}�b�v����e�����T���v�����O����
	if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f &&
		shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
	{
		 //�n�[�h�V���h�E
		//float zshadowMap = depthtex.Sample(depth, shadowMapUV).r;
		//if (zlpos > zshadowMap + 0.005f)
		//{
		//	// �Ւf����Ă���
		//	output.Color0.xyz *= 0.5f;
		//}

		// �\�t�g�V���h�E
		
		//float shadowMap_0 = depthtex.Sample(depth, shadowMapUV).r;
		//float shadowMap_1 = depthtex.Sample(depth, shadowMapUV + float2(0.5,0.0f)).r;
		//float shadowMap_2 = depthtex.Sample(depth, shadowMapUV + float2(0.5, 0.5)).r;
		//float shadowMap_3 = depthtex.Sample(depth, shadowMapUV + float2(0.0f, 0.5)).r;
		//float shadowRate = 0.0f;
		//if (zlpos > shadowMap_0)
		//{
		//	// �Օ�����Ă���̂ŁA�Օ������P���Z
		//	shadowRate += 1.0f;
		//}
		//if (zlpos > shadowMap_1)
		//{
		//	// �Օ�����Ă���̂ŁA�Օ������P���Z
		//	shadowRate += 1.0f;
		//}
		//if (zlpos > shadowMap_2)
		//{
		//	// �Օ�����Ă���̂ŁA�Օ������P���Z
		//	shadowRate += 1.0f;
		//}
		//if (zlpos > shadowMap_3)
		//{
		//	// �Օ�����Ă���̂ŁA�Օ������P���Z
		//	shadowRate += 1.0f;
		//}
		//shadowRate /= 4.0f;
		//float3 shadowColor = output.Color0.xyz;
		//float3 finalColor = lerp(output.Color0.xyz, shadowColor, shadowRate);
		//output.Color0.xyz = finalColor;
		
		output.Color0 = depthtex.Sample(depthSmp, input.uv0.xy);
		return output;

		float shadow = depthtex.SampleCmpLevelZero(
			depthSmp,	// �g�p����T���v���[�X�e�[�g
			shadowMapUV, // �V���h�E�}�b�v�ɃA�N�Z�X����UV���W
			zlpos	// ��r����Z�l
		);

		// �V���h�E�J���[���v�Z
		float3 shadowColor = output.Color0.xyz * 0.5f;
		// �Օ������g���Đ��`���
		output.Color0.xyz = lerp(output.Color0.xyz, shadowColor,  shadow);
	}
	return output;
}


