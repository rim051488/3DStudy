#include <DxLib.h>
#include "GameScene.h"
#include "../common/ImageMng.h"

// �t�B�[���h�̐��𒆐S���牽�{�`�悷�邩
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
    // �Q�[���ɕK�v�ȕ�����������
    InitGame();

    // �X�N���[�������������
    InitScreen();

    return true;
}

bool GameScene::InitScreen(void)
{
    // �J�����̃Z�b�g�A�b�v
    cPos_ = { 0,0,0 };
    cAngle_ = { 0,70,-100 };
    cRot_ = { 0,0,0 };
    return true;
}

bool GameScene::InitGame(void)
{
    // ���f���̍��W�Ɗp�x�̏�����
    pos_ = Vector3(0, 0, 0);
    angle_ = 0.0f;
    size_ = Vector3{ 20.0f,20.0f,20.0f };
    // ���f���̃Z�b�g
    stage_ = MV1LoadModel("./Resource/Model/Stage2.mv1");
    //stage_ = MV1LoadModel("./Resource/Model/Stage1.mv1");
    model_ = MV1LoadModel("./Resource/Model/OM01.mv1");
    SetUseDirect3DVersion(DX_DIRECT3D_11);
    // ���C�g�̃Z�b�g�A�b�v
    LightSetUp();
    //// �萔�o�b�t�@�̊m��(�����ł���܂�dxlib���̃��C�g���g������)
    //cbuff = CreateShaderConstantBuffer(sizeof(DirectionLight) * 4);
    //direction_ = static_cast<DirectionLight*>(GetBufferShaderConstantBuffer(cbuff));
    ps_[0] = LoadPixelShader("Shader/Shadow/shadowMap.pso");
    ps_[1] = LoadPixelShader("Shader/Shadow/setShadowMap.pso");
    vs_[0] = LoadVertexShader("Shader/Shadow/stage1VS.vso");
    vs_[1] = LoadVertexShader("Shader/Shadow/model1VS.vso");
    vs_[2] = LoadVertexShader("Shader/Shadow/stage2VS.vso");
    vs_[3] = LoadVertexShader("Shader/Shadow/model2VS.vso");
    // �쐬����摜�̃t�H�[�}�b�g�𕂓������_�^�łP�`�����l���A�P�U�r�b�g�ɂ���
    SetDrawValidFloatTypeGraphCreateFlag(true);
    SetCreateDrawValidGraphChannelNum(1);
    SetCreateGraphColorBitDepth(16);
    DepthBufferGraphHandle_ = MakeScreen(screenSize_.x, screenSize_.y ,false);
    // �ݒ�����ɖ߂�
    SetDrawValidFloatTypeGraphCreateFlag(false);
    SetCreateDrawValidGraphChannelNum(4);
    SetCreateGraphColorBitDepth(32);
    // �g�[���p�̉摜���Z�b�g
    toonMap_ = LoadGraph("./Resource/Model/ToonMap.png");
    MV1SetPosition(model_, VGet(pos_.x, pos_.y, pos_.z));
    MV1SetPosition(stage_, VGet(0.0f, 0.0f, 0.0f));
    MV1SetScale(stage_, VGet(1.0f, 1.0f, 1.0f));
    //MV1SetScale(stage_, VGet(0.5f, 0.5f, 0.5f));
    MV1SetScale(model_, VGet(size_.x, size_.y, size_.z));
    ShaderSetUp(model_);
    lightMat_.projection = MGetIdent();
    lightMat_.view = MGetIdent();
    cbufferVS = CreateShaderConstantBuffer(sizeof(LIGHT_MATRIX));
    cbufferPS = CreateShaderConstantBuffer(sizeof(FLOAT4));
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
    if (CheckHitKey(KEY_INPUT_3))
    {
        SetUsePixelShader(toon);
    }
    if (CheckHitKey(KEY_INPUT_A))
    {
        size_.x -= 0.1;
        size_.y -= 0.1;
        size_.z -= 0.1;
    }
    if (CheckHitKey(KEY_INPUT_D))
    {
        size_.x += 0.1;
        size_.y += 0.1;
        size_.z += 0.1;
    }
    MATRIX Rot = MMult(MGetRotX(cRot_.x), MGetRotY(cRot_.y));
    VECTOR offset = VTransform(VGet(0, 100, -150), Rot);
    cPos_ = { (cAngle_.x + offset.x),(cAngle_.y + offset.y),(cAngle_.z + offset.z) };
    MV1SetRotationXYZ(model_, VGet(0, angle_, 0));
    MV1SetScale(model_, VGet(size_.x, size_.y, size_.z));
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
    MV1SetPosition(model_, VGet(pos_.x, pos_.y, pos_.z));

    Render_Process();
    // ���C�g�̈ʒu�ɂ���J����(�{���Ƀ��C�g�̈ʒu�H)
    //auto light = VScale(GetLightDirection(), 2);
    //auto lightPos = VAdd(VGet(cAngle_.x, cAngle_.y, cAngle_.z), VScale(light, -100));
    //SetCameraPositionAndTarget_UpVecY(lightPos, VGet(cAngle_.x, cAngle_.y, cAngle_.z));
    
    // 0,0,0�̈ʒu�̃J����
    //SetCameraPositionAndTarget_UpVecY(VGet(cPos_.x, cPos_.y, cPos_.z), VGet(cAngle_.x, cAngle_.y, cAngle_.z));
    //SetCameraNearFar(1.0f, 1500.0f);

    //SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

    //// �I���W�i���̃��C�g���g��
    ////direction_[0] = directionLight_;
    ////UpdateShaderConstantBuffer(cbuff);
    ////SetShaderConstantBuffer(cbuff, DX_SHADERTYPE_PIXEL, 0);

    //MV1SetUseOrigShader(true);
    //MV1SetUseZBuffer(model_, true);
    //MV1SetWriteZBuffer(model_, true);
    //MV1DrawModel(model_);
    //MV1SetUseOrigShader(false);
    //MV1DrawModel(stage_);
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
    //// ���C�g�͎΂ߏォ�炠�����Ă���
    //directionLight_.direction = Vector3{ -1.0f,-1.0f,1.0f };
    //// ���K������
    //directionLight_.direction.Normalized();
    //// �l�ߕ�
    //directionLight_.pading = 0.0f;
    //directionLight_.pading1 = 0.0f;
    //// ���C�g�̃J���[�͔��F
    //directionLight_.color = Vector3{ 1.0f,1.0f,1.0f };
    //// �J�����̖ڐ�
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
    SetUseTextureToShader(3, toonMap_);
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(true);
}

