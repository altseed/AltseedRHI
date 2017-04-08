
Texture2D	tex0		: register( t0 );
SamplerState	smp0		: register( s0 );

struct PS_Input
{
	float4 Pos		: SV_POSITION;
	float2 UV		: TEXCOORD0;
	float4 Color		: COLOR;
};


float4 PS( const PS_Input Input ) : SV_Target
{
	float4 Output = Input.Color * tex0.Sample(smp0, Input.UV);

	if(Output.a == 0.0f) discard;

	return Output;
}
