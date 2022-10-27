// ピクセルシェーダーの入力
struct PSInput
{
	float4 diff         : COLOR0 ;       // ディフューズカラー
	float4 spec        : COLOR1 ;       // スペキュラカラー
	float4 uv0      : TEXCOORD0 ;    // テクスチャ座標
	float4 lpos: POSITION;    // ライトからみた座標( xとyはライトの射影座標、zはビュー座標 )
} ;

// ピクセルシェーダーの出力
struct PSOutput
{
	float4 Color0          : SV_TARGET0 ;	// 色
} ;

// 定数バッファピクセルシェーダー基本パラメータ
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
	float4		FactorColor ;			// アルファ値等

	float		MulAlphaColor ;			// カラーにアルファ値を乗算するかどうか( 0.0f:乗算しない  1.0f:乗算する )
	float		AlphaTestRef ;			// アルファテストで使用する比較値
	float2		Padding1 ;

	int			AlphaTestCmpMode ;		// アルファテスト比較モード( DX_CMP_NEVER など )
	int3		Padding2 ;

	float4		IgnoreTextureColor ;	// テクスチャカラー無視処理用カラー
} ;

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register( b1 )
{
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base ;
} ;

cbuffer DEPTH_CONST:register(b5)
{
	float4 test;
};

SamplerState sam            : register( s0 ) ;		// ディフューズマップテクスチャ
Texture2D    tex            : register( t0 ) ;		// ディフューズマップテクスチャ

SamplerState depth              : register( s1 ) ;		// 深度バッファテクスチャ
Texture2D    depthtex              : register( t1 ) ;		// 深度バッファテクスチャ


// main関数
PSOutput main( PSInput input )
{
	PSOutput output ;
	float4 TextureDiffuseColor ;
	float TextureDepth ;
	float2 DepthTexCoord ;
	float4 DefaultOutput ;


	// テクスチャカラーの読み込み
	TextureDiffuseColor = tex.Sample(sam, input.uv0.xy ) ;

	// 出力カラー = ディフューズカラー * テクスチャカラー + スペキュラカラー
	DefaultOutput = input.diff * TextureDiffuseColor + input.spec ;

	// 出力アルファ = ディフューズアルファ * テクスチャアルファ * 不透明度
	DefaultOutput.a = input.diff.a * TextureDiffuseColor.a * g_Base.FactorColor.a ;
	output.Color0 = DefaultOutput;
	//return output;

	// //深度テクスチャの座標を算出
	// //PSInput.LPPosition.xy は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
	//DepthTexCoord.x = (input.lpos.x + 1.0f ) / 2.0f;

	// //yは更に上下反転
	//DepthTexCoord.y = 1.0f - (input.lpos.y + 1.0f ) / 2.0f;

	// //深度バッファテクスチャから深度を取得
	//TextureDepth = depthtex.Sample(depth, DepthTexCoord );

	// //テクスチャに記録されている深度( +補正値 )よりＺ値が大きかったら奥にあるということで輝度を半分にする
	//if(input.lpos.z > TextureDepth + 60.0f )
	//{
	//	DefaultOutput.rgb *= 0.5f;
	//}

	// //出力カラーをセット
	//output.Color0 = DefaultOutput;

	// //出力パラメータを返す
	//return output;

	//ライトビュースクリーン空間からUV座標空間に変換している
	float2 shadowMapUV = input.lpos.xy / input.lpos.w;
	shadowMapUV *= float2(0.5f, -0.5f);
	shadowMapUV += 0.5f;
	// UV座標を使ってシャドウマップから影情報をサンプリングする
	float3 shadowMap = 1.0f;
	if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f &&
		shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
	{
		shadowMap = depthtex.Sample(depth, shadowMapUV);
	}
	// テクスチャにシャドウマップからサンプリングした情報を掛け算する
	output.Color0.xyz = shadowMap.xyz * DefaultOutput.xyz;
	return output;
}


