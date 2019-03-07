
//#include "stdafx.h"
#include "Dx10.h"
 int countttt=1;

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
ID3D10EffectMatrixVariable* g_pWorldVariable1 = NULL;
ID3DX10Font *pFont = NULL;
ID3DX10Sprite* m_pFontSprite;


ID3D10Buffer* VertexBuffer;

D3DXVECTOR2 g_RenderTargetSize;
 D3DXMATRIX   g_World, g_World1;
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
D3D10_DRIVER_TYPE           g_driverType = D3D10_DRIVER_TYPE_HARDWARE;//D3D10_DRIVER_TYPE_NULL;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;


	//D3D10_RECT rectFontGrid = {0, 0, 40, 20};

// stride 
//----------------------------------------------------------------------------------------------------------------------------------
Vertex *vert = NULL; 
	D3D10_BUFFER_DESC bufferDescDx10;
	UINT createDeviceFlags = 0;

	FLOAT OriginalBlendFactor[4];
	UINT   OriginalSampleMask = 0;
	ID3D10BlendState* pFontBlendState10=NULL;

	ID3D10BlendState *pOriginalBlendState10;

	
//--------------------------------------------------------------------------------------------
HRESULT  TDx10_My::Dx10_DeviceCreate(HWND HandDst,int Width,int Height)
{
	D3D10_DRIVER_TYPE driverTypes[] =
	{
		D3D10_DRIVER_TYPE_HARDWARE,
		D3D10_DRIVER_TYPE_REFERENCE,
	};
	UINT driverTypeIndex;
	D3D10_DRIVER_TYPE           g_driverType = D3D10_DRIVER_TYPE_HARDWARE;//D3D10_DRIVER_TYPE_NULL;



	HWND HanPan = (HWND) HandDst;//.ToPointer();

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );

	swapChainDesc.BufferCount = 1 ;

	swapChainDesc.BufferDesc.Width = Width; // Width-2 if if BorderStyle is FixedSingle; Width-4; - for VCL  if BorderStyle is bsSingle
	swapChainDesc.BufferDesc.Height = Height; // Width-2 if if BorderStyle is FixedSingle; Height-4; - - for VCL  if BorderStyle is bsSingle  
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST;//DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//DXGI_FORMAT_R16G16B16A16_FLOAT;//DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;//DXGI_MODE_SCALING_CENTERED;//DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 200; //60
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;   //1

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = HanPan;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect =DXGI_SWAP_EFFECT_DISCARD;// DXGI_SWAP_EFFECT_SEQUENTIAL;//DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags=0;

	//UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

	unsigned int driver = 0;

	UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

//	IDXGIDevice * pDXGIDevice;
//	result = d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
//	IDXGIAdapter *pDXGIAdapter;
	//pDXGIDevice->GetAdapter(&pDXGIAdapter);
//	DXGI_ADAPTER_DESC *adapterDesc;
//	pDXGIAdapter->GetDesc(adapterDesc);





	for( driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )  //for automatic define type driver - Hardware or Programm
	{ 
		//g_driverType = driverTypes[driverTypeIndex];

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

	/*	result = D3D10CreateDevice(
			NULL,
			g_driverType,
			NULL,
			createDeviceFlags,
			D3D10_SDK_VERSION,
			&d3dDevice
			);*/
		if( SUCCEEDED( result ) )   {//d3dDevice = d3dDevice; 
			//swapChainDx10_My = swapChain; 
			break;}	
	}


	if(SUCCEEDED(result))d3dDevice->ClearState();

	if(SUCCEEDED(result))result = swapChain->GetBuffer(0, __uuidof(pBuffer),(LPVOID*)&pBuffer);
	//D3D10_RENDER_TARGET_VIEW_DESC pDesc;//=NULL;
	if(SUCCEEDED(result))result = d3dDevice->CreateRenderTargetView(pBuffer, NULL ,  &g_pd3d10RenderTargetView);
	// pBuffer->Release();
	if(SUCCEEDED(result))d3dDevice->OMSetRenderTargets(1,&g_pd3d10RenderTargetView, NULL);
	if(SUCCEEDED(result))g_pd3d10RenderTargetView=g_pd3d10RenderTargetView;

	viewport.TopLeftX 	= 0;//-Width/2;
	viewport.TopLeftY 	= 0;//Height/2;
	viewport.Width 		= (float)Width;
	viewport.Height 	= (float)Height;
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
		 g_pWorldVariable1 = effect1->GetVariableByName( "WorldViewProjection1" )->AsMatrix();
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
		TempDx10->d3dDeviceDx10->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

		TempDx10->swapChainDx10=swapChain;
		TempDx10->g_pd3d10RenderTargetViewDx10=g_pd3d10RenderTargetView;
		TempDx10->effect1Dx10=effect1;
		TempDx10->g_pTechniqueDx10=g_pTechnique;


		
		float Width_line=0.02;
		dx_width->SetFloat(Width_line);
		float Shift_line=1.0; 
		dx_shift->SetFloat(Shift_line);

		TempDx10->dx_widthDx10=dx_width;
		TempDx10->dx_shiftDx10=dx_shift;

		g_RenderTargetSize.x=Width;
		g_RenderTargetSize.y=Height;

	//	g_RenderTargetSizeVariable->SetFloatVector((float*)&g_RenderTargetSize);
	//	D3DXMatrixIdentity( &g_World );  // Initialize the world matrix   обнуляет матрицу

	//	D3DXMatrixOrthoLH(&g_World, (float)Width, (float)Height, 0.0f, 1.0f);
	
	//	g_pWorldVariable->SetMatrix((float*)&g_World); // Update variables -

		TempDx10->g_RenderTargetSizeVariableDx10=g_RenderTargetSizeVariable;
        TempDx10->g_RenderTargetSizeVariableDx10->SetFloatVector((float*)&g_RenderTargetSize);

		D3DXMatrixIdentity( &g_World );  // Initialize the world matrix   обнуляет матрицу
		D3DXMatrixOrthoRH(&g_World, (float)Width, (float)Height, -1.0f, 1.0f);//Convert screen coordinates to format -1:1
	 	TempDx10->g_pWorldVariableDx10=g_pWorldVariable;
		TempDx10->g_pWorldVariableDx10->SetMatrix((float*)&g_World); // Update variables -
  //for test
	//	D3DXMatrixIdentity( &g_World1 );
	//	D3DXMatrixOrthoLH(&g_World1, 2, 2/(Width/Height), -1.0f, 1.0f);
	//	TempDx10->g_pWorldVariableDx10_1=g_pWorldVariable1;
	//	TempDx10->g_pWorldVariableDx10_1->SetMatrix((float*)&g_World1); // Update variables -

		TempDx10->X_leftMax=-Width/2;
		TempDx10->X_rightMax=Width/2;
		TempDx10->Y_bottomMax=-Height/2;
		TempDx10->Y_topMax=Height/2;


		TempDx10->VertexBufferDx10=NULL;
		TempDx10->Size_Vertex_Buffer=0;
		TempDx10->ResultCreateBuff=E_UNEXPECTED;
		//TempDx10->GridDx10=NULL;
		TempDx10->verticesGridDx10=NULL;
		TempDx10->verticesCurveDx10=NULL;
		
		TempDx10->SizeBuffGridDx10=0;
		TempDx10->SizeBuffCurveDx10=0;
		
		TempDx10->NumberElementsGridDx10=0;
		TempDx10->CountCreateBuffGrid=0;
		//TempDx10->GridDx10->Size_BufferGrid_P=0;
		TempDx10->DxCurveCountPoints=0;
		TempDx10->NumberCurves=0;

		//TempDx10->d3dDeviceDx10->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		
		
	}
	
	if(FAILED(result))
		{
			TempDx10->VertexBufferDx10 = NULL;
			TempDx10 = NULL;
			
	    }


	
		return TempDx10;
}
//-------------------------------------------------------------------------------
HRESULT  TDx10_My::CreateVertexBuffer()
{
   // this->VertexBufferDx10 = NULL;



   if(SUCCEEDED(this->ResultCreateBuff)) this->DeleteBuffDx10();
   //delete [] this->verticesDx10;
   //this->verticesDx10=new Vertex[this->Size_Vertex_Buffer/sizeof(Vertex)];
   // this->verticesDx10 = (Vertex*)realloc(this->verticesDx10,this->Size_Vertex_Buffer);
    ZeroMemory( &bufferDescDx10, sizeof(bufferDescDx10) );   
	bufferDescDx10.Usage = D3D10_USAGE_DYNAMIC;//D3D10_USAGE_DEFAULT;
	// Размер, в байтах
	bufferDescDx10.ByteWidth = (this->Size_Vertex_Buffer%16) + this->Size_Vertex_Buffer;// sizeof(Vertex)*(2000*300+4);//3;
	// Используется как буфер вершин
	bufferDescDx10.BindFlags = D3D10_BIND_VERTEX_BUFFER;//D3D10_BIND_SHADER_RESOURCE;//D3D10_BIND_VERTEX_BUFFER;//D3D10_BIND_CONSTANT_BUFFER;//D3D10_BIND_VERTEX_BUFFER;
	// Прочие флаги
	bufferDescDx10.CPUAccessFlags =D3D10_CPU_ACCESS_WRITE;//D3D10_CPU_ACCESS_READ;//D3D10_CPU_ACCESS_WRITE;//0;
	bufferDescDx10.MiscFlags = 0;
	//Создаём буфер,

	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = (void*)this->verticesDx10;
	InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

	
	this->ResultCreateBuff = this->d3dDeviceDx10->CreateBuffer(&bufferDescDx10, NULL, &this->VertexBufferDx10);
	
	  if(SUCCEEDED(this->ResultCreateBuff))
	    {
        this->verticesDx10=NULL;
		//delete [] this->verticesDx10;
	   // this->verticesDx10  = new  Vertex[this->Size_Vertex_Buffer/sizeof(Vertex)];
	   // this->verticesDx10 = (Vertex*)realloc(this->verticesDx10,this->Size_Vertex_Buffer);
		this->AddressBuffVertexBeforeMap =(int)&this->verticesDx10[0];

		
		this->VertexBufferDx10->Map(D3D10_MAP_WRITE_DISCARD, NULL, (void**)&this->verticesDx10);
		//this->verticesDx10 = (Vertex*)realloc(this->verticesDx10,this->Size_Vertex_Buffer);
		/*D3D10_BOX b;
		b.bottom = 1;
		b.back = 1;
		b.top = 0;;
		b.front = 0;
		b.left = 0;
		b.right = 0;
		this->d3dDeviceDx10->UpdateSubresource(this->VertexBufferDx10, 0, &b, this->verticesDx10, this->Size_Vertex_Buffer, 0);
		*/
	    }
	   if(FAILED(this->ResultCreateBuff))
	   {
this->Size_Vertex_Buffer=this->Size_Vertex_Buffer;
	   }
	//  	this->d3dDeviceDx10->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
this->d3dDeviceDx10->IASetVertexBuffers( 0, 1, &this->VertexBufferDx10, &stride, &offset );	 
return this->ResultCreateBuff; 
	
}
//--------------------------------------------------------------------------------------------------
void TDx10_My::CopyBuffersGrid()
{
  memmove_s(&this->verticesDx10[0], this->SizeBuffGridDx10,&this->verticesGridDx10[0], this->SizeBuffGridDx10);
  this->SizeBuffGridDx10=this->SizeBuffGridDx10;
//  memcpy_s(&this->verticesDx10[this->SizeBuffGridDx10/sizeof(Vertex)], this->SizeBuffCurveDx10,&this->verticesCurveDx10[0], this->SizeBuffCurveDx10); 
//  memcpy_s(&this->verticesDx10[BeginIndexDest], Size_BufferGrid_temp,&BufferGridVertex_temp[0], Size_BufferGrid_temp);  // copy for 1 byte
 //  wmemcpy_s((wchar_t*)&this->verticesDx10[0], this->Size_Vertex_Buffer/2,(wchar_t*)&BufferGridVertex_temp[0], Size_BufferGrid_temp/2);// copy for 2 bytes? - maybe fast???????!!!!!!!!!	
}

