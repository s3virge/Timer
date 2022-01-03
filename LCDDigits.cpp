// lcd7.cpp : implementation file
//

#include "stdafx.h"
//#include "bigclock.h"
#include "LCDDigits.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLCDDigits


CLCDDigits::CLCDDigits()
{
	// Set initial values
	gap		= 1; //щель
	width	= 0;
	num = 0;

	low		= 0;
	high	= 9;
	
	firstpaint = true;

	seg1= seg2= seg3= seg4= seg5= seg6= seg7 = false;

	LigntColor = RGB(0, 200, 0); //черный цвет
	NotLightColor = RGB(100,100,100); //серый
	bg = RGB(255, 0, 0);
}

CLCDDigits::~CLCDDigits()
{
}


BEGIN_MESSAGE_MAP(CLCDDigits, CWnd)
	//{{AFX_MSG_MAP(CLCDDigits)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLCDDigits message handlers

void CLCDDigits::OnPaint()
{
	RECT rc; 
	GetClientRect(&rc);

	//если рисуем впервые, изменяем стиль окна
	if( firstpaint )
	{
		//TRACE("firstpaint\r\n");
		firstpaint = false;

		if(width == 0)
			width = rc.right / 4;

		//уберем рамку вокруг статик конторола
		ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE, 0);
		
		if (!ModifyStyle(SS_SUNKEN, 0))
			TRACE("Облом с ModifyStyle\n");
	}

	CPaintDC dc(this); // device context for painting

	//зарисовываем фон нужным цветом
	dc.FillSolidRect(&rc, bg);
	
	// Start from all reset
	seg1 = seg2 = seg3 = seg4 = seg5 = seg6 = seg7 = false;

	// Coding of segments:
	//				  1
	//			 ---------
	//		4	|    3    | 6
	//			 ---------
	//		5	|         | 7
	//			 ---------
	//               2

	// Set the ones that need to be set:
	switch(num)
	{
	case 0: seg1 = seg2 = seg4 = seg5 = seg6 = seg7 = true;	break;
	case 1:	seg6= seg7 = true; break;
	case 2: seg1 = seg6 = seg3 = seg5 = seg2 = true; break;
	case 3: seg1 = seg6 = seg3 = seg7 = seg2 = true; break;
	case 4: seg4  = seg3 = seg6 = seg7 = true;	break;
	case 5: seg1  = seg4 = seg3 = seg7 = seg2 = true; break;
	case 6: seg1 = seg4  = seg5 = seg2 = seg7 = seg3 = true; break;
	case 7: seg1  = seg6  = seg7 = true; break;
	case 8: seg1 = seg2 = seg3 = seg4 = seg5 = seg6 = seg7 = true; break;
	case 9: seg1 = seg3 = seg4 = seg6 = seg7 = seg2 = true; break;
	//case 10: seg1 = seg4 = seg5 = seg3 = seg6 = seg7 = true; break;  // A
	//case 11: seg4 = seg5 = seg2 = seg3 = seg7 = true; break;  // b
	//case 12: seg3 = seg5 = seg2 = true; break;  // c
	//case 13: seg3 = seg5 = seg2 = seg7 = seg6 = true; break;  // d
	//case 14: seg1 = seg4 = seg3 = seg5 = seg2 = true; break;  // e
	//case 15: seg1 = seg4 = seg3 = seg5 = true; break;  // f
	}
	
	int vhalf = rc.bottom / 2;

	CBrush	fgBrush(LigntColor);
	CBrush	bgBrush(NotLightColor);
	CPen	nullpen (PS_NULL, 0, RGB(0,0,0)); 

	CBrush *old = (CBrush*) dc.SelectObject(&fgBrush);
	CPen   *oldp = (CPen*) dc.SelectObject(&nullpen);
	
	POINT poly[6];

	//================================================================
	///////////////////////////////////////////////////
	///    0---------------------------1
	//        3--------------------2
	//

	poly[0].x = 0;						poly[0].y = 0;
	poly[1].x = rc.right;				    poly[1].y = 0;
	poly[2].x = rc.right - (width);		poly[2].y = width;
	poly[3].x = width;					poly[3].y = width;

	if(seg1)
		dc.SelectObject(&fgBrush);
	else
		dc.SelectObject(&bgBrush);

	//Draws a polygon consisting of two or more points (vertices) connected by lines, using the current pen
	dc.Polygon(poly, 4);

	///////////////////////////////////////////////////
	//        0--------------------1
	///    3---------------------------2

	poly[0].x = (width );				poly[0].y = rc.bottom - (width );
	poly[1].x = rc.right - (width);		poly[1].y = rc.bottom - (width );
	poly[2].x = rc.right;				poly[2].y = rc.bottom;
	poly[3].x = rc.left;				poly[3].y = rc.bottom ;
	
	if(seg2)
		dc.SelectObject(&fgBrush);
	else
		dc.SelectObject(&bgBrush);

	dc.Polygon(poly, 4);

	///////////////////////////////////////////////////
	//        0--------------------1
	///    5---------------------------2
	///        4--------------------3
	// 
	poly[0].x = width ;					poly[0].y = vhalf - width/2 + gap;
	poly[1].x = rc.right - width;		poly[1].y = vhalf - width/2 + gap;
	poly[2].x = rc.right;				poly[2].y = vhalf;
	poly[3].x = rc.right - width;		poly[3].y = vhalf + width/2 + gap;
	poly[4].x = width;					poly[4].y = vhalf + width/2 + gap;
	poly[5].x = 0;						poly[5].y = vhalf + gap;

	if(seg3)
		dc.SelectObject(&fgBrush);
	else
		dc.SelectObject(&bgBrush);

	dc.Polygon(poly, 6);

	//////////////////////////////////////////////////
	//	 0
	//  |  3
	//  | |
	//  |  2
	//   1

	poly[0].x = 0;						poly[0].y = gap;
	poly[1].x = 0;						poly[1].y = vhalf;
	poly[2].x = width;					poly[2].y = vhalf - (width/2);
	poly[3].x = width;					poly[3].y = width + gap;

	if(seg4)
		dc.SelectObject(&fgBrush);
	else
		dc.SelectObject(&bgBrush);

	dc.Polygon(poly, 4);

	poly[0].y += vhalf + gap;
	poly[1].y += vhalf -  gap;
	poly[2].y += vhalf -  gap - width/2;
	poly[3].y += vhalf - width/2 + gap;

	if(seg5)
		dc.SelectObject(&fgBrush);
	else
		dc.SelectObject(&bgBrush);

	dc.Polygon(poly, 4);

	//////////////////////////////////////////////////
	//	        3
	//      0  |
	//       | |
	//      1  | 
	//          2

	poly[0].x = rc.right - (width);			poly[0].y = (width)  + gap;
	poly[1].x = rc.right - (width);			poly[1].y = vhalf - (width/2);
	poly[2].x = rc.right;					poly[2].y = vhalf - gap ;
	poly[3].x = rc.right;					poly[3].y = gap;

	if(seg6)
		dc.SelectObject(&fgBrush);
	else
		dc.SelectObject(&bgBrush);

	dc.Polygon(poly, 4);

	poly[0].y += vhalf - width/2 + gap;
	poly[1].y += vhalf - width/2 - gap;
	poly[2].y += vhalf; 
	poly[3].y += vhalf;

	if(seg7)
		dc.SelectObject(&fgBrush);
	else
		dc.SelectObject(&bgBrush);

	dc.Polygon(poly, 4);

	dc.SelectObject(old);
	dc.SelectObject(oldp);

	// Just for testing
	/*dc.SetBkMode(TRANSPARENT );
	CString str; str.Format("%d", num);
	rc.bottom = rc.bottom / 2;
	dc.DrawText(str, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);*/

	// Do not call CWnd::OnPaint() for painting messages
}

void CLCDDigits::SetMinMax(int minn, int maxx)
{
	low = minn;
	high = maxx;

	if(num < low) num = low;
	if(num > high) num = high;

	if(::IsWindow(m_hWnd))
		Invalidate();
}

void CLCDDigits::SetLigntColor(COLORREF col)
{
	LigntColor = col;

	if(::IsWindow(m_hWnd))
		Invalidate();
}

void CLCDDigits::SetNotLigntColor(COLORREF col)
{
	NotLightColor = col;

	if(::IsWindow(m_hWnd))
		Invalidate();
}

void CLCDDigits::SetBgColor(COLORREF col)
{
	bg = col;
}