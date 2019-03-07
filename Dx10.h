#ifndef Dx10H
#define Dx10H
//#include "stdafx.h"

#pragma once

#include <windows.h>
#include <cwchar>


#include <d2d1.h>
#include <d2d1helper.h>
#include <DXGI.h>
#include <D3D11.h>
#include <D3D10.h>
#include <D3DX10Async.h>
//#include <d3dUtil.h>
#include <dinput.h>
#include <dwrite.h>
#include <D3dx9core.h>
#include <d3d9.h>

#include <math.h> 

#include <assert.h> 

#include "shader.h"
#include "Dx10.h"

#include "DXGI.h"

//#include "stdio.h"

//using namespace System;
//



#pragma pack (1)
struct Vertex
{
	float x,y;        
	float size;             
	UINT Color;
};
#pragma pack (8)


class TGrid;

//===============================================================================================================
class ColorRGB
{
private:

protected:

public:
	unsigned char R;
	unsigned char G;
	unsigned char B;	

	ColorRGB() {}
	//copy constructor
	ColorRGB(ColorRGB &rgb){R=rgb.R;G=rgb.G; B=rgb.B;}// '%'  in C++/CLI instead '&' C++ standart 
	ColorRGB operator = (ColorRGB &rgb) {R=rgb.R;G=rgb.G; B=rgb.B; return rgb;}	
};
//--------------------------------------------------------------------------------------------------------------------
class FontDx10 
{
private:
	ID3D10Device* dx10deviceFont;
	ID3DX10Font *pFontDx10_My;
	ID3DX10Sprite* m_pFontSpriteDx10_My;

	int HeightF; 
	UINT WidthF; 
	UINT SizeSprite;

	HRESULT CreateFontSprite(ID3D10Device*, int , UINT , UINT);
	FontDx10(){HeightF=20; WidthF = 0; SizeSprite=128; };//default
protected:

public:

	FontDx10(ID3D10Device*, int , UINT , UINT);

	//property int HeightF_P{int get(){return HeightF;}; void set(int value){InHeightF(value);};}
	void  InHeightF(int);
	int OutHeightF_P(){return HeightF;};
	 __declspec(property(get = OutHeightF_P, put = InHeightF)) int HeightF_P;

	 //	property UINT WidthF_P{UINT get(){return WidthF;}; void set(UINT value){InWidthF(value);};}
    void  InWidthF(UINT);
	UINT OutWidthF(){return WidthF;};
	__declspec(property(get = OutWidthF, put = InWidthF)) UINT WidthF_P;

	//	property UINT SizeSprite_P{UINT get(){return SizeSprite;}; void set(UINT value){InSizeSprite(value);};}
	void  InSizeSprite(UINT);
	UINT OutSizeSprite(){return SizeSprite;};
	__declspec(property(get = OutSizeSprite, put = InSizeSprite)) UINT SizeSprite_P;
};

//====================================================================================================

 class TDx10_My
{
private:
	//double OutData; 
	//void _fastcall InData(double);
	
	//static TDx10_My* TempDx10;
	

	 HRESULT  Dx10_DeviceCreate(HWND HandDst,int Width,int Height);
	 HRESULT  Dx10_CreateEffect();
	 
	 TDx10_My(HWND ,int ,int);

	// static ID3D10Effect *effect1Dx10_My;
     int Width_Target_Window;
	
	 int Height_Target_Window;
	
	 int X_leftMax;
	 int X_rightMax;
	 int Y_bottomMax;
	 int Y_topMax;

	 
	
	 
	 ID3D10Effect* effect1Dx10;
	 ID3D10RenderTargetView* g_pd3d10RenderTargetViewDx10;
	 ID3D10EffectTechnique*  g_pTechniqueDx10;
	 IDXGISwapChain* swapChainDx10;
	 

	 
	 
	 ID3D10EffectVectorVariable* g_RenderTargetSizeVariableDx10;
	 ID3D10EffectMatrixVariable* g_pWorldVariableDx10;
	 ID3D10EffectMatrixVariable* g_pWorldVariableDx10_1;
	 
    
	// Vertex  *verticesDx10; 

