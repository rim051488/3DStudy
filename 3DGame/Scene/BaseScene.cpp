#include <DxLib.h>
#include <string>
#include "BaseScene.h"
#include "../Input/Keyboard.h"
#include "../Input/Pad.h"

BaseScene::BaseScene()
{
    SetDrawScreen(DX_SCREEN_BACK);
    GetDrawScreenSize(&screenSize_.x, &screenSize_.y);
    screenID_ = MakeScreen(screenSize_.x, screenSize_.y, true);
    controller_ = std::make_unique<Keyboard>();

}

BaseScene::~BaseScene()
{
    DeleteGraph(screenID_);
}

void BaseScene::Draw(float delta)
{
    DrawGraph(0, 0, screenID_, true);
}
