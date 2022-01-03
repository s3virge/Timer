// CWindowToFollowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Timer.h"
#include "DlgWindowToFollow.h"
#include "TimerDlg.h"


// CWindowToFollowDlg dialog

IMPLEMENT_DYNAMIC(CDlgWindowToFollow, CDialog)

CDlgWindowToFollow::CDlgWindowToFollow(CWnd* pParent /*=NULL*/)
: CDialog(CDlgWindowToFollow::IDD, pParent)
, m_csFoundWindowText(_T(""))
, m_bCloseWhenWindowSelect(FALSE)
, m_hCursorSearchWindow(FALSE)
{	
	m_bStartSearchWindow = false;
	m_RectanglePen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	m_hwndFoundWindow = false;	

	/////////////// выдел€ем им€ программы //////////////////
	char szFileName[MAX_PATH] = {'0'}; //название конфигурационного файла
	char* ptr; //нужен чтобы получить название экзешника

	//полный путь к исполн€емому файлу
	GetModuleFileNameA( NULL, szFileName, MAX_PATH );
	//убираем расширение .exe
	szFileName[strlen(szFileName)-4] = 0;

	//выделить название файла
	ptr = strrchr(szFileName, '\\') + 1; //+1 чтобы отбросить символ \, инача будет \filename.exe

	m_csProgrammName.Format("%s", ptr);
	///////////////////////////////////////////////////////////
}

CDlgWindowToFollow::~CDlgWindowToFollow()
{
}

void CDlgWindowToFollow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AIM, m_wndAim);
	DDX_Text(pDX, IDC_WINDOW_TEXT, m_csFoundWindowText);
	DDX_Check(pDX, IDC_CLOSEWINTOFOLLOWDLG, m_bCloseWhenWindowSelect);
}


BEGIN_MESSAGE_MAP(CDlgWindowToFollow, CDialog)	
	ON_STN_CLICKED(IDC_AIM, &CDlgWindowToFollow::OnStnClickedAim)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CLOSEWINTOFOLLOWDLG, &CDlgWindowToFollow::OnBnClickedClosewintofollowdlg)
END_MESSAGE_MAP()


// CWindowToFollowDlg message handlers

//=== перемещаем окно за любую область ===
void CDlgWindowToFollow::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgWindowToFollow::OnStnClickedAim()
{
	// Hide the windows.
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
	::ShowWindow (GetParent()->GetSafeHwnd(), SW_HIDE);
	::ShowWindow (this->GetSafeHwnd(), SW_HIDE);
	
	//пр€чем окно с тенью==================
	//нужно получить доступ к m_Shadow который определЄн в TimerDlg
	::ShowWindow(CTimerDlg::m_staticHwndWndShadow, SW_HIDE);
	
	//флаг начала пойска окна true-начинаем пойск
	m_bStartSearchWindow = TRUE;

	m_hCursorSearchWindow = LoadCursor (AfxGetApp()->m_hInstance,  MAKEINTRESOURCE(IDC_CURSOR_AIM));
	
	// Display the empty window bitmap image in the Finder Tool static control.
	//SetFinderToolImage (hwndDialog, FALSE);

	//MoveCursorPositionToBullsEye (hwndDialog);

	// Set the screen cursor to the BullsEye cursor.
	if (m_hCursorSearchWindow)
	{
		//мен€ем курсор на прицел
		m_hCursorPrevious = SetCursor (m_hCursorSearchWindow);
	}
	else
	{
		m_hCursorPrevious = NULL;
	}

	// Very important : capture all mouse activities from now onwards and
	// direct all mouse messages to the "Search Window" dialog box procedure.
	SetCapture ();	
}