void TDx10_My::CopyBuffersCurve(UINT sizebuffcurve_temp)
{
memmove_s(&this->verticesDx10[this->SizeBuffGridDx10/sizeof(Vertex)], sizebuffcurve_temp,&this->verticesCurveDx10[0], sizebuffcurve_temp);
}
//--------------------------------------------------------------------------------
void TDx10_My::CopyBuffersArrCurve()
{
//	memcpy_s(&this->verticesDx10[this->SizeBuffGridDx10/sizeof(Vertex)], sizebuffcurve_temp,&this->verticesCurveDx10[0], sizebuffcurve_temp);
}
//--------------------------------------------------------------------------------
//BegindrawGrid_or_Curve=0 - begin draw Grid and then Curve
//BegindrawGrid_or_Curve=1 - begin draw Curve and then Grid
void TDx10_My::CopyBuffersDraw(UINT BegindrawGrid_or_Curve)
{
	int Temp_index=(this->SizeBuffGridDx10)/sizeof(Vertex);//+2 add goriz line and vertic line in GRID on border - to the down Y and to the right X


this->CopyBuffersGrid();
if(this->DxCurveCountPoints>0)this->CopyBuffersCurve((this->DxCurveCountPoints-1) * sizeof(Vertex));
if(BegindrawGrid_or_Curve==0){
this->DrawDx10(true,1,(this->NumberElementsGridDx10+2)*2,0); //+2 add goriz line and vertic line in GRID on border - to the down Y and to the right X
this->DrawDx10(false,0,Temp_index+this->DxCurveCountPoints,Temp_index);
                             }

if(BegindrawGrid_or_Curve==1) {
	this->DrawDx10(true,0,Temp_index+this->DxCurveCountPoints,Temp_index);
	this->DrawDx10(false,1,Temp_index,0); 
                              }
//this->PresentDx10();
}

//---------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//BegindrawGrid_or_Curve=0 - begin draw Grid and then Curve
//BegindrawGrid_or_Curve=1 - begin draw Curve and then Grid
void TDx10_My::CopyBuffersDraw_1(UINT BegindrawGrid_or_Curve)
{
	int Temp_index=(this->SizeBuffGridDx10)/sizeof(Vertex);//+2 add goriz line and vertic line in GRID on border - to the down Y and to the right X


	this->CopyBuffersGrid();

	if(BegindrawGrid_or_Curve==0){
		this->DrawDx10(true,1,Temp_index,0); //+2 add goriz line and vertic line in GRID on border - to the down Y and to the right X
		this->DrawDx10(false,0,Temp_index+this->DxCurveCountPoints,Temp_index);
	}

	if(BegindrawGrid_or_Curve==1) {
		this->DrawDx10(true,0,Temp_index+this->DxCurveCountPoints,Temp_index);
		this->DrawDx10(false,1,Temp_index,0); 
	}
	//this->PresentDx10();
}

//---------------------------------------------------------------------------------
void TDx10_My::DrawDx10(bool SetClear, UINT PassByIndex, UINT EndPointVertexBuff,UINT BeginPointVertexBuff)
{
	if(SetClear)this->d3dDeviceDx10->ClearRenderTargetView(this->g_pd3d10RenderTargetViewDx10, D3DXCOLOR(255,255,255,0) );

	//this->d3dDeviceDx10->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	//this->d3dDeviceDx10->IASetVertexBuffers( 0, 1, &this->VertexBufferDx10, &stride, &offset );

	this->g_pTechniqueDx10->GetPassByIndex(PassByIndex)->Apply(0);            //GetPassByIndex( 3 ) - single width; GetPassByIndex( 0 ) - with set width
	this->d3dDeviceDx10->Draw(EndPointVertexBuff, BeginPointVertexBuff );
    Flag_DrawDX10=true;
}

//---------------------------------------------------------------------------------
void TDx10_My::PresentDx10()

