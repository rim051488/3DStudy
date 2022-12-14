//struct PSInput {
//	//float4 svpos:SV_POSITION;
//	//float3 pos:POSITION;
//	//float3 norm:NORMAL;
//	//float2 uv:TECOORD;
//	//float2 toon:TECOORD;
//	//float3 col:COLOR;
//	//float3 tan:TANGENT;
//	//float3 bin:BINORMAL;
//	float4 Diffuse         : COLOR0;		// ディフューズカラー
//	float4 Specular        : COLOR1;		// スペキュラカラー
//	float2 uv    : TEXCOORD0;	// xy:テクスチャ座標 zw:サブテクスチャ座標
//	float3 VPosition       : TEXCOORD1;	// 頂点座標から視線へのベクトル( ビュー空間 )
//	float3 VNormal         : TEXCOORD2;	// 法線( ビュー空間 )
//	float3 VTan            : TEXCOORD3;    // 接線( ビュー空間 )
//	float3 VBin            : TEXCOORD4;    // 従法線( ビュー空間 )
//};

struct PSInput {
	float4 svpos:SV_POSITION;
	float3 pos:POSITION;
	float3 norm:NORMAL;
	float2 uv:TECOORD;
	float3 col:COLOR0;
};

// ディレクションライト用の定数バッファ
cbuffer DirectionLightCb : register(b1)
{
	float3 ligDirection;	//ライトの方向
	float3 ligColor;		//ライトのカラー
}

cbuffer BaseCBuffer : register(b0) {
	matrix AntiViewportM;//4x4ビューポート逆行列
	matrix ProjectionM;//4x4プロジェクション行列
	float4x3 ViewM;//4x3(ビュー行列)
	float4x3 LocalM;//4x3(回転拡縮平行移動)
	float4		ToonOutLineSize;						// トゥーンの輪郭線の大きさ
	float		DiffuseSource;							// ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
	float		SpecularSource;						// スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
	float		MulSpecularColor;						// スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
	float		Padding;//詰め物(無視)
}

SamplerState sam:register(s0);
sampler Toon:register(s1);
Texture2D<float4> tex:register(t0);
Texture2D<float4> toon:register(t1);


float4 main(PSInput input) : SV_TARGET
{
	// ↓色が変わるだけでテクスチャ関係なし
	//return float4(0.0f, 1.0f, 1.0f, 1.0f);
	//return float4(0,0,1,1);
	// ↓テクスチャを貼り付けるだけ
	//return float4(input.uv,1,1);
	return tex.Sample(sam,float2(input.uv.xy));

	// トゥーンシェーダを使った描画
	//float4 color = tex.Sample(sam,input.uv);
	//// ハーフランバート拡散照明によるライティング計算
	//float p = saturate(dot(input.norm * -1.0f, ligDirection));
	//p = p * 0.5f + 0.5f;
	//p = saturate(p * p);
	//p = clamp(p, 0.0f, 0.99f);
	////return toon.Sample(sam, float2(p, 0.0f));
	//// 計算結果よりトゥーンシェーダのテクスチャからフェッチ
	//float4 Col = toon.Sample(sam, float2(p, 0.0f));
	//return color *= Col;

	//float4 color;
	//float p = dot(input.norm * -1.0f, ligDirection);
	//p = p * 0.5f + 0.5f;
	//p = p * p;

	//float4 Col = toon.Sample(Toon, float2(p, 0.0f));
	//color = Col * tex.Sample(sam, input.uv);
	//return color;
}