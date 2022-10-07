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
    pos_ = Vector3(530.0f, 330.0f, 400.0f);
    //pos_ = Vector3(530.0f, 330.0f, 750.0f);
    angle = 0.0f;
    //z = -550.0f;
    // �������p�V�F�[�_
    //ps = LoadPixelShader("DrawPS.pso");
    // Lambert�p�̃V�F�[�_
    //ps = LoadPixelShader("Lambert.pso");
    // ToonShader�p�̃V�F�[�_
    //ps = LoadPixelShader("ToonShader.pso");
    // Phong�p�̃V�F�[�_
    ps = LoadPixelShader("Phong.pso");

    vs = LoadVertexShader("DrawVS.vso");
    // ���C�g�͎΂ߏォ�炠�����Ă���
    directionLight_.direction = Vector3{ -1.0f,-1.0f,1.0f };
    // ���K������
    directionLight_.direction.Normalized();
    // �l�ߕ�
    directionLight_.pading = 0.0f;
    directionLight_.pading1 = 0.0f;
    // ���C�g�̃J���[�͔��F
    directionLight_.color = Vector3{ 1.0f,1.0f,1.0f };
    // �J�����̖ڐ�
    directionLight_.eyePos = {1000.0f,500.0f,1.0f};
    // �萔�o�b�t�@�̊m��
    cbuff = CreateShaderConstantBuffer(sizeof(DirectionLight) * 4);
    direction_ = static_cast<DirectionLight*>(GetBufferShaderConstantBuffer(cbuff));
    //model_handl = MV1LoadModel("./Resource/Model/sphere.mv1");
    model_handl = MV1LoadModel("./Resource/Model/mc.mv1");
    //model_handl = MV1LoadModel("./Resource/Model/OM01.mv1");
    toonMap_ = LoadGraph("./Resource/Model/ToonMap.png");
    MV1SetPosition(model_handl, VGet(pos_.x, pos_.y, pos_.z));
    SetUseVertexShader(vs);
    SetUsePixelShader(ps);
    SetUseTextureToShader(1, toonMap_);
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(true);
    // �������f���𕡐��g���ꍇ�͂��������g��
    //model_handl_copy = MV1DuplicateModel(model_handl);
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
    MV1SetPosition(model_handl, VGet(pos_.x, pos_.y, pos_.z));
}

void Player::Draw(void)
{
    if (tlbertType == DX_MV1_VERTEX_TYPE_1FRAME) {
        DrawString(10, 10, "not normal not skinning", 0xffffff);
    }
    else if (tlbertType == DX_MV1_VERTEX_TYPE_4FRAME) {
        DrawString(10, 10, "not normal use skinning", 0xffffff);
    }
    else if (tlbertType == DX_MV1_VERTEX_TYPE_NMAP_1FRAME) {
        DrawString(10, 10, "use normal not skinning", 0xffffff);
    }
    else if (tlbertType == DX_MV1_VERTEX_TYPE_NMAP_4FRAME) {
        DrawString(10, 10, "use normal use skinning", 0xffffff);
    }
    // ���̏��������Ǝ���V�F�[�_���g���Ƌ@�\���Ȃ�����l���邱��
    //SetCameraNearFar(1.0f, 1000.0f);
    // ����̃V�F�[�_���g��Ȃ�--------------------------------------------------
    //MV1SetUseOrigShader(false);
    //MV1SetRotationXYZ(model_handl, VGet(0, angle, 0));
    //MV1DrawModel(model_handl);
    //// �����܂ł��V�F�[�_���g��Ȃ�----------------------------------------------
    // �������炪�V�F�[�_���g������----------------------------------------------
    SetBackgroundColor(128, 128, 128);
    SetTextureAddressMode(DX_TEXADDRESS_CLAMP);
    direction_[0] = directionLight_;
    UpdateShaderConstantBuffer(cbuff);
    SetShaderConstantBuffer(cbuff, DX_SHADERTYPE_PIXEL, 0);
    MV1SetUseOrigShader(true);
    MV1SetUseZBuffer(model_handl, true);
    MV1SetWriteZBuffer(model_handl, true);
    MV1SetRotationXYZ(model_handl, VGet(0, angle, 0));
    MV1DrawModel(model_handl);
    // �����܂ł��V�F�[�_���g��������---------------------------------------------
}