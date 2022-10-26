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
	float4 diff         : COLOR0 ;
	float4 spec        : COLOR1 ;
	float4 uv0      : TEXCOORD0 ;
	float4 lpos      : TEXCOORD1 ;    // ライトからみた座標( ライトの射影空間 )
	float4 pos        : SV_POSITION ;	// 座標( プロジェクション空間 )
} ;


// マテリアルパラメータ
struct DX_D3D11_CONST_MATERIAL
{
	float4		Diffuse ;				// ディフューズカラー
	float4		Specular ;				// スペキュラカラー
	float4		Ambient_Emissive ;		// マテリアルエミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー

	float		Power ;					// スペキュラの強さ
	float		TypeParam0 ;			// マテリアルタイプパラメータ0
	float		TypeParam1 ;			// マテリアルタイプパラメータ1
	float		TypeParam2 ;			// マテリアルタイプパラメータ2
} ;

// フォグパラメータ
struct DX_D3D11_VS_CONST_FOG
{
	float		LinearAdd ;				// フォグ用パラメータ end / ( end - start )
	float		LinearDiv ;				// フォグ用パラメータ -1  / ( end - start )
	float		Density ;				// フォグ用パラメータ density
	float		E ;						// フォグ用パラメータ 自然対数の低

	float4		Color ;					// カラー
} ;

// ライトパラメータ
struct DX_D3D11_CONST_LIGHT
{
	int			Type ;					// ライトタイプ( DX_LIGHTTYPE_POINT など )
	int3		Padding1 ;				// パディング１

	float3		Position ;				// 座標( ビュー空間 )
	float		RangePow2 ;				// 有効距離の２乗

	float3		Direction ;				// 方向( ビュー空間 )
	float		FallOff ;				// スポットライト用FallOff

	float3		Diffuse ;				// ディフューズカラー
	float		SpotParam0 ;			// スポットライト用パラメータ０( cos( Phi / 2.0f ) )

	float3		Specular ;				// スペキュラカラー
	float		SpotParam1 ;			// スポットライト用パラメータ１( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

	float4		Ambient ;				// アンビエントカラーとマテリアルのアンビエントカラーを乗算したもの

	float		Attenuation0 ;			// 距離による減衰処理用パラメータ０
	float		Attenuation1 ;			// 距離による減衰処理用パラメータ１
	float		Attenuation2 ;			// 距離による減衰処理用パラメータ２
	float		Padding2 ;				// パディング２
} ;

