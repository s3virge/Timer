
#if !defined(AFX_LCD7_H__2D19C590_DD9E_4371_9827_4E02E4826C02__INCLUDED_)
#define AFX_LCD7_H__2D19C590_DD9E_4371_9827_4E02E4826C02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
//
// lcd7.h : header file
//

// Make sure these messages are not used in your application already
// If in doubt, use RegisterWindowMessages

//#define WM_OVERFLOW			(WM_USER + 1)
//#define WM_UNDERFLOW		(WM_USER + 2)
//#define	WM_TCHANGE			(WM_USER + 3)

/////////////////////////////////////////////////////////////////////////////
// LCDDigits window

class CLCDDigits : public CWnd
{
// Construction
public:	
	CLCDDigits();
	int  gap, width; //gap - щель
	int  num;		 //отображаемая цифра
	int  low, high;
	int  firstpaint;

	CPoint oldpoint;
	
	bool seg1, seg2, seg3, seg4, seg5, seg6, seg7;
	
	COLORREF LigntColor; //цывет "светящегося" сегмента 
	COLORREF NotLightColor; //цвет не "светящегося" сегмента
	COLORREF bg; //цвет фона 

// Attributes
public:

// Operations
public:
	void SetLigntColor(COLORREF col);
	void SetNotLigntColor(COLORREF col);
	void SetBgColor(COLORREF col);

public:
	void SetMinMax(int minn, int maxx);
	virtual ~CLCDDigits();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLCDDigits)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_LCD7_H__2D19C590_DD9E_4371_9827_4E02E4826C02__INCLUDED_)
