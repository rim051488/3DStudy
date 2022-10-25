// ピクセルシェーダーの入力
struct PSInput
{
	float4 pos       : TEXCOORD0 ;        // 座標( ビュー空間 )
} ;

// ピクセルシェーダーの出力
struct PSOutput
{
	float4 Color0          : SV_TARGET0 ;	// 色
} ;

// main関数
PSOutput main(PSInput input )
{
	PSOutput output ;

	// Ｚ値を色として出力
	output.Color0 = input.pos.z;

	// 透明にならないようにアルファは必ず１
	output.Color0.a = 1.0f;

	// 出力パラメータを返す
	return output;
}