// ピクセルシェーダー・頂点シェーダー共通パラメータ
struct DX_D3D11_CONST_BUFFER_COMMON
{
	DX_D3D11_CONST_LIGHT		Light[ 6 ] ;			// ライトパラメータ
	DX_D3D11_CONST_MATERIAL		Material ;				// マテリアルパラメータ
	DX_D3D11_VS_CONST_FOG		Fog ;					// フォグパラメータ
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

// 影用の深度記録画像を作成した際のカメラのビュー行列と射影行列
struct LIGHTCAMERA_MATRIX
{
	MATRIX		ViewMatrix;
	MATRIX		ProjectionMatrix;
};

// スキニングメッシュ用の　ローカル　→　ワールド行列
struct DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX
{
	float4		Matrix[ 54 * 3 ] ;					// ローカル　→　ワールド行列
} ;

// 頂点シェーダー・ピクセルシェーダー共通パラメータ
cbuffer cbD3D11_CONST_BUFFER_COMMON					: register( b0 )
{
	DX_D3D11_CONST_BUFFER_COMMON				g_Common ;
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

//cbuffer cbLIGHTCAMERA_MATRIX						: register( b4 )
//{
//	LIGHTCAMERA_MATRIX							g_LightMatrix;
//} ;


cbuffer LIGHT_VIEW		: register(b4)
{
	matrix g_lightView;
	matrix g_lightProjection;
};

// main関数
VSOutput main( VSInput input )
{
	VSOutput output ;
	float4 lWorldPosition ;
	float4 lViewPosition ;
	float4 lLViewPosition ;
	float3 lWorldNrm ;
	float3 lViewNrm ;
	float3 lLightHalfVec ;
	float4 lLightLitParam ;
	float4 lLightLitDest ;
	float4 lLocalWorldMatrix[ 3 ] ;
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
	output.pos.x = dot( lViewPosition, g_Base.ProjectionMatrix[ 0 ] ) ;
	output.pos.y = dot( lViewPosition, g_Base.ProjectionMatrix[ 1 ] ) ;
	output.pos.z = dot( lViewPosition, g_Base.ProjectionMatrix[ 2 ] ) ;
	output.pos.w = dot( lViewPosition, g_Base.ProjectionMatrix[ 3 ] ) ;

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )



	// ライトの処理 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// 法線をビュー空間の角度に変換 =========================================( 開始 )

	// ローカルベクトルをワールドベクトルに変換
	lWorldNrm.x = dot(input.norm, lLocalWorldMatrix[ 0 ].xyz ) ;
	lWorldNrm.y = dot(input.norm, lLocalWorldMatrix[ 1 ].xyz ) ;
	lWorldNrm.z = dot(input.norm, lLocalWorldMatrix[ 2 ].xyz ) ;

	// ワールドベクトルをビューベクトルに変換
	lViewNrm.x = dot( lWorldNrm, g_Base.ViewMatrix[ 0 ].xyz ) ;
	lViewNrm.y = dot( lWorldNrm, g_Base.ViewMatrix[ 1 ].xyz ) ;
	lViewNrm.z = dot( lWorldNrm, g_Base.ViewMatrix[ 2 ].xyz ) ;

	// 法線を正規化
	lViewNrm = normalize( lViewNrm ) ;

	// 法線をビュー空間の角度に変換 =========================================( 終了 )


	// ライトディフューズカラーとライトスペキュラカラーの角度減衰計算 =======( 開始 )

	// 法線とライトの逆方向ベクトルとの内積を lLightLitParam.x にセット
	lLightLitParam.x = dot( lViewNrm, -g_Common.Light[ 0 ].Direction ) ;

	// ハーフベクトルの計算 norm( ( norm( 頂点位置から視点へのベクトル ) + ライトの方向 ) )
	lLightHalfVec = normalize( normalize( -lViewPosition.xyz ) - g_Common.Light[ 0 ].Direction ) ;

	// 法線とハーフベクトルの内積を lLightLitParam.y にセット
	lLightLitParam.y = dot( lLightHalfVec, lViewNrm ) ;

	// スペキュラ反射率を lLightLitParam.w にセット
	lLightLitParam.w = g_Common.Material.Power ;

	// ライトパラメータ計算
	lLightLitDest = lit( lLightLitParam.x, lLightLitParam.y, lLightLitParam.w ) ;

	// ライトディフューズカラーとライトスペキュラカラーの角度減衰計算 =======( 終了 )

	// ライトの処理 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )



	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ディフューズカラー =
	//            ディフューズ角度減衰計算結果 *
	//            ライトのディフューズカラー *
	//            マテリアルのディフューズカラー +
	//            ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの +
	//            マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの
	output.diff.xyz = lLightLitDest.y * g_Common.Light[ 0 ].Diffuse * g_Common.Material.Diffuse.xyz + g_Common.Light[ 0 ].Ambient.xyz + g_Common.Material.Ambient_Emissive.xyz ;

	// ディフューズアルファはマテリアルのディフューズカラーのアルファをそのまま使う
	output.diff.w = g_Common.Material.Diffuse.w ;

	// スペキュラカラー = スペキュラ角度減衰計算結果 * ライトのスペキュラカラー * マテリアルのスペキュラカラー
	output.spec.xyz = lLightLitDest.z * g_Common.Light[ 0 ].Specular * g_Common.Material.Specular.xyz ;

	// スペキュラアルファはマテリアルのスペキュラカラーのアルファをそのまま使う
	output.spec.w = g_Common.Material.Specular.w ;


	// テクスチャ座標変換行列による変換を行った結果のテクスチャ座標をセット
	output.uv0.x = dot(input.uv0, g_OtherMatrix.TextureMatrix[ 0 ][ 0 ] ) ;
	output.uv0.y = dot(input.uv0, g_OtherMatrix.TextureMatrix[ 0 ][ 1 ] ) ;

	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )



	// 深度影用のライトから見た射影座標を算出 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ワールド座標をライトのビュー座標に変換
	lLViewPosition = mul(g_lightView, lWorldPosition ) ;
	//lLViewPosition = mul(g_LightMatrix.ViewMatrix, lWorldPosition ) ;
	
	// ライトのビュー座標をライトの射影座標に変換
	output.lpos = mul(g_lightProjection, lLViewPosition ) ;
	//output.lpos = mul(g_LightMatrix.ProjectionMatrix, lLViewPosition ) ;
	
	// Ｚ値だけはライトのビュー座標にする
	output.lpos.z = lLViewPosition.z ;

	// 深度影用のライトから見た射影座標を算出 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )


	// 出力パラメータを返す
	return output;
}

