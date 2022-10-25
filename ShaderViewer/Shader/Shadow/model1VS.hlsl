// 頂点シェーダーの入力
struct VSInput
{
	float4 pos        : POSITION ;         // 座標( ローカル空間 )
	float3 norm          : NORMAL0 ;          // 法線( ローカル空間 )
	float4 diff         : COLOR0 ;           // ディフューズカラー
	float4 spec        : COLOR1 ;           // スペキュラカラー
	float4 uv0      : TEXCOORD0 ;        // テクスチャ座標
	float4 uv1      : TEXCOORD1 ;		// サブテクスチャ座標
	int4   BlendIndices0   : BLENDINDICES0 ;    // スキニング処理用 Float型定数配列インデックス
	float4 BlendWeight0    : BLENDWEIGHT0 ;     // スキニング処理用ウエイト値
} ;

// 頂点シェーダーの出力
struct VSOutput
{
	float4 pos       : TEXCOORD0 ;        // 座標( 射影空間 )
	float4 svpos        : SV_POSITION ;	// 座標( プロジェクション空間 )
} ;


// 基本パラメータ
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
	float4		AntiViewportMatrix[ 4 ] ;				// アンチビューポート行列
	float4		ProjectionMatrix[ 4 ] ;					// ビュー　→　プロジェクション行列
	float4		ViewMatrix[ 3 ] ;						// ワールド　→　ビュー行列
	float4		LocalWorldMatrix[ 3 ] ;					// ローカル　→　ワールド行列

	float4		ToonOutLineSize ;						// トゥーンの輪郭線の大きさ
	float		DiffuseSource ;							// ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
	float		SpecularSource ;						// スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
	float		MulSpecularColor ;						// スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
	float		Padding ;
} ;

// その他の行列
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
{
	float4		ShadowMapLightViewProjectionMatrix[ 3 ][ 4 ] ;			// シャドウマップ用のライトビュー行列とライト射影行列を乗算したもの
	float4		TextureMatrix[ 3 ][ 2 ] ;								// テクスチャ座標操作用行列
} ;

// スキニングメッシュ用の　ローカル　→　ワールド行列
struct DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX
{
	float4		Matrix[ 54 * 3 ] ;					// ローカル　→　ワールド行列
} ;

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_VS_BASE				: register( b1 )
{
	DX_D3D11_VS_CONST_BUFFER_BASE				g_Base ;
} ;

// その他の行列
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX			: register( b2 )
{
	DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX		g_OtherMatrix ;
} ;

// スキニングメッシュ用の　ローカル　→　ワールド行列
cbuffer cbD3D11_CONST_BUFFER_VS_LOCALWORLDMATRIX	: register( b3 )
{
	DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX	g_LocalWorldMatrix ;
} ;




// main関数
VSOutput main(VSInput input )
{
	VSOutput output ;
	float4 lLocalWorldMatrix[ 3 ] ;
	float4 lWorldPosition ;
	float4 lViewPosition ;

	// ローカル座標のセット
	float4 pos = float4(input.pos.xyz, 1);

	// 複数のフレームのブレンド行列の作成
	lLocalWorldMatrix[ 0 ]  = g_LocalWorldMatrix.Matrix[input.BlendIndices0.x + 0 ] * input.BlendWeight0.x;
	lLocalWorldMatrix[ 1 ]  = g_LocalWorldMatrix.Matrix[input.BlendIndices0.x + 1 ] * input.BlendWeight0.x;
	lLocalWorldMatrix[ 2 ]  = g_LocalWorldMatrix.Matrix[input.BlendIndices0.x + 2 ] * input.BlendWeight0.x;

	lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.y + 0 ] * input.BlendWeight0.y;
	lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.y + 1 ] * input.BlendWeight0.y;
	lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.y + 2 ] * input.BlendWeight0.y;

	lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.z + 0 ] * input.BlendWeight0.z;
	lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.z + 1 ] * input.BlendWeight0.z;
	lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.z + 2 ] * input.BlendWeight0.z;

	lLocalWorldMatrix[ 0 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.w + 0 ] * input.BlendWeight0.w;
	lLocalWorldMatrix[ 1 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.w + 1 ] * input.BlendWeight0.w;
	lLocalWorldMatrix[ 2 ] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.w + 2 ] * input.BlendWeight0.w;


	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ローカル座標をワールド座標に変換
	lWorldPosition.x = dot(pos, lLocalWorldMatrix[ 0 ] ) ;
	lWorldPosition.y = dot(pos, lLocalWorldMatrix[ 1 ] ) ;
	lWorldPosition.z = dot(pos, lLocalWorldMatrix[ 2 ] ) ;
	lWorldPosition.w = 1.0f ;

	// ワールド座標をビュー座標に変換
	lViewPosition.x = dot( lWorldPosition, g_Base.ViewMatrix[ 0 ] ) ;
	lViewPosition.y = dot( lWorldPosition, g_Base.ViewMatrix[ 1 ] ) ;
	lViewPosition.z = dot( lWorldPosition, g_Base.ViewMatrix[ 2 ] ) ;
	lViewPosition.w = 1.0f ;

	// ビュー座標を射影座標に変換
	output.svpos.x = dot( lViewPosition, g_Base.ProjectionMatrix[ 0 ] ) ;
	output.svpos.y = dot( lViewPosition, g_Base.ProjectionMatrix[ 1 ] ) ;
	output.svpos.z = dot( lViewPosition, g_Base.ProjectionMatrix[ 2 ] ) ;
	output.svpos.w = dot( lViewPosition, g_Base.ProjectionMatrix[ 3 ] ) ;

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )



	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ビュー座標をテクスチャ座標として出力する
	output.pos = lViewPosition ;

	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )


	// 出力パラメータを返す
	return output;
}
