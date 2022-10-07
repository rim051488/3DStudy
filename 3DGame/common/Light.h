#pragma once
#include "Vector3.h"
struct DirectionLight
{
	Vector3 direction;//���C�g�̕����B�R�v�f�̃x�N�g���ŕ\�������
	float pading;//�l�ߕ�
	Vector3 color;//���C�g�̃J���[�B���̂R���FRGB�ŕ\�����
	float pading1;//�l�ߕ�
	Vector3 eyePos;//���_�̈ʒu
};

struct PointLight
{
	Vector3 position;//���C�g�̈ʒu�B�R�v�f�̃x�N�g���ŕ\�������
	float pading1;//�l�ߕ�
	Vector3 color;//���C�g�̃J���[�B���̂R���FRGB�ŕ\�����
	float pading2;//�l�ߕ�
	float influenceRange;//�e���͈́B�P�ʂ̓��[�g��
};
struct SpotLight
{
	Vector3 position;//���C�g�̈ʒu�B�R�v�f�̃x�N�g���ŕ\�������
	float pading1;//�l�ߕ�
	Vector3 color;//���C�g�̃J���[�B���̂R���FRGB�ł���킳���
	float pading2;//�l�ߕ�
	Vector3 direction;//���˕����B�R�v�f�̃x�N�g���ł���킳���
	float pading3;//�l�ߕ�
	float angle;//���ˊp�x
	float influenceRange;//�e���͈́B�P�ʃ��[�g��
};