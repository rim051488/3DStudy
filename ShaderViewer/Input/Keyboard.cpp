#include <DxLib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Keyboard.h"
#include "InputConfig.h"

Keyboard::Keyboard()
{
	keyData_.fill(0);
	Init();
}

Keyboard::~Keyboard()
{
}

bool Keyboard::Init(void)
{
	return true;
}

void Keyboard::Update(double delta)
{
	GetHitKeyStateAll(keyData_.data());		// キーデータをまとめて取得
	for (auto& code : lpConfigMng.GetKeyInputCode())
	{
		data_[static_cast<size_t>(code.first)].second = data_[static_cast<size_t>(code.first)].first;
		data_[static_cast<size_t>(code.first)].first = keyData_[code.second];
	}
}
