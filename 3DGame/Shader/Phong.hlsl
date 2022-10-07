struct PSInput {
	float4 svpos:SV_POSITION;
	float3 pos:POSITION;
	float3 norm:NORMAL;
	float2 uv:TECOORD;
	float2 toon:TECOORD;
	float3 col:COLOR;
	float3 tan:TANGENT;
	float3 bin:BINORMAL;
};

SamplerState sam:register(s0);
Texture2D<float4> tex:register(t0);

// ディレクションライト用の定数バッファ
cbuffer DirectionLightCb : register(b0)
{
	float3 ligDirection;	//ライトの方向
	float3 ligColor;		//ライトのカラー
	float3 eyePos;			//視点の位置
}

float4 main(PSInput input) : SV_TARGET
{
	//ピクセルの法線とライトの方向の内積を計算する
	float t = dot(input.norm,ligDirection);
	t *= -1.0f;

	//内積の結果が０以下なら０にする
	if (t < 0.0f)
	{
		t = 0.0f;
	}
	//拡散反射光を求める
	float3 diffuseLig = ligColor * t;
	//反射ベクトルを求める
	float3 refVec = reflect(ligDirection, input.norm);
	//光が当たったサーフェイスから視点に伸びるベクトルを求める
	float3 toEye = eyePos - input.pos;
	//正規化する
	toEye = normalize(toEye);
	//鏡面反射の強さを求める
	//dot関数を利用してrefVecとtoEyeの内積を求める
	t = dot(refVec, toEye);
	//内積の結果はマイナスになるので、マイナスの場合は0にする
	if (t < 0.0f)
	{
		t = 0.0f;
	}
	//鏡面反射の強さを絞る
	t = pow(t, 20.0f);
	//鏡面反射光を求める
	float3 specularLig = ligColor * t;
	//拡散反射光と鏡面反射光を足し算して、最終的な光を求める
	float3 lig = diffuseLig + specularLig;
	float4 color = tex.Sample(sam, input.uv);
	color.xyz *= lig;
	return color;
}