{   	
this->swapChainDx10->Present( 0, 0);  //flickering image if this code in cause GRID and on CURVE - twice in a row!!!!!!!!!!!!!!!!
//this->VertexBufferDx10->Unmap(); //!!!!!!!!!!!!!!!If you made the MAP, then you need to make the DRAW and UNMAP. otherwise it would be a memory leak
}
//--------------------------------------------------------------
void TDx10_My::ArrAllCurveY_Shift(int ShiftY)
{
	int d;
	for (d=0;d<this->DxCurveCountPoints;d++)
	{
		this->verticesCurveDx10[d].y+=ShiftY;
	}
}
///-------------------------------------GRID---------------------------------------------------
//==============================================================================================================================================
TGrid::TGrid(TDx10_My* NewDX10Temp)
{
NewDX10=NewDX10Temp;
//NewDX10->VertexBufferDx10=NewDX10Temp->VertexBufferDx10;
//NewDX10->verticesDx10=NewDX10Temp->verticesDx10;
Grid_Width=NewDX10->Width_Target_Window_P;
Grid_Height=NewDX10->Height_Target_Window_P;

GridHeight1_2=Grid_Height/2;
//dx_width_line = effect1Dx10_My->GetVariableByName( "width" )->AsScalar();

widt_line_curent=1;      //default
NumbHorizLineGrid = 6;   //default
NumbVerticLineGrid = 10; //default
ScaleDepth=true;         //default
BackgroundScale = true;
BackgroundWidth = 50;  //default
WidthScaleDepth = 3;   //default

HeightFontScale = 15; //default
WidthFontScale = FW_BOLD;  //width font default
SizeSprite_forFontScale=128;  //default
rectFontGrid.left=4;//default
rectFontGrid.top=0;//default
rectFontGrid.right=rectFontGrid.left+100;//default
rectFontGrid.bottom=20;//default

ValueShiftForText=0;
this->TempForShiftFont=0;
tempSh1=0;

colorFontScale = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);//default

InWidth_line(widt_line_curent); 

ColorGridLine = 0xC0C0C0;//set color default 'gray' DXGI_FORMAT_R32_UINT A single-component, 32-bit unsigned-integer format
ColorBackgroundScale = 0xC0C0C0;//default
ColorScaleDepth = 0;//default
//FontGrid = new FontDx10(NewDX10->d3dDeviceDx10_P,20,0,128);  //default create font for scale

this->Size_BufferGrid=0;

NumberElementsGrid_P=NumberElementsGrid_Set; 
NewDX10->NumberElementsGridDx10 += this->NumberElementsGrid;
IntervalY_betweenHorizLine = (NewDX10->Y_topMax_P+abs(NewDX10->Y_bottomMax_P))/this->NumbHorizLineGrid;
IntervalY_betweenVertLine = (abs(NewDX10->X_leftMax_P)+NewDX10->X_rightMax_P)/this->NumbVerticLineGrid;
Temp_forSiftY=0;   

CreateBufferGrid();
NewDX10->CreateVertexBuffer(); 
GridBufferFilling();

d_beg =(this->BackgroundScale+this->ScaleDepth)*2;
d_end =(this->BackgroundScale+this->ScaleDepth+this->NumbHorizLineGrid+2)*2;



//create font for scale
result=CreateFontSpriteScale(this->NewDX10->d3dDeviceDx10, this->HeightFontScale, this->WidthFontScale, this->SizeSprite_forFontScale);

this->ArrForTextScale=NULL; this->ArrForTextScale1=NULL;
this->ArrForTextScale = (float*)realloc(this->ArrForTextScale, sizeof(float)*(this->NumbHorizLineGrid+3));
this->ArrForTextScale1 = (float*)realloc(this->ArrForTextScale1, sizeof(float)*(this->NumbHorizLineGrid+3));
SizeForTextArr=(this->NumbHorizLineGrid+1) * sizeof(float);//for move array
for(int i=0;i<=this->NumbHorizLineGrid+2;i++)
{
	ArrForTextScale[i]=0;
	ArrForTextScale1[i] = GridHeight1_2 - i*IntervalY_betweenHorizLine;
}
//----------------------------

