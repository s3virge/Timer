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
	bool IsPushed(); // Проверка нажата ли клавиша
	void SetPushed( bool bPushed=true ); // Сделать клавишу нажатой/отжатой
	void SetImagePosition( int nImPos ); // SBTN_LEFT or SBTN_TOP
	
	void SetStyleFlat( bool bFlat=false );//установить плоский вид кнопки
	void SetStyleHotText( bool bHot=true );
	void SetStyleToggle( bool bToggle=true ); // Клавиша с 2 состояниями( нажата/отжата )
	
	void SetTextColor(COLORREF clrTextColor); //цвет текста
	void SetHotTextColor(COLORREF clrHotTextColor); //цвет текста для стиля HotText
	void SetBgColor(COLORREF clrBtnBg); //цвета фона кнопки
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

	bool	m_bPushed;      // Для Toggle button означает нажата ли клавиша
	int     m_nWidthFrame;	// Толщина 3D-рамки
// Styles
	//bool    m_bColored;
	bool	m_bFlat;
	bool	m_bToggle;
	bool	m_bShowFocus;
	bool	m_bHotText;
// Image data
	HANDLE	m_hImage;		// Цветное изображение
	HANDLE	m_hImageBW;		// ЧБ изображение
	UINT	m_uTypeImage;   // Тип изображения(Icon or Bitmap)
	int		m_nImPos;		// Позиция изображения: над текстом или слева
	int		m_nImHeight;
	int		m_nImWidht;
	COLORREF m_clrBtnBg; //цвет фона кнопки
	COLORREF m_clrTextColor; //цвет текста
	COLORREF m_clrHotTextColor; //цвет текста при наведении мыши

	bool	m_bMouseInside; // мышка внутри клавиши

	static Type_TrackMouseEvent TrackMouseEvent;
};