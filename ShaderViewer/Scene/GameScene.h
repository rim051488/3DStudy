#pragma once
#include <memory>
#include <functional>
#include <list>
#include "BaseScene.h"
#include "../common/Light.h"

class GameScene :
    public BaseScene
{
public:
    GameScene();
    ~GameScene();
private:
    bool Init(void) override;

    bool InitScreen(void);

    bool InitGame(void);

    uniqueScene Update(float delta, uniqueScene ownScene) override;
    void DrawOwnScreen(float delta) override;
    Scene GetSceneID(void) override { return Scene::Game; }
    bool GetFlag(void) override { return SceneFlag_; }

    /// <summary> �J�n���̍X�V���� </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    uniqueScene UpdateStart(float delta, uniqueScene ownScene);

    /// <summary> �Q�[�����̍X�V���� </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    uniqueScene GameUpdate(float delta, uniqueScene ownScene);

    uniqueScene UpdateGameEnd(float delta, uniqueScene ownScene);

    void DrawGame(float delta);

    void DrawGameEnd(float delta);

    void Relese(void);
    // ���C�g�̃Z�b�g�A�b�v
    void LightSetUp(void);
    // �V�F�[�_�̃Z�b�g�A�b�v
    void ShaderSetUp(int model);
    // ���̕`��
    void DrawAxis(void);
    void DrawFilde(void);
    // ���f���`��
    int model_;
    // ��]�ƃ|�W�V�����ƃT�C�Y
    float angle_;
    Vector3 pos_;
    Vector3 size_;
    // �J�����̉�]�ƃ|�W�V����
    Vector3 cPos_;
    Vector3 cRot_;
    Vector3 cAngle_;
    // �V�F�[�_�p
    int lam,tex,toon, vs;
    int toonMap_;
    int tlbertType;
    // �f�B���N�V����
    DirectionLight directionLight_;
    // �萔�o�b�t�@�̊m�ۗp�ϐ�
    int cbuff;
    DirectionLight* direction_;
};