	 D3D10_RASTERIZER_DESC rdDx10;
	
	//ID3D10Buffer* VertexBufferDx10;

	 
//-------------------------------------------------	
protected:
	//TDx10_My^ NewDX10;

	


	HWND Handl_Target_Window;


 //HRESULT  CreateBufferDescDX10(TDx10_My &TDx10, UINT sizeBuff);


//TDx10_My(){};  	
//------------------------------------------------
public:
	 int NumberCurves;
	int DxCurveCountPoints;
	 Vertex  *verticesDx10;
	 UINT64 AddressBuffVertexBeforeMap;
	 Vertex  *verticesGridDx10;
	 
	 UINT SizeBuffGridDx10;
	 UINT NumberElementsGridDx10;
	 int BeginIndexForCurveBuff;


	 Vertex  *verticesCurveDx10;
	 UINT SizeBuffCurveDx10;

	 ID3D10Buffer* VertexBufferDx10;
	 ID3D10Device* d3dDeviceDx10;

	 ID3D10EffectScalarVariable* dx_widthDx10;
	 ID3D10EffectScalarVariable* dx_shiftDx10;

	//__property double Prop_Max = {read = OutData, write = InData}; //Builer C++
	//__declspec(property(get = getprop, put = putprop)) int the_prop;

	//property NumberCurves
//	int OutNumberCurves() {return NumberCurves;};	
 //   __declspec(property(get = OutNumberCurves)) int NumberCurves_P;

	//property float X_leftMax_P{float get(){return X_leftMax;};}
	int OutX_leftMax() {return X_leftMax;};	
	__declspec(property(get = OutX_leftMax)) int X_leftMax_P;

	//	property float X_rightMax_P{float get(){return X_rightMax;};}
	int OutX_rightMax() {return X_rightMax;};
	__declspec(property(get = OutX_rightMax)) int X_rightMax_P;

	//property float Y_bottomMax_P{float get(){return Y_bottomMax;};}
	int OutY_bottomMax() {return Y_bottomMax;};
	__declspec(property(get = OutY_bottomMax)) int Y_bottomMax_P;

	//property float Y_topMax_P{float get(){return Y_topMax;};}
	int OutY_topMax() {return Y_topMax;};
	__declspec(property(get = OutY_topMax)) int Y_topMax_P;

	//property double Prop_Max_P{double get(){return OutData;}; void set(double value){InData(value);};}
   // __declspec(property(get = OutData, put = InData)) double Prop_Max_P;

	//property int Width_Target_Window_P{int get(){return Width_Target_Window;};}
    int OutWidth_Target_Window() {return Width_Target_Window;};	
	__declspec(property(get = OutWidth_Target_Window)) int Width_Target_Window_P;

	//	property int Height_Target_Window_P{int get(){return Height_Target_Window;};}
	int OutHeight_Target_Window() {return Height_Target_Window;};
	__declspec(property(get = OutHeight_Target_Window)) int Height_Target_Window_P;

	//property ID3D10Device* d3dDeviceDx10_P{ID3D10Device* get(){return d3dDeviceDx10_My;};}
	ID3D10Device* Outd3dDeviceDx10(){return d3dDeviceDx10;};
	__declspec(property(get = Outd3dDeviceDx10)) ID3D10Device* d3dDeviceDx10_P;

	//	property IDXGISwapChain* swapChainDx10_P{IDXGISwapChain* get(){return swapChainDx10_My;};}
	IDXGISwapChain* OutswapChainDx10(){return swapChainDx10;};
    __declspec(property(get = OutswapChainDx10)) IDXGISwapChain* swapChainDx10_P;

	//	property ID3D10RenderTargetView* g_pd3d10RenderTargetViewDx10_P{ID3D10RenderTargetView* get(){return g_pd3d10RenderTargetViewDx10_My;};}
	ID3D10RenderTargetView* Outg_pd3d10RenderTargetViewDx10(){return g_pd3d10RenderTargetViewDx10;};
	__declspec(property(get = Outg_pd3d10RenderTargetViewDx10)) ID3D10RenderTargetView* g_pd3d10RenderTargetViewDx10_P;
	
