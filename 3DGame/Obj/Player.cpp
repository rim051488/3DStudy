#include <DxLib.h>
#include "Player.h"
#include "../Input/Keyboard.h"
Player::Player()
{
    controller_ = std::make_unique<Keyboard>();
    Init();
}

Player::~Player()
{
    DeleteShaderConstantBuffer(cbuff);
    DeleteShader(vs);
    DeleteShader(ps);
}

bool Player::Init(void)
{
    pos_ = Vector3(0, 0, 0);
    angle = 0.0f; 
    
    // お試し用シェーダ
    //ps = LoadPixelShader("ShaderPolygon3DTestPS.pso");
    //ps = LoadPixelShader("DrawPS.pso");
    // Lambert用のシェーダ
    ps = LoadPixelShader("Lambert.pso");
    // ToonShader用のシェーダ
    //ps = LoadPixelShader("ToonShader.pso");
    // Phong用のシェーダ
    //ps = LoadPixelShader("Phong.pso");

    vs = LoadVertexShader("s.vso");
    //vs = LoadVertexShader("ns4.vso");
    //vs = LoadVertexShader("DrawVS.vso");
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
    directionLight_.eyePos = {1000.0f,500.0f,1.0f};
    // カメラのセットアップ
    cPos_ = { 0,100,0 };
    cTarget_ = { 0,100,0 };
    cRot_ = { 0,0,0 };
    // 定数バッファの確保
    cbuff = CreateShaderConstantBuffer(sizeof(DirectionLight) * 4);
    direction_ = static_cast<DirectionLight*>(GetBufferShaderConstantBuffer(cbuff));
    //model_handl = MV1LoadModel("./Resource/Model/sphere.mv1");
    //model_handl = MV1LoadModel("./Resource/Model/player_model.mv1");
    //model_handl = MV1LoadModel("./Resource/Model/mc.mv1");
    model_handl = MV1LoadModel("./Resource/Model/OM01.mv1");
    //model_handl = MV1LoadModel("./Resource/Model/ki.mv1");
    toonMap_ = LoadGraph("./Resource/Model/ToonMap.png");
    MV1SetPosition(model_handl, VGet(pos_.x, pos_.y, pos_.z));
    SetUseVertexShader(vs);
    SetUsePixelShader(ps);
    SetUseTextureToShader(1, toonMap_);
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(true);
    auto tlNum = MV1GetTriangleListNum(model_handl);
    tlbertType = -1;
    for (int i = 0; i < tlNum; ++i)
    {
        tlbertType = MV1GetTriangleListVertexType(model_handl, i);
        break;
    }
    return true;
}

void Player::Update(float delta)
{
    angle += 0.01f;
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
    if (CheckHitKey(KEY_INPUT_Z))
    {
        pos_.y += 1;
    }
    if (CheckHitKey(KEY_INPUT_X))
    {
        pos_.y -= 1;
    }
    if (controller_->MousePress(InputID::Decision))
    {
        pos_.y -= 10;
    }
    MATRIX Rot = MMult(MGetRotX(cRot_.x), MGetRotY(cRot_.y));
    VECTOR offset = VTransform(VGet(0, 100, -150), Rot);
    cPos_ = VAdd(VGet(cTarget_.x, cTarget_.y, cTarget_.z), offset);
}

void Player::Draw(void)
{
    SetBackgroundColor(128, 128, 128);
    MV1SetPosition(model_handl, VGet(pos_.x, pos_.y, pos_.z));
    SetCameraPositionAndTarget_UpVecY(cPos_, VGet(cTarget_.x, cTarget_.y, cTarget_.z));
    if (tlbertType == DX_MV1_VERTEX_TYPE_1FRAME) {
        DrawString(10, 10, "not normal not skinning", 0xffffff);
    }
    else if (tlbertType == DX_MV1_VERTEX_TYPE_4FRAME) {
        DrawString(10, 10, "not normal use skinning4", 0xffffff);
    }
    else if (tlbertType == DX_MV1_VERTEX_TYPE_8FRAME) {

        DrawString(10, 10, "not normal use skinning8", 0xffffff);
    }
    else if (tlbertType == DX_MV1_VERTEX_TYPE_NMAP_1FRAME) {
        DrawString(10, 10, "use normal not skinning", 0xffffff);
    }
    else if (tlbertType == DX_MV1_VERTEX_TYPE_NMAP_4FRAME) {
        DrawString(10, 10, "use normal use skinning4", 0xffffff);
    }
    else if (tlbertType == DX_MV1_VERTEX_TYPE_NMAP_8FRAME) {
        DrawString(10, 10, "use normal use skinning8", 0xffffff);
    }
    DrawFormatString(10, 25, 0xffffff, "X座標：%d",static_cast<int>(pos_.x));
    // この書き方だと自作シェーダを使うと機能しないから考えること
    SetCameraNearFar(1.0f, 1000.0f);
    // 自作のシェーダを使わない--------------------------------------------------
    //MV1SetUseOrigShader(false);
    //MV1SetRotationXYZ(model_handl, VGet(0, angle, 0));
    //MV1DrawModel(model_handl);
    //// ここまでがシェーダを使わない----------------------------------------------
    // ここからがシェーダを使った物----------------------------------------------
    SetTextureAddressMode(DX_TEXADDRESS_CLAMP);
    direction_[0] = directionLight_;
    UpdateShaderConstantBuffer(cbuff);
    SetShaderConstantBuffer(cbuff, DX_SHADERTYPE_PIXEL, 0);
    MV1SetUseOrigShader(true);
    MV1SetUseZBuffer(model_handl, true);
    MV1SetWriteZBuffer(model_handl, true);
    MV1SetRotationXYZ(model_handl, VGet(0, angle, 0));
    MV1DrawModel(model_handl);
    // ここまでがシェーダを使ったもの---------------------------------------------
}
