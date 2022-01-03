#if !defined(AFX_LABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_)
#define AFX_LABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Label.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLCDDots window
enum FlashType {None, Text, Background };

class CLCDDots : public CStatic
{
	DECLARE_DYNAMIC(CLCDDots)
	
// Construction
public:
	CLCDDots();
	void SetBkColor(COLORREF crBkgnd);

protected:
	COLORREF	m_bkcolor;
	
public:
	virtual ~CLCDDots();
	afx_msg void OnPaint(void);
	
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_LABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_)
