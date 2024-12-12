
struct VertexShaderInput
{
    float3 position : POSITION;
	float2 Tex : TEXCOORD0;
};

struct VertexToPixel
{
    float4 position : SV_POSITION;
	float2 Tex : TEXCOORD0;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;
    output.position = float4(input.position, 1.0f);
	output.Tex = input.Tex;
    return output;
}