void GameScene::SetupDepthImage(void)
{
    // �`�����e�p�[�x�L�^�摜�ɂ���
    SetDrawScreen(DepthBufferGraphHandle_);
    //SetDrawScreen(screenID_);
    // �e�p�[�x�L�^�摜��^�����ɂ���
    SetBackgroundColor(255, 255, 255);
    ClearDrawScreen();
    SetBackgroundColor(0, 0, 0);
    // �J�����̃^�C�v�𐳎ˉe�^�C�v�Z�b�g�A�`��͈͂��w��
    //SetupCamera_Ortho(1000);
    // �`�悷�鉜�s�͈͂��Z�b�g
    //SetCameraNearFar(1.0f, 13000.0f);
   
    // �J�����̌����̓��C�g�̌���
    LightDirecion_ = GetLightDirection();

    auto light = GetLightDirection();
    auto lightPos = VAdd(VGet(pos_.x, pos_.y, pos_.z), VScale(light, -500));
    SetCameraPositionAndTarget_UpVecY(lightPos, VGet(pos_.x, pos_.y, pos_.z));

    // �ݒ肵���J�����̃r���[�s��Ǝˉe�\����擾���Ă���
    //LightCamera_ViewMatrix = GetCameraViewMatrix();
    //LightCamera_ProjectionMatrix = GetCameraProjectionMatrix();

    MV1SetUseOrigShader(true);
    // �[�x�l�ւ̕`��p�̃s�N�Z���V�F�[�_���Z�b�g
    SetUsePixelShader(ps_[0]);
    // �[�x�L�^�摜�ւ̍��̃��b�V���`��p�̒��_�V�F�[�_���Z�b�g
    SetUseVertexShader(vs_[0]);
    //// �X�e�[�W��`��
    //MV1DrawModel(stage_);
    // �[�x�n�L�^�摜�ւ̃X�L�j���O���b�V���`��p�̒��_�V�F�[�_���Z�b�g
    SetUseVertexShader(vs_[1]);
    MV1DrawModel(model_);
    MV1SetUseOrigShader(false);
    // ���C�g�̍��W��������Ă���(�J���������C�g�̏��������Ă���)
    LIGHT_MATRIX* p = reinterpret_cast<LIGHT_MATRIX*>(GetBufferShaderConstantBuffer(cbufferVS));
    p->projection = GetCameraViewMatrix();
    //p->projection = lightMat_.projection;
    p->view = GetCameraProjectionMatrix();
    //p->view = lightMat_.view; 
    
    // �`���𗠉�ʂɖ߂�
    SetDrawScreen(screenID_);
}

