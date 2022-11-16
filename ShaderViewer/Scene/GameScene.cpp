#include <DxLib.h>
#include <array>
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
    size_ = Vector3{ 20.0f,20.0f,20.0f };
    // モデルのセット
    stage_ = MV1LoadModel("./Resource/Model/Stage2.mv1");
    //stage_ = MV1LoadModel("./Resource/Model/Stage1.mv1");
    model_ = MV1LoadModel("./Resource/Model/OM01.mv1");
    SetUseDirect3DVersion(DX_DIRECT3D_11);
    // ライトのセットアップ
    LightSetUp();
    //// 定数バッファの確保(理解できるまでdxlib内のライトを使うこと)
    //cbuff = CreateShaderConstantBuffer(sizeof(DirectionLight) * 4);
    //direction_ = static_cast<DirectionLight*>(GetBufferShaderConstantBuffer(cbuff));
    cbufferVS = CreateShaderConstantBuffer(sizeof(LIGHT_MATRIX)*4);
    lightMat_ = static_cast<LIGHT_MATRIX*>(GetBufferShaderConstantBuffer(cbufferVS));
    //cbufferPS = CreateShaderConstantBuffer(sizeof(FLOAT4));
    lightM_.view = MGetIdent();
    lightM_.projection = MGetIdent();

    // 影用のシェーダを初期化
    ps_[0] = LoadPixelShader("Shader/Shadow/shadowMap.pso");
    ps_[1] = LoadPixelShader("Shader/Shadow/setShadowMap.pso");
    vs_[0] = LoadVertexShader("Shader/Shadow/stage1VS.vso");
    vs_[1] = LoadVertexShader("Shader/Shadow/model1VS.vso");
    vs_[2] = LoadVertexShader("Shader/Shadow/stage2VS.vso");
    vs_[3] = LoadVertexShader("Shader/Shadow/model2VS.vso");

    // 作成する画像のフォーマットを浮動小数点型で１チャンネル、１６ビットにする
    SetDrawValidFloatTypeGraphCreateFlag(true);
    SetCreateDrawValidGraphChannelNum(1);
    SetCreateGraphColorBitDepth(16);
    ShadowMap_ = MakeScreen(screenSize_.x, screenSize_.y, false);
    // 設定を元に戻す
    SetDrawValidFloatTypeGraphCreateFlag(false);
    SetCreateDrawValidGraphChannelNum(4);
    SetCreateGraphColorBitDepth(32);
    // ポストエフェクト用の変数の初期化
    PostTex_ = MakeScreen(screenSize_.x, screenSize_.y, false);
    PostPS_ = LoadPixelShader("Shader/PostEffect/mono.pso");
    sideBlur_ = MakeScreen(screenSize_.x / 2, screenSize_.y, false);
    vertBlur_ = MakeScreen(screenSize_.x / 2, screenSize_.y / 2, false);
    // トーン用の画像をセット
    toonMap_ = LoadGraph("./Resource/Model/ToonMap.png");
    MV1SetPosition(model_, VGet(pos_.x, pos_.y, pos_.z));
    MV1SetPosition(stage_, VGet(0.0f, 0.0f, 0.0f));
    MV1SetScale(stage_, VGet(1.0f, 1.0f, 1.0f));
    //MV1SetScale(stage_, VGet(0.5f, 0.5f, 0.5f));
    MV1SetScale(model_, VGet(size_.x, size_.y, size_.z));
    ShaderSetUp(model_);
    return true;
}

void GameScene::SetUpPostEffect(bool flag, int x, int y, int img, int Postps)
{
    int width, height;
    GetGraphSize(img, &width, &height);
    std::array <VERTEX2DSHADER, 4> verts;

    if (flag)
    {
        for (auto& v : verts)
        {
            v.rhw = 1.0;
            v.dif = GetColorU8(0xff, 0xff, 0xff, 0xff); // ディフューズ
            v.spc = GetColorU8(255, 255, 255, 255);		// スペキュラ
            v.su = 0.0f;
            v.sv = 0.0f;
            v.pos.z = 0.0f;
        }
        // 左上
        verts[0].pos.x = x;
        verts[0].pos.y = y;
        verts[0].u = 0.0f;
        verts[0].v = 0.0f;
        // 右上
        verts[1].pos.x = x + width;
        verts[1].pos.y = y;
        verts[1].u = 1.0f;
        verts[1].v = 0.0f;
        // 左下
        verts[2].pos.x = x;
        verts[2].pos.y = y + height;
        verts[2].u = 0.0f;
        verts[2].v = 1.0f;
        // 右下
        verts[3].pos.x = x + width;
        verts[3].pos.y = y + height;
        verts[3].u = 1.0f;
        verts[3].v = 1.0f;
        SetUsePixelShader(Postps);

        SetUseTextureToShader(0, img);
        DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);
        MV1SetUseOrigShader(false);
    }
    else if (!flag)
    {
        DrawGraph(0, 0, img, true);
    }
    SetUseTextureToShader(0, -1);
}