if(SUCCEEDED(NewDX10->ResultCreateBuff ))
	{
  //  UINT numbeginpoint = NewDX10->SizeBuffGridDx10 - this->Size_BufferGrid;
	NewDX10->CopyBuffersGrid();
// if one time - ERROR!!!!!!!!! 
	NewDX10->DrawDx10(false,0,0,0);// пришлось пока вызывать предварительно, иначе ошибка когда сразу увеличиаем размер через свойство, когда постепенно - то нормально - приходится 2 раза вначале
    NewDX10->PresentDx10();
	// //!!!!!!!!!!!!!!!If you made the MAP, then you need to make the DRAW and UNMAP. otherwise it would be a memory leak
    NewDX10->DrawDx10(true,1,(NewDX10->NumberElementsGridDx10+2)*2,0); //+2 add goriz line and vertic line in GRID on border - to the down Y and to the right X

    // DrawTextScale(ValueShiftForText,2154.223, colorFontScale);

    NewDX10->PresentDx10();

    }
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
HRESULT  TGrid::CreateFontSpriteScale(ID3D10Device* dx10dev, int HeightF, UINT WidthF, UINT SizeSprite)
{
	//Create Font for depth and etc.
	HRESULT result1 = D3DX10CreateFont(
		dx10dev,
		HeightF,
		0,
		WidthF,  //width font
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

	if(SUCCEEDED(result)) 
	{  
		this->m_pFontSpriteGrid = m_pFontSprite;
		this->pFontGrid=pFont;
	}
	return result1;
}
//----------------------------------------- 
//-----------------------------------------------------------------
int TGrid::DrawTextScale(int PosspriteY, float ValueforText, D3DXCOLOR colorFont)
{   
	int drawres;
	int PosspriteY_temp; 
	PosspriteY_temp = (this->NewDX10->Y_topMax_P-PosspriteY); //converting (Height/2,0,-Height/2)  to (0, Height) 

	D3DX10_SPRITE *pSprites = NULL;
	
	rectFontGrid.top = PosspriteY_temp; 
	rectFontGrid.bottom = PosspriteY_temp+20;
	swprintf(ValueforFontScale, 10, L"%4.1f", ValueforText);

  //   result=this->m_pFontSpriteGrid->Begin(D3DX10_SPRITE_SORT_TEXTURE|D3DX10_SPRITE_SAVE_STATE);
//	this->m_pFontSpriteGrid->DrawSpritesImmediate(pSprites,0,128*128,0);
//		result=this->m_pFontSpriteGrid->End();
	
	result=this->m_pFontSpriteGrid->Begin(D3DX10_SPRITE_SORT_TEXTURE|D3DX10_SPRITE_SAVE_STATE);
	
	if(SUCCEEDED(result))drawres = this->pFontGrid->DrawText(this->m_pFontSpriteGrid,  ValueforFontScale, -1, &rectFontGrid, DT_LEFT, colorFont);
	
	//this->NewDX10->d3dDeviceDx10->OMGetBlendState(&pOriginalBlendState10,OriginalBlendFactor,&OriginalSampleMask);
	// Set the blend state for font drawing

	result=this->m_pFontSpriteGrid->End();

	

	return drawres;
}
//--------------------------------------------------------------------------
void TGrid::DrawTextGr(float *ValueforText)
{
	int temp=0;
	
	for (d=d_beg+1;d<=d_end-2;d+=2)
	{
	//if(ShiftGridEvent)
		DrawTextScale(this->NewDX10->verticesGridDx10[d].y+10, ValueforText[temp], colorFontScale);
	//if(ShiftGridEvent) DrawTextScale((this->NewDX10->verticesGridDx10[d].y), ValueforText[temp], colorFontScale);
	temp++;
	}
	temp=temp;
	//this->NewDX10->d3dDeviceDx10->OMSetBlendState(pOriginalBlendState10,OriginalBlendFactor, OriginalSampleMask);
	//this->NewDX10->d3dDeviceDx10->OMSetDepthStencilState( 0, 0 );
}
//----------------------------------------------------------------- void TCurve::ArrCurveY_Shift(int ShiftY)
void TGrid::ArrGridY_Shift(int ShiftY)
{

	int tempSh = ShiftY;
	int diff; 

	this->Temp_forSiftY += ShiftY;
	if(this->TempForShiftFont<this->NewDX10->Y_topMax_P) this->TempForShiftFont += ShiftY;

	if(abs(this->Temp_forSiftY)==(this->IntervalY_betweenHorizLine))
	{ 
	//	diff = abs(this->Temp_forSiftY)-this->IntervalY_betweenHorizLine;

		Temp_forSiftY=0; 
	
		if(ShiftY>0){ tempSh=(-1)*(this->IntervalY_betweenHorizLine-ShiftY);tempSh1=tempSh;}
		if(ShiftY<0){tempSh=(this->IntervalY_betweenHorizLine+ShiftY);tempSh1=tempSh;}

		// DataforText=ValueforText;
	}

	for (d=d_beg;d<=d_end-2;d++)
	{

		this->NewDX10->verticesGridDx10[d].y+=tempSh;
	}
  
//	ValueShiftForText+=ShiftY;
	
				  
	//DrawTextScale(ValueShift,2154.223, colorFontScale);

}
//------------------------------------------------------------------------------------------------------------------------------------
//void TGrid::CreateBufferGrid(bool BackgroundScale_temp, bool ScaleDepth_temp, UINT NumbHorizLineGrid_temp, UINT NumbVerticLineGrid_temp)
void TGrid::CreateBufferGrid()
{   
	this->NewDX10->Size_Vertex_Buffer -=this->Size_BufferGrid; // remove from the buffer dx10 'buffer grid'
	this->NewDX10->SizeBuffGridDx10 -= this->Size_BufferGrid;	

	UINT SclaleSizeBuffer=2;//With 10 stock
	UINT Before_Size_BufferGrid=this->Size_BufferGrid;
	this->Size_BufferGrid = SclaleSizeBuffer * sizeof(Vertex)*(2*(this->NumberElementsGrid+2)); //2 - чтобы сформировать отрезки

	this->NewDX10->Size_Vertex_Buffer += this->Size_BufferGrid;//add size buffer for Dx10
	this->NewDX10->SizeBuffGridDx10 += this->Size_BufferGrid;
	this->NewDX10->BeginIndexForCurveBuff=this->NewDX10->SizeBuffGridDx10;

//	UINT64  AddrBeginGridBeforeRealloc =(UINT64)this->NewDX10->verticesGridDx10;
//	UINT64  AddrEndGridBeforeRealloc = AddrBeginGridBeforeRealloc + Before_Size_BufferGrid;

	this->NewDX10->verticesGridDx10 = (Vertex*)realloc(this->NewDX10->verticesGridDx10, this->NewDX10->SizeBuffGridDx10);
	this->NewDX10->CountCreateBuffGrid++;

//	UINT64 AddrBeginGridAfterRealloc = (UINT64)this->NewDX10->verticesGridDx10;
//	UINT64 AddrEndGridAfterRealloc = AddrBeginGridAfterRealloc + (UINT64)this->NewDX10->verticesGridDx10;

	//clear, if remove address buffer NewDX10->verticesGridDx10 after realloc!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	/*
	if(AddrBeginGridBeforeRealloc != AddrBeginGridAfterRealloc)
	{
		if((AddrEndGridAfterRealloc>AddrBeginGridBeforeRealloc)&&(AddrEndGridAfterRealloc<AddrEndGridBeforeRealloc))
			delete [AddrEndGridBeforeRealloc-AddrEndGridAfterRealloc] (byte*) (AddrEndGridBeforeRealloc);
		if((AddrBeginGridAfterRealloc<AddrBeginGridBeforeRealloc)&&(AddrEndGridAfterRealloc<AddrBeginGridBeforeRealloc))
			delete [Before_Size_BufferGrid] (byte*) AddrBeginGridBeforeRealloc;
		if((AddrBeginGridAfterRealloc>AddrEndGridBeforeRealloc)&&(AddrEndGridAfterRealloc>AddrEndGridBeforeRealloc))
			delete [Before_Size_BufferGrid] (byte*) AddrBeginGridBeforeRealloc;
		if(AddrBeginGridAfterRealloc>AddrBeginGridBeforeRealloc&&AddrBeginGridAfterRealloc<AddrEndGridBeforeRealloc&&AddrEndGridAfterRealloc>AddrEndGridBeforeRealloc)
			delete [AddrBeginGridAfterRealloc-AddrBeginGridBeforeRealloc] (byte*) AddrBeginGridBeforeRealloc;
	}*/
}
//----------------------------------------------------- 
void TGrid::GridBufferFilling()
	
{
	int countVertLines;
	UINT countPoints=0;
	UINT numbeginpoint = NewDX10->NumberElementsGridDx10*2 - this->NumberElementsGrid*2;
	if(this->BackgroundScale)
	{
		//first point for BackgroundScale
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].size = this->BackgroundWidth;    //!!!!!!!size use for width
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].x = NewDX10->X_leftMax_P+this->BackgroundWidth/2;
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].y = NewDX10->Y_topMax_P;
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].Color=this->ColorBackgroundScale;
		countPoints++;
		//second point for BackgroundScale
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].size = this->BackgroundWidth;    //!!!!!!!size use for width
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].x = NewDX10->X_leftMax_P+this->BackgroundWidth/2;
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].y = NewDX10->Y_bottomMax_P-1;
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].Color=this->ColorBackgroundScale;
		countPoints++;
	}
	if(this->ScaleDepth)
	{
		//first point for ScaleDepth
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].size = this->WidthScaleDepth;    //!!!!!!!size use for width
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].x = NewDX10->X_leftMax_P+this->BackgroundWidth;
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].y = NewDX10->Y_topMax_P;
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].Color=this->ColorScaleDepth;
		countPoints++;
		//second point for ScaleDepth
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].size = this->WidthScaleDepth;    //!!!!!!!size use for width
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].x = NewDX10->X_leftMax_P+this->BackgroundWidth;
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].y = NewDX10->Y_bottomMax_P-1;
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].Color=this->ColorScaleDepth;
		countPoints++;
	}
	//-------------------------

	if(this->NumbHorizLineGrid>0)
	{
		int tempY=0; int countHorLines=0; 
		//IntervalY_betweenHorizLine = (NewDX10->Y_topMax_P+abs(NewDX10->Y_bottomMax_P))/this->NumbHorizLineGrid;
mGorLines:
		tempY = countHorLines *IntervalY_betweenHorizLine;
	      //	tempY = tempY-1; //for stable visualization line grid (if isn't then no Horis. line - for number 8, 16, 24 ?????????????????????????????
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].size = this->widt_line_curent;    //!!!!!!!size use for width
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].x = NewDX10->X_leftMax_P;
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].y = NewDX10->Y_topMax_P-tempY;
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].Color = this->ColorGridLine;
		countPoints++;

		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].size = this->widt_line_curent;    //!!!!!!!size use for width
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].x = NewDX10->X_rightMax_P;
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].y = NewDX10->Y_topMax_P-tempY;
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].Color = this->ColorGridLine;
		countPoints++;

		countHorLines++;
		if(countHorLines<=this->NumbHorizLineGrid) { goto mGorLines;}
		countHorLines=countHorLines;
	}
	//---------------------------------------------------

	if(this->NumbVerticLineGrid>0)
	{
		int tempX=0; countVertLines=0;
		//IntervalY_betweenVertLine = (abs(NewDX10->X_leftMax_P)+NewDX10->X_rightMax_P)/this->NumbVerticLineGrid;
mVertLines:
		tempX = countVertLines * IntervalY_betweenVertLine;
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].size = this->widt_line_curent;    //!!!!!!!size use for width
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].x = NewDX10->X_leftMax_P+tempX;
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].y = NewDX10->Y_topMax_P;
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].Color= this->ColorGridLine;
		countPoints++;

		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].size = this->widt_line_curent;    //!!!!!!!size use for width
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].x = NewDX10->X_leftMax_P+tempX;
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].y = NewDX10->Y_bottomMax_P;
		this->NewDX10->verticesGridDx10[numbeginpoint+countPoints].Color = this->ColorGridLine;
		countPoints++;

		countVertLines++;
		if(countVertLines<=this->NumbVerticLineGrid) { goto mVertLines;}
	}
	int i;
	i=0;
}
//--------------------------------------------------------------------------------------------------------------------------------------

