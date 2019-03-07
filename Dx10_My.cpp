
//#include "stdafx.h"
#include "Dx10_My.h"

IDXGISwapChain* swapChain;
ID3D10Device* d3dDevice;
ID3D10Texture2D *pBuffer;
ID3D10RenderTargetView* g_pd3d10RenderTargetView = NULL;
D3D10_VIEWPORT viewport;
ID3D10RasterizerState* pRS = NULL;
D3D10_RASTERIZER_DESC rd;
ID3D10Effect *effect1;
ID3D10EffectTechnique*  g_pTechnique = NULL;
 D3D10_PASS_DESC PassDesc;
 D3D10_INPUT_ELEMENT_DESC layout[] =
 {
	 //----------type var into shader------------------nuber of bytes for variable
	 //	 { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	 { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	 //	 { "POSITION", 0, DXGI_FORMAT_R32_SINT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	 //	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 12,D3D10_INPUT_PER_VERTEX_DATA, 0 },
	 { "PSIZE", 0, DXGI_FORMAT_R32_FLOAT, 0, 8, D3D10_INPUT_PER_VERTEX_DATA, 0 },    //use for shift between line into shader
	 //	 { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,D3D10_INPUT_PER_VERTEX_DATA, 0 },    //формат без альфа канала для увелич скорости
	 { "COLOR", 0, DXGI_FORMAT_R32_UINT, 0, 12,D3D10_INPUT_PER_VERTEX_DATA, 0 },    //цвет через TColor (Builder C++) для увелич скорости

 };
UINT numElements = sizeof(layout)/sizeof(layout[0]);
ID3D10InputLayout*  g_pVertexLayout_SI = NULL;

ID3D10EffectScalarVariable* dx_width;
ID3D10EffectScalarVariable* dx_shift;
ID3D10EffectVectorVariable* g_RenderTargetSizeVariable = NULL;
ID3D10EffectMatrixVariable* g_pWorldVariable = NULL;
ID3DX10Font *pFont = NULL;
ID3DX10Sprite* m_pFontSprite;


ID3D10Buffer* VertexBuffer;

D3DXVECTOR2 g_RenderTargetSize;
 D3DXMATRIX   g_World;
 Vertex  *verticesN;
  Vertex  *vertices;
 HRESULT result;

 UINT stride = sizeof( Vertex );
 UINT offset = 0;

D3D10_BUFFER_DESC bufferDesc;
D3D10_DRIVER_TYPE driverTypes[] =
{
	D3D10_DRIVER_TYPE_HARDWARE,
	D3D10_DRIVER_TYPE_REFERENCE,
};
UINT driverTypeIndex;
D3D10_DRIVER_TYPE           g_driverType = D3D10_DRIVER_TYPE_NULL;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

// stride 
//----------------------------------------------------------------------------------------------------------------------------------
Vertex *vert = NULL; int address;
	D3D10_BUFFER_DESC bufferDescDx10;
