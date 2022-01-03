//////////////////////////////////////////////////////////////////////////
// Label.cpp : implementation file
//

#include "stdafx.h"
#include "LCDDots.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CLCDDots, CStatic)

/////////////////////////////////////////////////////////////////////////////
// CLCDDots

CLCDDots::CLCDDots()
{	
}

CLCDDots::~CLCDDots()
{
}


BEGIN_MESSAGE_MAP(CLCDDots, CStatic)
	//{{AFX_MSG_MAP(CLCDDots)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CLCDDots::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	RECT	rc;
	GetClientRect(&rc);
	
	dc.FillSolidRect(&rc, m_bkcolor);

	CStatic::OnPaint();
}

void CLCDDots::SetBkColor(COLORREF crBkgnd)
{
	m_bkcolor = crBkgnd;
}