//-----------property line width
void  TGrid::InWidth_line(UINT Data)
{
 this->widt_line_curent=Data;
}


void  TGrid::InNumbHorizLine(UINT Data1)
{
 this->NumbHorizLineGrid=Data1;
 this->NewDX10->NumberElementsGridDx10 -= this->NumberElementsGrid;
 this->NumberElementsGrid_P=NumberElementsGrid_Set; 
 this->NewDX10->NumberElementsGridDx10 += this->NumberElementsGrid;

IntervalY_betweenHorizLine = (NewDX10->Y_topMax_P+abs(NewDX10->Y_bottomMax_P))/this->NumbHorizLineGrid;
d_beg =(this->BackgroundScale+this->ScaleDepth)*2;
d_end =(this->BackgroundScale+this->ScaleDepth+this->NumbHorizLineGrid+2)*2;

   if(this->Size_BufferGrid <(sizeof(Vertex)*(2*this->NumberElementsGrid_P)))
   {
	 this->CreateBufferGrid();
	 this->NewDX10->CreateVertexBuffer(); 
   }

 this->GridBufferFilling();
    if(SUCCEEDED(this->NewDX10->ResultCreateBuff))
    {
     this->NewDX10->CopyBuffersDraw(1);
    } 

    
}
//---------------property num vertic. line-------------

void  TGrid::InNumbVerticLine(UINT Data2)
{
this->NumbVerticLineGrid=Data2;
this->NewDX10->NumberElementsGridDx10 -= this->NumberElementsGrid;
this->NumberElementsGrid_P=NumberElementsGrid_Set; 
this->NewDX10->NumberElementsGridDx10 += this->NumberElementsGrid;

IntervalY_betweenVertLine = (abs(NewDX10->X_leftMax_P)+NewDX10->X_rightMax_P)/this->NumbVerticLineGrid;

if(this->Size_BufferGrid <(sizeof(Vertex)*(2*this->NumberElementsGrid_P)))
{
	this->CreateBufferGrid();
	this->NewDX10->CreateVertexBuffer(); 
}

this->GridBufferFilling();
if(SUCCEEDED(this->NewDX10->ResultCreateBuff))
{
	this->NewDX10->CopyBuffersDraw(1);
} 
}
//---------------property color grid-------------
void TGrid::InWidthScaleDepth(int scaleDept_temp)
{
this->WidthScaleDepth=scaleDept_temp;
GridBufferFilling();
if(SUCCEEDED(this->NewDX10->ResultCreateBuff))
{
this->NewDX10->CopyBuffersDraw(1);
} 
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
GridBufferFilling();
if(SUCCEEDED(this->NewDX10->ResultCreateBuff))
{
	this->NewDX10->CopyBuffersDraw(1);
} 
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
//=============================LINE===================LINE===================================LINE=====================LINE=========================
 TCurve::TCurve(TDx10_My* CNewDX10temp)
 {
  this->CNewDX10=CNewDX10temp;

  this->CNewDX10->NumberCurves++;
  this->NumberObjCurve=this->CNewDX10->NumberCurves;

  this->NumberPoints=0;//for test
  this->ColorCurve = 0x0000FF;//default;//default
  this->CurveCountPoints=0;
  this->beg_index_for_CopyBuffer=0;
  HeightDraw1_2=this->CNewDX10->Height_Target_Window_P/2;
  HeightDraw = HeightDraw1_2*4;

  this->BufferCurveVertex=NULL;
  this->Numb_for_ShiftCurve=0; //default//!!!!!!!size use for shift curve - number curve
  this->WidthCurve=2; //default

  this->CNewDX10->dx_shiftDx10->SetFloat(0);
  this->CNewDX10->dx_widthDx10->SetFloat(this->WidthCurve);
  // для толщины использовать для одиночных кривых, а для мультикр. - для смещения

  this->BeginIndexCurve = this->CNewDX10->DxCurveCountPoints;
  
//default for TCurve - size is Height window
  UINT ScaleSizeBufferL=10;//With 10 stock for generate segments plus reserve
  this->Size_BufferCurve = ScaleSizeBufferL+ sizeof(Vertex)*2*this->CNewDX10->Height_Target_Window_P;

  this->verticesCurve = NULL;
  this->verticesCurve = (Vertex*)realloc(this->verticesCurve, this->Size_BufferCurve);

  this->CNewDX10->Size_Vertex_Buffer += this->Size_BufferCurve;
  
  countttt++;
  this->CNewDX10->SizeBuffCurveDx10 += this->Size_BufferCurve;
 // this->CNewDX10->verticesCurveDx10 = (Vertex*)realloc(this->CNewDX10->verticesCurveDx10, this->CNewDX10->SizeBuffCurveDx10);

  CNewDX10->CreateVertexBuffer(); 
  
 }
 //----------------------------------------------------------

void TCurve::InWidthCurve(int width_temp)
{
this->WidthCurve=width_temp;
this->CNewDX10->dx_shiftDx10->SetFloat(0);
this->CNewDX10->dx_widthDx10->SetFloat(this->WidthCurve);
}
 //--------------------------------------------------------------
void TCurve::CreateBufferCurve(int Size_BufferCurve_temp)
{

}
//----------------------------------------
void TCurve::putXY(float Xtemp, float Ytemp)
{
	this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].size = this->Numb_for_ShiftCurve;    //!!!!!!!size use for shift curve - number curve
	this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].x = Xtemp;
	this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].y = Ytemp;
	this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].Color= this->ColorCurve;
	this->CurveCountPoints++; 
	this->CNewDX10->DxCurveCountPoints++;
	if(this->CurveCountPoints>1)
	{
	this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].size = this->Numb_for_ShiftCurve;    //!!!!!!!size use for shift curve - number curve
	this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].x = Xtemp;
	this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].y = Ytemp;
	this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].Color= this->ColorCurve;
	this->CurveCountPoints++; 
	this->CNewDX10->DxCurveCountPoints++;
	}
	//this->CNewDX10->DxCurveCountPoints=this->CurveCountPoints;
	this->FlagArrayPutXY_BeginIndex=false;
}
//---------------------------------------------
//----------------------------------------
void TCurve::putXY_1(float Xtemp, float Ytemp)
{
	this->Numb_for_ShiftCurve=this->WidthCurve;
	this->verticesCurve[this->CurveCountPoints].size = this->Numb_for_ShiftCurve;    //!!!!!!!size use for shift curve - number curve
	this->verticesCurve[this->CurveCountPoints].x = Xtemp;
	this->verticesCurve[this->CurveCountPoints].y = Ytemp;
	this->verticesCurve[this->CurveCountPoints].Color= this->ColorCurve;
	this->CurveCountPoints++; 
	this->CNewDX10->DxCurveCountPoints++;
	if(this->CurveCountPoints>1)
	{
		this->verticesCurve[this->CurveCountPoints].size = this->Numb_for_ShiftCurve;    //!!!!!!!size use for shift curve - number curve
		this->verticesCurve[this->CurveCountPoints].x = Xtemp;
		this->verticesCurve[this->CurveCountPoints].y = Ytemp;
		this->verticesCurve[this->CurveCountPoints].Color= this->ColorCurve;
		this->CurveCountPoints++; 
		this->CNewDX10->DxCurveCountPoints++;
	}
	//this->CNewDX10->DxCurveCountPoints=this->CurveCountPoints;
	this->FlagArrayPutXY_BeginIndex=false;
}
//---------------------------------------------

