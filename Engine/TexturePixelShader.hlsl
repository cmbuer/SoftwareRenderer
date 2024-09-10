Texture2D tex;

SamplerState samplerState;


float4 main(float2 tc: TexCoord) : SV_TARGET
{
		return tex.Sample(samplerState, tc);
		//return (0.5f, 0.5f, 0.5f, 1.0f);
}