uniqueScene GameScene::Update(float delta, uniqueScene ownScene)
{
    angle_ += 0.01f;
    controller_->Update(delta);
    if (controller_->Press(InputID::Left))
    {
        cAngle_.x -= 5;
    }
    if (controller_->Press(InputID::Right))
    {
        cAngle_.x += 5;
    }
    if (controller_->Press(InputID::Up))
    {
        cAngle_.z += 5;
    }
    if (controller_->Press(InputID::Down))
    {
        cAngle_.z -= 5;
    }
    if (CheckHitKey(KEY_INPUT_W))
    {
        pos_.z += 5;
    }
    if (CheckHitKey(KEY_INPUT_S))
    {
        pos_.z -= 5;
    }
    if (CheckHitKey(KEY_INPUT_A))
    {
        pos_.x -= 5;
    }
    if (CheckHitKey(KEY_INPUT_D))
    {
        pos_.x += 5;
    }
    if (CheckHitKey(KEY_INPUT_1))
    {
        SetUsePixelShader(tex);
    }
    if (CheckHitKey(KEY_INPUT_2))
    {
        SetUsePixelShader(lam);
    }
    if (CheckHitKey(KEY_INPUT_3))
    {
        SetUsePixelShader(toon);
    }
    MATRIX Rot = MMult(MGetRotX(cRot_.x), MGetRotY(cRot_.y));
    VECTOR offset = VTransform(VGet(0, 100, -150), Rot);
    cPos_ = { (cAngle_.x + offset.x),(cAngle_.y + offset.y),(cAngle_.z + offset.z) };
    MV1SetRotationXYZ(model_, VGet(0, angle_, 0));
    MV1SetScale(model_, VGet(size_.x, size_.y, size_.z));
    MV1SetPosition(model_, VGet(pos_.x, pos_.y, pos_.z));
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
    //SetDrawScreen(screenID_);
    //ClsDrawScreen();
    //SetBackgroundColor(128, 128, 128);

        // ライトの位置にあるカメラ(本当にライトの位置？)
    //auto light = VScale(GetLightDirection(), 2);
    //auto lightPos = VAdd(VGet(cAngle_.x, cAngle_.y, cAngle_.z), VScale(light, -100));
    //SetCameraPositionAndTarget_UpVecY(lightPos, VGet(cAngle_.x, cAngle_.y, cAngle_.z));
    // 0,0,0の位置のカメラ
    //SetCameraPositionAndTarget_UpVecY(VGet(cPos_.x, cPos_.y, cPos_.z), VGet(cAngle_.x, cAngle_.y, cAngle_.z));
    //SetCameraNearFar(1.0f, 1500.0f);
    //SetTextureAddressMode(DX_TEXADDRESS_CLAMP);
    //// オリジナルのライトを使う
    ////direction_[0] = directionLight_;
    ////UpdateShaderConstantBuffer(cbuff);
    ////SetShaderConstantBuffer(cbuff, DX_SHADERTYPE_PIXEL, 0);
    //MV1SetUseOrigShader(true);
    //MV1SetUseZBuffer(model_, true);
    //MV1SetWriteZBuffer(model_, true);
    //MV1DrawModel(model_);
    //MV1SetUseOrigShader(false);
    //MV1DrawModel(stage_);
Render_Process();    ScreenFlip();
}

void GameScene::DrawGameEnd(float delta)
{
}

void GameScene::Relese(void)
{
}

void GameScene::LightSetUp(void)
{
    //// ライトは斜め上からあたっている
    //directionLight_.direction = Vector3{ -1.0f,-1.0f,1.0f };
    //// 正規化する
    //directionLight_.direction.Normalized();
    //// 詰め物
    //directionLight_.pading = 0.0f;
    //directionLight_.pading1 = 0.0f;
    //// ライトのカラーは白色
    //directionLight_.color = Vector3{ 1.0f,1.0f,1.0f };
    //// カメラの目線
    //directionLight_.eyePos = { 1000.0f,500.0f,1.0f };

    SetLightDirection(VGet(1.f, -1.f, 1.f));
    SetLightDifColor(GetColorF(1.f, 1.f, 1.f, 1.f));
    SetLightSpcColor(GetColorF(1.f, 1.f, 1.f, 1.f));
    SetLightAmbColor(GetColorF(0.5f, 0.5f, 0.5f, 1.f));
}

void GameScene::ShaderSetUp(int model)
{
    auto tlNum = MV1GetTriangleListNum(model);
    tlbertType = -1;
    for (int i = 0; i < tlNum; ++i)
    {
        tlbertType = MV1GetTriangleListVertexType(model, i);
        break;
    }
    if (tlbertType == DX_MV1_VERTEX_TYPE_1FRAME) {
        vs = LoadVertexShader("Shader/Vertex/Mesh.vso");
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
        tex = LoadPixelShader("Shader/Pixel/tex.pso");
        lam = LoadPixelShader("Shader/Pixel/k.pso");
        toon = LoadPixelShader("Shader/Pixel/Toon.pso");
    }
    else
    {
        tex = LoadPixelShader("Shader/Pixel/NormTex.pso");
        lam = LoadPixelShader("Shader/Pixel/NormLambert.pso");
        toon = LoadPixelShader("Shader/Pixel/NormToon.pso");
    }
    SetUseVertexShader(vs);
    SetUsePixelShader(tex);
    SetDrawMode(DX_DRAWMODE_BILINEAR);
    SetUseTextureToShader(3, toonMap_);
    SetDrawMode(DX_DRAWMODE_NEAREST);
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(true);
}

