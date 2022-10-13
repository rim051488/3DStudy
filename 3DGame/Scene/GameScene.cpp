#include <DxLib.h>
#include "GameScene.h"
#include "../Obj/Player.h"
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
    player_ = std::make_shared<Player>();

    //x = screenSize_.x / 2;
    //y = screenSize_.y / 2;
    //z = 750.0f;
    ////z = -550.0f;
    // //お試し用シェーダ
    ////ps = LoadPixelShader("DrawPS.pso");
    //// //Lambert用のシェーダ
    //ps = LoadPixelShader("Lambert.pso");
    //// //ToonShader用のシェーダ
    ////ps = LoadPixelShader("ToonShader.pso");

    //vs = LoadVertexShader("DrawVS.vso");
    // //ライトは斜め上からあたっている
    //directionLight_.direction = Vector3{ -1.0f,-1.0f,1.0f };
    // //正規化する
    //directionLight_.direction.Normalized();
    // //詰め物
    //directionLight_.pading = 0.0;
    // //ライトのカラーは灰色
    //directionLight_.color = Vector3{1.0f,1.0f,1.0f};
    // //定数バッファの確保
    //cbuff = CreateShaderConstantBuffer(sizeof(Vector3) * 4);
    //direction_ = static_cast<Vector3*>(GetBufferShaderConstantBuffer(cbuff));
    //color_ = static_cast<Vector3*>(GetBufferShaderConstantBuffer(cbuff));
    //model_handl = MV1LoadModel("./Resource/Model/sphere.mv1");
    ////model_handl = MV1LoadModel("./Resource/Model/kuruma.mv1");
    ////model_handl = MV1LoadModel("./Resource/Model/OM01.mv1");
    //toonMap_ = LoadGraph("./Resource/Model/ToonMap.png");
    //MV1SetPosition(model_handl, VGet(x, y, z));
    //SetUseVertexShader(vs);
    //SetUsePixelShader(ps);
    //SetUseTextureToShader(1, toonMap_);
    //SetUseZBuffer3D(true);
    //SetWriteZBuffer3D(true);
    // //同じモデルを複数使う場合はこっちを使う
    ////model_handl_copy = MV1DuplicateModel(model_handl);
    //auto tlNum = MV1GetTriangleListNum(model_handl);
    //tlbertType = -1;
    //for (int i = 0; i < tlNum; ++i)
    //{
    //    tlbertType = MV1GetTriangleListVertexType(model_handl, i);
    //    break;
    //}
    return true;
}

uniqueScene GameScene::Update(float delta, uniqueScene ownScene)
{
    //angle += 0.01f;
    //controller_->Update(delta);
    //if (controller_->Press(InputID::Left))
    //{
    //    x -= 1;
    //}
    //if (controller_->Press(InputID::Right))
    //{
    //    x += 1;
    //}
    //if (controller_->Press(InputID::Up))
    //{
    //    z += 1;
    //}
    //if (controller_->Press(InputID::Down))
    //{
    //    z -= 1;
    //}
    //if (CheckHitKey(KEY_INPUT_Z))
    //{
    //    y += 1;
    //}
    //if (CheckHitKey(KEY_INPUT_X))
    //{
    //    y -= 1;
    //}
    //MV1SetPosition(model_handl, VGet(x, y, z));
    player_->Update(delta);
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
    player_->Draw();
}

void GameScene::DrawGameEnd(float delta)
{
}

void GameScene::Relese(void)
{
    //DeleteShaderConstantBuffer(cbuff);
    //DeleteShader(vs);
    //DeleteShader(ps);
}
