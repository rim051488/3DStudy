#include <DxLib.h>
#include "GameScene.h"
#include "../common/ImageMng.h"

// フィールドの線を中心から何本描画するか
#define FIELD_EXTEND  (20)

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
    // カメラのセットアップ
    cPos_ = { 0,0,0 };
    cAngle_ = { 0,70,-100 };
    cRot_ = { 0,0,0 };
    return true;
}

bool GameScene::InitGame(void)
{
    // モデルの座標と角度の初期化
    pos_ = Vector3(0, 0, 0);
    angle_ = 0.0f;

    // ライトのセットアップ
    LightSetUp();
    // 定数バッファの確保
    cbuff = CreateShaderConstantBuffer(sizeof(DirectionLight) * 4);
    direction_ = static_cast<DirectionLight*>(GetBufferShaderConstantBuffer(cbuff));
    // モデルの描画準備
    //model_ = MV1LoadModel("./Resource/Model/sphere.mv1");
    //model_ = MV1LoadModel("./Resource/Model/player_model.mv1");
    model_ = MV1LoadModel("./Resource/Model/miku.mv1");
    //model_ = MV1LoadModel("./Resource/Model/OM01.mv1");
    toonMap_ = LoadGraph("./Resource/Model/ToonMap.png");
    MV1SetPosition(model_, VGet(pos_.x, pos_.y, pos_.z));
    ShaderSetUp(model_);
    return true;
}

uniqueScene GameScene::Update(float delta, uniqueScene ownScene)
{
    angle_ += 0.01f;
    controller_->Update(delta);
    if (controller_->Press(InputID::Left))
    {
        pos_.x -= 1;
    }
    if (controller_->Press(InputID::Right))
    {
        pos_.x += 1;
    }
    if (controller_->Press(InputID::Up))
    {
        pos_.z += 1;
    }
    if (controller_->Press(InputID::Down))
    {
        pos_.z -= 1;
    }
    if (CheckHitKey(KEY_INPUT_W))
    {
        pos_.y += 1;
    }
    if (CheckHitKey(KEY_INPUT_S))
    {
        pos_.y -= 1;
    }
    if (CheckHitKey(KEY_INPUT_1))
    {
        SetUsePixelShader(tex);
    }
    if (CheckHitKey(KEY_INPUT_2))
    {
        SetUsePixelShader(lam);
    }
    MATRIX Rot = MMult(MGetRotX(cRot_.x), MGetRotY(cRot_.y));
    VECTOR offset = VTransform(VGet(0, 100, -150), Rot);
    cPos_ = { (cAngle_.x + offset.x),(cAngle_.y + offset.y),(cAngle_.z + offset.z) };
    MV1SetRotationXYZ(model_, VGet(0, angle_, 0));
    //MV1SetScale(model_, VGet(2, 2, 2));
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
    SetBackgroundColor(128, 128, 128);
    MV1SetPosition(model_, VGet(pos_.x, pos_.y, pos_.z));

    SetCameraPositionAndTarget_UpVecY(VGet(cPos_.x, cPos_.y, cPos_.z), VGet(cAngle_.x, cAngle_.y, cAngle_.z));
    SetCameraNearFar(1.0f, 1000.0f);

    SetTextureAddressMode(DX_TEXADDRESS_CLAMP);
    direction_[0] = directionLight_;
    UpdateShaderConstantBuffer(cbuff);
    SetShaderConstantBuffer(cbuff, DX_SHADERTYPE_PIXEL, 0);
    MV1SetUseOrigShader(true);
    MV1SetUseZBuffer(model_, true);
    MV1SetWriteZBuffer(model_, true);
    MV1DrawModel(model_);
    MV1SetUseOrigShader(false);
    DrawFilde();
    DrawAxis();
    ScreenFlip();
}

void GameScene::DrawGameEnd(float delta)
{
}

void GameScene::Relese(void)
{
}

void GameScene::LightSetUp(void)
{
    // ライトは斜め上からあたっている
    directionLight_.direction = Vector3{ -1.0f,-1.0f,1.0f };
    // 正規化する
    directionLight_.direction.Normalized();
    // 詰め物
    directionLight_.pading = 0.0f;
    directionLight_.pading1 = 0.0f;
    // ライトのカラーは白色
    directionLight_.color = Vector3{ 1.0f,1.0f,1.0f };
    // カメラの目線
    directionLight_.eyePos = { 1000.0f,500.0f,1.0f };
}

void GameScene::ShaderSetUp(int model)
{
    auto tlNum = MV1GetTriangleListNum(model_);
    tlbertType = -1;
    for (int i = 0; i < tlNum; ++i)
    {
        tlbertType = MV1GetTriangleListVertexType(model_, i);
        break;
    }
    if (tlbertType == DX_MV1_VERTEX_TYPE_1FRAME) {
    }
    else if (tlbertType == DX_MV1_VERTEX_TYPE_4FRAME) {
        vs = LoadVertexShader("Shader/Vertex/Mesh4.vso");
    }
    else if (tlbertType == DX_MV1_VERTEX_TYPE_8FRAME) {
    }
    else if (tlbertType == DX_MV1_VERTEX_TYPE_NMAP_1FRAME) {
        vs = LoadVertexShader("Shader/Vertex/NormMesh.vso");
    }
    else if (tlbertType == DX_MV1_VERTEX_TYPE_NMAP_4FRAME) {
        vs = LoadVertexShader("Shader/Vertex/NormMesh4.vso");
    }
    else if (tlbertType == DX_MV1_VERTEX_TYPE_NMAP_8FRAME) {

    }
    if (tlbertType < 3)
    {
        tex = LoadPixelShader("Shader/Pixel/Tex.pso");
        lam = LoadPixelShader("Shader/Pixel/Lambert.pso");
    }
    else
    {
        tex = LoadPixelShader("Shader/Pixel/NormTex.pso");
        lam = LoadPixelShader("Shader/Pixel/NormLambert.pso");
    }
    SetUseVertexShader(vs);
    SetUsePixelShader(tex);
    SetUseTextureToShader(1, toonMap_);
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(true);
}

void GameScene::DrawAxis(void)
{
    // ３Ｄの線分を描画する
    DrawLine3D(
        VGet(0, 0, 0), 
        VGet(100, 0, 0),
        0xff0000
    );
    DrawLine3D(
        VGet(0, 0, 0),
        VGet(0, 100, 0),
        0x00ff00
    );
    DrawLine3D(
        VGet(0, 0, 0),
        VGet(0, 0, -100),
        0x0000ff
    );
}

void GameScene::DrawFilde(void)
{
    for (int i = -FIELD_EXTEND; i <= FIELD_EXTEND; i++)
    {
        DrawLine3D(
            VGet((float)i * 10, 0, -200),
            VGet((float)i * 10, 0, 200),
            0x00ff00
        );
        DrawLine3D(
            VGet(-200, 0, (float)i * 10),
            VGet(200, 0, (float)i * 10),
            0x00ff00
        );
    }
}
