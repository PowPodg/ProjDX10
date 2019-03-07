
#ifndef shaderH
#define shaderH
	const char ArrShader[] =

" cbuffer cbChangeRare\n\
{  \n\
	float width;\n\
	float shift; \n\
	float count_vector=0.01;\n\
	float2 RenderTargetSize; \n\
} ; \n\
cbuffer cbChangePerObject \n\
{\n\
	matrix WorldViewProjection;\n\
} ;\n\
struct VS_INPUT\n\
{\n\
	float2 Position : POSITION;\n\
	float num : PSIZE;\n\
	float3 Color : COLOR0;\n\
};\n\
struct GEO_IN\n\
{\n\
	float4 Position : POSITION;\n\
//	float num : PSIZE;\n\
	float4 Color : COLOR0;\n\
};\n\
struct GEO_IN_GRID\n\
{\n\
	float4 Position : POSITION;\n\
	float width : PSIZE;  // //!!!!!!!size use for width\n\
	float4 Color : COLOR0;\n\
};\n\
struct GEO_OUT\n\
{\n\
	float4 Position : SV_POSITION;\n\
	float4 Color : COLOR0;\n\
   //	float4 Color : SV_Target0;\n\
   //	float2 TexCoord : TEXCOORD0;\n\
};\n\
//-----------------------------------------------------------------------------\n\
struct PS_OUTPUT\n\
{\n\
	float4 Color : SV_Target0;\n\
};\n\
//-------------------------------------------------------------------------------\n\
GEO_IN VS_LineV2(in VS_INPUT input)      // VS for Curves\n\
{\n\
	GEO_IN output;\n\
  //	input.Position.x *= shift; //для масштабирования\n\
 //	output.Position = mul(float4(input.Position, 0.0f, 1.0f), WorldViewProjection);\n\
	input.Position.x += shift*input.num;    //!!!!!!!size use for shift beetwen curves\n\
  //	output.Position = float4(input.Position,0.0f,1.0f); \n\
	output.Position = mul(float4(input.Position, 0.0f, 1.0f), WorldViewProjection);\n\
	output.Color = float4(input.Color,1.0f);\n\
	return output;\n\
}\n\
//-------------------------------------------------------------------------------\n\
GEO_IN_GRID VS_Line_GRID(in VS_INPUT input)    // VS for GRID\n\
{\n\
	GEO_IN_GRID output; \n\
	output.width = input.num;      //!!!!!!!size use for width \n\
	output.Position = float4(input.Position,0.0f,1.0f);   //последн. 1.0 для масштабирования\n\
  //	output.Position = mul(float4(input.Position, 0.0f, 1.0f), WorldViewProjection);\n\
	output.Color = float4(input.Color,1.0f);\n\
	return output;\n\
}\n\
//--------------------------------------------------------------------------------\n\
										// TriangleStream<GEO_OUT> output)\n\
 [maxvertexcount(6)]\n\
void GS_Curve(line GEO_IN points[2], inout TriangleStream<GEO_OUT> output)    // GS for CURVES\n\
{\n\
	float4 p0 = points[0].Position;\n\
	float4 p1 = points[1].Position;\n\
	float3 line01 = p1 - p0;\n\
	float3 dir = normalize(line01);\n\
	// scale to correct window aspect ratio\n\
	float3 ratio = float3(RenderTargetSize.y, RenderTargetSize.x, 0);\n\
	ratio = normalize(ratio);\n\
	float3 unit_z = normalize(float3(0, 0, -1));\n\
	float3 normal = normalize(cross(unit_z, dir) * ratio);\n\
  //	float width = 0.1;\n\
	GEO_OUT v[4];\n\
	float3 dir_offset = dir * ratio * width;          // для перекрытия концов линий \n\
	float3 normal_scaled = normal * ratio * width; \n\
	float3 p0_ex = p0 - dir_offset; \n\
	float3 p1_ex = p1 + dir_offset; \n\
	v[0].Position = float4(p0_ex - normal_scaled, 1); //  v[0].Position.x += v[0].Position.z*shift; \n\
	v[0].Color = points[0].Color;\n\
	v[1].Position = float4(p0_ex + normal_scaled, 1); //  v[1].Position.x += v[1].Position.z*shift;\n\
	v[1].Color = points[0].Color;\n\
	v[2].Position = float4(p1_ex + normal_scaled, 1); //  v[2].Position.x += v[2].Position.z*shift;\n\
	v[2].Color = points[0].Color;\n\
	v[3].Position = float4(p1_ex - normal_scaled, 1); //  v[3].Position.x += v[3].Position.z*shift;\n\
	v[3].Color = points[0].Color;\n\
	output.Append(v[2]);\n\
	output.Append(v[1]);\n\
	output.Append(v[0]);\n\
	output.RestartStrip();\n\
	output.Append(v[3]);\n\
	output.Append(v[2]);\n\
	output.Append(v[0]);\n\
	output.RestartStrip();\n\
}\n\
//-----------------------------------------------------------------------------------------\n\
										// TriangleStream<GEO_OUT> output)\n\
 [maxvertexcount(6)]\n\
void GS_GRID(line GEO_IN_GRID points[2], inout TriangleStream<GEO_OUT> output)       // GS for GRID\n\
{\n\
	float4 p0 = points[0].Position;\n\
	float4 p1 = points[1].Position;\n\
	GEO_OUT v[4];\n\
	p0.z=0; p1.z=0;\n\
	float tempXp0 = p0.x;\n\
	float tempXp1 = p1.x;\n\
	float tempYp0 = p0.y;\n\
	float tempYp1 = p1.y;\n\
	float tempWidth=points[0].width/2;\n\
	if(tempXp0==tempXp1)p0.x=tempXp0-tempWidth;\n\
	if(tempYp0==tempYp1)p0.y=tempYp0-tempWidth;\n\
	v[0].Position = p0;\n\
	v[0].Color = points[0].Color; \n\
	if(tempXp0==tempXp1)p0.x=tempXp0+tempWidth;\n\
	if(tempYp0==tempYp1)p0.y=tempYp0+tempWidth;\n\
	v[1].Position = p0;\n\
	v[1].Color = points[0].Color;\n\
	if(tempXp0==tempXp1)p1.x=tempXp1+tempWidth;\n\
	if(tempYp0==tempYp1)p1.y=tempYp1+tempWidth;\n\
	v[2].Position = p1;\n\
	v[2].Color = points[0].Color;\n\
	if(tempXp0==tempXp1)p1.x=tempXp1-tempWidth;\n\
	if(tempYp0==tempYp1)p1.y=tempYp1-tempWidth;\n\
	v[3].Position = p1;\n\
	v[3].Color = points[0].Color;\n\
   //	output.Append(v[0]);\n\
	output.Append(v[2]);\n\
	output.Append(v[1]);\n\
	output.Append(v[0]);\n\
	output.RestartStrip();\n\
  //	output.Append(v[0]);\n\
	output.Append(v[3]);\n\
	output.Append(v[2]);\n\
	output.Append(v[0]);\n\
	output.RestartStrip();\n\
}\n\
//----------------------------------------------------------------------------------------\n\
					   //for line single width\n\
[maxvertexcount(2)]\n\
void GS_GRID_Line(line GEO_IN_GRID points[2], inout LineStream<GEO_OUT> output)       // GS for GRID\n\
{\n\
  //	float4 p0 = points[0].Position;\n\
  //	float4 p1 = points[1].Position;\n\
	GEO_OUT v[2];\n\
	v[0].Position = points[0].Position;\n\
	v[0].Color = points[0].Color;\n\
	v[1].Position = points[1].Position;\n\
	v[1].Color = points[0].Color;\n\
	output.Append(v[1]);\n\
	output.Append(v[0]);\n\
	output.RestartStrip();\n\
}\n\
//-----------------------------------------------------------------------------------------\n\
[maxvertexcount(2)]\n\
void GS_CurveSingleWidth(line GEO_IN points[2], inout LineStream<GEO_OUT> output)       // GS for Line single width \n\
{ \n\
  //	float4 p0 = points[0].Position; \n\
  //	float4 p1 = points[1].Position; \n\
	GEO_OUT v[2];\n\
	v[0].Position = points[0].Position;\n\
	v[0].Color = points[0].Color;\n\
	v[1].Position = points[1].Position;\n\
	v[1].Color = points[0].Color;\n\
	output.Append(v[1]);\n\
	output.Append(v[0]);\n\
	output.RestartStrip();\n\
}\n\
//----------------------------------------------------------------------------------------- \n\
PS_OUTPUT PS_LineV2(GEO_OUT input) //: SV_Target \n\
{\n\
	PS_OUTPUT output; \n\
	//output.Color = LineTexture.Sample(LinearSampler, input.TexCoord); \n\
   // output.Color = float4(input.TexCoord.xy, 0, 1); \n\
   //	output.Color = float4(0.0f, 0.0f, 1.0f, 1.0f); \n\
	output.Color=input.Color; \n\
 //	output.Color = float4(0.0f, 0.0f, 1.0f, 1.0f); \n\
  // output.Color=output.Color;\n\
	return output; \n\
}\n\
 /* \n\
float4 PS( float4 Pos : SV_POSITION ) : SV_Target\n\
{ \n\
	return float4( 0.0f, 0.0f, 1.0f, 1.0f );    // Yellow, with Alpha = 1  \n\
}  */ \n\
technique10 Render\n\
{\n\
	pass P0\n\
	{\n\
		SetVertexShader(CompileShader(vs_4_0, VS_LineV2()));\n\
	  //   	SetGeometryShader( NULL );\n\
		SetGeometryShader(CompileShader(gs_4_0, GS_Curve()));\n\
		SetPixelShader(CompileShader(ps_4_0, PS_LineV2()));\n\
	  //	SetPixelShader(NULL); \n\
	}\n\
	pass P1\n\
	{\n\
		SetVertexShader(CompileShader(vs_4_0, VS_Line_GRID()));\n\
		SetGeometryShader(CompileShader(gs_4_0, GS_GRID()));\n\
		SetPixelShader(CompileShader(ps_4_0, PS_LineV2()));\n\
	}\n\
	pass P2\n\
	{\n\
		SetVertexShader(CompileShader(vs_4_0, VS_Line_GRID())); \n\
		SetGeometryShader(CompileShader(gs_4_0, GS_GRID_Line()));   //for line grid single width\n\
		SetPixelShader(CompileShader(ps_4_0, PS_LineV2()));\n\
	}\n\
	pass P3\n\
	{\n\
		SetVertexShader(CompileShader(vs_4_0, VS_LineV2()));\n\
		SetGeometryShader(CompileShader(gs_4_0, GS_CurveSingleWidth()));   //for line curve single width\n\
		SetPixelShader(CompileShader(ps_4_0, PS_LineV2()));\n\
	}\n\
}"
;

#endif