//---------------------------------------------
//--------------------------------------------- use after ArrayPutXY!!!!!!!!!!!!!!!!!!!!
void TCurve::ArrayPutXY_BeginIndex(UINT BeginIndex,float *ArrayXtemp, float *ArrayYtemp, UINT SizeArray)
{
	UINT TempIndex=1+BeginIndex*2;
	if(BeginIndex==0) TempIndex=0;
	int d;
	//#pragma omp parallel  // not valid????????? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	for (d=0;d<SizeArray;d++)
	{
		this->CNewDX10->verticesCurveDx10[TempIndex].size = this->Numb_for_ShiftCurve;    //!!!!!!!size use for shift curve - number curve
		this->CNewDX10->verticesCurveDx10[TempIndex].x = ArrayXtemp[d];
		this->CNewDX10->verticesCurveDx10[TempIndex].y = ArrayYtemp[d];
		this->CNewDX10->verticesCurveDx10[TempIndex].Color= this->ColorCurve;
		TempIndex++; 
		if(TempIndex>1)
		{
			this->CNewDX10->verticesCurveDx10[TempIndex].size = this->Numb_for_ShiftCurve;    //!!!!!!!size use for shift curve - number curve
			this->CNewDX10->verticesCurveDx10[TempIndex].x = ArrayXtemp[d];
			this->CNewDX10->verticesCurveDx10[TempIndex].y = ArrayYtemp[d];
			this->CNewDX10->verticesCurveDx10[TempIndex].Color= this->ColorCurve;
			TempIndex++; 
	   }
    }
	this->FlagArrayPutXY_BeginIndex=true;
}
//------------------------------------------
//--------------------------------------------- use after ArrayPutXY!!!!!!!!!!!!!!!!!!!!
void TCurve::ArrayPutXY_BeginIndex_1(UINT BeginIndex,float *ArrayXtemp, float *ArrayYtemp, UINT SizeArray)
{
	this->Numb_for_ShiftCurve=this->WidthCurve;
	UINT TempIndex=1+BeginIndex*2;
	if(BeginIndex==0) TempIndex=0;
	int d;
	//#pragma omp parallel  // not valid????????? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	for (d=0;d<SizeArray;d++)
	{
		this->verticesCurve[TempIndex].size = this->Numb_for_ShiftCurve;    //!!!!!!!size use for shift curve - number curve
		this->verticesCurve[TempIndex].x = ArrayXtemp[d];
		this->verticesCurve[TempIndex].y = ArrayYtemp[d];
		this->verticesCurve[TempIndex].Color= this->ColorCurve;
		TempIndex++; 
		if(TempIndex>1)
		{
			this->verticesCurve[TempIndex].size = this->Numb_for_ShiftCurve;    //!!!!!!!size use for shift curve - number curve
			this->verticesCurve[TempIndex].x = ArrayXtemp[d];
			this->verticesCurve[TempIndex].y = ArrayYtemp[d];
			this->verticesCurve[TempIndex].Color= this->ColorCurve;
			TempIndex++; 
		}
	}
	this->FlagArrayPutXY_BeginIndex=true;
}
//------------------------------------------
//----------------------------------------------
void TCurve::ArrayPutXY(float *ArrayXtemp,float*ArrayYtemp, UINT SizeArray)
{
int d;
//#pragma omp parallel  // not valid????????? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
for (d=0;d<SizeArray;d++)
                       {
	                    this->putXY(ArrayXtemp[d],ArrayYtemp[d]);
                        }
this->FlagArrayPutXY_BeginIndex=false;
}
//------------------------------------------
//----------------1-------------------------- //----------------------------------------------

void TCurve::ArrCurveY_Shift(int ShiftY)
{
	int d;
	for (d=0;d<this->CurveCountPoints;d++)
	{
	  this->CNewDX10->verticesCurveDx10[d].y+=ShiftY;
	 
	}
}
//----------------1-------------------------- //----------------------------------------------

void TCurve::ArrCurveY_Shift_1(int ShiftY)
{
	int d;
	for (d=0;d<this->CurveCountPoints;d++)
	{
	  this->verticesCurve[d].y+=ShiftY;
	}
}
//------------------------------------------
//Direct=1 - free begin array, Direct=0 - free end array
//------------------------------------------ 
UINT TCurve::MoveArrayXY(int Direct, int HowMany)
{
UINT BegIndexforReturn=0;
//	HowMany-=1;
int TempSizeForMove=this->CurveCountPoints-HowMany*2;
if(Direct==0)         {//free end array
memmove_s(&this->CNewDX10->verticesCurveDx10[0], TempSizeForMove * sizeof(Vertex),&this->CNewDX10->verticesCurveDx10[HowMany*2], TempSizeForMove * sizeof(Vertex));
//this->CurveCountPoints-=HowMany*2;
BegIndexforReturn=(this->CurveCountPoints)/2-HowMany;  // /2 - т.к отрезки, точек меньше в 2 раза
                      }
if(Direct==1)  {//free begin array
memmove_s(&this->CNewDX10->verticesCurveDx10[HowMany*2], TempSizeForMove * sizeof(Vertex),&this->CNewDX10->verticesCurveDx10[0], TempSizeForMove * sizeof(Vertex));
this->CNewDX10->verticesCurveDx10[HowMany*2-1]=this->CNewDX10->verticesCurveDx10[0];
BegIndexforReturn=0;
               }
this->FlagArrayPutXY_BeginIndex=false;
return BegIndexforReturn;
}
// ----------------------------------------------------
//------------------------------------------
//Direct=1 - free begin array, Direct=0 - free end array
//------------------------------------------ 
UINT TCurve::MoveArrayXY_1(int Direct, int HowMany)
{
	UINT BegIndexforReturn=0;
	//	HowMany-=1;
	int TempSizeForMove=this->CurveCountPoints-HowMany*2;
	if(Direct==0)         {//free end array
		memmove_s(&this->verticesCurve[0], TempSizeForMove * sizeof(Vertex),&this->verticesCurve[HowMany*2], TempSizeForMove * sizeof(Vertex));
		//this->CurveCountPoints-=HowMany*2;
		BegIndexforReturn=(this->CurveCountPoints)/2-HowMany;  // /2 - т.к отрезки, точек меньше в 2 раза
	}
	if(Direct==1)  {//free begin array
		memmove_s(&this->verticesCurve[HowMany*2], TempSizeForMove * sizeof(Vertex),&this->verticesCurve[0], TempSizeForMove * sizeof(Vertex));
		this->verticesCurve[HowMany*2-1]=this->verticesCurve[0];
		BegIndexforReturn=0;
	}
	this->FlagArrayPutXY_BeginIndex=false;
	return BegIndexforReturn;
}
// ----------------------------------------------------
//------------------------------------------
//Direct=1 - free begin array, Direct=0 - free end array, Number - number curve
//------------------------------------------ 
UINT TCurve::MoveArrayXY_Nimber(int Direct, int HowMany, int Number)
{
	UINT BegIndexforReturn=0;
	//	HowMany-=1;
	int TempSizeForMove=this->CurveCountPoints-HowMany*2;
	int BeginIndexForMove=Number*(this->CurveCountPoints-3);
	if(Direct==0)         {//free end array
		memmove_s(&this->CNewDX10->verticesCurveDx10[BeginIndexForMove], TempSizeForMove * sizeof(Vertex),&this->CNewDX10->verticesCurveDx10[HowMany*2], TempSizeForMove * sizeof(Vertex));
		//this->CurveCountPoints-=HowMany*2;
		BegIndexforReturn=BeginIndexForMove/2+(this->CurveCountPoints)/2-HowMany;
	}
	if(Direct==1)  {//free begin array
		memmove_s(&this->CNewDX10->verticesCurveDx10[HowMany*2], TempSizeForMove * sizeof(Vertex),&this->CNewDX10->verticesCurveDx10[BeginIndexForMove], TempSizeForMove * sizeof(Vertex));
		this->CNewDX10->verticesCurveDx10[HowMany*2-1]=this->CNewDX10->verticesCurveDx10[0];
		BegIndexforReturn=BeginIndexForMove/2;
	}
	this->FlagArrayPutXY_BeginIndex=false;
	return BegIndexforReturn;
}
// ----------------------------------------------------
bool TCurve::ArrayDrawXY()
{
 bool ResultDraw;
 if(!this->FlagArrayPutXY_BeginIndex){
 this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].size = this->Numb_for_ShiftCurve;    //!!!!!!!size use for shift curve - number curve
 this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].x = this->CNewDX10->verticesCurveDx10[this->CurveCountPoints-1].x;
 this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].y = this->CNewDX10->verticesCurveDx10[this->CurveCountPoints-1].y;
 this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].Color= this->ColorCurve;
 } 
 if(this->CurveCountPoints%2>0) 
 {
	 ResultDraw=true;
	 //this->CNewDX10->CopyBuffersCurve((this->CurveCountPoints-1) * sizeof(Vertex));
	 this->drawTemp();//if an even number then draw - due to the fact that it is necessary to form segments
 }
 else ResultDraw=false;
 this->FlagArrayPutXY_BeginIndex=false;
 return ResultDraw;
}
//-----------------------------------------------
// ----------------------------------------------------
bool TCurve::ArrayDrawXY_1()
{
	this->Numb_for_ShiftCurve=this->WidthCurve;
	bool ResultDraw;
	if(!this->FlagArrayPutXY_BeginIndex){
		this->verticesCurve[this->CurveCountPoints].size = this->Numb_for_ShiftCurve;    //!!!!!!!size use for shift curve - number curve
		this->verticesCurve[this->CurveCountPoints].x = this->verticesCurve[this->CurveCountPoints-1].x;
		this->verticesCurve[this->CurveCountPoints].y = this->verticesCurve[this->CurveCountPoints-1].y;
		this->verticesCurve[this->CurveCountPoints].Color= this->ColorCurve;
	} 
	if(this->CurveCountPoints%2>0) 
	{
		ResultDraw=true;
		//this->CNewDX10->CopyBuffersCurve((this->CurveCountPoints-1) * sizeof(Vertex));//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		this->drawTemp_1();//if an even number then draw - due to the fact that it is necessary to form segments!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	  
	}
	else ResultDraw=false;
	this->FlagArrayPutXY_BeginIndex=false;
	return ResultDraw;
}
//-----------------------------------------------
bool TCurve::DrawXY(float Xtemp, float Ytemp)
{
   bool ResultDraw;
   this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].size = this->Numb_for_ShiftCurve;    //!!!!!!!size use for shift curve - number curve
   this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].x = Xtemp;
   this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].y = Ytemp;
   this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].Color= this->ColorCurve;
