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

    /// <summary> 開始時の更新処理 </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    uniqueScene UpdateStart(float delta, uniqueScene ownScene);

    /// <summary> ゲーム中の更新処理 </summary>
    /// <param name="delta"></param>
    /// <param name="ownScene"></param>
    /// <returns></returns>
    uniqueScene GameUpdate(float delta, uniqueScene ownScene);

    uniqueScene UpdateGameEnd(float delta, uniqueScene ownScene);

    void DrawGame(float delta);

    void DrawGameEnd(float delta);

    void Relese(void);
    // ライトのセットアップ
    void LightSetUp(void);
    // シェーダのセットアップ
    void ShaderSetUp(int model);
    // 影用の震度記録画像の準備
    void SetupDepthImage(void);
    // 影用の震度記録画像を使った影を落とす処理も含めたモデルの描画
    void DrawModelWithDepthShadow(void);
    // 描画処理
    void Render_Process();
    // 軸の描画
    void DrawAxis(void);
    void DrawFilde(void);
    // モデル描画
    int model_;
    int stage_;
    // 回転とポジションとサイズ
    float angle_;
    Vector3 pos_;
    Vector3 size_;
    // カメラの回転とポジション
    Vector3 cPos_;
    Vector3 cRot_;
    Vector3 cAngle_;
    // シェーダ用
    int toon,lam,tex, vs;
    int toonMap_;
    int tlbertType;
    // 影表現用
    VECTOR Lightdir_;
    VECTOR Lightpos_;
    VECTOR Lighttarget_;
    int DepthBufferGraphHandle_;
    int ps_[2];
    int vs_[4];
    // カメラのビュー行列と射影行列
    MATRIX LightCamera_ViewMatrix;
    MATRIX LightCamera_ProjectionMatrix;
    // 影表現用の変数
    LIGHT_MATRIX lightMat_;
    int cbufferVS, cbufferPS;

    // ディレクション
    VECTOR LightDirecion_;
    DirectionLight directionLight_;
    // 定数バッファの確保用変数
    int cbuff;
    DirectionLight* direction_;
};

