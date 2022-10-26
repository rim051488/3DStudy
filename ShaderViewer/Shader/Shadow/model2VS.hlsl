// ���_�V�F�[�_�[�̓���
struct VSInput
{
	float4 pos        : POSITION ;         // ���W( ���[�J����� )
	float3 norm          : NORMAL0 ;          // �@��( ���[�J����� )
	float4 diff         : COLOR0 ;           // �f�B�t���[�Y�J���[
	float4 spec        : COLOR1 ;           // �X�y�L�����J���[
	float4 uv0      : TEXCOORD0 ;        // �e�N�X�`�����W
	float4 uv1      : TEXCOORD1 ;		// �T�u�e�N�X�`�����W
	int4   BlendIndices0   : BLENDINDICES0 ;    // �X�L�j���O�����p Float�^�萔�z��C���f�b�N�X
	float4 BlendWeight0    : BLENDWEIGHT0 ;     // �X�L�j���O�����p�E�G�C�g�l
} ;

// ���_�V�F�[�_�[�̏o��
struct VSOutput
{
	float4 diff         : COLOR0 ;
	float4 spec        : COLOR1 ;
	float4 uv0      : TEXCOORD0 ;
	float4 lpos      : TEXCOORD1 ;    // ���C�g����݂����W( ���C�g�̎ˉe��� )
	float4 pos        : SV_POSITION ;	// ���W( �v���W�F�N�V������� )
} ;


// �}�e���A���p�����[�^
struct DX_D3D11_CONST_MATERIAL
{
	float4		Diffuse ;				// �f�B�t���[�Y�J���[
	float4		Specular ;				// �X�y�L�����J���[
	float4		Ambient_Emissive ;		// �}�e���A���G�~�b�V�u�J���[ + �}�e���A���A���r�G���g�J���[ * �O���[�o���A���r�G���g�J���[

	float		Power ;					// �X�y�L�����̋���
	float		TypeParam0 ;			// �}�e���A���^�C�v�p�����[�^0
	float		TypeParam1 ;			// �}�e���A���^�C�v�p�����[�^1
	float		TypeParam2 ;			// �}�e���A���^�C�v�p�����[�^2
} ;

// �t�H�O�p�����[�^
struct DX_D3D11_VS_CONST_FOG
{
	float		LinearAdd ;				// �t�H�O�p�p�����[�^ end / ( end - start )
	float		LinearDiv ;				// �t�H�O�p�p�����[�^ -1  / ( end - start )
	float		Density ;				// �t�H�O�p�p�����[�^ density
	float		E ;						// �t�H�O�p�p�����[�^ ���R�ΐ��̒�

	float4		Color ;					// �J���[
} ;

// ���C�g�p�����[�^
struct DX_D3D11_CONST_LIGHT
{
	int			Type ;					// ���C�g�^�C�v( DX_LIGHTTYPE_POINT �Ȃ� )
	int3		Padding1 ;				// �p�f�B���O�P

	float3		Position ;				// ���W( �r���[��� )
	float		RangePow2 ;				// �L�������̂Q��

	float3		Direction ;				// ����( �r���[��� )
	float		FallOff ;				// �X�|�b�g���C�g�pFallOff

	float3		Diffuse ;				// �f�B�t���[�Y�J���[
	float		SpotParam0 ;			// �X�|�b�g���C�g�p�p�����[�^�O( cos( Phi / 2.0f ) )

	float3		Specular ;				// �X�y�L�����J���[
	float		SpotParam1 ;			// �X�|�b�g���C�g�p�p�����[�^�P( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

	float4		Ambient ;				// �A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������

	float		Attenuation0 ;			// �����ɂ�錸�������p�p�����[�^�O
	float		Attenuation1 ;			// �����ɂ�錸�������p�p�����[�^�P
	float		Attenuation2 ;			// �����ɂ�錸�������p�p�����[�^�Q
	float		Padding2 ;				// �p�f�B���O�Q
} ;

