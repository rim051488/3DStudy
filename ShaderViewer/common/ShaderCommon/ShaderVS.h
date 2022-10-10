#pragma once
#include "ShaderCommon.h"

#define CONST_TEXTUER_MATRIX_NUM			3			//テクスチャ座標変換行列の転置行列の数
#define CONST_WORLD_MAT_NUM					54			//トライアングルリスト1つで同時に使用するローカル→ワールド行列の最大数

// 基本パラメータ
struct CONST_BUFFER_BASE
{
	SHADER_FLOAT4		AntiViewportMatrix[4];			//アンチビューポート行列
};