void GameScene::SetupShadowMap(void)
{
    // 描画先を影用深度記録画像にする
    SetDrawScreen(ShadowMap_);
    //SetDrawScreen(screenID_);
    // 影用深度記録画像を真っ白にする
    SetBackgroundColor(255, 255, 255);
    ClearDrawScreen();
    SetBackgroundColor(0, 0, 0);
     //カメラのタイプを正射影タイプセット、描画範囲も指定
    SetupCamera_Ortho(offsetOrtho);
     //描画する奥行範囲をセット
    SetCameraNearFar(offsetNear, offsetFar);

    // カメラの向きはライトの向き
    LightDirecion_ = GetLightDirection();

    auto light = GetLightDirection();
    auto lightPos = VAdd(VGet(camTar.x, camTar.y, camTar.z), VScale(light, -2000));
    SetCameraPositionAndTarget_UpVecY(lightPos, VGet(camTar.x, camTar.y, camTar.z));
    //auto lightPos = VAdd(VGet(-500, 600.0f, -1000.0f), VScale(light, -500));
    //SetCameraPositionAndTarget_UpVecY(lightPos, VGet(0.0f, 0.0f, 0.0f));

    MV1SetUseOrigShader(true);
    // 深度値への描画用のピクセルシェーダをセット
    SetUsePixelShader(ps_[0]);
    // 深度記録画像への剛体メッシュ描画用の頂点シェーダをセット
    SetUseVertexShader(vs_[0]);
    // ステージを描画
    MV1DrawModel(stage_);
    // 深度地記録画像へのスキニングメッシュ描画用の頂点シェーダをセット
    SetUseVertexShader(vs_[1]);
    MV1DrawModel(model_);
    MV1SetUseOrigShader(false);
    // ライトの座標をもらっておく(カメラがライトの情報をもっている)
    lightM_.view = GetCameraViewMatrix();
    lightM_.projection = GetCameraProjectionMatrix();

    // 描画先を裏画面に戻す
    SetDrawScreen(PostTex_);
    ClsDrawScreen();
}

void GameScene::DrawOffScreen(void)
{
    // カメラの設定を行う
    SetCameraPositionAndTarget_UpVecY(VGet(pos_.x + cPos_.x, pos_.y + cPos_.y + 100, pos_.z + cPos_.z - 300), VGet(pos_.x, pos_.y, pos_.z));

    //auto light = VScale(GetLightDirection(), 2);
    //auto lightPos = VAdd(VGet(pos_.x, pos_.y, pos_.z), VScale(light, -400));
    //SetCameraPositionAndTarget_UpVecY(lightPos, VGet(pos_.x, pos_.y, pos_.z));

    MV1SetUseOrigShader(true);
    // 深度記録画面を使った影＋ディレクショナルライト１つ描画用のピクセルシェーダをセット
    SetUsePixelShader(ps_[1]);
    
    // (DirectX9じゃないと適応されない)影用深度記録画像を描画した時のカメラのビュー行列と射影行列を定数に設定する
    //SetVSConstFMtx(43, LightCamera_ViewMatrix);
    //SetVSConstFMtx(47, LightCamera_ProjectionMatrix);
    lightMat_[0] = lightM_;
    UpdateShaderConstantBuffer(cbufferVS);
    SetShaderConstantBuffer(cbufferVS, DX_SHADERTYPE_VERTEX, 4);

    // 影用深度記録画像をテクスチャ１にセット
    SetUseTextureToShader(1, ShadowMap_);
    // ステージの描画
    SetUseVertexShader(vs_[2]);
    MV1DrawModel(stage_);
    // モデルの描画
    SetUseVertexShader(vs_[3]);
    MV1DrawModel(model_);
    MV1SetUseOrigShader(false);
    // 使ったテクスチャを解除
    SetUseTextureToShader(1, -1);
    // 設定した定数を解除
    DrawFilde();
    DrawAxis();

    SetDrawScreen(screenID_);
    ClsDrawScreen();
}


void GameScene::Render_Process()
{
    // 影用の深度記録画像の準備を行う
    SetupShadowMap();
    // 影用の深度記録画像を使った影を落とす処理も含めたモデルの描画
    DrawOffScreen();    
    // ポストエフェクトを書けるかどうか
    SetUpPostEffect(true, 0, 0, PostTex_, PostPS_);
    DrawRotaGraph(133, 83, 0.25, 0.0, ShadowMap_, true);

    SetUpPostEffect(true, 0, 0, PostTex_, PostPS_);
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
            VGet((float)i * 20, 0, -400),
            VGet((float)i * 20, 0, 400),
            0x00ff00
        );
        DrawLine3D(
            VGet(-400, 0, (float)i * 20),
            VGet(400, 0, (float)i * 20),
            0x00ff00
        );
    }

}
