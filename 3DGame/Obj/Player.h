#pragma once
#include "ObjMng.h"
#include "../common/Light.h"
class Player :
    public ObjMng
{
public:
    Player();
    ~Player();
    bool Init(void) override;
    void Update(float delta) override;
    void Draw(void) override;
private:
    // モデル描画
    int model_handl;
    // 回転とポジション
    float angle;
    Vector3 pos_;
    Vector3 Cpos_;
    Vector3 CAngle_;
    // シェーダ用
    int ps, vs;
    int toonMap_;
    // モデルのタイプを調べる
    int tlbertType;
    // ディレクション
    DirectionLight directionLight_;
    // 定数バッファの確保用変数
    int cbuff;
    DirectionLight* direction_;
    //Vector3* direction_;
    //Vector3* color_;
    std::unique_ptr<Controller> controller_;
    // カメラの制御
    VECTOR cPos_;
    Vector3 cTarget_;
    Vector3 cRot_;
};

