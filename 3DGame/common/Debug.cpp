#ifdef _DEBUG
#include <DxLib.h>
#include <cassert>
#include <iomanip>
#include "Time.h"
#include "Debug.h"

std::unique_ptr<DebugClass, DebugClass::Deleter> DebugClass::instance_ = nullptr;

void DebugClass::Create(TimeClass& timeClass)
{
	if (instance_ == nullptr)
	{
		// ヌルポインターの時実体を作る
		instance_.reset(new DebugClass(timeClass));
	}
}

DebugClass& DebugClass::GetInstance(void)
{
	return (*instance_);
}

void DebugClass::SetUp(const int& targetSc)
{
	targetSc_ = targetSc;
	// スクリーンの幅と高さを取得
	SetDrawScreen(DX_SCREEN_BACK);
	GetDrawScreenSize(&scW, &scH);
	screen_ = MakeScreen(scW, scH,true);
}


void DebugClass::DebugDraw(void)
{
	keyOld_ = key_;
	GetHitKeyStateAll(key_.data());
	if (key_[KEY_INPUT_F1] && !keyOld_[KEY_INPUT_F1])
	{
		infoDrawFlag_ = !infoDrawFlag_;
	}
	
	if (key_[KEY_INPUT_F2] && !keyOld_[KEY_INPUT_F2])
	{
		stopFlag_ = true;
	}
	
	SetDrawScreen(screen_);
	ClsDrawScreen();
#ifdef UNICODE
	std::wstringstream tmpss;
#else
	std::stringstream tmpss;
#endif
	if (infoDrawFlag_)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawBox(0, 0, scW, scH, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		tmpss << TEXT("DeltaTime-") << std::setprecision(4) <<  time_.GetDeltaTime<float>() << TEXT("-") << std::endl;
		tmpss << TEXT("GameTime-") << std::setprecision(6) << time_.GetGameTime<double>() << TEXT("-") << std::endl;
		tmpss << TEXT("Fps-") << std::setprecision(4) << GetFPS() << TEXT("-") << std::endl;
		tmpss << TEXT("DorawCall-") << GetDrawCallCount() << TEXT("-") << std::endl;
		DrawFormatString(0, 0, 0xffffff, tmpss.str().c_str());
		SetDrawScreen(targetSc_);
		DrawGraph(0, 0, screen_, true);
	}
	
	if (stopFlag_)
	{
		WaitKey();
		stopFlag_ = false;
	}
}



DebugClass::DebugClass(TimeClass& timeClass) :
	time_{timeClass}
{
	// デバッグ出力用のコンソールを生成
	AllocConsole();

	// 標準出力で出力できるようにする
	conFp_ = nullptr;
	freopen_s(&conFp_, "CONOUT$", "w", stdout);
	freopen_s(&conFp_, "CONIN$", "r", stdin);

	
	screen_ = DX_SCREEN_BACK;
	targetSc_= DX_SCREEN_BACK;

	key_.fill(0);
	keyOld_.fill(0);
	infoDrawFlag_ = false;
	stopFlag_ = false;

	scH = 0;
	scW = 0;

}

DebugClass::~DebugClass()
{

	fclose(conFp_);

}

#endif

