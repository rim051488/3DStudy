struct VertexInput {
	float3 pos : POSITION0;//座標
	float3 norm : NORMAL;//法線
	float4 dif : COLOR0;//ディフューズカラー
	float4 spec : COLOR1;//スペキュラカラー
	float4 uv0 : TEXCOORD0;//テクスチャ座標
	float4 uv1 : TEXCOORD1;//サブテクスチャ座標
};
struct VSOutput {
	float4 svpos:SV_POSITION;
	float3 pos:POSITION;
	float3 norm:NORMAL;
	float2 uv:TECOORD;
	float4 diff:COLOR0;
	float4 spec:COLOR1;
};
// 基本パラメータ
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
	float4		AntiViewportM[4];		// アンチビューポート行列
	float4		ProjectionM[4];		// ビュー　→　プロジェクション行列
	float4		viewM[3];				// ワールド　→　ビュー行列
	float4		localM[3];		// ローカル　→　ワールド行列

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

VSOutput main(VertexInput input)
{
	VSOutput output;
	float4 pos = float4(input.pos, 1);

	// ローカル座標をワールド座標に変換
	float4 IWorldPos;
	IWorldPos.x = dot(pos, g_Base.localM[0]);
	IWorldPos.y = dot(pos, g_Base.localM[1]);
	IWorldPos.z = dot(pos, g_Base.localM[2]);
	IWorldPos.w = 1.0f;
	// ワールド座標をビュー座標に変換
	float4 IViewPos;
	IViewPos.x = dot(IWorldPos, g_Base.viewM[0]);
	IViewPos.y = dot(IWorldPos, g_Base.viewM[1]);
	IViewPos.z = dot(IWorldPos, g_Base.viewM[2]);
	IViewPos.w = 1.0f;
	//ビュー座標を射影座標に変換
	output.svpos.x = dot(IViewPos, g_Base.ProjectionM[0]);
	output.svpos.y = dot(IViewPos, g_Base.ProjectionM[1]);
	output.svpos.z = dot(IViewPos, g_Base.ProjectionM[2]);
	output.svpos.w = dot(IViewPos, g_Base.ProjectionM[3]);

	// 従法線、接線、法線をビューベクトルに変換
	float3 IWorldNrm;
	IWorldNrm.x = dot(input.norm, g_Base.localM[0].xyz);
	IWorldNrm.y = dot(input.norm, g_Base.localM[1].xyz);
	IWorldNrm.z = dot(input.norm, g_Base.localM[2].xyz);

	float3 IViewNrm;
	IViewNrm.x = dot(IWorldNrm, g_Base.viewM[0].xyz);
	IViewNrm.y = dot(IWorldNrm, g_Base.viewM[1].xyz);
	IViewNrm.z = dot(IWorldNrm, g_Base.viewM[2].xyz);

	output.uv.x = dot(input.uv0, g_OtherMatrix.TextureMatrix[0][0]);
	output.uv.y = dot(input.uv0, g_OtherMatrix.TextureMatrix[0][1]);

	output.pos = IViewPos.xyz;
	output.norm = IViewNrm;
	return output;
}