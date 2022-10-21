// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
	//���̏��ԂłȂ��ƃo�O��?
	float3 pos				: POSITION;			// ���W(���[�J�����)
	float3 norm				: NORMAL;			// �@��(���[�J�����)
	float4 diffuse			: COLOR0;			// �f�B�t���[�Y�J���[
	float4 specular			: COLOR1;			// �X�y�L�����J���[
	float4 uv0		: TEXCOORD0;		// �e�N�X�`�����W
	float4 uv1		: TEXCOORD1;		// �T�u�e�N�X�`�����W
	int4   blendIndices0	: BLENDINDICES0;	// �X�L�j���O�����p Float�^�萔�z��C���f�b�N�X
	float4 blendWeight0		: BLENDWEIGHT0;		// �X�L�j���O�����p�E�G�C�g�l
};

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT
{
	float4 svpos:SV_POSITION;
	float3 pos:POSITION;
	float3 norm:NORMAL;
	float4 uv:TECOORD;
	float4 diff:COLOR0;
	float4 spec:COLOR1;
};

// ��{�p�����[�^
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
	float4		AntiViewportMatrix[4];		// �A���`�r���[�|�[�g�s��
	float4		ProjectionMatrix[4];		// �r���[�@���@�v���W�F�N�V�����s��
	float4		ViewMatrix[3];				// ���[���h�@���@�r���[�s��
	float4		LocalWorldMatrix[3];		// ���[�J���@���@���[���h�s��

	float4		ToonOutLineSize;			// �g�D�[���̗֊s���̑傫��
	float		DiffuseSource;				// �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
	float		SpecularSource;				// �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
	float		MulSpecularColor;			// �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
	float		Padding;
};

// �X�L�j���O���b�V���p�́@���[�J���@���@���[���h�s��
struct DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX
{
	// ���[�J���@���@���[���h�s��
	float4		Matrix[54 * 3];
};

// ���̑��̍s��
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
{
	float4		ShadowMapLightViewProjectionMatrix[3][4];			// �V���h�E�}�b�v�p�̃��C�g�r���[�s��ƃ��C�g�ˉe�s�����Z��������
	float4		TextureMatrix[3][2];								// �e�N�X�`�����W����p�s��
};

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_VS_BASE			: register(b1)
{
	DX_D3D11_VS_CONST_BUFFER_BASE				g_Base;
};

// ���̑��̍s��
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX		: register(b2)
{
	DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX		g_OtherMatrix;
};

// �X�L�j���O���b�V���p�́@���[�J���@���@���[���h�s��
cbuffer cbD3D11_CONST_BUFFER_VS_LOCALWORLDMATRIX	: register(b3)
{
	DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX	g_LocalWorldMatrix;
};

// main�֐�
VS_OUTPUT main(VS_INPUT VSInput)
{
	VS_OUTPUT VSOutput;
	float4 pos = float4(VSInput.pos.xyz, 1);

	// �����̃t���[���̃u�����h�s��̍쐬
	float4 lLocalWorldMatrix[3];
	lLocalWorldMatrix[0] = g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.x + 0] * VSInput.blendWeight0.x;
	lLocalWorldMatrix[1] = g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.x + 1] * VSInput.blendWeight0.x;
	lLocalWorldMatrix[2] = g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.x + 2] * VSInput.blendWeight0.x;

	lLocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.y + 0] * VSInput.blendWeight0.y;
	lLocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.y + 1] * VSInput.blendWeight0.y;
	lLocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.y + 2] * VSInput.blendWeight0.y;

	lLocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.z + 0] * VSInput.blendWeight0.z;
	lLocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.z + 1] * VSInput.blendWeight0.z;
	lLocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.z + 2] * VSInput.blendWeight0.z;

	lLocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.w + 0] * VSInput.blendWeight0.w;
	lLocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.w + 1] * VSInput.blendWeight0.w;
	lLocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.w + 2] * VSInput.blendWeight0.w;
	
	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// ���[�J�����W�̃Z�b�g

	// ���W�v�Z( ���[�J�����r���[���v���W�F�N�V���� )
	float4 lWorldPosition;
	lWorldPosition.x = dot(pos, lLocalWorldMatrix[0]);
	lWorldPosition.y = dot(pos, lLocalWorldMatrix[1]);
	lWorldPosition.z = dot(pos, lLocalWorldMatrix[2]);
	lWorldPosition.w = 1.0f;

	float4 lViewPosition;
	lViewPosition.x = dot(lWorldPosition, g_Base.ViewMatrix[0]);
	lViewPosition.y = dot(lWorldPosition, g_Base.ViewMatrix[1]);
	lViewPosition.z = dot(lWorldPosition, g_Base.ViewMatrix[2]);
	lViewPosition.w = 1.0f;

	VSOutput.svpos.x = dot(lViewPosition, g_Base.ProjectionMatrix[0]);
	VSOutput.svpos.y = dot(lViewPosition, g_Base.ProjectionMatrix[1]);
	VSOutput.svpos.z = dot(lViewPosition, g_Base.ProjectionMatrix[2]);
	VSOutput.svpos.w = dot(lViewPosition, g_Base.ProjectionMatrix[3]);

	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )

	// �@�����r���[��Ԃ̊p�x�ɕϊ� =====================================================( �J�n )

	float3 lWorldNrm;
	// �]�@���A�ڐ��A�@�����r���[�x�N�g���ɕϊ�
	lWorldNrm.x = dot(VSInput.norm, lLocalWorldMatrix[0].xyz);
	lWorldNrm.y = dot(VSInput.norm, lLocalWorldMatrix[1].xyz);
	lWorldNrm.z = dot(VSInput.norm, lLocalWorldMatrix[2].xyz);

	float3 lViewNrm;
	lViewNrm.x = dot(lWorldNrm, g_Base.ViewMatrix[0].xyz);
	lViewNrm.y = dot(lWorldNrm, g_Base.ViewMatrix[1].xyz);
	lViewNrm.z = dot(lWorldNrm, g_Base.ViewMatrix[2].xyz);

	// �@�����r���[��Ԃ̊p�x�ɕϊ� =====================================================( �I�� )


	// �o�̓p�����[�^�Z�b�g ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// �e�N�X�`�����W�ϊ��s��ɂ��ϊ����s�������ʂ̃e�N�X�`�����W���Z�b�g
	VSOutput.uv.x = dot(VSInput.uv0, g_OtherMatrix.TextureMatrix[0][0]);
	VSOutput.uv.y = dot(VSInput.uv0, g_OtherMatrix.TextureMatrix[0][1]);

	// ���_���W��ۑ�
	VSOutput.pos = lViewPosition.xyz;
	// �@����ۑ�
	VSOutput.norm = lViewNrm;


	// �o�̓p�����[�^�Z�b�g ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )


	// �o�̓p�����[�^��Ԃ�
	return VSOutput;
}
