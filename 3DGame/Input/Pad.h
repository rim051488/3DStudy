#pragma once
#include <DxLib.h>
#include "Controller.h"

constexpr int motionRange = 30000;                                  // スティックの反応可動域

class Pad :
    public Controller
{
public:
    Pad();
    ~Pad();
    bool Init(void) override;										// 初期化
    void Update(double delta) override;								// 更新 Update(deltaTime)
    CntType GetCntType(void) override { return CntType::Pad; };		// コントローラータイプの取得
private:
    DINPUT_JOYSTATE state_;
    int padState_;
};

