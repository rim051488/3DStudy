#include <DxLib.h>
#include "Pad.h"
#include "InputConfig.h"
#include "../common/Debug.h"

Pad::Pad()
{
	std::fill(std::begin(state_.Buttons), std::end(state_.Buttons), 0);
	padState_ = 0;
	Init();
}

Pad::~Pad()
{
}

bool Pad::Init(void)
{
	return true;
}

void Pad::Update(double delta)
{
	padState_ = GetJoypadInputState(DX_INPUT_PAD1);
	GetJoypadDirectInputState(DX_INPUT_PAD1, &state_);

	for (auto& move : lpConfigMng.GetJoypadInputMove())
	{
		data_[static_cast<size_t>(move.first)].second = data_[static_cast<size_t>(move.first)].first;

		data_[static_cast<size_t>(move.first)].first = (padState_ & move.second) != 0;
	}


	for (auto& code : lpConfigMng.GetJoypadInputCode())
	{
		data_[static_cast<size_t>(code.first)].second = data_[static_cast<size_t>(code.first)].first;
		data_[static_cast<size_t>(code.first)].first = state_.Buttons[code.second];
	}
}
