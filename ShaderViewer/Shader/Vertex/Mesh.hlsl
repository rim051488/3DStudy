struct VSInput
{
    float3 pos : POSITION;// 座標
    float3 norm : NORMAL;// 法線
    float4 diff : COLOR0;// ディフューズカラー
    float4 spec : COLOR1;// スペキュラカラー
    float4 uv0 : TEXCOORD0;// テクスチャ座標
    float4 uv1 : TEXCOORD1;// サブテクスチャ座標
};

struct VSOutput
{
    float4 pos : POSITION0;
    float4 lpos : POSITION1;
    float3 norm : NORMAL;
    float4 uv : TEXCOORD;
    float4 diff : COLOR0;
    float4 spec : COLOR1;
    float4 svpos : SV_POSITION;
};

// 基本パラメータ
struct CONST_BUFFER_BASE
{
    float4x4 AntiViewportMat;
    float4x4 ProjMat;
    float4x3 ViewMat;
    float4x3 LacalWorldMat;
    
    float4 ToonOutLineSize;
    float DiffuseSource;
    float SpecularSource;
    float MulSpecularColor;
    float Padding;
};

// その他の行列
struct CONST_BUFFER_OTHERMAT
{
    float4 ShadowMapLightViewProjectionMat[3][4];
    float4 TextureMat[3][2];
};

cbuffer BUFFER_BASE : register(b1)
{
    CONST_BUFFER_BASE g_Base;
}

cbuffer BUFFER_OTHERMAT : register(b2)
{
    CONST_BUFFER_OTHERMAT g_OtherMat;
}

cbuffer LIGHT_VIEW : register(b4)
{
    float4x4 g_lightView;
    float4x4 g_lightProj;
}

VSOutput main(VSInput input)
{
    VSOutput output;
    float4 pos = float4(input.pos.xyz, 1.0f);
    float4 WorldPos;
    float4 ViewPos;
    // ローカル座標をワールド座標に変換
    WorldPos.xyz = mul(pos, g_Base.LacalWorldMat);
    WorldPos.w = 1.0f;
    // ワールド座標をビュー座標に変換
    ViewPos.xyz = mul(WorldPos, g_Base.ViewMat);
    ViewPos.w = 1.0f;
    // ビュー座標を射影座標に変換
    output.svpos = mul(ViewPos, g_Base.ProjMat);
    
    // 従法線・接線・法線をビューベクトルに変換
    float3 WorldNrm;
    float3 ViewNrm;
    
    WorldNrm.x = dot(input.norm, g_Base.LacalWorldMat[0].xyz);
    WorldNrm.y = dot(input.norm, g_Base.LacalWorldMat[1].xyz);
    WorldNrm.z = dot(input.norm, g_Base.LacalWorldMat[2].xyz);
    
    ViewNrm.x = dot(WorldNrm, g_Base.ViewMat[0].xyz);
    ViewNrm.y = dot(WorldNrm, g_Base.ViewMat[1].xyz);
    ViewNrm.z = dot(WorldNrm, g_Base.ViewMat[2].xyz);
    
    output.pos = ViewPos;
    output.norm = ViewNrm;
    
    // カメラ情報をセット
    float4 lViewPos = mul(g_lightView, WorldPos);
    output.lpos = mul(g_lightProj, lViewPos);
    
    return output;
}

//struct VertexInput {
//	float3 pos : POSITION0;//座標
//	float3 norm : NORMAL;//法線
//	float4 dif : COLOR0;//ディフューズカラー
//	float4 spec : COLOR1;//スペキュラカラー
//	float4 uv0 : TEXCOORD0;//テクスチャ座標
//	float4 uv1 : TEXCOORD1;//サブテクスチャ座標
//};
//struct VSOutput {
//	float4 svpos:SV_POSITION;
//	float3 pos:POSITION;
//	float3 norm:NORMAL;
//	float4 uv:TECOORD;
//	float4 diff:COLOR0;
//	float4 spec:COLOR1;
//};
//// 基本パラメータ
//struct DX_D3D11_VS_CONST_BUFFER_BASE
//{
//	float4		AntiViewportM[4];		// アンチビューポート行列
//	float4		ProjectionM[4];		// ビュー　→　プロジェクション行列
//	float4		viewM[3];				// ワールド　→　ビュー行列
//	float4		localM[3];		// ローカル　→　ワールド行列

