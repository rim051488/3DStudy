#pragma once
#include <concepts>
#include <chrono>

template<class T>
// 時間用コンセプト整数値もしくは浮動小数点数を受け付けるようにする
concept TimeC = std::integral<T> || std::floating_point<T>;

class TimeClass
{
public:

	template<TimeC T>
	T GetDeltaTime(void) const;

	template<TimeC T>
	T GetGameTime(void) const;
	
	void DeltaTimeStart(void);

	void DeltaTimeEnd(void);

	void GameTimeEnd(void);

	template<TimeC T>
	static T GetTime(void)
	{
		std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
		return static_cast<T>((std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count() / 1000000000.0));
	}

private:

	double delta_;

	double gameTime_;


	// デルタタイム計測の終点
	std::chrono::system_clock::time_point deltaEnd_;

	// デルタタイム計測の始点
	std::chrono::system_clock::time_point deltaStart_;

	// 
	std::chrono::system_clock::time_point gameTimeEnd_;

	std::chrono::system_clock::time_point gameTimeStart_;
};

template<TimeC T>
constexpr T  convertNano = static_cast<T>(1) / static_cast<T>(1000000000.0);


template<TimeC T >
inline T TimeClass::GetDeltaTime(void) const
{
	return static_cast<T>(delta_);
}

template<TimeC T>
inline T TimeClass::GetGameTime(void) const
{
	return static_cast<T>(gameTime_);
}
