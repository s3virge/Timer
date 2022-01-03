//#include "../stdafx.h"

#pragma once

typedef BOOL (FAR WINAPI *Type_TrackMouseEvent)(LPTRACKMOUSEEVENT);

#define ON_WM_MOUSELEAVE() \
	{ WM_MOUSELEAVE, 0, 0, 0, AfxSig_vv, \
		(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)())&OnMouseLeave },

/////////////////////////////////////////////////////////////////////////////
// SButton window

class SButton : public CButton
{
	struct SBITMAPINFO 
	{
		BITMAPINFOHEADER    bmiHeader;
		DWORD				bmiColors[256];			
		
		operator BITMAPINFO() { return *this; }
		operator LPBITMAPINFO() { return (LPBITMAPINFO)this; }
		operator LPBITMAPINFOHEADER() { return (LPBITMAPINFOHEADER)this; }	
	};
	
	HBITMAP BWBitmap( HBITMAP hColorBM, BITMAPINFOHEADER &BMInfo );

public:
	SButton();
	virtual ~SButton();

// Attributes
	bool IsPushed(); // �������� ������ �� �������
	void SetPushed( bool bPushed=true ); // ������� ������� �������/�������
	void SetImagePosition( int nImPos ); // SBTN_LEFT or SBTN_TOP
	
	void SetStyleFlat( bool bFlat=false );//���������� ������� ��� ������
	void SetStyleHotText( bool bHot=true );
	void SetStyleToggle( bool bToggle=true ); // ������� � 2 �����������( ������/������ )
	
	void SetTextColor(COLORREF clrTextColor); //���� ������
	void SetHotTextColor(COLORREF clrHotTextColor); //���� ������ ��� ����� HotText
	void SetBgColor(COLORREF clrBtnBg); //����� ���� ������
	bool SetImages( DWORD dwResourceID, DWORD dwResourceID_BW=NULL, bool bHotImage=true, UINT uType=IMAGE_ICON/*or IMAGE_BITMAP*/ );
	void SetFrameWidth(int nWidth ); //NO_FRAME, THIN_FRAME, THICK_FRAME, IMGPOS_LEFT, IMGPOS_TOP
	void ShowFocus(bool bShow=true);
		
	inline DWORD BWColor( DWORD b, DWORD g, DWORD r );
		
	enum { NO_FRAME, THIN_FRAME, THICK_FRAME, IMGPOS_LEFT, IMGPOS_TOP };

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnMouseLeave();

	DECLARE_MESSAGE_MAP()

private:

	bool	m_bPushed;      // ��� Toggle button �������� ������ �� �������
	int     m_nWidthFrame;	// ������� 3D-�����
// Styles
	//bool    m_bColored;
	bool	m_bFlat;
	bool	m_bToggle;
	bool	m_bShowFocus;
	bool	m_bHotText;
// Image data
	HANDLE	m_hImage;		// ������� �����������
	HANDLE	m_hImageBW;		// �� �����������
	UINT	m_uTypeImage;   // ��� �����������(Icon or Bitmap)
	int		m_nImPos;		// ������� �����������: ��� ������� ��� �����
	int		m_nImHeight;
	int		m_nImWidht;
	COLORREF m_clrBtnBg; //���� ���� ������
	COLORREF m_clrTextColor; //���� ������
	COLORREF m_clrHotTextColor; //���� ������ ��� ��������� ����

	bool	m_bMouseInside; // ����� ������ �������

	static Type_TrackMouseEvent TrackMouseEvent;
};