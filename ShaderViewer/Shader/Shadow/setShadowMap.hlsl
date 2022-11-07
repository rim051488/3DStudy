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

static const uint OFFSET_X = 1;
static const uint OFFSET_Y = 1;

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

// ハードシャドウ
//SamplerState depth              : register( s1 ) ;		// 深度バッファテクスチャ
// ソフトシャドウ
SamplerComparisonState depthSmp          // 深度バッファテクスチャ
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	MaxAnisotropy = 1;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler conmparison state
	ComparisonFunc = GREATER;
};
Texture2D    depthtex              : register(t1) ;		// 深度バッファテクスチャ

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

	//ライトビュースクリーン空間からUV座標空間に変換している
	float2 shadowMapUV = input.lpos.xy / input.lpos.w;
	shadowMapUV *= float2(0.5f, -0.5f);
	shadowMapUV += 0.5f;
	// ライトビュースクリーン空間でのZ値を計算する
	float zlpos = pow(input.lpos.z/input.lpos.w,10);
	 //UV座標を使ってシャドウマップから影情報をサンプリングする
	if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f &&
		shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
	{
		 //ハードシャドウ
		//float zshadowMap = depthtex.Sample(depth, shadowMapUV).r;
		//if (zlpos > zshadowMap + 0.005f)
		//{
		//	// 遮断されている
		//	output.Color0.xyz *= 0.5f;
		//}

		// ソフトシャドウ
		
		//float shadowMap_0 = depthtex.Sample(depth, shadowMapUV).r;
		//float shadowMap_1 = depthtex.Sample(depth, shadowMapUV + float2(0.5,0.0f)).r;
		//float shadowMap_2 = depthtex.Sample(depth, shadowMapUV + float2(0.5, 0.5)).r;
		//float shadowMap_3 = depthtex.Sample(depth, shadowMapUV + float2(0.0f, 0.5)).r;
		//float shadowRate = 0.0f;
		//if (zlpos > shadowMap_0)
		//{
		//	// 遮蔽されているので、遮蔽率を１加算
		//	shadowRate += 1.0f;
		//}
		//if (zlpos > shadowMap_1)
		//{
		//	// 遮蔽されているので、遮蔽率を１加算
		//	shadowRate += 1.0f;
		//}
		//if (zlpos > shadowMap_2)
		//{
		//	// 遮蔽されているので、遮蔽率を１加算
		//	shadowRate += 1.0f;
		//}
		//if (zlpos > shadowMap_3)
		//{
		//	// 遮蔽されているので、遮蔽率を１加算
		//	shadowRate += 1.0f;
		//}
		//shadowRate /= 4.0f;
		//float3 shadowColor = output.Color0.xyz;
		//float3 finalColor = lerp(output.Color0.xyz, shadowColor, shadowRate);
		//output.Color0.xyz = finalColor;
		
		output.Color0 = depthtex.Sample(depthSmp, input.uv0.xy);
		return output;

		float shadow = depthtex.SampleCmpLevelZero(
			depthSmp,	// 使用するサンプラーステート
			shadowMapUV, // シャドウマップにアクセスするUV座標
			zlpos	// 比較するZ値
		);

		// シャドウカラーを計算
		float3 shadowColor = output.Color0.xyz * 0.5f;
		// 遮蔽率を使って線形補間
		output.Color0.xyz = lerp(output.Color0.xyz, shadowColor,  shadow);
	}
	return output;
}


