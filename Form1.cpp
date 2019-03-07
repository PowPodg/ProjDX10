#include "stdafx.h"
#include "Form1.h"
#include <iostream>
#include <algorithm>
#include <functional>
#include <string>
#include <vector>

HRESULT res;
HWND hh;
int a=0;

int i;
UINT R,G,B;

using namespace std;
int resultn;
bool Flag_DrawDX10 = false;
bool Flag_Button=false;
#define SIZEOF_ARRAY(x) ((sizeof(x))/(sizeof(x[0]))) 
TDx10_My* TDx10_MyU;
TGrid *GridU;
TCurve *CurveMy, *CurveMy1;
TManagerDraw *ManagerDraw, *ManagerDraw1;
TManagerDrawArrCrv *ManagerDrawArrCrv;
float x_t=0; float y_t=200;

float *x_n;

float temp_x_t=0;

float* ArrForTextScale;
float* ArrForTextScale1;
int CountForTextScale=0;
bool FlagArrForTextScale=false;

UINT BegInd;
int tempPosScrol;
int count_timer =0;int count_timer1=0;
bool FlagTimer1 =false; bool FlagTimer12 =false;
bool ShiftHorisGridEvent=false;
float Data_Depth_Curr=0.0;
float Begin_Data_Depth=100.0;
bool Flag_text=false;
int NumbObjCurve=300;


//------------------------------------------------------------------------------------------
void WinForm::Form1::button1_Click(System::Object^  sender, System::EventArgs^  e)
{  
	
	System::Drawing::Color MyColor; 
	
	x_n= new float[NumbObjCurve];

	TDx10_MyU=TDx10_MyU->Create_ObjectDx10((HWND)panel2->Handle.ToPointer(), panel2->Width, panel2->Height);
	//propertyGrid1->SelectedObject=My1;
	
	if(TDx10_MyU)
	{
     GridU = new  TGrid(TDx10_MyU);
	
	// CurveMy = new TCurve(TDx10_MyU);
	//  ManagerDraw = new TManagerDraw(TDx10_MyU,GridU,CurveMy,0);

	  CurveMy = new TCurve [NumbObjCurve];
	 // CurveMy = (TCurve*)operator new( sizeof( TCurve ) * NumbObjCurve  );
	 for(int i=0; i<NumbObjCurve; ++i ) 
		 {
         new (&CurveMy[i]) TCurve(TDx10_MyU); 
			//  &CurveMy[i]  TCurve(TDx10_MyU);
		 if(i==320)
		       {
			    i=i;
				}
	     }
	// CurveMy[0].WidthCurve_P=8;
	// CurveMy[1].ColorCurve_P=0;  CurveMy[1].WidthCurve_P=4;
	//  CurveMy[2].ColorCurve_P=255000;
	//  CurveMy[3].ColorCurve_P=0xff0000;
	//  CurveMy[3].WidthCurve_P=15;

	 ManagerDrawArrCrv = new TManagerDrawArrCrv(TDx10_MyU,GridU,CurveMy,NumbObjCurve,250);

	}
	else
		MessageBox::Show("Can't Created device DX10!", "ERROR", MessageBoxButtons::OK);//|MB_ICONERROR|MB_TASKMODAL);


}
//-------------------------------------------------------------------------------------------------------
void WinForm::Form1::button2_Click(System::Object^  sender, System::EventArgs^  e)
{   

	if(TDx10_MyU)
	{
	
		Flag_DrawDX10 = true;

		//CurveMy = new TCurve(TDx10_MyU);
	//	CurveMy->WidthCurve_P=5;
		
		timer1->Enabled=true; 
	//	 count_timer =0; count_timer1=0;
		vScrollBar1->Maximum=panel2->Height;
		vScrollBar1->Value=panel2->Height/2;

	}
}
//-----------------------------------------------------------------------
//-------------------------------------------------------------
void WinForm::Form1::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
{
	x_t=10+sin(count_timer/40.0)*10-600;
	float x_t1=10+cos(count_timer/40.0)*100;
	int n=0;
	for(int i=0; i<NumbObjCurve; ++i ){x_n[i]=x_t+n;n+=5;}
	//float x_t1=x_t+50;
	//x_n[0]=x_t; x_n[1]=x_t1; x_n[2]=x_t1+100; x_n[3]=x_t1/2.55;

	//ManagerDraw->DrawIncY(x_t,1,count_timer,Begin_Data_Depth+Data_Depth_Curr);
	ManagerDrawArrCrv->DrawArrCrvIncY(x_n,-1,count_timer,Begin_Data_Depth+Data_Depth_Curr);
	count_timer++;
	

	Data_Depth_Curr+=0.01;
    textBox1->Text=(Begin_Data_Depth+Data_Depth_Curr).ToString();	
}
void WinForm::Form1::panel2_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
{
	//if(Flag_DrawDX10)
//	{
		//swapChain->Present( 0, 0 );
	//}
//	if(Flag_DrawDX10)My1->TDx10_MyU->UpdateDx10();
}
//----------------------------------------------------------------
void WinForm::Form1::Form1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
{
//if(Flag_DrawDX10)My1->TDx10_MyU->UpdateDx10();
}
//--------------------------------------------------------
void WinForm::Form1::button3_Click(System::Object^  sender, System::EventArgs^  e)
{
timer1->Enabled=false;
}
void WinForm::Form1::button4_Click(System::Object^  sender, System::EventArgs^  e)
{
	timer1->Enabled=true;
}
//-------------------------------------------------------
void WinForm::Form1::vScrollBar1_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e)
{   
	int tempScr;
	//if(tempPosScrol>(vScrollBar1->Value - panel2->Height/2))
	//{
		tempScr = tempPosScrol -(vScrollBar1->Value - panel2->Height/2);
	//}
	//CurveMy->ArrCurveY_Shift(tempScr);
//	CurveMy->ArrayDrawXY();
	tempPosScrol = vScrollBar1->Value - panel2->Height/2;

}