//--------------------------------------------------------------------------------------------
HRESULT  TDx10_My::Dx10_DeviceCreate(HWND HandDst,int Width,int Height)
{
	D3D10_DRIVER_TYPE driverTypes[] =
	{
		D3D10_DRIVER_TYPE_HARDWARE,
		D3D10_DRIVER_TYPE_REFERENCE,
	};
	UINT driverTypeIndex;
	D3D10_DRIVER_TYPE           g_driverType = D3D10_DRIVER_TYPE_NULL;



	HWND HanPan = (HWND) HandDst;//.ToPointer();

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );

	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = Width;
	swapChainDesc.BufferDesc.Height = Height;
	//swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;//DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//DXGI_FORMAT_R16G16B16A16_FLOAT;//DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//DXGI_FORMAT_R8G8B8A8_UNORM;
	//swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;//DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0; //60
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;   //1

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = HanPan;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect =  DXGI_SWAP_EFFECT_SEQUENTIAL;//DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags=0;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

	unsigned int driver = 0;

	UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

	for( driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )  //for automatic define type driver - Hardware or Programm
	{ 
		g_driverType = driverTypes[driverTypeIndex];

		result = D3D10CreateDeviceAndSwapChain(
			NULL,
			g_driverType,//D3D10_DRIVER_TYPE_HARDWARE,//D3D10_DRIVER_TYPE_REFERENCE,//D3D10_DRIVER_TYPE_HARDWARE,//D3D_DRIVER_TYPE_UNKNOWN,
			NULL,
			createDeviceFlags,
			D3D10_SDK_VERSION,
			&swapChainDesc,
			&swapChain,
			&d3dDevice
			);
		if( SUCCEEDED( result ) )   {//d3dDevice = d3dDevice; 
			//swapChainDx10_My = swapChain; 
			break;}	
	}


	if(SUCCEEDED(result))d3dDevice->ClearState();

	if(SUCCEEDED(result))result = swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D),(LPVOID*)&pBuffer);
	if(SUCCEEDED(result))result = d3dDevice->CreateRenderTargetView(pBuffer, NULL,  &g_pd3d10RenderTargetView);
	// pBuffer->Release();
	if(SUCCEEDED(result))d3dDevice->OMSetRenderTargets(1,&g_pd3d10RenderTargetView, NULL);
	if(SUCCEEDED(result))g_pd3d10RenderTargetView=g_pd3d10RenderTargetView;

	viewport.TopLeftX 	= 0;
	viewport.TopLeftY 	= 0;
	viewport.Width 		= Width;
	viewport.Height 	= Height;
	viewport.MinDepth 	= 0.0f;
	viewport.MaxDepth 	= 1.0f;

	if(SUCCEEDED(result))
	{
		d3dDevice->RSSetViewports(1, &viewport);

		rd.CullMode = D3D10_CULL_NONE;//D3D10_CULL_BACK;//D3D10_CULL_FRONT;//D3D10_CULL_NONE;
		rd.FillMode = D3D10_FILL_SOLID;//D3D10_FILL_WIREFRAME;//D3D10_FILL_SOLID;
		rd.FrontCounterClockwise = true;
		rd.DepthBias = 0;
		rd.DepthBiasClamp = 0;
		rd.SlopeScaledDepthBias = 0;
		rd.DepthClipEnable = true;
		rd.ScissorEnable = false;
		rd.MultisampleEnable = false;
		rd.AntialiasedLineEnable = false;
		result = d3dDevice->CreateRasterizerState(&rd, &pRS);
	}

	if(SUCCEEDED(result))	d3dDevice->RSSetState(pRS);

	return result;
}
//-------------------------------------------------------------------------------------------------------
HRESULT  TDx10_My::Dx10_CreateEffect()
{
		 result =  D3DX10CreateEffectFromFile(
										 L"LineDr.hlsl",
										  NULL,
										  NULL,
										  "fx_4_0",
										  D3D10_SHADER_ENABLE_STRICTNESS,//D3D10_SHADER_OPTIMIZATION_LEVEL3,//D3D10_SHADER_ENABLE_STRICTNESS,
										  NULL,
										  d3dDevice,
										  NULL,
										  NULL,
										  &effect1,
										  NULL,
										  NULL );
    
	/*
	  ID3D10Blob* errors = 0;
	  result = D3DX10CreateEffectFromMemory(
											ArrShader,
											sizeof(ArrShader),
											0,
											0,
											0,
											"fx_4_0",
											D3D10_SHADER_ENABLE_STRICTNESS|D3D10_SHADER_OPTIMIZATION_LEVEL3,
											0,//D3D10_EFFECT_SINGLE_THREADED,
											d3dDevice,
											0,
											0,
											&effect1,
											&errors,
											0);
		*/		
//if( SUCCEEDED( result ) ) effect1Dx10_My = effect1;
if(SUCCEEDED(result)){
		 g_pTechnique = effect1->GetTechniqueByName( "Render" );
		 dx_width = effect1->GetVariableByName( "width" )->AsScalar();
		 dx_shift = effect1->GetVariableByName( "shift" )->AsScalar();

		 g_RenderTargetSizeVariable = effect1->GetVariableByName( "RenderTargetSize" )->AsVector();
		 g_pWorldVariable = effect1->GetVariableByName( "WorldViewProjection" )->AsMatrix();
		 g_pTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );


                      }