// �s�N�Z���V�F�[�_�[�E���_�V�F�[�_�[���ʃp�����[�^
struct DX_D3D11_CONST_BUFFER_COMMON
{
	DX_D3D11_CONST_LIGHT		Light[ 6 ] ;			// ���C�g�p�����[�^
	DX_D3D11_CONST_MATERIAL		Material ;				// �}�e���A���p�����[�^
	DX_D3D11_VS_CONST_FOG		Fog ;					// �t�H�O�p�����[�^
} ;

// ��{�p�����[�^
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
	float4		AntiViewportMatrix[ 4 ] ;				// �A���`�r���[�|�[�g�s��
	float4		ProjectionMatrix[ 4 ] ;					// �r���[�@���@�v���W�F�N�V�����s��
	float4		ViewMatrix[ 3 ] ;						// ���[���h�@���@�r���[�s��
	float4		LocalWorldMatrix[ 3 ] ;					// ���[�J���@���@���[���h�s��

	float4		ToonOutLineSize ;						// �g�D�[���̗֊s���̑傫��
	float		DiffuseSource ;							// �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
	float		SpecularSource ;						// �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
	float		MulSpecularColor ;						// �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
	float		Padding ;
} ;

// ���̑��̍s��
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
{
	float4		ShadowMapLightViewProjectionMatrix[ 3 ][ 4 ] ;			// �V���h�E�}�b�v�p�̃��C�g�r���[�s��ƃ��C�g�ˉe�s�����Z��������
	float4		TextureMatrix[ 3 ][ 2 ] ;								// �e�N�X�`�����W����p�s��
} ;

// �e�p�̐[�x�L�^�摜���쐬�����ۂ̃J�����̃r���[�s��Ǝˉe�s��
struct LIGHTCAMERA_MATRIX
{
	MATRIX		ViewMatrix;
	MATRIX		ProjectionMatrix;
};

// �X�L�j���O���b�V���p�́@���[�J���@���@���[���h�s��
struct DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX
{
	float4		Matrix[ 54 * 3 ] ;					// ���[�J���@���@���[���h�s��
} ;

// ���_�V�F�[�_�[�E�s�N�Z���V�F�[�_�[���ʃp�����[�^
cbuffer cbD3D11_CONST_BUFFER_COMMON					: register( b0 )
{
	DX_D3D11_CONST_BUFFER_COMMON				g_Common ;
} ;

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_VS_BASE				: register( b1 )
{
	DX_D3D11_VS_CONST_BUFFER_BASE				g_Base ;
} ;

// ���̑��̍s��
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX			: register( b2 )
{
	DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX		g_OtherMatrix ;
} ;

// �X�L�j���O���b�V���p�́@���[�J���@���@���[���h�s��
cbuffer cbD3D11_CONST_BUFFER_VS_LOCALWORLDMATRIX	: register( b3 )
{
	DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX	g_LocalWorldMatrix ;
} ;

//cbuffer cbLIGHTCAMERA_MATRIX						: register( b4 )
//{
//	LIGHTCAMERA_MATRIX							g_LightMatrix;
//} ;


cbuffer LIGHT_VIEW		: register(b4)
{
	matrix g_lightView;
	matrix g_lightProjection;
};

