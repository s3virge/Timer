//
//	CSkinBaseButton (SkinBaseButton.h)
//
//		версия	1.0
//		автор	Денис Солоненков
//		mail	2Los@land.RU
//

// SkinBaseButton.cpp : implementation file
//
#include "stdafx.h"
#include "SkinBaseButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinBaseButton

CSkinBaseButton::CSkinBaseButton()
{
	m_bHotState = FALSE;	
	m_bCheck = FALSE;
	m_bAutoCheckBox = FALSE;
	m_szButton = CSize(-1, -1);
}

CSkinBaseButton::~CSkinBaseButton()
{
}


BEGIN_MESSAGE_MAP(CSkinBaseButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinBaseButton message handlers

BOOL CSkinBaseButton::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= BS_OWNERDRAW;

	return CButton::PreCreateWindow(cs);
}

void CSkinBaseButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{		
	CDC dc;
	dc.Attach(lpDIS->hDC);
	CRect rc = lpDIS->rcItem;
	//GetWindowRect(&rc);

	switch(lpDIS->itemAction)
	{
	case ODA_SELECT: //This bit is set when only the selection status has changed. The itemState member should be checked to determine the new selection state. 
		{
			CPoint pt;
			GetCursorPos(&pt);
			//если курсор над окном и GetCheck = true, то рисуем кнопку нажатой
			if( HitTest(pt) && GetCheck() )
			{
				DrawButton(&dc, rc, SBB_PRESS);	//рисуем кнопку нажатой
			}
			else
			{
				DrawButton(&dc, rc, SBB_NORMAL);
			}
		}
		break;
	
	case ODA_DRAWENTIRE: //This bit is set when the entire control needs to be drawn
		{
			if( !IsWindowEnabled() )
				DrawButton(&dc, rc, SBB_DISABLE);				
			else if( m_bHotState && !GetCheck() )
				DrawButton(&dc, rc, SBB_HOT);				
			else if( GetCheck() )
			{
				DrawButton(&dc, rc, SBB_PRESS); //рисуем кнопку нажатой
			}
			else
			{				
				DrawButton(&dc, rc, SBB_NORMAL);
			}
		}
		break;
	
	case ODA_FOCUS:
		DrawButton(&dc, rc, SBB_FOCUS);		
		break;
	
	default:
		DrawButton(&dc, rc, SBB_NORMAL);			
		break;
	}

	dc.Detach();	
}

void CSkinBaseButton::OnMouseLeave()
{
	m_bHotState = FALSE;	
	Invalidate(FALSE);
}

void CSkinBaseButton::OnMouseMove(UINT nFlags, CPoint point) 
{		
	if(HitTest(point, TRUE))
	{	
		if(!m_bHotState && !(nFlags & MK_LBUTTON))
		{				
			m_bHotState = TRUE;		
			Invalidate(FALSE);

			TRACKMOUSEEVENT tm;
			tm.cbSize = sizeof(tm);
			tm.dwFlags = TME_LEAVE;
			tm.dwHoverTime = HOVER_DEFAULT;
			tm.hwndTrack = m_hWnd;

			TrackMouseEvent(&tm);
		}

		if(CButton::GetCheck())//()!=8 && GetState())
		{
			CString msg;
			msg.Format("Button state %d", GetState());
			MessageBox(msg);
		}
	}
	else if(m_bHotState)
	{
		m_bHotState = FALSE;	
		Invalidate(FALSE);
	}	
	CButton::OnMouseMove(nFlags, point);
}


BOOL CSkinBaseButton::OnEraseBkgnd(CDC* pDC) 
{	
	return TRUE;//CButton::OnEraseBkgnd(pDC);
}

void CSkinBaseButton::DrawButton(CDC* pDC, CRect rc, int iButton)
{
	ASSERT(m_dibButtonStates.m_lpImage!=NULL);

	//CSize sz=m_dibButtonStates
	ASSERT(m_szButton.cx!=-1 && m_szButton.cy!=-1);
	//ASSERT(m_dibButtonStates.GetDimensions().cy>=m_szButton.cy);
	//ASSERT(m_dibButtonStates.GetDimensions().cx>=(m_szButton.cx*5));
	m_dibButtonStates.Draw(pDC, rc.TopLeft(), rc.Size(), CPoint(m_szButton.cx*iButton, 0), CSize(m_szButton.cx, m_szButton.cy));
}

CSize CSkinBaseButton::CalcSize()
{
	ASSERT(m_dibButtonStates.m_lpImage!=NULL);
	m_szButton = m_dibButtonStates.GetDimensions();
	m_szButton.cx /= 5;
	
	return m_szButton;
}

void CSkinBaseButton::SizeToDib(BOOL bRepaint)
{
	CRect rc;
	GetWindowRect(&rc);
	GetParent()->ScreenToClient(&rc);

	if(m_szButton.cx < 0 || m_szButton.cy < 0)		
		CalcSize();

	rc.right = rc.left + m_szButton.cx;
	rc.bottom = rc.top + m_szButton.cy;
	
	MoveWindow(&rc, bRepaint);
}

void CSkinBaseButton::SetCheck(int nCheck)
{
	m_bCheck = nCheck ? TRUE : FALSE;
	//TRACE("m_bCheck = %d", m_bCheck );
}

BOOL CSkinBaseButton::GetCheck()
{
	return m_bCheck;
}

BOOL CSkinBaseButton::HitTest(CPoint point, BOOL bClientPoint)
{
	if(bClientPoint)		
		ClientToScreen(&point);

	CRect rc;
	GetWindowRect(&rc);		

	return rc.PtInRect(point);
}
//=== мышиные нажатия ===
void CSkinBaseButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCheck(!GetCheck());
	
	CButton::OnLButtonDown(nFlags, point);
}

void CSkinBaseButton::OnLButtonUp(UINT nFlags, CPoint point) 
{	
	SetCheck(!GetCheck());
	
	CButton::OnLButtonUp(nFlags, point);			
}