//	float4		ToonOutLineSize;			// トゥーンの輪郭線の大きさ
//	float		DiffuseSource;				// ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
//	float		SpecularSource;				// スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
//	float		MulSpecularColor;			// スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
//	float		Padding;
//};

//// スキニングメッシュ用の　ローカル　→　ワールド行列
//struct DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX
//{
//	// ローカル　→　ワールド行列
//	float4		Matrix[54 * 3];
//};

//// その他の行列
//struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
//{
//	float4		ShadowMapLightViewProjectionMatrix[3][4];			// シャドウマップ用のライトビュー行列とライト射影行列を乗算したもの
//	float4		TextureMatrix[3][2];								// テクスチャ座標操作用行列
//};

//// 基本パラメータ
//cbuffer cbD3D11_CONST_BUFFER_VS_BASE			: register(b1)
//{
//	DX_D3D11_VS_CONST_BUFFER_BASE				g_Base;
//};

//// その他の行列
//cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX		: register(b2)
//{
//	DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX		g_OtherMatrix;
//};

//// スキニングメッシュ用の　ローカル　→　ワールド行列
//cbuffer cbD3D11_CONST_BUFFER_VS_LOCALWORLDMATRIX	: register(b3)
//{
//	DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX	g_LocalWorldMatrix;
//};

//VSOutput main(VertexInput input)
//{
//	VSOutput output;
//	float4 pos = float4(input.pos, 1);

//	// ローカル座標をワールド座標に変換
//	float4 IWorldPos;
//	IWorldPos.x = dot(pos, g_Base.localM[0]);
//	IWorldPos.y = dot(pos, g_Base.localM[1]);
//	IWorldPos.z = dot(pos, g_Base.localM[2]);
//	IWorldPos.w = 1.0f;
//	// ワールド座標をビュー座標に変換
//	float4 IViewPos;
//	IViewPos.x = dot(IWorldPos, g_Base.viewM[0]);
//	IViewPos.y = dot(IWorldPos, g_Base.viewM[1]);
//	IViewPos.z = dot(IWorldPos, g_Base.viewM[2]);
//	IViewPos.w = 1.0f;
//	//ビュー座標を射影座標に変換
//	output.svpos.x = dot(IViewPos, g_Base.ProjectionM[0]);
//	output.svpos.y = dot(IViewPos, g_Base.ProjectionM[1]);
//	output.svpos.z = dot(IViewPos, g_Base.ProjectionM[2]);
//	output.svpos.w = dot(IViewPos, g_Base.ProjectionM[3]);

//	// 従法線、接線、法線をビューベクトルに変換
//	float3 IWorldNrm;
//	IWorldNrm.x = dot(input.norm, g_Base.localM[0].xyz);
//	IWorldNrm.y = dot(input.norm, g_Base.localM[1].xyz);
//	IWorldNrm.z = dot(input.norm, g_Base.localM[2].xyz);

//	float3 IViewNrm;
//	IViewNrm.x = dot(IWorldNrm, g_Base.viewM[0].xyz);
//	IViewNrm.y = dot(IWorldNrm, g_Base.viewM[1].xyz);
//	IViewNrm.z = dot(IWorldNrm, g_Base.viewM[2].xyz);

//	output.uv.x = dot(input.uv0, g_OtherMatrix.TextureMatrix[0][0]);
//	output.uv.y = dot(input.uv0, g_OtherMatrix.TextureMatrix[0][1]);

//	output.pos = IViewPos.xyz;
//	output.norm = IViewNrm;
//	return output;
//}