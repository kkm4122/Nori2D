Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer cbChangesEveryFrame : register(b0)
{
	float4 vMeshColor;
};

struct VertexToPixel
{
    float4 position : SV_POSITION;
	float2 Tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	// return float4(1.0f, 0.5f, 0.5f, 1.f);
	return txDiffuse.Sample(samLinear, input.Tex) * vMeshColor;
}
