#pragma once
#include <map>
#include <array>
#include <string>
#include "InputID.h"

// トリガー情報
enum class Trg
{
	Now,				// 現在
	Old,				// 一つ前
	Max
};

// 入力装置の種類
enum class CntType
{
	Key,				// キーボード
	Pad,				// パッド
	Max
};

//using TrgBool = std::array<bool, static_cast<size_t>(Trg::Max)>;	// 入力状況		std::array<入力真偽, トリガー情報>
//using CntData = std::map<InputID, TrgBool>;							// 入力情報		std::map<キー情報, TrgBool>
using inputData = std::array<std::pair<bool, bool>, static_cast<size_t>(InputID::Max) + 1>;

class Controller
{
public:
	bool operator()(double delta)
	{
		if (this != nullptr)
		{
			Update(delta);
		}
		return this != nullptr;
	}
	Controller();
	virtual ~Controller();

	/// <summary> 初期化 </summary>
	/// <returns> 成功時true失敗時false </returns>
	virtual bool Init(void) = 0;

	/// <summary> 更新 </summary>
	/// <param name="delta"> デルタタイム </param>
	virtual void Update(double delta) = 0;

	/// <summary> コントローラーの種類の取得 </summary>
	/// <returns> コントローラーの種類 </returns>
	virtual CntType GetCntType(void) = 0;

	/// <summary> 今押しているか </summary>
	/// <param name="id"> キーの種類 </param>
	/// <returns> 成功時true失敗時false </returns>
	bool Press(InputID id);

	/// <summary> 押した瞬間 </summary>
	/// <param name="id"> キーの種類 </param>
	/// <returns> 成功時true失敗時false </returns>
	bool Pressed(InputID id);

	/// <summary> 離した瞬間 </summary>
	/// <param name="id"> キーの種類 </param>
	/// <returns> 成功時true失敗時false </returns>
	bool Released(InputID id);

	/// <summary> 今押されていないとき </summary>
	/// <param name="id"> キーの種類 </param>
	/// <returns> 成功時true失敗時false </returns>
	bool NotPress(InputID id);

	/// <summary> 今動いているか </summary>
	/// <returns> 成功時true失敗時false </returns>
	bool MovePress(bool isMove);

	/// <summary> 今動かなくなったか </summary>
	/// <returns> 成功時true失敗時false </returns>
	bool MoveReleased();

	/// <summary> 今動き始めたか </summary>
	/// <returns> 成功時true失敗時false </returns>
	bool MovePressed();

	/// <summary> 今歩いているか </summary>
	/// <returns> 成功時true失敗時false </returns>
	bool WalkPress();

	/// <summary> 今何も押されていないか </summary>
	/// <returns> 成功時true失敗時false </returns>
	bool IsNeutral();

	/// <summary> 何か押されたか </summary>
	/// <returns> 成功時true失敗時false </returns>
	bool IsAnyPress();

	/// <summary> 長押し </summary>
	/// <param name="id"> キーの種類 </param>
	/// <param name="limit"> 長押しの最大時間 </param>
	/// <param name="delta"> デルタタイム </param>
	/// <returns> 成功時true失敗時false </returns>
	std::string LongPress(InputID id, double limit, double delta);
	const inputData& Get(void);
private:
protected:
	/// <summary> 入力情報 </summary>
	//CntData cntData_;
	inputData data_;
	/// <summary> キーのリスト情報 </summary>
	//std::map<InputID, unsigned int> keyList_ = {};

	/// <summary> LongPress関係 </summary>
	bool isPress_;
	bool isFinish_;
	float PressCount_;
};