void GameScene::DrawModelWithDepthShadow(void)
{
    ClsDrawScreen();
    // �J�����̐ݒ���s��
    SetCameraPositionAndTarget_UpVecY(VGet(cPos_.x, cPos_.y, cPos_.z - 200), VGet(cAngle_.x, cAngle_.y, cAngle_.z));
    //auto light = VScale(GetLightDirection(), 2);
    //auto lightPos = VAdd(VGet(pos_.x, pos_.y, pos_.z), VScale(light, -400));
    //SetCameraPositionAndTarget_UpVecY(lightPos, VGet(pos_.x, pos_.y, pos_.z));

 //SetCameraNearFar(1.0f, 13000.0f);
 //SetupCamera_Ortho(5000);

    MV1SetUseOrigShader(true);
    // �[�x�L�^��ʂ��g�����e�{�f�B���N�V���i�����C�g�P�`��p�̃s�N�Z���V�F�[�_���Z�b�g
    SetUsePixelShader(ps_[1]);
    
    // (DirectX9����Ȃ��ƓK������Ȃ�)�e�p�[�x�L�^�摜��`�悵�����̃J�����̃r���[�s��Ǝˉe�s���萔�ɐݒ肷��
    //SetVSConstFMtx(43, LightCamera_ViewMatrix);
    //SetVSConstFMtx(47, LightCamera_ProjectionMatrix);

    UpdateShaderConstantBuffer(cbufferVS);
    SetShaderConstantBuffer(cbufferVS, DX_SHADERTYPE_VERTEX, 4);
    FLOAT4* f = (FLOAT4*)GetBufferShaderConstantBuffer(cbufferPS);
    f->x = 60;
    f->y = 0;
    f->z = 0;
    f->w = 1;
    UpdateShaderConstantBuffer(cbufferPS);
    SetShaderConstantBuffer(cbufferPS, DX_SHADERTYPE_PIXEL, 4);

    // �e�p�[�x�L�^�摜���e�N�X�`���P�ɃZ�b�g
    SetUseTextureToShader(1, DepthBufferGraphHandle_);
    // �X�e�[�W�̕`��
    SetUseVertexShader(vs_[2]);
    MV1DrawModel(stage_);
    // ���f���̕`��
    SetUseVertexShader(vs_[3]);
    MV1DrawModel(model_);
    MV1SetUseOrigShader(false);
    DrawRotaGraph(133, 83, 0.25, 0.0, DepthBufferGraphHandle_, true);
    // �g�����e�N�X�`��������
    SetUseTextureToShader(1, -1);
    // �ݒ肵���萔������
    ResetVSConstF(43, 8);
}

void GameScene::Render_Process()
{
    // �e�p�̐[�x�L�^�摜�̏������s��
    SetupDepthImage();
    // �e�p�̐[�x�L�^�摜���g�����e�𗎂Ƃ��������܂߂����f���̕`��
    DrawModelWithDepthShadow();
}

void GameScene::DrawAxis(void)
{
    // �R�c�̐�����`�悷��
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