// main�֐�
VSOutput main( VSInput input )
{
	VSOutput output ;
	float4 lWorldPosition ;
	float4 lViewPosition ;
	float4 lLViewPosition ;
	float3 lWorldNrm ;
	float3 lViewNrm ;
	float3 lLightHalfVec ;
	float4 lLightLitParam ;
	float4 lLightLitDest ;
	float4 lLocalWorldMatrix[ 3 ] ;
	// ���[�J�����W�̃Z�b�g
	float4 pos = float4(input.pos.xyz, 1);

	// �����̃t���[���̃u�����h�s��̍쐬
	lLocalWorldMatrix[ 0 ]  = g_LocalWorldMatrix.Matrix[input.BlendIndices0.x + 0 ] * input.BlendWeight0.x;
	lLocalWorldMatrix[ 1 ]  = g_LocalWorldMatrix.Matrix[input.BlendIndices0.x + 1 ] * input.BlendWeight0.x;
	lLocalWorldMatrix[ 2 ]  = g_LocalWorldMatrix.Matrix[input.BlendIndices0.x + 2 ] * input.BlendWeight0.x;

	lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.y + 0 ] * input.BlendWeight0.y;
	lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.y + 1 ] * input.BlendWeight0.y;
	lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.y + 2 ] * input.BlendWeight0.y;

	lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.z + 0 ] * input.BlendWeight0.z;
	lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.z + 1 ] * input.BlendWeight0.z;
	lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.z + 2 ] * input.BlendWeight0.z;

	lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.w + 0 ] * input.BlendWeight0.w;
	lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.w + 1 ] * input.BlendWeight0.w;
	lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.w + 2 ] * input.BlendWeight0.w;


	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// ���[�J�����W�����[���h���W�ɕϊ�
	lWorldPosition.x = dot(pos, lLocalWorldMatrix[ 0 ] ) ;
	lWorldPosition.y = dot(pos, lLocalWorldMatrix[ 1 ] ) ;
	lWorldPosition.z = dot(pos, lLocalWorldMatrix[ 2 ] ) ;
	lWorldPosition.w = 1.0f ;

	// ���[���h���W���r���[���W�ɕϊ�
	lViewPosition.x = dot( lWorldPosition, g_Base.ViewMatrix[ 0 ] ) ;
	lViewPosition.y = dot( lWorldPosition, g_Base.ViewMatrix[ 1 ] ) ;
	lViewPosition.z = dot( lWorldPosition, g_Base.ViewMatrix[ 2 ] ) ;
	lViewPosition.w = 1.0f ;

	// �r���[���W���ˉe���W�ɕϊ�
	output.pos.x = dot( lViewPosition, g_Base.ProjectionMatrix[ 0 ] ) ;
	output.pos.y = dot( lViewPosition, g_Base.ProjectionMatrix[ 1 ] ) ;
	output.pos.z = dot( lViewPosition, g_Base.ProjectionMatrix[ 2 ] ) ;
	output.pos.w = dot( lViewPosition, g_Base.ProjectionMatrix[ 3 ] ) ;

	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )



	// ���C�g�̏��� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// �@�����r���[��Ԃ̊p�x�ɕϊ� =========================================( �J�n )

	// ���[�J���x�N�g�������[���h�x�N�g���ɕϊ�
	lWorldNrm.x = dot(input.norm, lLocalWorldMatrix[ 0 ].xyz ) ;
	lWorldNrm.y = dot(input.norm, lLocalWorldMatrix[ 1 ].xyz ) ;
	lWorldNrm.z = dot(input.norm, lLocalWorldMatrix[ 2 ].xyz ) ;

	// ���[���h�x�N�g�����r���[�x�N�g���ɕϊ�
	lViewNrm.x = dot( lWorldNrm, g_Base.ViewMatrix[ 0 ].xyz ) ;
	lViewNrm.y = dot( lWorldNrm, g_Base.ViewMatrix[ 1 ].xyz ) ;
	lViewNrm.z = dot( lWorldNrm, g_Base.ViewMatrix[ 2 ].xyz ) ;

	// �@���𐳋K��
	lViewNrm = normalize( lViewNrm ) ;

	// �@�����r���[��Ԃ̊p�x�ɕϊ� =========================================( �I�� )


	// ���C�g�f�B�t���[�Y�J���[�ƃ��C�g�X�y�L�����J���[�̊p�x�����v�Z =======( �J�n )

	// �@���ƃ��C�g�̋t�����x�N�g���Ƃ̓��ς� lLightLitParam.x �ɃZ�b�g
	lLightLitParam.x = dot( lViewNrm, -g_Common.Light[ 0 ].Direction ) ;

	// �n�[�t�x�N�g���̌v�Z norm( ( norm( ���_�ʒu���王�_�ւ̃x�N�g�� ) + ���C�g�̕��� ) )
	lLightHalfVec = normalize( normalize( -lViewPosition.xyz ) - g_Common.Light[ 0 ].Direction ) ;

	// �@���ƃn�[�t�x�N�g���̓��ς� lLightLitParam.y �ɃZ�b�g
	lLightLitParam.y = dot( lLightHalfVec, lViewNrm ) ;

	// �X�y�L�������˗��� lLightLitParam.w �ɃZ�b�g
	lLightLitParam.w = g_Common.Material.Power ;

	// ���C�g�p�����[�^�v�Z
	lLightLitDest = lit( lLightLitParam.x, lLightLitParam.y, lLightLitParam.w ) ;

	// ���C�g�f�B�t���[�Y�J���[�ƃ��C�g�X�y�L�����J���[�̊p�x�����v�Z =======( �I�� )

	// ���C�g�̏��� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )



	// �o�̓p�����[�^�Z�b�g ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// �f�B�t���[�Y�J���[ =
	//            �f�B�t���[�Y�p�x�����v�Z���� *
	//            ���C�g�̃f�B�t���[�Y�J���[ *
	//            �}�e���A���̃f�B�t���[�Y�J���[ +
	//            ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� +
	//            �}�e���A���̃A���r�G���g�J���[�ƃO���[�o���A���r�G���g�J���[����Z�������̂ƃ}�e���A���G�~�b�V�u�J���[�����Z��������
	output.diff.xyz = lLightLitDest.y * g_Common.Light[ 0 ].Diffuse * g_Common.Material.Diffuse.xyz + g_Common.Light[ 0 ].Ambient.xyz + g_Common.Material.Ambient_Emissive.xyz ;

	// �f�B�t���[�Y�A���t�@�̓}�e���A���̃f�B�t���[�Y�J���[�̃A���t�@�����̂܂܎g��
	output.diff.w = g_Common.Material.Diffuse.w ;

	// �X�y�L�����J���[ = �X�y�L�����p�x�����v�Z���� * ���C�g�̃X�y�L�����J���[ * �}�e���A���̃X�y�L�����J���[
	output.spec.xyz = lLightLitDest.z * g_Common.Light[ 0 ].Specular * g_Common.Material.Specular.xyz ;

	// �X�y�L�����A���t�@�̓}�e���A���̃X�y�L�����J���[�̃A���t�@�����̂܂܎g��
	output.spec.w = g_Common.Material.Specular.w ;


	// �e�N�X�`�����W�ϊ��s��ɂ��ϊ����s�������ʂ̃e�N�X�`�����W���Z�b�g
	output.uv0.x = dot(input.uv0, g_OtherMatrix.TextureMatrix[ 0 ][ 0 ] ) ;
	output.uv0.y = dot(input.uv0, g_OtherMatrix.TextureMatrix[ 0 ][ 1 ] ) ;

	// �o�̓p�����[�^�Z�b�g ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )



	// �[�x�e�p�̃��C�g���猩���ˉe���W���Z�o ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// ���[���h���W�����C�g�̃r���[���W�ɕϊ�
	lLViewPosition = mul(g_lightView, lWorldPosition ) ;
	//lLViewPosition = mul(g_LightMatrix.ViewMatrix, lWorldPosition ) ;
	
	// ���C�g�̃r���[���W�����C�g�̎ˉe���W�ɕϊ�
	output.lpos = mul(g_lightProjection, lLViewPosition ) ;
	//output.lpos = mul(g_LightMatrix.ProjectionMatrix, lLViewPosition ) ;
	
	// �y�l�����̓��C�g�̃r���[���W�ɂ���
	output.lpos.z = lLViewPosition.z ;

	// �[�x�e�p�̃��C�g���猩���ˉe���W���Z�o ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )


	// �o�̓p�����[�^��Ԃ�
	return output;
}