if(SUCCEEDED(result))
result = d3dDevice->CreateInputLayout(
	layout,
	numElements,
	PassDesc.pIAInputSignature,
	PassDesc.IAInputSignatureSize,
	&g_pVertexLayout_SI
	);

if(SUCCEEDED(result))d3dDevice->IASetInputLayout( g_pVertexLayout_SI );

	
		 return result;
}

//--------------------------constructor-------------------------------------------------
TDx10_My::TDx10_My(HWND HandDst,int Width,int Height)
{
result = Dx10_DeviceCreate(HandDst,Width,Height);
if(SUCCEEDED(result)) result =  Dx10_CreateEffect();
}
//-------------------------------------------------------------------------
/*
HRESULT _fastcall TDx10_My::Create_Object(TDx10_My^ TempDx, HWND HandDst,int Width,int Height)
{
TempDx = gcnew TDx10_My(HandDst, Width, Height);
if(SUCCEEDED(result))
  {
TempDx10 = TempDx;
TempDx10->Handl_Target_Window=HandDst; TempDx10->Width_Target_Window=Width; TempDx10->Height_Target_Window=Height;
TempDx10->SinglePixelX = 2/((float)Width);
TempDx10->SinglePixelY = 2/((float)Height);
TempDx10->X_leftMax=-1.0;
TempDx10->X_rightMax=1.0;
TempDx10->Y_bottomMax=-1.0;
TempDx10->Y_topMax=1.0;
}
return result;
}
*/
//------------------------------------------------------------------------------------------
TDx10_My* TDx10_My::Create_ObjectDx10(HWND HandDst,int Width,int Height)
{   
	//TDx10_My TempDx10(HandDst, Width, Height);
	TDx10_My* TempDx10 = new TDx10_My(HandDst, Width, Height);
	
	if(SUCCEEDED(result))
	{
		TempDx10->Handl_Target_Window=HandDst; TempDx10->Width_Target_Window=Width; TempDx10->Height_Target_Window=Height;
		TempDx10->d3dDeviceDx10=d3dDevice; 

		TempDx10->swapChainDx10=swapChain;
		TempDx10->g_pd3d10RenderTargetViewDx10=g_pd3d10RenderTargetView;
		TempDx10->effect1Dx10=effect1;
		TempDx10->g_pTechniqueDx10=g_pTechnique;

		TempDx10->dx_widthDx10=dx_width;
		TempDx10->dx_shiftDx10=dx_shift;
		
		float Width_line=13;
		dx_width->SetFloat(Width_line);

		g_RenderTargetSize.x=Width;
		g_RenderTargetSize.y=Height;

	//	g_RenderTargetSizeVariable->SetFloatVector((float*)&g_RenderTargetSize);
	//	D3DXMatrixIdentity( &g_World );  // Initialize the world matrix   обнуляет матрицу

	//	D3DXMatrixOrthoLH(&g_World, (float)Width, (float)Height, 0.0f, 1.0f);
	
	//	g_pWorldVariable->SetMatrix((float*)&g_World); // Update variables -

		TempDx10->g_RenderTargetSizeVariableDx10=g_RenderTargetSizeVariable;
        TempDx10->g_RenderTargetSizeVariableDx10->SetFloatVector((float*)&g_RenderTargetSize);

		D3DXMatrixIdentity( &g_World );  // Initialize the world matrix   обнуляет матрицу
		D3DXMatrixOrthoLH(&g_World, (float)Width, (float)Height, 0.0f, 1.0f);//Convert screen coordinates to format -1:1
	 	TempDx10->g_pWorldVariableDx10=g_pWorldVariable;
		TempDx10->g_pWorldVariableDx10->SetMatrix((float*)&g_World); // Update variables -

		TempDx10->X_leftMax=-Width/2;
		TempDx10->X_rightMax=Width/2;
		TempDx10->Y_bottomMax=-Height/2;
		TempDx10->Y_topMax=Height/2;

		TempDx10->Size_Vertex_Buffer=0;
	}
	
	if(FAILED(result))
		{
			TempDx10 = nullptr;
	    }

	  TempDx10->VertexBufferDx10 = NULL;

	UINT SizeArrVertices =  TempDx10->Size_Vertex_Buffer;
	bufferDescDx10.Usage = D3D10_USAGE_DYNAMIC;//D3D10_USAGE_DEFAULT;
	// Размер, в байтах
	bufferDescDx10.ByteWidth = SizeArrVertices;// sizeof(Vertex)*(2000*300+4);//3;
	// Используется как буфер вершин
	bufferDescDx10.BindFlags = D3D10_BIND_VERTEX_BUFFER;//D3D10_BIND_CONSTANT_BUFFER;//D3D10_BIND_VERTEX_BUFFER;
	// Прочие флаги
	bufferDescDx10.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;//D3D10_CPU_ACCESS_READ;//D3D10_CPU_ACCESS_WRITE;//0;
	bufferDescDx10.MiscFlags = 0;
	//Создаём буфер,

	//this->VertexBufferDx10->GetDevice(&this->d3dDeviceDx10);
	HRESULT result1 = TempDx10->d3dDeviceDx10->CreateBuffer(&bufferDescDx10, NULL, &TempDx10->VertexBufferDx10);

	TempDx10->verticesDx10 = (Vertex *)malloc(544);//default You must first call to malloc, realloc to work!!!!!!!!!!!!!!!!!!!!

	//vert = (Vertex*) realloc(vert,544);
	
		return TempDx10;
}
//-------------------------------------------------------------------------------
HRESULT TDx10_My::CreateArrVerices(Vertex  *BufferGridVertex_temp, UINT Size_BufferGrid_temp)
{
	//this->d3dDeviceDx10->ClearState();
/*
   this->VertexBufferDx10 = NULL;

	UINT SizeArrVertices =  this->Size_Vertex_Buffer;
	bufferDescDx10.Usage = D3D10_USAGE_DYNAMIC;//D3D10_USAGE_DEFAULT;
	// Размер, в байтах
	bufferDescDx10.ByteWidth = SizeArrVertices;// sizeof(Vertex)*(2000*300+4);//3;
	// Используется как буфер вершин
	bufferDescDx10.BindFlags = D3D10_BIND_VERTEX_BUFFER;//D3D10_BIND_CONSTANT_BUFFER;//D3D10_BIND_VERTEX_BUFFER;
	// Прочие флаги
	bufferDescDx10.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;//D3D10_CPU_ACCESS_READ;//D3D10_CPU_ACCESS_WRITE;//0;
	bufferDescDx10.MiscFlags = 0;
	//Создаём буфер,

	//this->VertexBufferDx10->GetDevice(&this->d3dDeviceDx10);
	HRESULT result1 = this->d3dDeviceDx10->CreateBuffer(&bufferDescDx10, NULL, &this->VertexBufferDx10);*/
	//
HRESULT result1;	
	//  if(SUCCEEDED(result1))
//	    {
	   // this->verticesDx10  = new  Vertex[SizeArrVertices/sizeof(Vertex)];
		//free(this->verticesDx10);
	//	this->verticesDx10 = NULL;
		
	//	this->verticesDx10 = (Vertex*) realloc(this->verticesDx10,SizeArrVertices);
		//vert=vert;
		 
	//    }
	 
return result1;
}
//--------------------------------------------------------------------------------------------------
void TDx10_My::CopyBuffers(Vertex  *BufferGridVertex_temp, UINT Size_BufferGrid_temp)
{
//	this->verticesDx10 = (Vertex*) realloc(this->verticesDx10,Size_BufferGrid_temp);
//	this->VertexBufferDx10->Map(D3D10_MAP_WRITE_DISCARD, NULL, (void**)&this->verticesDx10);
//	memcpy_s(&this->verticesDx10[0], Size_BufferGrid_temp, &BufferGridVertex_temp[0], Size_BufferGrid_temp);	
	//this->VertexBufferDx10->Unmap();

  //  this->verticesDx10 = (Vertex*)address;

   //  this->VertexBufferDx10->Unmap();
	 //   vert=(Vertex*)address;
}
//---------------------------------------------------------------------------------
void TDx10_My::DrawDx10(bool SetClear, UINT PassByIndex, UINT EndPointVertexBuff,UINT BeginPointVertexBuff)
{
	if(SetClear)this->d3dDeviceDx10->ClearRenderTargetView(this->g_pd3d10RenderTargetViewDx10, D3DXCOLOR(255,255,255,0) );

	this->d3dDeviceDx10->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	this->d3dDeviceDx10->IASetVertexBuffers( 0, 1, &this->VertexBufferDx10, &stride, &offset );

	this->g_pTechniqueDx10->GetPassByIndex(PassByIndex)->Apply(0);            //GetPassByIndex( 3 ) - single width; GetPassByIndex( 0 ) - with set width
	this->d3dDeviceDx10->Draw(EndPointVertexBuff, BeginPointVertexBuff );

	this->swapChainDx10->Present( 0, 0 );
}

