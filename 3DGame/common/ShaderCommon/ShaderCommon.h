#pragma once

// �f�[�^�^��`
typedef float SHADER_FLOAT;
typedef float SHADER_FLOAT2[2];
typedef float SHADER_FLOAT3[3];
typedef float SHADER_FLOAT4[4];

typedef int SHADER_INT;
typedef int SHADER_INT2[2];
typedef int SHADER_INT3[3];
typedef int SHADER_INT4[4];

#define CONST_LIGHT_NUM			(6)				//���ʃp�����[�^�̃��C�g�̍ő吔

// �\���̒�`
// �}�e���A���p�����[�^
struct CONST_MATERIAL
{
	SHADER_FLOAT4		Diffuse;				//�f�B�t���[�Y�J���[
	SHADER_FLOAT4		Specular;				//�X�y�L�����J���[
	SHADER_FLOAT4		Ambient_Emissive;		//�}�e���A��

	SHADER_FLOAT		Power;					//�X�y�L�����̋���
	SHADER_FLOAT3		Padding;				//�p�f�B���O
};
//�t�H�O�̃p�����[�^
struct CONST_FOG
{
	SHADER_FLOAT		LinearAdd;				//�t�H�O�p�p�����[�^ end /(end-start)
	SHADER_FLOAT		LinearDiv;				//�t�H�O�p�p�����[�^ -1 /(end-start)
	SHADER_FLOAT		Density;				//�t�H�O�p�p�����[�^ density
	SHADER_FLOAT		E;						//�t�H�O�p�p�����[�^ ���R�ΐ��̒�

	SHADER_FLOAT4		Color;					//�J���[
};
//���C�g�p�����[�^
struct CONST_LIGHT
{
	SHADER_INT			Type;					//���C�g�^�C�v(DX_LIGHTTYPE_POINT �Ȃ�)
	SHADER_INT3			Padding1;				//�p�f�B���O�P

	SHADER_FLOAT3		Position;				//���W(�r���[���)
	SHADER_FLOAT		RangePow2;				//�L��������2��

	SHADER_FLOAT3		Direction;				//����(�r���[���)
	SHADER_FLOAT		FallOff;				//�X�|�b�g���C�g�pFallOff

	SHADER_FLOAT3		Diffuse;				//�f�B�t���[�Y�J���[
	SHADER_FLOAT		SpotParam0;				//�X�|�b�g���C�g�p�p�����[�^0(cos(Phi/2.0f))

	SHADER_FLOAT3		Specular;				//�X�y�L�����J���[
	SHADER_FLOAT		SpotParam1;				//�X�|�b�g���C�g�p�p�����[�^1(1.0f/(cos(Theta/2.0f) - cos(Phi / 2.0f)))

	SHADER_FLOAT4		Ambient;				//�A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������

	SHADER_FLOAT		Attenuation0;			//�����ɂ�錸�������p�p�����[�^0
	SHADER_FLOAT		Attenuation1;			//�����ɂ�錸�������p�p�����[�^1
	SHADER_FLOAT		Attenuation2;			//�����ɂ�錸�������p�p�����[�^2
	SHADER_FLOAT		Padding2;				//�p�f�B���O2
};
//�s�N�Z���V�F�[�_�E���_�V�F�[�_���ʃp�����[�^
struct CONST_BUFFER_COMMON
{
	CONST_LIGHT		Light[CONST_LIGHT_NUM];		//���C�g�̃p�����[�^
	CONST_MATERIAL	Material;					//�}�e���A���p�����[�^
	CONST_FOG		Fog;						//�t�H�O�p�����[�^
};