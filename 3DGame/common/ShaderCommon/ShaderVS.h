#pragma once
#include "ShaderCommon.h"

#define CONST_TEXTUER_MATRIX_NUM			3			//�e�N�X�`�����W�ϊ��s��̓]�u�s��̐�
#define CONST_WORLD_MAT_NUM					54			//�g���C�A���O�����X�g1�œ����Ɏg�p���郍�[�J�������[���h�s��̍ő吔

// ��{�p�����[�^
struct CONST_BUFFER_BASE
{
	SHADER_FLOAT4		AntiViewportMatrix[4];			//�A���`�r���[�|�[�g�s��
};