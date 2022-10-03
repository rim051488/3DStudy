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

    // ���f���`��
    int model_handl;
    // ��]�ƃ|�W�V����
    float angle;
    float x, y, z;
    // �V�F�[�_�p
    int ps, vs;
    int toonMap_;
    DirectionLight directionLight_;
    // �萔�o�b�t�@�̊m�ۗp�ϐ�
    int cbuff;
    Vector3* direction_;
    Vector3* color_;
};

