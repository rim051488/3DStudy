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
    // �Q�[���ɕK�v�ȕ�����������
    InitGame();

    // �X�N���[�������������
    InitScreen();

    return true;
}

bool GameScene::InitScreen(void)
{
    return true;
}

bool GameScene::InitGame(void)
{
    x = screenSize_.x / 2;
    y = screenSize_.y / 2;
    z = 0.0f;
    ps = LoadPixelShader("DrawPS.pso");
    vs = LoadVertexShader("DrawVS.vso");
    // ���C�g�͎΂ߏォ�炠�����Ă���
    directionLight.direction.x = 1.0f;
    directionLight.direction.y = -1.0f;
    directionLight.direction.z = 0.0f;
    // ���K������
    directionLight.direction.Normalized();
    // ���C�g�̃J���[�͊D�F
    directionLight.color.x = 1.0f;
    directionLight.color.y = 1.0f;
    directionLight.color.z = 0.0f;
    // �萔�o�b�t�@�̊m��
    cbuff = CreateShaderConstantBuffer(sizeof(Vector3) * 4);
    threshold = static_cast<Vector3*>(GetBufferShaderConstantBuffer(cbuff));
    model_handl = MV1LoadModel("./Resource/Model/sphere.mv1");
    MV1SetPosition(model_handl, VGet(x, y, z));
    // �������f���𕡐��g���ꍇ�͂��������g��
    //model_handl_copy = MV1DuplicateModel(model_handl);
    auto tlNum = MV1GetTriangleListNum(model_handl);
    int tlbertType = -1;
    for (int i = 0; i < tlNum; ++i)
    {
        tlbertType = MV1GetTriangleListVertexType(model_handl, i);
        break;
    }
    return true;
}

uniqueScene GameScene::Update(float delta, uniqueScene ownScene)
{
    angle += 0.01f;
    controller_->Update(delta);
    if (controller_->Press(InputID::Left))
    {
        x -= 1;
    }
    if (controller_->Press(InputID::Right))
    {
        x += 1;
    }
    if (controller_->Press(InputID::Up))
    {
        z += 1;
    }
    if (controller_->Press(InputID::Down))
    {
        z -= 1;
    }
    if (CheckHitKey(KEY_INPUT_Z))
    {
        y += 1;
    }
    if (CheckHitKey(KEY_INPUT_X))
    {
        y -= 1;
    }
    MV1SetPosition(model_handl, VGet(x, y, z));
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
    // ���̏��������Ǝ���V�F�[�_���g���Ƌ@�\���Ȃ�����l���邱��
    SetCameraNearFar(1.0f, 1000.0f);
    //SetCameraPositionAndTargetAndUpVec(VGet(0, 0, 0), VGet(320.0f, 240.0f, 1.0f), VGet(0, 1, 0));
    // ����̃V�F�[�_���g��Ȃ�--------------------------------------------------
    //MV1SetUseOrigShader(false);
    //MV1SetRotationXYZ(model_handl, VGet(0, angle, 0));
    //MV1DrawModel(model_handl);
    // �����܂ł��V�F�[�_���g��Ȃ�----------------------------------------------
    // �������炪�V�F�[�_���g������----------------------------------------------
    threshold[0] = directionLight.direction;
    threshold[1] = directionLight.color;
    UpdateShaderConstantBuffer(cbuff);
    SetShaderConstantBuffer(cbuff, DX_SHADERTYPE_PIXEL, 0);
    MV1SetUseOrigShader(true);
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(true);
    MV1SetUseZBuffer(model_handl, true);
    MV1SetWriteZBuffer(model_handl, true);
    SetUseVertexShader(vs);
    SetUsePixelShader(ps);
    MV1SetRotationXYZ(model_handl, VGet(0, angle, 0));
    MV1DrawModel(model_handl);
    // �����܂ł��V�F�[�_���g��������---------------------------------------------
}

void GameScene::DrawGameEnd(float delta)
{
}

void GameScene::Relese(void)
{
    DeleteShaderConstantBuffer(cbuff);
    DeleteShader(vs);
    DeleteShader(ps);
}