	//property ID3D10Effect* effect1Dx10_P{ID3D10Effect* get(){return effect1Dx10_My;};}
	ID3D10Effect* Outeffect1Dx10(){return effect1Dx10;};
	__declspec(property(get = Outeffect1Dx10)) ID3D10Effect* effect1Dx10_P; 

	
	UINT64 OutAddressBuffVertexBeforeMap(){return AddressBuffVertexBeforeMap;};
	__declspec(property(get = OutAddressBuffVertexBeforeMap)) UINT64 AddressBuffVertexBeforeMap_P; 
	
	//property ID3D10Buffer* VertexBufferDx10_P{ID3D10Buffer* get(){return VertexBufferDx10_My;};void set(ID3D10Buffer* value){InVertexBuffer(value);};}
	void  InVertexBuffer(ID3D10Buffer* VertexBuffertemp) { VertexBufferDx10 = VertexBuffertemp;}
	ID3D10Buffer* OutVertexBufferDx10(){return VertexBufferDx10;};
	__declspec(property(get = OutVertexBufferDx10, put = InVertexBuffer)) ID3D10Buffer* VertexBufferDx10_P;

	//static HRESULT _fastcall Create_Object(TDx10_My^, HWND ,int ,int);
	static TDx10_My*  Create_ObjectDx10(HWND ,int , int );

	//static HRESULT result;
	
	//static HRESULT _fastcall Dx10_DeviceCreate(IntPtr HandDst,int Width,int Height);
	//static HRESULT _fastcall Dx10_DeviceCreated(System::Windows::Forms::Control^ ObjectDst);
	//static HRESULT _fastcall Dx10_CreateEffect();
	//property UINT Size_Vertex_BufferAdd_P{void set(UINT value){Size_Vertex_BufferAdd(value);};}	
	
	
	//property UINT Size_Vertex_Buffer_P{UINT get(){return Size_Vertex_Buffer;};void set(UINT value){InSize_Vertex_Buffer(value);};}
	 UINT Size_Vertex_Buffer;
	void  InSize_Vertex_Buffer(UINT size) { 
		Size_Vertex_Buffer=size;
	};
	UINT OutSize_Vertex_Buffer(){return Size_Vertex_Buffer;};
	__declspec(property(get = OutSize_Vertex_Buffer, put = InSize_Vertex_Buffer)) UINT Size_Vertex_Buffer_P;

	HRESULT  CreateVertexBuffer();
	void CopyBuffersGrid();
	void CopyBuffersCurve(UINT);
	void CopyBuffersArrCurve();
	void CopyBuffersDraw(UINT );
	void CopyBuffersDraw_1(UINT BegindrawGrid_or_Curve);

	void DeleteBuffDx10()
	{  
		this->verticesDx10=NULL;
		//this->VertexBufferDx10=NULL;
		this->VertexBufferDx10->Unmap();
		this->VertexBufferDx10->Release();  //!!!!!!!!!!!!!!!!That there was no memory leaks		
		delete [] (Vertex*)  this->AddressBuffVertexBeforeMap;
		this->ResultCreateBuff=E_UNEXPECTED;
	}
HRESULT ResultCreateBuff;
	 TDx10_My* DeleteObjectDx10() 
	{   
		delete  this;
		return NULL; 
	}
	 ~TDx10_My()	{free(this->verticesDx10);
	 this->DeleteBuffDx10();	
	 }	

    void DrawDx10(bool,UINT,UINT,UINT);

	void UpdateDx10(){
		this->swapChainDx10->Present( 0, 0 );
		
	};
	void PresentDx10();
	bool Flag_DrawDX10;

	UINT CountCreateBuffGrid;

	void ArrAllCurveY_Shift(int ShiftY);

	//TGrid* GridDx10;


};

