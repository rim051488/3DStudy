// 頂点シェーダーの入力
struct VS_INPUT
{
	//この順番でないとバグる?
	float3 pos				: POSITION;			// 座標(ローカル空間)
	float3 norm				: NORMAL;			// 法線(ローカル空間)
	float4 diffuse			: COLOR0;			// ディフューズカラー
	float4 specular			: COLOR1;			// スペキュラカラー
	float4 uv0		: TEXCOORD0;		// テクスチャ座標
	float4 uv1		: TEXCOORD1;		// サブテクスチャ座標
	int4   blendIndices0	: BLENDINDICES0;	// スキニング処理用 Float型定数配列インデックス
	float4 blendWeight0		: BLENDWEIGHT0;		// スキニング処理用ウエイト値
};

// 頂点シェーダーの出力
struct VS_OUTPUT
{
	float4 svpos:SV_POSITION;
	float3 pos:POSITION;
	float3 norm:NORMAL;
	float4 uv:TECOORD;
	float4 diff:COLOR0;
	float4 spec:COLOR1;
};

// 基本パラメータ
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
	float4		AntiViewportMatrix[4];		// アンチビューポート行列
	float4		ProjectionMatrix[4];		// ビュー　→　プロジェクション行列
	float4		ViewMatrix[3];				// ワールド　→　ビュー行列
	float4		LocalWorldMatrix[3];		// ローカル　→　ワールド行列

	float4		ToonOutLineSize;			// トゥーンの輪郭線の大きさ
	float		DiffuseSource;				// ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
	float		SpecularSource;				// スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
	float		MulSpecularColor;			// スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
	float		Padding;
};

// スキニングメッシュ用の　ローカル　→　ワールド行列
struct DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX
{
	// ローカル　→　ワールド行列
	float4		Matrix[54 * 3];
};

// その他の行列
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
{
	float4		ShadowMapLightViewProjectionMatrix[3][4];			// シャドウマップ用のライトビュー行列とライト射影行列を乗算したもの
	float4		TextureMatrix[3][2];								// テクスチャ座標操作用行列
};

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_VS_BASE			: register(b1)
{
	DX_D3D11_VS_CONST_BUFFER_BASE				g_Base;
};

// その他の行列
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX		: register(b2)
{
	DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX		g_OtherMatrix;
};

// スキニングメッシュ用の　ローカル　→　ワールド行列
cbuffer cbD3D11_CONST_BUFFER_VS_LOCALWORLDMATRIX	: register(b3)
{
	DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX	g_LocalWorldMatrix;
};

// main関数
VS_OUTPUT main(VS_INPUT VSInput)
{
	VS_OUTPUT VSOutput;
	float4 pos = float4(VSInput.pos.xyz, 1);

	// 複数のフレームのブレンド行列の作成
	float4 lLocalWorldMatrix[3];
	lLocalWorldMatrix[0] = g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.x + 0] * VSInput.blendWeight0.x;
	lLocalWorldMatrix[1] = g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.x + 1] * VSInput.blendWeight0.x;
	lLocalWorldMatrix[2] = g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.x + 2] * VSInput.blendWeight0.x;

	lLocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.y + 0] * VSInput.blendWeight0.y;
	lLocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.y + 1] * VSInput.blendWeight0.y;
	lLocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.y + 2] * VSInput.blendWeight0.y;

	lLocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.z + 0] * VSInput.blendWeight0.z;
	lLocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.z + 1] * VSInput.blendWeight0.z;
	lLocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.z + 2] * VSInput.blendWeight0.z;

	lLocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.w + 0] * VSInput.blendWeight0.w;
	lLocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.w + 1] * VSInput.blendWeight0.w;
	lLocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[VSInput.blendIndices0.w + 2] * VSInput.blendWeight0.w;
	
	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ローカル座標のセット

	// 座標計算( ローカル→ビュー→プロジェクション )
	float4 lWorldPosition;
	lWorldPosition.x = dot(pos, lLocalWorldMatrix[0]);
	lWorldPosition.y = dot(pos, lLocalWorldMatrix[1]);
	lWorldPosition.z = dot(pos, lLocalWorldMatrix[2]);
	lWorldPosition.w = 1.0f;

	float4 lViewPosition;
	lViewPosition.x = dot(lWorldPosition, g_Base.ViewMatrix[0]);
	lViewPosition.y = dot(lWorldPosition, g_Base.ViewMatrix[1]);
	lViewPosition.z = dot(lWorldPosition, g_Base.ViewMatrix[2]);
	lViewPosition.w = 1.0f;

	VSOutput.svpos.x = dot(lViewPosition, g_Base.ProjectionMatrix[0]);
	VSOutput.svpos.y = dot(lViewPosition, g_Base.ProjectionMatrix[1]);
	VSOutput.svpos.z = dot(lViewPosition, g_Base.ProjectionMatrix[2]);
	VSOutput.svpos.w = dot(lViewPosition, g_Base.ProjectionMatrix[3]);

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )

	// 法線をビュー空間の角度に変換 =====================================================( 開始 )

	float3 lWorldNrm;
	// 従法線、接線、法線をビューベクトルに変換
	lWorldNrm.x = dot(VSInput.norm, lLocalWorldMatrix[0].xyz);
	lWorldNrm.y = dot(VSInput.norm, lLocalWorldMatrix[1].xyz);
	lWorldNrm.z = dot(VSInput.norm, lLocalWorldMatrix[2].xyz);

	float3 lViewNrm;
	lViewNrm.x = dot(lWorldNrm, g_Base.ViewMatrix[0].xyz);
	lViewNrm.y = dot(lWorldNrm, g_Base.ViewMatrix[1].xyz);
	lViewNrm.z = dot(lWorldNrm, g_Base.ViewMatrix[2].xyz);

	// 法線をビュー空間の角度に変換 =====================================================( 終了 )


	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// テクスチャ座標変換行列による変換を行った結果のテクスチャ座標をセット
	VSOutput.uv.x = dot(VSInput.uv0, g_OtherMatrix.TextureMatrix[0][0]);
	VSOutput.uv.y = dot(VSInput.uv0, g_OtherMatrix.TextureMatrix[0][1]);

	// 頂点座標を保存
	VSOutput.pos = lViewPosition.xyz;
	// 法線を保存
	VSOutput.norm = lViewNrm;


	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )


	// 出力パラメータを返す
	return VSOutput;
}
