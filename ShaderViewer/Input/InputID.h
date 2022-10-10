#pragma once
#include <type_traits>

// �L�[���
enum class InputID
{
	Up,					// ��ړ�
	Down,				// ���ړ�
	Left,				// ���ړ�
	Right,				// �E�ړ�
	Decision,				// ����
	btn1,				// ���j���[
	Max
};

static InputID begin(InputID) { return InputID::Up; }
static InputID end(InputID) { return InputID::Max; }
static InputID operator++(InputID& state) { return (state = static_cast<InputID>(std::underlying_type<InputID>::type(state) + 1)); }
static InputID operator*(const InputID& state) { return state; }
