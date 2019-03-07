// Texture2D LineTexture;
//-----------------------------------------------------------------------------
// Constant Buffers
//-----------------------------------------------------------------------------

cbuffer cbChangeRare
{
	float width;
	float shift;
	float count_vector=0.01;
	float2 RenderTargetSize;
} ;

cbuffer cbChangePerObject
{
	matrix WorldViewProjection;
} ;
// Shader Input / Output Structures
//-----------------------------------------------------------------------------
//static float Temp;
//------------------------------------------------------------------------------
struct VS_INPUT
{
	float2 Position : POSITION;

	float num : PSIZE;
	uint Color : COLOR0;
};

struct GEO_IN
{
	float4 Position : POSITION;
//	float num : PSIZE;
	float4 Color : COLOR0;
};

struct GEO_IN_GRID
{
	float4 Position : POSITION;
	float width : PSIZE;  // //!!!!!!!size use for width
	float4 Color : COLOR0;
};

struct GEO_OUT
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
   //	float4 Color : SV_Target0;
   //	float2 TexCoord : TEXCOORD0;
};

//-----------------------------------------------------------------------------

struct PS_OUTPUT
{
	float4 Color : SV_Target0;
};
//-------------------------------------------------------------------------------
GEO_IN VS_LineV2(in VS_INPUT input)      // VS for Curves
{
	GEO_IN output;

  //	input.Position.x *= shift; //для масштабирования
 //	output.Position = mul(float4(input.Position, 0.0f, 1.0f), WorldViewProjection);

//  int TempPos = input.Position.x; TempPos &= 0x0000ffff; float x = TempPos;  	x /=32767;
 // TempPos = input.Position.y; TempPos >>= 16; TempPos &= 0x0000ffff; float y = TempPos;  	y /=32767;
  	input.Position.x += shift*input.num;    //!!!!!!!size use for shift beetwen curves
   //	x += shift*input.num;

    output.Position = float4(input.Position,0.0f,1.0f);
   //	output.Position = float4(x,y,0.0f,1.0f);

 //	output.Position = mul(float4(input.Position, 0.0f, 1.0f), WorldViewProjection);
	uint colorTemp = input.Color;   colorTemp &= 0x000000ff;  float r = (float)colorTemp/255;
	colorTemp = input.Color;   colorTemp >>= 8; colorTemp &= 0x000000ff;  float g = (float)colorTemp/255;
	colorTemp = input.Color;   colorTemp >>= 16; colorTemp &= 0x000000ff;  float b = (float)colorTemp/255;
	output.Color = float4(r,g,b,1.0f);



	return output;
}

//-------------------------------------------------------------------------------
GEO_IN_GRID VS_Line_GRID(in VS_INPUT input)    // VS for GRID
{
	GEO_IN_GRID output;


	output.width = input.num;      //!!!!!!!size use for width
	output.Position = float4(input.Position,0.0f,1.0f);   //последн. 1.0 для масштабирования
  //	output.Position = mul(float4(input.Position, 0.0f, 1.0f), WorldViewProjection);
	uint colorTemp = input.Color;   colorTemp &= 0x000000ff;  float r = (float)colorTemp/255;
	colorTemp = input.Color;   colorTemp >>= 8; colorTemp &= 0x000000ff;  float g = (float)colorTemp/255;
	colorTemp = input.Color;   colorTemp >>= 16; colorTemp &= 0x000000ff;  float b = (float)colorTemp/255;
	output.Color = float4(r,g,b,1.0f);

	return output;
}
//--------------------------------------------------------------------------------
										// TriangleStream<GEO_OUT> output)
 [maxvertexcount(6)]
void GS_Curve(line GEO_IN points[2], inout TriangleStream<GEO_OUT> output)    // GS for CURVES
{
	float4 p0 = points[0].Position;
	float4 p1 = points[1].Position;

	float3 line01 = p1 - p0;
	float3 dir = normalize(line01);

	// scale to correct window aspect ratio
	float3 ratio = float3(RenderTargetSize.y, RenderTargetSize.x, 0);
	ratio = normalize(ratio);

	float3 unit_z = normalize(float3(0, 0, -1));

	float3 normal = normalize(cross(unit_z, dir) * ratio);

  //	float width = 0.1;
	GEO_OUT v[4];

	float3 dir_offset = dir * ratio * width;          // для перекрытия концов линий
	float3 normal_scaled = normal * ratio * width;

	float3 p0_ex = p0 - dir_offset;
	float3 p1_ex = p1 + dir_offset;

	v[0].Position = float4(p0_ex - normal_scaled, 1); //  v[0].Position.x += v[0].Position.z*shift;
	v[0].Color = points[0].Color;

	v[1].Position = float4(p0_ex + normal_scaled, 1); //  v[1].Position.x += v[1].Position.z*shift;
	v[1].Color = points[0].Color;

	v[2].Position = float4(p1_ex + normal_scaled, 1); //  v[2].Position.x += v[2].Position.z*shift;
	v[2].Color = points[0].Color;

	v[3].Position = float4(p1_ex - normal_scaled, 1); //  v[3].Position.x += v[3].Position.z*shift;
	v[3].Color = points[0].Color;

	output.Append(v[2]);
	output.Append(v[1]);
	output.Append(v[0]);
	output.RestartStrip();

	output.Append(v[3]);
	output.Append(v[2]);
	output.Append(v[0]);
	output.RestartStrip();
}
//-----------------------------------------------------------------------------------------
										// TriangleStream<GEO_OUT> output)
 [maxvertexcount(6)]