//--------------------------------------------------------------------------------------------------------------
//------------------------------Grid----Grid--Grid---Grid---Grid----------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
 class TGrid
{

private:
	ID3D10EffectScalarVariable* dx_width_line;
	int Grid_Width, Grid_Height;	 	
	double widt_line_curent;		
	UINT NumbHorizLineGrid;	
	UINT NumbVerticLineGrid;
	TDx10_My* NewDX10;
	Vertex  *BufferGridVertex; 
	//int IntervalY_betweenHorizLine, IntervalY_betweenVertLine;
	int Temp_forSiftY;
	int d, d_beg, d_end;
	int HeightFontScale; UINT WidthFontScale; UINT SizeSprite_forFontScale;
	ID3DX10Sprite* m_pFontSpriteGrid;
	ID3DX10Font *pFontGrid;
	D3D10_RECT rectFontGrid;
	wchar_t ValueforFontScale[10];
	D3DXCOLOR colorFontScale;
		int TempForShiftFont;
		int tempSh1;

	int ValueShiftForText;
    float DataforText;

    //    ColorRGB ColorGrid;
	//	void __fastcall InColorGrid(ColorRGB);
	//	ColorRGB __fastcall OutColorGrid();
	
	//	ColorRGB ColorBackgroundScale;
	//	void __fastcall InColorBackgroundScale(ColorRGB);
	//	ColorRGB __fastcall OutColorBackgroundScale();

		UINT32 ColorGridLine;
		UINT32 ColorBackgroundScale;
		UINT32 ColorScaleDepth;
		int BackgroundWidth;
		int WidthScaleDepth;
		bool ScaleDepth;
	//	void __fastcall InScaleDepth(bool);
		bool BackgroundScale;
		UINT Size_BufferGrid;
		

		FontDx10* FontGrid;
		void CreateBufferGrid();
		void GridBufferFilling();

#define  NumberElementsGrid_Set    1
#define  NumberElementsGrid_Clear  0
		UINT NumberElementsGrid;
		void SetNumberElementsGrid(bool set)
		{
        if(set)NumberElementsGrid = (UINT)this->BackgroundScale+(UINT)this->ScaleDepth+this->NumbHorizLineGrid+this->NumbVerticLineGrid;
		else NumberElementsGrid = 0;
		};
		UINT GetNumberElementsGrid(){return NumberElementsGrid;};
		__declspec(property(get = GetNumberElementsGrid, put = SetNumberElementsGrid)) UINT NumberElementsGrid_P;
		HRESULT  CreateFontSpriteScale(ID3D10Device* dx10dev, int HeightF, UINT WidthF, UINT SizeSprite);
		int DrawTextScale(int PosspriteY, float ValueforText, D3DXCOLOR colorFont);

		int IntervalY_betweenHorizLine, IntervalY_betweenVertLine;

	//	float* ArrForTextScale;
	//	float* ArrForTextScale1;
	//	int SizeForTextArr;
		int GridHeight1_2;
//----------------------------------------------------------------------------------------------------
public:
	TGrid();
	
	TGrid(TDx10_My* NewDX10);
	


	TGrid* DeleteGridDx10() 
	{   
		delete  this;
		return NULL; 
		//
	}
	~TGrid(){//free(this->BufferGridVertex);

		delete [] this->BufferGridVertex;
		this->NewDX10->DeleteBuffDx10(); 
		//this->NewDX10->VertexBufferDx10->Release();  //!!!!!!!!!!!!!!!!That there was no memory leaks
		//delete [] (Vertex*)  NewDX10->AddressBuffVertexBeforeMap_P; //!!!!!!!!!!!!!!!!!!After the map is changed, so the buffer address, enter an additional value to a variable and store the addresses before map
		//delete [] NewDX10->verticesDx10;		
	};