void CDlgWindowToFollow::OnLButtonUp(UINT nFlags, CPoint point)
{
	// If we had a previous cursor, set the screen cursor to the previous one.
	// The cursor is to stay exactly where it is currently located when the 
	// left mouse button is lifted.
	if (m_hCursorPrevious)
	{
		SetCursor (m_hCursorPrevious);
	}

	// Very important : must release the mouse capture.
	ReleaseCapture ();

	// If there was a previously found window, we must instruct it to refresh itself. 
	// This is done to remove any highlighting effects drawn by us.
	if ( m_hwndFoundWindow )
	{
		RefreshWindow (m_hwndFoundWindow);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->SetFocus();
	}

	// Make the windows appear normally.
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW); //главное окно
	::ShowWindow(CTimerDlg::m_staticHwndWndShadow, SW_SHOW);//тень
	::ShowWindow (GetParent()->GetSafeHwnd(), SW_SHOW); //окно настройка
		
	if ( m_bCloseWhenWindowSelect && m_bStartSearchWindow )
	{
		EndDialog(IDOK);
	}
	else
	{
		::ShowWindow (this->GetSafeHwnd(), SW_SHOW); 
	}
	
	// Set the global search window flag to FALSE.
	m_bStartSearchWindow = FALSE;

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgWindowToFollow::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bStartSearchWindow)
	{
		CWnd*	pWndFoundWindow;
		CPoint screenpoint;
		// Must use GetCursorPos() instead of calculating from "lParam".
		GetCursorPos (&screenpoint);
		// Determine the window that lies underneath the mouse cursor.
		pWndFoundWindow = WindowFromPoint (screenpoint);		

		if(CheckWindowValidity (this->GetSafeHwnd(), pWndFoundWindow->GetSafeHwnd()))
		{
			// If there was a previously found window, we must instruct it to refresh itself. 
			// This is done to remove any highlighting effects drawn by us.
			if (m_hwndFoundWindow)
			{
				RefreshWindow (m_hwndFoundWindow);
			}

			// Indicate that this found window is now the current global found window.
			m_hwndFoundWindow = pWndFoundWindow->GetSafeHwnd();
			//подсвечиваем найденое окно
			HighlightFoundWindow (pWndFoundWindow);
			//получить текст окна
			pWndFoundWindow->GetWindowTextA(m_csFoundWindowText);

			//добавить к тексту окна его ’ендл
			CString temp = m_csFoundWindowText + " (ќписатель окна: %d)";			
			m_csFoundWindowText.Format(temp, m_hwndFoundWindow);

			//показать заголовок окна в диалоге
			UpdateData(FALSE);
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CDlgWindowToFollow::RefreshWindow (HWND hwndWindowToBeRefreshed)
{
	::InvalidateRect (hwndWindowToBeRefreshed, NULL, TRUE);
	::UpdateWindow (hwndWindowToBeRefreshed);
	::RedrawWindow (hwndWindowToBeRefreshed, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
}


BOOL CDlgWindowToFollow::CheckWindowValidity (HWND hwndDialog, HWND hWndToCheck )
{
	HWND hwndTemp = NULL;
	BOOL bRet = TRUE;

	// ќкно не должно быть NULL.
	if (hWndToCheck == NULL)
	{
		bRet = FALSE;
		goto CheckWindowValidity_0;
	}

	// описатель принадлежит существующему окну?
	if (IsWindow(hWndToCheck) == FALSE)
	{
		bRet = FALSE;
		goto CheckWindowValidity_0;
	}

	// убеждаемс€ что не нашли тоже окно что раньше
	if (hWndToCheck == m_hwndFoundWindow)
	{
		bRet = FALSE;
		goto CheckWindowValidity_0;
	}

	// исключаем главное окно
	/*if (hWndToCheck == m_hwndMainWnd)
	{
		bRet = FALSE;
		goto CheckWindowValidity_0;
	}*/

	// исключаем диалог пойска
	if (hWndToCheck == hwndDialog)
	{
		bRet = FALSE;
		goto CheckWindowValidity_0;
	}

	// исключаем child окна диалога пойска
	hwndTemp = ::GetParent (hWndToCheck);

	if ((hwndTemp == hwndDialog))// || (hwndTemp == m_hwndMainWnd))
	{
		bRet = FALSE;
		goto CheckWindowValidity_0;
	}

CheckWindowValidity_0:

	return bRet;

}

void CDlgWindowToFollow::HighlightFoundWindow (CWnd* pFoundWindow)
{
	CPen*	pPrevPen = NULL;   // Handle of the existing pen in the DC of the found window.
	CBrush*	pPrevBrush = NULL; // Handle of the existing brush in the DC of the found window.
	RECT	rect;              // Rectangle area of the found window.

	CPen* m_pRectanglePen = &m_RectanglePen;;
	// Get the screen coordinates of the rectangle of the found window.
	pFoundWindow->GetWindowRect( &rect );

	//// Get the window DC of the found window.
	CDC* pWindowDC = pFoundWindow->GetWindowDC();

	if (pWindowDC)
	{
		// Select our created pen into the DC and backup the previous pen.
		pPrevPen = (CPen*)pWindowDC->SelectObject (m_pRectanglePen);

		CBrush HollowBrush; 
		HollowBrush.CreateStockObject(HOLLOW_BRUSH);

		// Select a transparent brush into the DC and backup the previous brush.
		pPrevBrush = (CBrush*)pWindowDC->SelectObject ( &HollowBrush );

		// Draw a rectangle in the DC covering the entire window area of the found window.
		pWindowDC->Rectangle (0, 0, rect.right - rect.left, rect.bottom - rect.top);

		// Reinsert the previous pen and brush into the found window's DC.
		pWindowDC->SelectObject ( pPrevPen );
		pWindowDC->SelectObject ( pPrevBrush );

		// Finally release the DC.
		ReleaseDC (pWindowDC);
	}
}
//получить текст окна за которым будем следить
//CString CWindowToFollowDlg::GetWinToFollowText()
//{
//	return m_csFoundWindowText;
//}
HWND CDlgWindowToFollow::GetWinToFollow()
{
	return m_hwndFoundWindow;
}
void CDlgWindowToFollow::OnBnClickedClosewintofollowdlg()
{
	UpdateData(TRUE);
	//сохранить установку в реестр
	RegWriteSettings(m_bCloseWhenWindowSelect);
}
//записываем установки в реестр
void CDlgWindowToFollow::RegWriteSettings(BOOL bCloseWhenWindowSelect)
{
	//сохраним настройки в реестре
	HKEY hk; 
	DWORD dwDisp; 
	char szBuf[MAX_PATH]; 
	
	CString ValueName = CloseWhenWindowSelect;
		
	sprintf(szBuf, "SOFTWARE\\%s", m_csProgrammName); 

	if (RegCreateKeyEx(HKEY_CURRENT_USER, szBuf, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hk, &dwDisp) == ERROR_SUCCESS) 
	{
		//--- сохран€ем ---
		if (RegSetValueEx(hk,				// subkey handle 
			ValueName,						// value name 
			0,								// must be zero 
			REG_BINARY,						// value type 
			(BYTE*) &bCloseWhenWindowSelect,         // pointer to value data 
			(DWORD) sizeof (BYTE)) != ERROR_SUCCESS ) // data size
		{
			TRACE("Could not set reg value StartClose.\n");
			return;
		}
	}
}

BOOL CDlgWindowToFollow::RegReadSettings()
{
	char szBuf[MAX_PATH]; 
		
	CString pszValue = CloseWhenWindowSelect;
	DWORD Data = 0;  //destination data buffer
	DWORD DataSize = sizeof(BYTE);
	DWORD pdwType	= REG_BINARY;

	sprintf(szBuf, "SOFTWARE\\%s", m_csProgrammName);

	if(::SHGetValue( HKEY_CURRENT_USER, szBuf, pszValue, &pdwType, &Data, &DataSize) != ERROR_SUCCESS)
	{
		TRACE("ќблом RegReadSettings()\n");
		return false;
	}

	return (BOOL)Data;
}
BOOL CDlgWindowToFollow::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	//сохранить установку в реестр
	m_bCloseWhenWindowSelect = RegReadSettings();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//=== устанавливаем цвет окна ===
BOOL CDlgWindowToFollow::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect( rect, WindowColor ); //цвет окна
	
	//Nonzero if it erases the background; otherwise 0.
	return true; //предотвращаем мерцание изображини€

	//return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgWindowToFollow::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	CBrush* m_pEditBkBrush;

	// Instantiate and initialize the background brush to black.
	m_pEditBkBrush = new CBrush( WindowColor );

	switch (nCtlColor) 
	{
	case CTLCOLOR_STATIC:
	//case CTLCOLOR_EDIT:
	//case CTLCOLOR_MSGBOX:
	//case CTLCOLOR_BTN:
		// Set color to green on black and return the backgroundbrush.
		pDC->SetTextColor(TextColor);
		//pDC->SetBkColor(RGB(200, 200, 200));
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
	}

	// Return a different brush if the default is not desired
	return hbr;
}