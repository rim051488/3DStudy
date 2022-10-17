#include <DxLib.h>
#include "GameScene.h"
#include "../common/ImageMng.h"

GameScene::GameScene()
{
    SceneFlag_ = false;
    Init();
}

GameScene::~GameScene()
{
    Relese();
}

bool GameScene::Init(void)
{
    // ゲームに必要な部分を初期化
    InitGame();

    // スクリーン周りを初期化
    InitScreen();

    return true;
}

bool GameScene::InitScreen(void)
{
    return true;
}

bool GameScene::InitGame(void)
{

    return true;
}

uniqueScene GameScene::Update(float delta, uniqueScene ownScene)
{
    DrawOwnScreen(delta);
    return ownScene;
}

void GameScene::DrawOwnScreen(float delta)
{
    DrawGame(delta);
}

uniqueScene GameScene::UpdateStart(float delta, uniqueScene ownScene)
{
    return uniqueScene();
}

uniqueScene GameScene::GameUpdate(float delta, uniqueScene ownScene)
{
    return uniqueScene();
}

uniqueScene GameScene::UpdateGameEnd(float delta, uniqueScene ownScene)
{
    return uniqueScene();
}

void GameScene::DrawGame(float delta)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    SetBackgroundColor(255, 255, 255);
}

void GameScene::DrawGameEnd(float delta)
{
}

void GameScene::Relese(void)
{
}