	//property UINT Width_line_P{UINT get(){return widt_line_curent;}; void set(UINT value){InLinewidth(value);};}
	void  InWidth_line(UINT Data);
	UINT OutWidth_line(){return widt_line_curent;};
	__declspec(property(get = OutWidth_line, put = InWidth_line)) UINT Width_line_P;



//	property UINT NumbHorizLine_P{UINT get(){return NumbHorizLineGrid;}; void set(UINT value){InNumbHorizLine(value);};}
	void  InNumbHorizLine(UINT);
	UINT  OutNumbHorizLine(){return NumbHorizLineGrid;};
	__declspec(property(get = OutNumbHorizLine, put = InNumbHorizLine)) UINT NumbHorizLine_P;

	//	property UINT NumbVerticLine_P{UINT get(){return NumbVerticLineGrid;}; void set(UINT value){InNumbVerticLine(value);};}
	void  InNumbVerticLine(UINT);
	UINT  OutNumbVerticLine(){return NumbVerticLineGrid;};
	__declspec(property(get = OutNumbVerticLine, put = InNumbVerticLine)) UINT NumbVerticLine_P;

//	void setColorGrid(unsigned char, unsigned char, unsigned char);
//	property ColorRGB ColorGrid_P{ColorRGB get(){return OutColorGrid();}; void set(ColorRGB value){InColorGrid(value);};}	
	
//	void setColorBackgroundScale(unsigned char, unsigned char, unsigned char);
//	property ColorRGB ColorBackgroundScale_P{ColorRGB get(){return OutColorBackgroundScale();}; void set(ColorRGB value){InColorBackgroundScale(value);};}	

	//property UINT32 ColorGrid32_P{UINT32 get(){return ColorGrid32;}; void set(UINT32 value){InColorGrid32(value);};}
	void  InColorGridLine(UINT32);
	UINT32 OutColorGridLine(){return ColorGridLine;};
	__declspec(property(get = OutColorGridLine, put = InColorGridLine)) UINT32 ColorGridLine_P;

	//property UINT32 ColorBackgroundScale32_P{UINT32 get(){return ColorBackgroundScale32;}; void set(UINT32 value){InColorBackgroundScale32(value);};}
	void  InColorBackgroundScale(UINT32);
	UINT32 OutColorBackgroundScale(){return ColorBackgroundScale;};
    __declspec(property(get = OutColorBackgroundScale, put = InColorBackgroundScale)) UINT32 ColorBackgroundScale_P;

	void  InColorScaleDepth(UINT32);
	UINT32 OutColorScaleDepth(){return ColorScaleDepth;};
	__declspec(property(get = OutColorScaleDepth, put = InColorScaleDepth)) UINT32 ColorScaleDepth_P;

	//property UINT BackgroundWidth_P{UINT get(){return BackgroundWidth;}; void set(UINT value){InBackgroundWidth(value);};}
	void  InBackgroundWidth(int);
	int OutBackgroundWidth(){return BackgroundWidth;};
	__declspec(property(get = OutBackgroundWidth, put = InBackgroundWidth)) int BackgroundWidth_P;

	void InSize_Buffer(UINT sizebuf){Size_BufferGrid = sizebuf;};
	UINT OutSize_Buffer(){return Size_BufferGrid;};
	__declspec(property(get = OutSize_Buffer, put = InSize_Buffer)) UINT Size_BufferGrid_P;

	//property UINT WidthScaleDepth_P{UINT get(){return WidthScaleDepth;}; void set(UINT value){InWidthScaleDepth(value);};}
	void InWidthScaleDepth(int);
	int OutWidthScaleDepth(){return WidthScaleDepth;}
	__declspec(property(get = OutWidthScaleDepth, put = InWidthScaleDepth)) int WidthScaleDepth_P;
//	property bool ScaleDepth_P{bool get(){return ScaleDepth;}; void set(bool value){InScaleDepth(value);};}	

	void ArrGridY_Shift(int ShiftY);
		void TGrid::DrawTextGr(float* ValueforText);

