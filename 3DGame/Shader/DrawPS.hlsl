struct PSInput {
	float4 svpos:SV_POSITION;
	float3 pos:POSITION;
	float3 norm:NORMAL;
	float2 uv:TECOORD;
	float2 toon:TECOORD;
	float3 col:COLOR;
	float3 tan:TANGENT;
	float3 bin:BINORMAL;
};

SamplerState sam:register(s0);
sampler Toon:register(s1);
Texture2D<float4> tex:register(t0);
Texture2D<float4> toon:register(t1);

// ディレクションライト用の定数バッファ
cbuffer DirectionLightCb : register(b0)
{
	float3 ligDirection;	//ライトの方向
	float3 ligColor;		//ライトのカラー
}

float4 main(PSInput input) : SV_TARGET
{
	// ↓色が変わるだけでテクスチャ関係なし
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
	// ↓テクスチャを貼り付けるだけ
	//return tex.Sample(sam,input.uv);
	// ↓ディレクションライトで照らしているバージョン
	// ピクセルの法線とライトの方向の内積を計算する
	//float3 i = float3(1.0f,1.0f,1.0f);
	//float  t = dot(input.norm,ligDirection);
	//// 内積の結果に-1を乗算する
	//t *= -1.0f;
	//if (t < 0.0f)
	//{
	//	t = 0.0f;
	//}
	//float3 diffuseLig = ligColor * t;
	//float4 finalColor = tex.Sample(sam, input.uv);
	//finalColor.xyz *= diffuseLig;
	//return finalColor;
	//-------------------------------------------------
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

	float4 color;
	float p = dot(input.norm * -1.0f, ligDirection);
	p = p * 0.5f + 0.5f;
	p = p * p;

	float4 Col = toon.Sample(Toon, float2(p, 0.0f));
	color = Col * tex.Sample(sam, input.uv);
	return color;
}