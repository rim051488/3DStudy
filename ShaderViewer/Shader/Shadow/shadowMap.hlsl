// ピクセルシェーダーの入力
struct PSInput
{
	float4 pos       : POSITION0 ;        // 座標( ビュー空間 )
	float4 vpos : POSITION1;
	float4 svpos        : SV_POSITION;	// 座標( プロジェクション空間 )
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



	// Z値を色として出力
	output.Color0 = pow(input.vpos.z/ input.vpos.w,10);
	//output.Color0 = input.pos.r;

	// 透明にならないようにアルファは必ず１
	output.Color0.a = 1.0f;

	// 試しに灰色で出してみる
	//output.Color0 = float4(0.5f, 0.5f, 0.5f, 1.0f);
	// 出力パラメータを返す
	return output;
}


