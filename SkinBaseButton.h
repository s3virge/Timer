//
//	CSkinBaseButton (SkinBaseButton.h)
//
//		версия	1.0
//		автор	Денис Солоненков
//		mail	2Los@land.RU
//

#pragma once

#include "CDIB.h"

/////////////////////////////////////////////////////////////////////////////
// CSkinBaseButton window
#define ON_WM_MOUSELEAVE() \
	{ WM_MOUSELEAVE, 0, 0, 0, AfxSig_vv, \
		(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)())&OnMouseLeave },

#define SBB_NORMAL		0
#define SBB_PRESS		1
#define SBB_HOT			2
#define SBB_FOCUS		3
#define SBB_DISABLE		4

class CSkinBaseButton : public CButton
{
	BOOL m_bHotState;	
	BOOL m_bCheck;

public:
	// Construction
	CSkinBaseButton();
	// Attributes
	BOOL m_bAutoCheckBox;
	CSize	m_szButton;			//размер кнопки
	CDib	m_dibButtonStates;	//картинка с состояниями кнопки
								// 0 - normal
								// 1 - press
								// 2 - hot
								// 3 - focus
								// 4 - disable


	void DrawButton(CDC* pDC, CRect rc, int iButton);
	CSize CalcSize();
	void SizeToDib(BOOL bRepaint=TRUE);
	void SetCheck(int nCheck);
	BOOL GetCheck();
	//void SkinDraw(CDC* pDC);
	BOOL HitTest(CPoint point, BOOL bClientPoint=FALSE);

// Overrides
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual ~CSkinBaseButton();

	// Generated message map functions
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseLeave();

	DECLARE_MESSAGE_MAP()	
	
};

