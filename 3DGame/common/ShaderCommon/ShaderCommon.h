#pragma once

// データ型定義
typedef float SHADER_FLOAT;
typedef float SHADER_FLOAT2[2];
typedef float SHADER_FLOAT3[3];
typedef float SHADER_FLOAT4[4];

typedef int SHADER_INT;
typedef int SHADER_INT2[2];
typedef int SHADER_INT3[3];
typedef int SHADER_INT4[4];

#define CONST_LIGHT_NUM			(6)				//共通パラメータのライトの最大数

// 構造体定義
// マテリアルパラメータ
struct CONST_MATERIAL
{
	SHADER_FLOAT4		Diffuse;				//ディフューズカラー
	SHADER_FLOAT4		Specular;				//スペキュラカラー
	SHADER_FLOAT4		Ambient_Emissive;		//マテリアル

	SHADER_FLOAT		Power;					//スペキュラの強さ
	SHADER_FLOAT3		Padding;				//パディング
};
//フォグのパラメータ
struct CONST_FOG
{
	SHADER_FLOAT		LinearAdd;				//フォグ用パラメータ end /(end-start)
	SHADER_FLOAT		LinearDiv;				//フォグ用パラメータ -1 /(end-start)
	SHADER_FLOAT		Density;				//フォグ用パラメータ density
	SHADER_FLOAT		E;						//フォグ用パラメータ 自然対数の低

	SHADER_FLOAT4		Color;					//カラー
};
//ライトパラメータ
struct CONST_LIGHT
{
	SHADER_INT			Type;					//ライトタイプ(DX_LIGHTTYPE_POINT など)
	SHADER_INT3			Padding1;				//パディング１

	SHADER_FLOAT3		Position;				//座標(ビュー空間)
	SHADER_FLOAT		RangePow2;				//有効距離の2乗

	SHADER_FLOAT3		Direction;				//方向(ビュー空間)
	SHADER_FLOAT		FallOff;				//スポットライト用FallOff

	SHADER_FLOAT3		Diffuse;				//ディフューズカラー
	SHADER_FLOAT		SpotParam0;				//スポットライト用パラメータ0(cos(Phi/2.0f))

	SHADER_FLOAT3		Specular;				//スペキュラカラー
	SHADER_FLOAT		SpotParam1;				//スポットライト用パラメータ1(1.0f/(cos(Theta/2.0f) - cos(Phi / 2.0f)))

	SHADER_FLOAT4		Ambient;				//アンビエントカラーとマテリアルのアンビエントカラーを乗算したもの

	SHADER_FLOAT		Attenuation0;			//距離による減衰処理用パラメータ0
	SHADER_FLOAT		Attenuation1;			//距離による減衰処理用パラメータ1
	SHADER_FLOAT		Attenuation2;			//距離による減衰処理用パラメータ2
	SHADER_FLOAT		Padding2;				//パディング2
};
//ピクセルシェーダ・頂点シェーダ共通パラメータ
struct CONST_BUFFER_COMMON
{
	CONST_LIGHT		Light[CONST_LIGHT_NUM];		//ライトのパラメータ
	CONST_MATERIAL	Material;					//マテリアルパラメータ
	CONST_FOG		Fog;						//フォグパラメータ
};