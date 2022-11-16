#include <DxLib.h>
#include <array>
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
    cbufferVS = CreateShaderConstantBuffer(sizeof(LIGHT_MATRIX)*4);
    lightMat_ = static_cast<LIGHT_MATRIX*>(GetBufferShaderConstantBuffer(cbufferVS));
    //cbufferPS = CreateShaderConstantBuffer(sizeof(FLOAT4));
    lightM_.view = MGetIdent();
    lightM_.projection = MGetIdent();

    // �e�p�̃V�F�[�_��������
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
    ShadowMap_ = MakeScreen(screenSize_.x, screenSize_.y, false);
    // �ݒ�����ɖ߂�
    SetDrawValidFloatTypeGraphCreateFlag(false);
    SetCreateDrawValidGraphChannelNum(4);
    SetCreateGraphColorBitDepth(32);
    // �|�X�g�G�t�F�N�g�p�̕ϐ��̏�����
    PostTex_ = MakeScreen(screenSize_.x, screenSize_.y, false);
    PostPS_ = LoadPixelShader("Shader/PostEffect/mono.pso");
    sideBlur_ = MakeScreen(screenSize_.x / 2, screenSize_.y, false);
    vertBlur_ = MakeScreen(screenSize_.x / 2, screenSize_.y / 2, false);
    // �g�[���p�̉摜���Z�b�g
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
            v.dif = GetColorU8(0xff, 0xff, 0xff, 0xff); // �f�B�t���[�Y
            v.spc = GetColorU8(255, 255, 255, 255);		// �X�y�L����
            v.su = 0.0f;
            v.sv = 0.0f;
            v.pos.z = 0.0f;
        }
        // ����
        verts[0].pos.x = x;
        verts[0].pos.y = y;
        verts[0].u = 0.0f;
        verts[0].v = 0.0f;
        // �E��
        verts[1].pos.x = x + width;
        verts[1].pos.y = y;
        verts[1].u = 1.0f;
        verts[1].v = 0.0f;
        // ����
        verts[2].pos.x = x;
        verts[2].pos.y = y + height;
        verts[2].u = 0.0f;
        verts[2].v = 1.0f;
        // �E��
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
    SetDrawMode(DX_DRAWMODE_BILINEAR);
    SetUseTextureToShader(3, toonMap_);
    SetDrawMode(DX_DRAWMODE_NEAREST);
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(true);
}

void GameScene::SetupShadowMap(void)
{
    // �`�����e�p�[�x�L�^�摜�ɂ���
    SetDrawScreen(ShadowMap_);
    //SetDrawScreen(screenID_);
    // �e�p�[�x�L�^�摜��^�����ɂ���
    SetBackgroundColor(255, 255, 255);
    ClearDrawScreen();
    SetBackgroundColor(0, 0, 0);
     //�J�����̃^�C�v�𐳎ˉe�^�C�v�Z�b�g�A�`��͈͂��w��
    SetupCamera_Ortho(offsetOrtho);
     //�`�悷�鉜�s�͈͂��Z�b�g
    SetCameraNearFar(offsetNear, offsetFar);

    // �J�����̌����̓��C�g�̌���
    LightDirecion_ = GetLightDirection();

    auto light = GetLightDirection();
    auto lightPos = VAdd(VGet(camTar.x, camTar.y, camTar.z), VScale(light, -2000));
    SetCameraPositionAndTarget_UpVecY(lightPos, VGet(camTar.x, camTar.y, camTar.z));
    //auto lightPos = VAdd(VGet(-500, 600.0f, -1000.0f), VScale(light, -500));
    //SetCameraPositionAndTarget_UpVecY(lightPos, VGet(0.0f, 0.0f, 0.0f));

    MV1SetUseOrigShader(true);
    // �[�x�l�ւ̕`��p�̃s�N�Z���V�F�[�_���Z�b�g
    SetUsePixelShader(ps_[0]);
    // �[�x�L�^�摜�ւ̍��̃��b�V���`��p�̒��_�V�F�[�_���Z�b�g
    SetUseVertexShader(vs_[0]);
    // �X�e�[�W��`��
    MV1DrawModel(stage_);
    // �[�x�n�L�^�摜�ւ̃X�L�j���O���b�V���`��p�̒��_�V�F�[�_���Z�b�g
    SetUseVertexShader(vs_[1]);
    MV1DrawModel(model_);
    MV1SetUseOrigShader(false);
    // ���C�g�̍��W��������Ă���(�J���������C�g�̏��������Ă���)
    lightM_.view = GetCameraViewMatrix();
    lightM_.projection = GetCameraProjectionMatrix();

    // �`���𗠉�ʂɖ߂�
    SetDrawScreen(PostTex_);
    ClsDrawScreen();
}

void GameScene::DrawOffScreen(void)
{
    // �J�����̐ݒ���s��
    SetCameraPositionAndTarget_UpVecY(VGet(pos_.x + cPos_.x, pos_.y + cPos_.y + 100, pos_.z + cPos_.z - 300), VGet(pos_.x, pos_.y, pos_.z));

    //auto light = VScale(GetLightDirection(), 2);
    //auto lightPos = VAdd(VGet(pos_.x, pos_.y, pos_.z), VScale(light, -400));
    //SetCameraPositionAndTarget_UpVecY(lightPos, VGet(pos_.x, pos_.y, pos_.z));

    MV1SetUseOrigShader(true);
    // �[�x�L�^��ʂ��g�����e�{�f�B���N�V���i�����C�g�P�`��p�̃s�N�Z���V�F�[�_���Z�b�g
    SetUsePixelShader(ps_[1]);
    
    // (DirectX9����Ȃ��ƓK������Ȃ�)�e�p�[�x�L�^�摜��`�悵�����̃J�����̃r���[�s��Ǝˉe�s���萔�ɐݒ肷��
    //SetVSConstFMtx(43, LightCamera_ViewMatrix);
    //SetVSConstFMtx(47, LightCamera_ProjectionMatrix);
    lightMat_[0] = lightM_;
    UpdateShaderConstantBuffer(cbufferVS);
    SetShaderConstantBuffer(cbufferVS, DX_SHADERTYPE_VERTEX, 4);

    // �e�p�[�x�L�^�摜���e�N�X�`���P�ɃZ�b�g
    SetUseTextureToShader(1, ShadowMap_);
    // �X�e�[�W�̕`��
    SetUseVertexShader(vs_[2]);
    MV1DrawModel(stage_);
    // ���f���̕`��
    SetUseVertexShader(vs_[3]);
    MV1DrawModel(model_);
    MV1SetUseOrigShader(false);
    // �g�����e�N�X�`��������
    SetUseTextureToShader(1, -1);
    // �ݒ肵���萔������
    DrawFilde();
    DrawAxis();

    SetDrawScreen(screenID_);
    ClsDrawScreen();
}


void GameScene::Render_Process()
{
    // �e�p�̐[�x�L�^�摜�̏������s��
    SetupShadowMap();
    // �e�p�̐[�x�L�^�摜���g�����e�𗎂Ƃ��������܂߂����f���̕`��
    DrawOffScreen();    
    // �|�X�g�G�t�F�N�g�������邩�ǂ���
    SetUpPostEffect(true, 0, 0, PostTex_, PostPS_);
    DrawRotaGraph(133, 83, 0.25, 0.0, ShadowMap_, true);

    SetUpPostEffect(true, 0, 0, PostTex_, PostPS_);
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