void GS_GRID(line GEO_IN_GRID points[2], inout TriangleStream<GEO_OUT> output)       // GS for GRID
{
 	float4 p0 = points[0].Position;
	float4 p1 = points[1].Position;

	GEO_OUT v[4];

	p0.z=0; p1.z=0;
	float tempXp0 = p0.x;
	float tempXp1 = p1.x;
	float tempYp0 = p0.y;
	float tempYp1 = p1.y;
	float tempWidth=points[0].width/2;

	if(tempXp0==tempXp1)p0.x=tempXp0-tempWidth;
	if(tempYp0==tempYp1)p0.y=tempYp0-tempWidth;
	v[0].Position = p0;
	v[0].Color = points[0].Color;

	if(tempXp0==tempXp1)p0.x=tempXp0+tempWidth;
	if(tempYp0==tempYp1)p0.y=tempYp0+tempWidth;
	v[1].Position = p0;
	v[1].Color = points[0].Color;

	if(tempXp0==tempXp1)p1.x=tempXp1+tempWidth;
	if(tempYp0==tempYp1)p1.y=tempYp1+tempWidth;
	v[2].Position = p1;
	v[2].Color = points[0].Color;

	if(tempXp0==tempXp1)p1.x=tempXp1-tempWidth;
	if(tempYp0==tempYp1)p1.y=tempYp1-tempWidth;
	v[3].Position = p1;
	v[3].Color = points[0].Color;

   //	output.Append(v[0]);
	output.Append(v[2]);
	output.Append(v[1]);
	output.Append(v[0]);
	output.RestartStrip();

  //	output.Append(v[0]);
	output.Append(v[3]);
	output.Append(v[2]);
	output.Append(v[0]);
	output.RestartStrip();
}
//----------------------------------------------------------------------------------------
					   //for line single width
[maxvertexcount(2)]
void GS_GRID_Line(line GEO_IN_GRID points[2], inout LineStream<GEO_OUT> output)       // GS for GRID
{
  //	float4 p0 = points[0].Position;
  //	float4 p1 = points[1].Position;

	GEO_OUT v[2];

	v[0].Position = points[0].Position;
	v[0].Color = points[0].Color;

	v[1].Position = points[1].Position;
	v[1].Color = points[0].Color;

	output.Append(v[1]);
	output.Append(v[0]);
	output.RestartStrip();

}
//-----------------------------------------------------------------------------------------
[maxvertexcount(2)]
void GS_CurveSingleWidth(line GEO_IN points[2], inout LineStream<GEO_OUT> output)       // GS for Line single width
{
  //	float4 p0 = points[0].Position;
  //	float4 p1 = points[1].Position;

	GEO_OUT v[2];

	v[0].Position = points[0].Position;
	v[0].Color = points[0].Color;

	v[1].Position = points[1].Position;
	v[1].Color = points[0].Color;

	output.Append(v[1]);
	output.Append(v[0]);
	output.RestartStrip();

}
//-----------------------------------------------------------------------------------------
PS_OUTPUT PS_LineV2(GEO_OUT input) //: SV_Target
{
	PS_OUTPUT output;

	//output.Color = LineTexture.Sample(LinearSampler, input.TexCoord);
   // output.Color = float4(input.TexCoord.xy, 0, 1);
   //	output.Color = float4(0.0f, 0.0f, 1.0f, 1.0f);
	output.Color=input.Color;
 //	output.Color = float4(0.0f, 0.0f, 1.0f, 1.0f);
  // output.Color=output.Color;
	return output;
}
 /*
float4 PS( float4 Pos : SV_POSITION ) : SV_Target
{
	return float4( 0.0f, 0.0f, 1.0f, 1.0f );    // Yellow, with Alpha = 1
}  */

technique10 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS_LineV2()));
	  //   	SetGeometryShader( NULL );
		SetGeometryShader(CompileShader(gs_4_0, GS_Curve()));
		SetPixelShader(CompileShader(ps_4_0, PS_LineV2()));
	  //	SetPixelShader(NULL);
	}

	pass P1
	{
		SetVertexShader(CompileShader(vs_4_0, VS_Line_GRID()));
		SetGeometryShader(CompileShader(gs_4_0, GS_GRID()));
		SetPixelShader(CompileShader(ps_4_0, PS_LineV2()));
	}

	pass P2
	{
		SetVertexShader(CompileShader(vs_4_0, VS_Line_GRID()));
		SetGeometryShader(CompileShader(gs_4_0, GS_GRID_Line()));   //for line grid single width
		SetPixelShader(CompileShader(ps_4_0, PS_LineV2()));
	}

	pass P3
	{
		SetVertexShader(CompileShader(vs_4_0, VS_LineV2()));
		SetGeometryShader(CompileShader(gs_4_0, GS_CurveSingleWidth()));   //for line curve single width
		SetPixelShader(CompileShader(ps_4_0, PS_LineV2()));
	}

}

