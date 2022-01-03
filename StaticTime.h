

#pragma once

const DWORD STCOUNTER0    = 252;
const DWORD STCOUNTER1    = 96;
const DWORD STCOUNTER2    = 218;
const DWORD STCOUNTER3    = 242;
const DWORD STCOUNTER4    = 102;
const DWORD STCOUNTER5    = 182;
const DWORD STCOUNTER6    = 190;
const DWORD STCOUNTER7    = 224;
const DWORD STCOUNTER8    = 254;
const DWORD STCOUNTER9    = 246;
const DWORD STCOUNTER10  = 2;		// The minus sign [2]
const DWORD STCOUNTER11  = 256;	  // The "point"
const DWORD STCOUNTER12  = 257;	  // The "colon" (:)
const DWORD STCOUNTERALL  = 999;

//--------------------------------------------------
/*
     1
   -----
6 |  7  | 2
   -----
5 |     | 3
   -----
     4
 */
const DWORD NOTCH1 = 128;
const DWORD NOTCH2 = 64;
const DWORD NOTCH3 = 32;
const DWORD NOTCH4 = 16;
const DWORD NOTCH5 = 8;
const DWORD NOTCH6 = 4;
const DWORD NOTCH7 = 2;		// The minus sign
const DWORD NOTCH8 = 1;		// Not used...

//--------------------------------------------------

// This class implements a memory Device Context

//этот класс нужен для реализации двойной буферизации

class CMemoryDC : public CDC 
{
private:
	CBitmap m_bitmap; // Offscreen bitmap
	CBitmap* m_oldBitmap; // bitmap originally found in CMemDC
	CDC* m_pDC; // Saves CDC passed in constructor
	CRect m_rect; // Rectangle of drawing area.
	BOOL m_bMemDC; // TRUE if CDC really is a Memory DC.

public:
	//конструктор
	CMemoryDC(CDC* pDC, CRect rect = CRect(0,0,0,0))
		:CDC(),
		m_oldBitmap(NULL),
		m_pDC(pDC)
	{
		ASSERT(m_pDC != NULL); // If you asserted here, you passed in a NULL CDC.
		//Determines whether the device context is being used for printing.
		m_bMemDC = !pDC->IsPrinting();
		
		if (m_bMemDC)
		{
			// Create a Memory DC
			CreateCompatibleDC(pDC);

			if ( rect == CRect(0,0,0,0) )
				pDC->GetClipBox(&m_rect);//Retrieves the dimensions of the tightest bounding rectangle around the current clipping boundary.
			else
				m_rect = rect;

			//Initializes a bitmap that is compatible with the device specified by pDC
			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_oldBitmap = SelectObject(&m_bitmap);

			//Sets the window origin of the device context.
			SetWindowOrg(m_rect.left, m_rect.top);
		} 
		else 
		{
			// Make a copy of the relevent parts of the current DC for printing
			m_bPrinting = pDC->m_bPrinting;
			m_hDC = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}
	}
	
	~CMemoryDC()
	{
		if (m_bMemDC) 
		{
			// Copy the offscreen bitmap onto the screen.
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), this, m_rect.left, m_rect.top, SRCCOPY);
			//Swap back the original bitmap.
			SelectObject(m_oldBitmap);
		} 
		else 
		{
			// All we need to do is replace the DC with an illegal value,
			// this keeps us from accidently deleting the handles associated with
			// the CDC that was passed to the constructor.
			m_hDC = m_hAttribDC = NULL;
		}
	}
	
	// Allow usage as a pointer
	CMemoryDC* operator->() {return this;}
	
	// Allow usage as a pointer
	operator CMemoryDC*() {return this;}
};

//--------------------------------------------------

class CStaticTime : public CStatic
{
public:
	CStaticTime();
	virtual ~CStaticTime();

	void DisplayTime( CString strFormat = "%H:%M:%S" );
	
	void SetDrawFaded(bool bState = true); //рисовать тусклые сегменты 
		
	void Display( CString strDisplay );	
	void Display(UINT uSecs=0, UINT uMins=0, UINT uHours=0, UINT uDays=0, CString strFormat="%D:%H:%M:%S");	

	//установить цвет светящегося сегмента
	void SetColourForeGround(COLORREF crColor = 0xffffffff);
	//установить цвет фано окна
	void SetColourBackGround(COLORREF crColor = 0xffffffff);
	//установить цвет тусклого сегмента
	void SetColourFaded(COLORREF crColor = 0xffffffff);

	void Update();
	void Draw( CMemoryDC* pDC, DWORD dwChar, int nCol);

private:
	bool m_bDrawFadedNotches;
	bool m_bGotMetrics;
	bool m_bSpecifiedFadeColour;

	CString m_strDisplay;

	RECT m_recClient;

	int m_nNotchWidth;  //толщина одного элемента цифры
	int m_nNotchLength; //длина одного элемента
	int m_nMargin;		//промежуток между цифрами

	COLORREF m_crBackground;
	COLORREF m_crForeground;
	COLORREF m_crDimForeground;

	CBrush m_brBackground;
	CBrush m_brForeground;


	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};