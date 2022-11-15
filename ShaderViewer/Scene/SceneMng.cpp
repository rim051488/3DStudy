#include <DxLib.h>
#include <memory>
#include <chrono>
#include "SceneMng.h"
#include "GameScene.h"
#include "../common/Debug.h"
#include "../Input/InputConfig.h"

using namespace std;

constexpr Vector2I screenSize{ 1280, 720 };

void SceneMng::Run(void)
{
	if (!InitFlag_)
	{
		// 確認のためにもう一度イニシャライズする
		if (!SysInit())
		{
			return;
		}
	}
	CreateMaskScreen();
	scene_ = std::make_unique<GameScene>();

	time_.DeltaTimeStart();
	time_.DeltaTimeEnd();
	time_.GameTimeEnd();
	auto flag = scene_->GetFlag();
	while (ProcessMessage() == 0)
	{
		time_.DeltaTimeStart();
		scene_ = scene_->Update(time_.GetDeltaTime<float>(), std::move(scene_));
		time_.GameTimeEnd();

		Draw(time_.GetDeltaTime<float>());

		flag = scene_->GetFlag();
		time_.DeltaTimeEnd();
	}
	InitGraph();
	InputConfig::Destroy();
	DxLib_End();
}

const Vector2I& SceneMng::GetScreenSize(void)
{
	return screenSize;
}

bool SceneMng::SysInit(void)
{
	DebugStart(time_);
	SetWindowText("3DDrawing");
	SetGraphMode(screenSize.x, screenSize.y, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return false;
	}
	InputConfig::Create();
	DebugSetUp();
	return true;
}

void SceneMng::Update(void)
{
}

void SceneMng::Draw(float delta)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	scene_->Draw(delta);
	DebugInfoDraw();
	ScreenFlip();
}

SceneMng::SceneMng()
{
	InitFlag_ = SysInit();
}

SceneMng::~SceneMng()
{
}