		float* ArrForTextScale;
		float* ArrForTextScale1;
		int SizeForTextArr;
		//int IntervalY_betweenHorizLine, IntervalY_betweenVertLine;
		int OutIntervalY_betweenHorizLine(){return IntervalY_betweenHorizLine;}
		__declspec(property(get = OutIntervalY_betweenHorizLine)) int IntervalY_betweenHorizLine_P;
	
};	
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++===============================================
//=================================================================================================
class TCurve
{
private:
Vertex  *BufferCurveVertex; 
 int NumberPoints;
 int CurveCountPoints;
 int WidthCurve;
 int Numb_for_ShiftCurve;
 UINT32 ColorCurve;
 void CreateBufferCurve(int);
 int Size_BufferCurve;
 void drawTemp();
  void drawTemp_1();
 bool FlagArrayPutXY_BeginIndex;
 TDx10_My* CNewDX10;	
 int BeginIndexCurve;
 int NumberObjCurve;
 int beg_index_for_CopyBuffer;
 int HeightDraw;
 int HeightDraw1_2;

 public:
	
	 TCurve(){};
	TCurve(TDx10_My* CNewDX10);
//	 Vertex  *TempVerticesCurve;
	Vertex  *verticesCurve;

	void InWidthCurve(int);
	int OutWidthCurve(){return WidthCurve;}
	__declspec(property(get = OutWidthCurve, put = InWidthCurve)) int WidthCurve_P;

	void  InColorCurve(UINT32 colorin)  {ColorCurve=colorin;};
	UINT32 OutColorCurve(){return ColorCurve;};
	__declspec(property(get = OutColorCurve, put = InColorCurve)) UINT32 ColorCurve_P;

	bool DrawXY(float X,float Y);
	void putXY(float X, float Y);
	void putXY_1(float Xtemp, float Ytemp);
	void ArrayPutXY(float *ArrayXtemp, float *ArrayYtemp, UINT SizeArray);
	void ArrayPutXY_BeginIndex(UINT BeginIndex,float *ArrayXtemp, float *ArrayYtemp, UINT SizeArray);
	void ArrayPutXY_BeginIndex_1(UINT BeginIndex,float *ArrayXtemp, float *ArrayYtemp, UINT SizeArray);
	bool ArrayDrawXY();
	bool ArrayDrawXY_1();
	UINT MoveArrayXY(int Direct, int HowMany);
	UINT MoveArrayXY_1(int Direct, int HowMany);
	UINT TCurve::MoveArrayXY_Nimber(int Direct, int HowMany, int Number);
	void ArrCurveY_Shift(int ShiftY);
	void ArrCurveY_Shift_1(int ShiftY);
	void CopyBuffersCurveToDx10();
	
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++===============================================
//=================================================================================================

//=================================================================================================
class TManagerDraw
{
	private:
		 TDx10_My* DX10Draw;
		 TGrid* GridDraw;
		 TCurve* CurveDraw;
		 int HeightDraw;
		 int HeightDraw1_2;
		 int count_for_IntervalY_betwHorizLine;
		 bool Flag_text;
		 float y_in;
		 void EndDraw();

	public:
		TManagerDraw(TDx10_My* Dx10_in , TGrid* Grid_in, TCurve* Curve_in, float Y_Begin);
		void DrawIncY(float x_in, int incr_in, int count_in, float Data_Depth);
		void TManagerDraw::DrawIncY_NumbCurv(float x_in, int incr_in, int count_in, float Data_Depth, int numbercrv);
		

};

//++++++++++++++++++++++=====================================================================
class TManagerDrawArrCrv
{
private:
	TDx10_My* DX10Draw;
	TGrid* GridDraw;
	TCurve* CurveDraw;
	int HeightDraw;
	int HeightDraw1_2;
	int count_for_IntervalY_betwHorizLine;
	bool Flag_text;
	float y_in;
	void EndDraw();
	int NumbArrCurve;
	//int *BegIndexCurv;

public:
	TManagerDrawArrCrv(TDx10_My* Dx10_in , TGrid* Grid_in, TCurve* Curve_in, int NumbArrCrv, float Y_Begin);
	void DrawArrCrvIncY(float *x_in, int incr_in, int count_in, float Data_Depth);
	void EndDrawArrCrv();

};

#endif


//#include "stdafx.h"