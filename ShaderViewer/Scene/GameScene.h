#pragma once
#include <memory>
#include <functional>
#include <list>
#include "BaseScene.h"
#include "../common/Light.h"

struct LIGHT_MATRIX
{
    MATRIX view;
    MATRIX projection;
};

// ���C�g�J�����̒����_
constexpr Vector3 camTar{ 0.0f, 0.0f, 0.0f };
// ���C�g�J�����̐��ˉe�̕\���͈�
constexpr float offsetOrtho = 2000.0f;
// ���C�g�J�����̎�O�̋����Ɖ��̋���
constexpr float offsetNear = 0.001f;
constexpr float offsetFar = 4000.0f;

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
    // �e�p�̐k�x�L�^�摜�̏���
    void SetupShadowMap(void);
    // �e�p�̐k�x�L�^�摜���g�����e�𗎂Ƃ��������܂߂����f���̕`��
    void DrawOffScreen(void);
    /// <summary>
    /// �쐬�����I�t�X�N���[���Ƀ|�X�g�G�t�F�N�g��������
    /// </summary>
    /// <param name="flag">�|�X�g�G�t�F�N�g�������邩</param>
    /// <param name="x">����̍��W��X���W</param>
    /// <param name="y">����̍��W��Y���W</param>
    /// <param name="tex">�I�t�X�N���[��</param>
    /// <param name="ps">�V�F�[�_</param>
    void SetUpPostEffect(bool flag, int x, int y, int tex, int ps);

    // �`�揈��
    void Render_Process();
    // ���̕`��
    void DrawAxis(void);
    void DrawFilde(void);
    // ���f���`��
    int model_;
    int stage_;
    // ��]�ƃ|�W�V�����ƃT�C�Y
    float angle_;
    Vector3 pos_;
    Vector3 size_;
    // �J�����̉�]�ƃ|�W�V����
    Vector3 cPos_;
    Vector3 cRot_;
    Vector3 cAngle_;
    // �V�F�[�_�p
    int toon,lam,tex, vs;
    int toonMap_;
    int tlbertType;
    // �e�\���p
    int ShadowMap_;
    int ps_[2];
    int vs_[4];
    // �|�X�g�G�t�F�N�g�p
    int PostTex_;
    int PostPS_;
    // �J�����̃r���[�s��Ǝˉe�s��
    MATRIX LightCamera_ViewMatrix;
    MATRIX LightCamera_ProjectionMatrix;
    // �e�\���p�̕ϐ�
    LIGHT_MATRIX* lightMat_;
    LIGHT_MATRIX lightM_;
    int cbufferVS, cbufferPS;

    // �f�B���N�V����
    VECTOR LightDirecion_;
    DirectionLight directionLight_;
    // �萔�o�b�t�@�̊m�ۗp�ϐ�
    int cbuff;
    DirectionLight* direction_;
};

