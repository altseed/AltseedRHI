
struct VS_Input
{
	float3 Pos		: Pos;
	float2 UV		: UV;
	float4 Color		: Color;
};

struct VS_Output
{
	float4 Pos		: SV_POSITION;
	float2 UV		: TEXCOORD0;
	float4 Color		: COLOR;
};

VS_Output VS( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;
	float4 pos4 = { Input.Pos.x, Input.Pos.y, Input.Pos.z, 1.0 };
	Output.Pos = pos4;
	Output.Color = Input.Color;
	Output.UV = Input.UV;
	return Output;
}
