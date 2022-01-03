#pragma once
#include "afxwin.h"


// CWindowToFollowDlg dialog

class CDlgWindowToFollow : public CDialog
{
	DECLARE_DYNAMIC(CDlgWindowToFollow)
	DECLARE_MESSAGE_MAP()

public:
	CDlgWindowToFollow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgWindowToFollow();
	// Dialog Data
	enum { IDD = IDD_DLG_WINDOWTOFOLLOW };

public:
	HWND GetWinToFollow();

private:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	bool m_bStartSearchWindow;
	HCURSOR m_hCursorSearchWindow;
	HCURSOR m_hCursorPrevious;
	CPen m_RectanglePen; //карандаш для выделения найденного окна
	
	CString m_csFoundWindowText; //заголовок найденного окна
	HWND m_hwndFoundWindow; //описатель найденного окна
	CStatic m_wndAim;

	CString m_csProgrammName;

	afx_msg void OnStnClickedAim();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedClosewintofollowdlg();
	
	void HighlightFoundWindow (CWnd* pFoundWindow);
	void RefreshWindow (HWND hwndWindowToBeRefreshed);
	BOOL CheckWindowValidity (HWND hwndDialog, HWND hWndToCheck);
	//CString GetWinToFollowText();
	void RegWriteSettings(BOOL bCloseIfWindowSelect);
	BOOL RegReadSettings();
	BOOL m_bCloseWhenWindowSelect;
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
