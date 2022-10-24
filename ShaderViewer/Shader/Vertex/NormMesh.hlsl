struct VertexInput {
	float3 pos : POSITION;//座標
	float3 norm : NORMAL;//法線
	float4 diffuse : COLOR0;
	float4 specular : COLOR1;
	float4 uv0 : TEXCOORD0;
	float4 uv1 : TEXCOORD1;
	float3 tangent:TANGENT;//接ベクトル
	float3 binormal:BINORMAL;//従法線
};

cbuffer BaseCBuffer : register(b1) {
	matrix AntiViewportM;//4*4ビューポート逆行列
	matrix ProjectionM;//プロジェクション行列
	float4x3 viewM;//4*3(ビュー行列)
	float4x3 localM;//4*3(回転拡縮平行移動)
	float4 ToonOutLineSize;// トゥーンの輪郭線の大きさ
	float DiffuseSource; //ディフューズカラー(0.0f:マテリアル 1.0f : 頂点)
	float SpecularSource;// スペキュラカラー(0.0f:マテリアル 1.0f : 頂点)
	float MulSpecularColor;// スペキュラカラー値に乗算する値(スペキュラ無効処理で使用)
	float Padding;//詰め物(無視)
}

struct VSOutput {
	float4 svpos:SV_POSITION;
	float4 pos:POSITION;
	float3 norm:NORMAL;
	float4 uv:TECOORD;
	float4 diff:COLOR0;
	float4 spec:COLOR1;
	float3 tan:TANGENT;
	float3 bin:BINORMAL;
};

VSOutput main(VertexInput input)
{
	VSOutput output;
	float4 pos = float4(input.pos.xyz, 1);

	pos.xyz = mul(pos, localM);//ワールド
	output.pos = pos;
	pos.xyz = mul(pos, viewM);//ビュー
	pos = mul(pos, ProjectionM);//プロジェクション

	//法線にも回転成分は乗算しよう
	float3 norm = mul(input.norm, localM);
	float3 tan = mul(input.tangent, localM);
	float3 bin = mul(input.binormal, localM);

	output.svpos = pos;

	output.uv = input.uv0;
	output.norm = normalize(norm);
	output.tan = normalize(tan);
	output.bin = normalize(bin);
	return output;
}