// this->CNewDX10->DxCurveCountPoints=this->CurveCountPoints;
 
if(this->CurveCountPoints%2>0) 
                            {
							ResultDraw=true;
							this->CNewDX10->CopyBuffersCurve((this->CurveCountPoints-1) * sizeof(Vertex));
							this->drawTemp();//if an even number then draw - due to the fact that it is necessary to form segments
                            }
                             else ResultDraw=false;

  	this->CurveCountPoints++; 
	this->CNewDX10->DxCurveCountPoints++;
	if(this->CurveCountPoints>1)
	{
	this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].size = this->Numb_for_ShiftCurve;    //!!!!!!!size use for shift curve - number curve
	this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].x = Xtemp;
	this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].y = Ytemp;
	this->CNewDX10->verticesCurveDx10[this->CurveCountPoints].Color= this->ColorCurve;
	this->CurveCountPoints++; 
	this->CNewDX10->DxCurveCountPoints++;
	}
//	this->CNewDX10->DxCurveCountPoints=this->CurveCountPoints;
//this->CurveCountPoints++;  //When adding a new point in the curve
	this->FlagArrayPutXY_BeginIndex=false;
return ResultDraw;
}
//---------------------------------------
void TCurve::drawTemp()
{
	if(SUCCEEDED(this->CNewDX10->ResultCreateBuff))
	{
		if(this->CNewDX10->CountCreateBuffGrid>1)
		                                    { 
 											this->CNewDX10->CountCreateBuffGrid=1;
		                                    }	
		this->CNewDX10->CopyBuffersDraw(1);

	} 
}
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------
void TCurve::drawTemp_1()
{
	if(SUCCEEDED(this->CNewDX10->ResultCreateBuff))
	{
		if(this->CNewDX10->CountCreateBuffGrid>1)
		{ 
			this->CNewDX10->CountCreateBuffGrid=1;
		}	
		this->CopyBuffersCurveToDx10();
	} 
}
//---------------------------------------------------------------------------------------------------------------------------
//NumCurve - номер кривой
//--------------------------------------------------------------------------------
void TCurve::CopyBuffersCurveToDx10()
{
int sizebuffcurve_temp =(this->CurveCountPoints-1) * sizeof(Vertex);
int beg_index;
//beg_index = this->NumberObjCurve*(this->CNewDX10->SizeBuffGridDx10)/sizeof(Vertex);
if(this->NumberObjCurve==1)beg_index = (this->CNewDX10->SizeBuffGridDx10)/sizeof(Vertex);
if(this->CurveCountPoints==1)beg_index = (this->CNewDX10->SizeBuffGridDx10)/sizeof(Vertex);

//if(this->NumberObjCurve>1)beg_index =  (this->CNewDX10->DxCurveCountPoints-(this->NumberObjCurve-1)*this->CurveCountPoints)+(this->CNewDX10->SizeBuffGridDx10)/sizeof(Vertex);
//if(this->NumberObjCurve>1)beg_index = this->CNewDX10->DxCurveCountPoints-(this->NumberObjCurve-1)*(this->CurveCountPoints+1)+(this->CNewDX10->SizeBuffGridDx10)/sizeof(Vertex);

if(this->NumberObjCurve!=1)
             {
if(this->CurveCountPoints==2)this->beg_index_for_CopyBuffer = ((this->NumberObjCurve*2)-2);
if(this->CurveCountPoints>2&&(this->CurveCountPoints+1)<(this->HeightDraw))this->beg_index_for_CopyBuffer += ((this->NumberObjCurve*2)-2);
if(this->CurveCountPoints>1)beg_index = this->beg_index_for_CopyBuffer+(this->CNewDX10->SizeBuffGridDx10)/sizeof(Vertex);
              }
if(beg_index>1120)
{
	beg_index=beg_index;
}

//this->CNewDX10->VertexBufferDx10->Unmap();
memmove_s(&this->CNewDX10->verticesDx10[beg_index], sizebuffcurve_temp,&this->verticesCurve[0], sizebuffcurve_temp);
//this->CNewDX10->VertexBufferDx10->Map(D3D10_MAP_WRITE_DISCARD, NULL, (void**)&this->CNewDX10->verticesDx10);
// this->CNewDX10->BeginIndexForCurveBuff+=this->CurveCountPoints-1;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}


//--------------------------------------------------------------------------------
//-----------------------------Manager Draw------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
TManagerDraw::TManagerDraw(TDx10_My* Dx10_in , TGrid* Grid_in, TCurve* Curve_in, float Y_Begin)
{
	DX10Draw=Dx10_in;
	GridDraw=Grid_in;
	CurveDraw=Curve_in;

	HeightDraw1_2=Dx10_in->Height_Target_Window_P/2;
	HeightDraw = HeightDraw1_2+HeightDraw1_2;
	count_for_IntervalY_betwHorizLine=0;
	Flag_text=false;
	y_in=Y_Begin;
}
//----------------------------------
void TManagerDraw::DrawIncY(float x_in, int incr_in, int count_in, float Data_Depth)
{
	int incr_abs = abs(incr_in);
	if(count_in>0)
	{
		if(((this->y_in<0&&this->y_in==(-1*this->HeightDraw1_2))||(this->y_in>0&&this->y_in==this->HeightDraw1_2)))
		{
			if((this->y_in<0&&this->y_in>(-1*HeightDraw))||(this->y_in>0&&this->y_in<HeightDraw))
			{
				GridDraw->ArrGridY_Shift(incr_in);
				CurveDraw->ArrCurveY_Shift(incr_in);
				//this->DX10Draw->ArrAllCurveY_Shift(incr_in);
				

				if(count_for_IntervalY_betwHorizLine==(GridDraw->IntervalY_betweenHorizLine_P-1)) 
				{
				   if(incr_in>0)memmove_s(&GridDraw->ArrForTextScale[0], GridDraw->SizeForTextArr,&GridDraw->ArrForTextScale[incr_abs], GridDraw->SizeForTextArr);
				   if(incr_in<0)memmove_s(&GridDraw->ArrForTextScale[incr_abs], GridDraw->SizeForTextArr,&GridDraw->ArrForTextScale[0], GridDraw->SizeForTextArr);
				}
				if(count_for_IntervalY_betwHorizLine==(GridDraw->IntervalY_betweenHorizLine_P)) 
				{
					if(incr_in>0) GridDraw->ArrForTextScale[GridDraw->NumbHorizLine_P]=Data_Depth;
					if(incr_in<0) GridDraw->ArrForTextScale[0]=Data_Depth;
					count_for_IntervalY_betwHorizLine=0;
				}		
			}
			if(count_in>=HeightDraw)
			{
				UINT BegIndex=CurveDraw->MoveArrayXY(0,incr_abs);
				//for(int i=0;i<this->DX10Draw->NumberCurves;i++){
			//	UINT BegIndex=CurveDraw->MoveArrayXY_Nimber(0,incr_abs,i);
				CurveDraw->ArrayPutXY_BeginIndex(BegIndex,&x_in,&this->y_in,incr_abs);
				                                     //  }
			}
			if(!this->Flag_text)
			{
				for(int i=0;i<=GridDraw->NumbHorizLine_P+2;i++)
				{     
					if(this->y_in==GridDraw->ArrForTextScale1[i])GridDraw->ArrForTextScale[i]=(float)Data_Depth;
					this->Flag_text=true;
				}          
			}
			count_for_IntervalY_betwHorizLine++;
		}
	}  /// if(count_in>0)

	//-------------------------
	if(!this->Flag_text&&(this->y_in>(-1*HeightDraw1_2))&&(this->y_in<HeightDraw1_2))
	{
		for(int i=0;i<=GridDraw->NumbHorizLine_P+2;i++)
		{     
			if(this->y_in==GridDraw->ArrForTextScale1[i])GridDraw->ArrForTextScale[i]=(float)Data_Depth;
		}          
	}

	if(count_in<this->HeightDraw)//чтобы не переполн. массив DX
	{
		if(incr_in<0&&this->y_in<this->HeightDraw1_2)this->y_in-=incr_in; 
		if(incr_in>0&&this->y_in>(-1*this->HeightDraw1_2))this->y_in-=incr_in;
		//y_t-=incr;
		this->CurveDraw->putXY(x_in,this->y_in);
	}

   this->EndDraw();
}
//----------------------------------------------------------

//----------------------------------------------------------
void TManagerDraw::EndDraw()
{
	this->CurveDraw->ArrayDrawXY();
	this->GridDraw->DrawTextGr(GridDraw->ArrForTextScale);
	this->DX10Draw->PresentDx10();
}
//-----------------------------------------------------------------
//-----------------------------Manager Draw Array Curves------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
TManagerDrawArrCrv::TManagerDrawArrCrv(TDx10_My* Dx10_in , TGrid* Grid_in, TCurve* Curve_in, int NumbArrCrv, float Y_Begin)
{
	DX10Draw=Dx10_in;
	GridDraw=Grid_in;
	CurveDraw=Curve_in;
	NumbArrCurve=NumbArrCrv;

	HeightDraw1_2=Dx10_in->Height_Target_Window_P/2;
	HeightDraw = HeightDraw1_2+HeightDraw1_2;
	count_for_IntervalY_betwHorizLine=0;
	Flag_text=false;
	y_in=Y_Begin;
	//BegIndexCurv = new int [DX10Draw->NumberCurves];
}
//----------------------------------
//x_in -  array data for add to curves (x_in[0]->Curve[0]...x_in[n]->Curve[n]
void TManagerDrawArrCrv::DrawArrCrvIncY(float *x_in, int incr_in, int count_in, float Data_Depth)
{
	int incr_abs = abs(incr_in);
	if(count_in>0)
	{
		if(((this->y_in<0&&this->y_in==(-1*this->HeightDraw1_2))||(this->y_in>0&&this->y_in==this->HeightDraw1_2)))
		{
			if((this->y_in<0&&this->y_in>(-1*HeightDraw))||(this->y_in>0&&this->y_in<HeightDraw))
			{
				GridDraw->ArrGridY_Shift(incr_in);
				
				for(int i=0;i<this->DX10Draw->NumberCurves;i++)CurveDraw[i].ArrCurveY_Shift_1(incr_in);

				if(count_for_IntervalY_betwHorizLine==(GridDraw->IntervalY_betweenHorizLine_P-1)) 
				{
					if(incr_in>0)memmove_s(&GridDraw->ArrForTextScale[0], GridDraw->SizeForTextArr,&GridDraw->ArrForTextScale[incr_abs], GridDraw->SizeForTextArr);
					if(incr_in<0)memmove_s(&GridDraw->ArrForTextScale[incr_abs], GridDraw->SizeForTextArr,&GridDraw->ArrForTextScale[0], GridDraw->SizeForTextArr);
				}

				if(count_for_IntervalY_betwHorizLine==(GridDraw->IntervalY_betweenHorizLine_P)) 
				{
					if(incr_in>0) GridDraw->ArrForTextScale[GridDraw->NumbHorizLine_P]=Data_Depth;
					if(incr_in<0) GridDraw->ArrForTextScale[0]=Data_Depth;
					count_for_IntervalY_betwHorizLine=0;
				}		
			}

			if(count_in>=HeightDraw)
			{
				for(int i=0;i<this->DX10Draw->NumberCurves;i++){
				int TempBegIndexCurv = CurveDraw[i].MoveArrayXY_1(0,incr_abs);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				CurveDraw[i].ArrayPutXY_BeginIndex_1(TempBegIndexCurv,&x_in[i],&this->y_in,incr_abs);
				  }
			}

			if(!this->Flag_text)
			{
				for(int i=0;i<=GridDraw->NumbHorizLine_P+2;i++)
				{     
					if(this->y_in==GridDraw->ArrForTextScale1[i])GridDraw->ArrForTextScale[i]=(float)Data_Depth;
					this->Flag_text=true;
				}          
			}
			count_for_IntervalY_betwHorizLine++;
		}

	}  // if(count_in>0)

	//-------------------------
	if(!this->Flag_text&&(this->y_in>(-1*HeightDraw1_2))&&(this->y_in<HeightDraw1_2))
	{
		for(int i=0;i<=GridDraw->NumbHorizLine_P+2;i++)
		{     
			if(this->y_in==GridDraw->ArrForTextScale1[i])GridDraw->ArrForTextScale[i]=(float)Data_Depth;
		}          
	}

	if(count_in<this->HeightDraw)//чтобы не переполн. массив DX
	{
		if(incr_in<0&&this->y_in<this->HeightDraw1_2)this->y_in-=incr_in; 
		if(incr_in>0&&this->y_in>(-1*this->HeightDraw1_2))this->y_in-=incr_in;
	
		for(int i=0;i<this->DX10Draw->NumberCurves;i++)
			{
              this->CurveDraw[i].putXY_1(x_in[i],this->y_in);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		    //  this->CurveDraw[i].ArrayDrawXY_1();
		    }
	}
	this->EndDrawArrCrv();
}
//----------------------------------------------------------
//----------------------------------------------------------
void TManagerDrawArrCrv::EndDrawArrCrv()
{
	for(int i=0;i<this->DX10Draw->NumberCurves;i++) this->CurveDraw[i].ArrayDrawXY_1();//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	this->DX10Draw->CopyBuffersDraw_1(1);
	this->GridDraw->DrawTextGr(GridDraw->ArrForTextScale);
	this->DX10Draw->PresentDx10();
}