//--------------------------------------------------------------------------------- 
///-------------------------------------GRID---------------------------------------------------
//==============================================================================================================================================
TGrid::TGrid(TDx10_My* NewDX10Temp)
{
NewDX10=NewDX10Temp;
NewDX10->VertexBufferDx10=NewDX10Temp->VertexBufferDx10;
NewDX10->verticesDx10=NewDX10Temp->verticesDx10;
Grid_Width=NewDX10->Width_Target_Window_P;
Grid_Height=NewDX10->Height_Target_Window_P;
//dx_width_line = effect1Dx10_My->GetVariableByName( "width" )->AsScalar();

widt_line_curent=1;      //default
NumbHorizLineGrid = 5;   //default
NumbVerticLineGrid = 10; //default
ScaleDepth=true;         //default
BackgroundScale = true;
BackgroundWidth = 40;  //default
WidthScaleDepth = 3;   //default
InWidth_line(widt_line_curent); 

ColorGridLine = 0xC0C0C0;//set color default 'gray' DXGI_FORMAT_R32_UINT A single-component, 32-bit unsigned-integer format
ColorBackgroundScale = 0xC0C0C0;//default
ColorScaleDepth = 0;//default
//FontGrid = new FontDx10(NewDX10->d3dDeviceDx10_P,20,0,128);  //default create font for scale


//BufferGridVertex = (Vertex*)malloc(this->Size_BufferGrid);//default You must first call to malloc, realloc to work!!!!!!!!!!!!!!!!!!!!

//CreateBufferGrid(BackgroundScale, ScaleDepth, NumbHorizLineGrid, NumbVerticLineGrid);

	
	BufferGridVertex = NULL;
	Size_BufferGrid = sizeof(Vertex)*(2*(UINT)BackgroundScale+2*(UINT)ScaleDepth+2*NumbHorizLineGrid+2*NumbVerticLineGrid); //2 - чтобы сформировать отрезки
	NewDX10->Size_Vertex_Buffer_P = Size_BufferGrid;
	
	BufferGridVertex = new Vertex[Size_BufferGrid/sizeof(Vertex)];
//	BufferGridVertex = (Vertex*)malloc(Size_BufferGrid);
	//BufferGridVertex = (Vertex*)realloc(this->BufferGridVertex,this->Size_BufferGrid);



//CreateBufferGrid();
GridBufferFilling();
//HRESULT result2 = NewDX10Temp->CreateArrVerices(BufferGridVertex,Size_BufferGrid); 
//NewDX10Temp->CopyBuffers(BufferGridVertex,Size_BufferGrid);

UINT SizeArrVertices =  544;
	bufferDescDx10.Usage = D3D10_USAGE_DYNAMIC;//D3D10_USAGE_DEFAULT;
	// Размер, в байтах
	bufferDescDx10.ByteWidth = SizeArrVertices;// sizeof(Vertex)*(2000*300+4);//3;
	// Используется как буфер вершин
	bufferDescDx10.BindFlags = D3D10_BIND_VERTEX_BUFFER;//D3D10_BIND_CONSTANT_BUFFER;//D3D10_BIND_VERTEX_BUFFER;
	// Прочие флаги
	bufferDescDx10.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;//D3D10_CPU_ACCESS_READ;//D3D10_CPU_ACCESS_WRITE;//0;
	bufferDescDx10.MiscFlags = 0;
	//Создаём буфер,


//NewDX10Temp->VertexBufferDx10->Unmap();
	NewDX10->VertexBufferDx10=NULL;
	//this->VertexBufferDx10->GetDevice(&this->d3dDeviceDx10);
	HRESULT result1 = NewDX10->d3dDeviceDx10->CreateBuffer(&bufferDescDx10, NULL, &NewDX10->VertexBufferDx10);
	// NewDX10Temp->verticesDx10=NULL;
//	NewDX10Temp->verticesDx10 = (Vertex*) realloc(NewDX10Temp->verticesDx10,544);
	
	// NewDX10Temp->verticesDx10=NULL;
//	free(NewDX10Temp->verticesDx10);
    NewDX10->verticesDx10=NULL;
	NewDX10->verticesDx10 = new Vertex[Size_BufferGrid/sizeof(Vertex)] ;
	address=(int)&NewDX10->verticesDx10[0];
   // NewDX10Temp->verticesDx10 = (Vertex*) realloc(NewDX10Temp->verticesDx10,544);
	NewDX10->VertexBufferDx10->Map(D3D10_MAP_WRITE_DISCARD, NULL, (void**)&NewDX10->verticesDx10);
	memcpy_s(&NewDX10->verticesDx10[0], 544, &BufferGridVertex[0], Size_BufferGrid);
//if(SUCCEEDED(result2))
	NewDX10->DrawDx10(true,1,Size_BufferGrid/sizeof(Vertex),0);
}
//------------------------------------------------------------------------------------------------------------------------------------
//void TGrid::CreateBufferGrid(bool BackgroundScale_temp, bool ScaleDepth_temp, UINT NumbHorizLineGrid_temp, UINT NumbVerticLineGrid_temp)
void TGrid::CreateBufferGrid()
{   
	this->BufferGridVertex = NULL;
	free(this->BufferGridVertex);
	this->BufferGridVertex = NULL;
	this->Size_BufferGrid = sizeof(Vertex)*(2*(UINT)this->BackgroundScale+2*(UINT)this->ScaleDepth+2*this->NumbHorizLineGrid+2*this->NumbVerticLineGrid); //2 - чтобы сформировать отрезки
	NewDX10->Size_Vertex_Buffer_P = this->Size_BufferGrid;
	//this->BufferGridVertex = new Vertex[this->Size_BufferGrid/sizeof(Vertex)];
	this->BufferGridVertex = (Vertex*)realloc(this->BufferGridVertex,this->Size_BufferGrid);
}
//----------------------------------------------------- 
void TGrid::GridBufferFilling()
{
	UINT countPoints=0;
	if(this->BackgroundScale)
	{
		//first point for BackgroundScale
		this->BufferGridVertex[countPoints].size = this->BackgroundWidth;    //!!!!!!!size use for width
		this->BufferGridVertex[countPoints].x = NewDX10->X_leftMax_P+this->BackgroundWidth/2;
		this->BufferGridVertex[countPoints].y = NewDX10->Y_topMax_P;
		this->BufferGridVertex[countPoints].Color=this->ColorBackgroundScale;
		countPoints++;
		//second point for BackgroundScale
		this->BufferGridVertex[countPoints].size = this->BackgroundWidth;    //!!!!!!!size use for width
		this->BufferGridVertex[countPoints].x = NewDX10->X_leftMax_P+this->BackgroundWidth/2;
		this->BufferGridVertex[countPoints].y = NewDX10->Y_bottomMax_P;
		this->BufferGridVertex[countPoints].Color=this->ColorBackgroundScale;
		countPoints++;
	}
	if(this->ScaleDepth)
	{
		//first point for ScaleDepth
		this->BufferGridVertex[countPoints].size = this->WidthScaleDepth;    //!!!!!!!size use for width
		this->BufferGridVertex[countPoints].x = NewDX10->X_leftMax_P+this->BackgroundWidth;
		this->BufferGridVertex[countPoints].y = NewDX10->Y_topMax_P;
		this->BufferGridVertex[countPoints].Color=this->ColorScaleDepth;
		countPoints++;
		//second point for ScaleDepth
		this->BufferGridVertex[countPoints].size = this->WidthScaleDepth;    //!!!!!!!size use for width
		this->BufferGridVertex[countPoints].x = NewDX10->X_leftMax_P+this->BackgroundWidth;
		this->BufferGridVertex[countPoints].y = NewDX10->Y_bottomMax_P;
		this->BufferGridVertex[countPoints].Color=this->ColorScaleDepth;
		countPoints++;
	}
	//-------------------------

	if(this->NumbHorizLineGrid>0)
	{
		float tempY=0; int countHorLines=0; 
mGorLines:
		tempY = (float)((float)countHorLines * (NewDX10->Y_topMax_P+fabs(NewDX10->Y_bottomMax_P))/(float)this->NumbHorizLineGrid);
		this->BufferGridVertex[countPoints].size = this->widt_line_curent;    //!!!!!!!size use for width
		this->BufferGridVertex[countPoints].x = NewDX10->X_leftMax_P;
		this->BufferGridVertex[countPoints].y = NewDX10->Y_topMax_P-tempY;
		this->BufferGridVertex[countPoints].Color = this->ColorGridLine;
		countPoints++;

		this->BufferGridVertex[countPoints].size = this->widt_line_curent;    //!!!!!!!size use for width
		this->BufferGridVertex[countPoints].x = NewDX10->X_rightMax_P;
		this->BufferGridVertex[countPoints].y = NewDX10->Y_topMax_P-tempY;
		this->BufferGridVertex[countPoints].Color = this->ColorGridLine;
		countPoints++;

		countHorLines++;
		if(countHorLines<this->NumbHorizLineGrid) { goto mGorLines;}
	}
	//---------------------------------------------------

	if(this->NumbVerticLineGrid>0)
	{
		float tempX=0; int countVertLines=0;
mVertLines:
		tempX = (float)((float)countVertLines * (fabs(NewDX10->X_leftMax_P)+NewDX10->X_rightMax_P)/(float)this->NumbVerticLineGrid);
		this->BufferGridVertex[countPoints].size = this->widt_line_curent;    //!!!!!!!size use for width
		this->BufferGridVertex[countPoints].x = NewDX10->X_leftMax_P+tempX;
		this->BufferGridVertex[countPoints].y = NewDX10->Y_topMax_P;
		this->BufferGridVertex[countPoints].Color= this->ColorGridLine;
		countPoints++;

		this->BufferGridVertex[countPoints].size = this->widt_line_curent;    //!!!!!!!size use for width
		this->BufferGridVertex[countPoints].x = NewDX10->X_leftMax_P+tempX;
		this->BufferGridVertex[countPoints].y = NewDX10->Y_bottomMax_P;
		this->BufferGridVertex[countPoints].Color = this->ColorGridLine;
		countPoints++;

		countVertLines++;
		if(countVertLines<this->NumbVerticLineGrid) { goto mVertLines;}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------

//-----------property line width
void  TGrid::InWidth_line(UINT Data)
{
 widt_line_curent=Data;
}


void  TGrid::InNumbHorizLine(UINT Data1)
{
 NumbHorizLineGrid=Data1;
}
//---------------property num vertic. line-------------

void  TGrid::InNumbVerticLine(UINT Data2)
{
	NumbVerticLineGrid=Data2;
}
//---------------property color grid-------------
void TGrid::InWidthScaleDepth(int scaleDept_temp)
{
this->WidthScaleDepth=scaleDept_temp;
GridBufferFilling();
HRESULT result2 = NewDX10->CreateArrVerices(this->BufferGridVertex,this->Size_BufferGrid); 
NewDX10->CopyBuffers(this->BufferGridVertex,this->Size_BufferGrid);

if(SUCCEEDED(result2))NewDX10->DrawDx10(true,1,this->Size_BufferGrid/sizeof(Vertex),0);
}
/*
void TGrid::setColorGrid(unsigned char R, unsigned char G, unsigned char B)
{
ColorGrid.R=R;
ColorGrid.G=G;
ColorGrid.B=B;
InColorGrid(ColorGrid);
}

void __fastcall TGrid::InColorGrid(ColorRGB rgb)
{
ColorGrid=rgb;
//place code here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
ColorRGB __fastcall TGrid::OutColorGrid()
{	
return ColorGrid;
}*/
void  TGrid::InColorGridLine(UINT32 rgb)
{
	ColorGridLine=rgb;
	//place code here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
//---------------property Color Background Scale-------------
void  TGrid::InColorBackgroundScale(UINT32 rgb)
{
	ColorBackgroundScale=rgb;
	//place code here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
//----------------------------------------------
void TGrid::InColorScaleDepth(UINT32 rgb)
{
	ColorScaleDepth=rgb;
}
//---------------property Background width-------------
void  TGrid::InBackgroundWidth(int width)
{
BackgroundWidth=width;
}

//---------------property WidthScaleDepth-------------
//void  TGrid::InWidthScaleDepth(UINT width1)
//{
//	WidthScaleDepth=width1;
//}

//--------------------class Font------------------------------
HRESULT  FontDx10::CreateFontSprite(ID3D10Device* dx10dev, int HeightF, UINT WidthF, UINT SizeSprite)
{
	//Create Font for depth and etc.
	HRESULT result1 = D3DX10CreateFont(
		dx10dev,
		HeightF,
		WidthF,
		FW_BOLD,  //width font
		1,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"System",
		&pFont
		);

	assert(pFont);
	//Create Sprite for text drawing
	if(SUCCEEDED(result1))D3DX10CreateSprite(dx10dev, SizeSprite, &m_pFontSprite);
return result1;
}
//-----------------------------------------
void  FontDx10::InHeightF(int HeightFtemp)
{
	HeightF=HeightFtemp;
HRESULT result1 = CreateFontSprite(dx10deviceFont,HeightF, WidthF, SizeSprite);
//place code here redraw font!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
//-----------------------------------
void  FontDx10::InWidthF(UINT WidthFtemp)
{
	HRESULT result1 = CreateFontSprite(dx10deviceFont,HeightF, WidthFtemp, SizeSprite);
	//place code here redraw font!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
//----------------------------------
void  FontDx10::InSizeSprite(UINT SizeSprite_temp)
{
	HRESULT result1 = CreateFontSprite(dx10deviceFont,HeightF, WidthF, SizeSprite_temp);
	//place code here redraw font!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
//------------------------------------
 FontDx10::FontDx10(ID3D10Device* dx10devtemp, int HeightFtemp, UINT WidthFtemp, UINT SizeSpritetemp)
{
	dx10deviceFont = dx10devtemp;
	HeightF=HeightFtemp;
	WidthF=WidthFtemp;
	SizeSprite=SizeSpritetemp;
	HRESULT result1 = CreateFontSprite(dx10deviceFont,HeightF, WidthF, SizeSprite);

	if(SUCCEEDED(result1)) 
		{  
			m_pFontSpriteDx10_My = m_pFontSprite;
			pFontDx10_My=pFont;
	    }

}
// #include "stdafx.h"

