#pragma once
#include "Vector3.h"
struct DirectionLight
{
	Vector3 direction;//ライトの方向。３要素のベクトルで表現される
	float pading;//詰め物
	Vector3 color;//ライトのカラー。光の３原色RGBで表される
	float pading1;//詰め物
	Vector3 eyePos;//視点の位置
};

struct PointLight
{
	Vector3 position;//ライトの位置。３要素のベクトルで表現される
	float pading1;//詰め物
	Vector3 color;//ライトのカラー。光の３原色RGBで表される
	float pading2;//詰め物
	float influenceRange;//影響範囲。単位はメートル
};
struct SpotLight
{
	Vector3 position;//ライトの位置。３要素のベクトルで表現される
	float pading1;//詰め物
	Vector3 color;//ライトのカラー。光の３原色RGBであらわされる
	float pading2;//詰め物
	Vector3 direction;//放射方向。３要素のベクトルであらわされる
	float pading3;//詰め物
	float angle;//放射角度
	float influenceRange;//影響範囲。単位メートル
};