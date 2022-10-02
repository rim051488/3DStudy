#pragma once
#include <memory>
#include "../common/Math.h"
#include"../Input/Controller.h"

class BaseScene;
using uniqueScene = std::unique_ptr<BaseScene>;

enum class Scene
{
	Game,
	Max
};

class BaseScene
{
public:
	// コンストラクタ
	BaseScene();
	virtual ~BaseScene();

	//初期化
	virtual bool Init(void) = 0;

	/// <summary> 更新 </summary>
	/// <param name="delta">デルタタイム</param>
	/// <param name="ownScene">現在のシーン</param>
	/// <returns> 現在のシーン </returns>
	virtual uniqueScene Update(float delta, uniqueScene ownScene) = 0;

	/// <summary> 描画 </summary>
	/// <param name="delta"> デルタタイム </param>
	virtual void Draw(float delta);

	/// <summary> 現在のシーンの描画 </summary>
	/// <param name="delta"> デルタタイム </param>
	virtual void DrawOwnScreen(float delta) = 0;

	/// <summary> 現在のシーンの取得 </summary>
	/// <returns> 現在のシーン </returns>
	virtual Scene GetSceneID(void) = 0;

	/// <summary> シーン遷移フラグ取得 </summary>
	/// <returns> シーン遷移:true </returns>
	virtual bool GetFlag(void) = 0;

protected:
	// シーンの遷移フラグ
	bool SceneFlag_;

	// 画面サイズ
	Vector2I screenSize_;

	// シーンデータの格納
	int screenID_;

	// コントローラ情報
	std::unique_ptr<Controller> controller_;

};

