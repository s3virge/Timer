
#include "stdafx.h"
#include "Timer.h"
#include "TimerDlg.h"

#include "ShutDown.h"
#include "DlgSettings.h"
#include "DlgWindowToFollow.h"
#include "DlgGetPassword.h"
#include "DefaultSettings.h"

#include "MultiLanguage.h"

#pragma warning(disable : 4996)
#pragma comment(lib, "Version.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


UINT __cdecl THREADPROCMinimizeToSystemTray( LPVOID pParam );
	
#define TRAYICONID	1			//	ID number for the Notify Icon
#define WM_TRAYMSG	WM_APP	//	the message ID sent to our window
#define SWM_SHOW	WM_APP + 1//	show the window
#define SWM_HIDE	WM_APP + 2//	hide the window
#define SWM_EXIT	WM_APP + 3//	close the window
//#define SWM_MODIFY  WM_APP + 4

//статическая переменная класса CTimerDlg в которой хранится Описатель окна Shadow
//чтобы иметь возможность скрывать окно Shadow в коде других классов
HWND CTimerDlg::m_staticHwndWndShadow;

//////////////////////////////// CAboutDlg //////////////////////////////////
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void GetAppVersion();


public:
	virtual BOOL OnInitDialog();	

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//m_sLink.SetURL("mailto:kobzar_v@list.ru");
	/*m_sLink.SetUnderline(FALSE);
	m_sLink.SetToolTip("write to the autor");
*/
	GetAppVersion();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::GetAppVersion()
{
	wchar_t fullyQualifiedPath[MAX_PATH]; 
	//определить путь к файлу
	::GetModuleFileNameW(NULL, fullyQualifiedPath, MAX_PATH);

	DWORD dwDummy;
	DWORD dwFVISize = GetFileVersionInfoSizeW( fullyQualifiedPath , &dwDummy ); 

	LPBYTE lpVersionInfo = new BYTE[dwFVISize]; 
	GetFileVersionInfoW( fullyQualifiedPath , 0 , dwFVISize , lpVersionInfo ); 

	UINT uLen; 
	VS_FIXEDFILEINFO *lpFfi; 

	VerQueryValue( lpVersionInfo , _T("\\") , (LPVOID *)&lpFfi , &uLen ); 

	DWORD dwFileVersionMS = lpFfi->dwFileVersionMS; 
	DWORD dwFileVersionLS = lpFfi->dwFileVersionLS; 

	delete [] lpVersionInfo; 

	TRACE( "Higher: %x\n" , dwFileVersionMS );
	TRACE( "Lower: %x\n" , dwFileVersionLS ); 

	DWORD dwLeftMost = HIWORD(dwFileVersionMS); 
	DWORD dwSecondLeft = LOWORD(dwFileVersionMS); 
	DWORD dwSecondRight = HIWORD(dwFileVersionLS); 
	DWORD dwRightMost = LOWORD(dwFileVersionLS);

	TRACE( "Version: %d.%d.%d.%d\n" , dwLeftMost, dwSecondLeft, dwSecondRight, dwRightMost );

	const int DestBuffSize = 30;
	char strVersionInfo[DestBuffSize];
	LPCTSTR pszFormat = "Version: %d.%d.%d.%d";

	StringCchPrintf(strVersionInfo, DestBuffSize, pszFormat, dwLeftMost, dwSecondLeft, dwSecondRight, dwRightMost);

	GetDlgItem(IDC_VERSION)->SetWindowTextA(strVersionInfo);
}


//////////////////////////////// CTimerDlg dialog ////////////////////////////////////////
CTimerDlg::CTimerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimerDlg::IDD, pParent)
	, m_bDragging(false)
	, m_bFirstPaint(true)
	, wndWorkArea(0, 0, 0, 0)
	, wndRect(0, 0, 0, 0)
	, StickAt(20)
	, m_csEstablishedTime("00:00:00")
	, m_iAction(TODO_SHUTDOWN)
	, m_iMode(OM_TIMER)
	, m_iAdditionAction( TODOAFTER_NOTSET )
	, m_nHour(0)
	, m_nMinute(0)
	, m_nSeconds(0)
	, m_bSrartStop(false)
	/*, m_bDoubleClickClose(false)
	, m_bKeepPosition(false)
	, m_bMinimizeToSystemArea(false)*/
	, m_bIconToShow(true)
	
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	CTimerDlg::m_staticHwndWndShadow = NULL;
	
	/////////////// выделяем имя программы //////////////////
	char szFileName[MAX_PATH] = {'0'}; //название конфигурационного файла
	char* ptr; //нужен чтобы получить название экзешника

	//полный путь к исполняемому файлу
	GetModuleFileNameA( NULL, szFileName, MAX_PATH );
	//убираем расширение .exe
	szFileName[strlen(szFileName)-4] = 0;

	//выделить название файла
	ptr = strrchr(szFileName, '\\') + 1; //+1 чтобы отбросить символ \, инача будет \filename.exe

	m_csProgrammName.Format("%s", ptr);
	///////////////////////////////////////////////////////////
}

void CTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

// CTimerDlg message handlers
BEGIN_MESSAGE_MAP(CTimerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_SKIN_BUTTON_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_SKIN_BUTTON_MINIMIZE, OnButtonMinimize)
	ON_BN_CLICKED(IDC_SKIN_BUTTON_STARTSTOP, OnButtonStartStop)
	ON_BN_CLICKED(IDC_SKIN_BUTTON_SETTINGS, OnButtonSettings)
	ON_BN_CLICKED(IDC_SKIN_BUTTON_ADJUST, OnButtonAdjust)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_TRAYMSG, ProcessingTrayMessage)
	ON_COMMAND(SWM_SHOW, TrayMenuShow)
	ON_COMMAND(SWM_HIDE, TrayMenuHide)
	ON_COMMAND(SWM_EXIT, TrayMenuExit)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


//====================== настраиваем главное окно ==================================
int CTimerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	/////////////// выделяем имя программы //////////////////
	char szFileName[MAX_PATH] = {'0'}; //название конфигурационного файла
	char* ptr; //нужен чтобы получить название экзешника

	//полный путь к исполняемому файлу
	GetModuleFileNameA( NULL, szFileName, MAX_PATH );
	//убираем расширение .exe
	szFileName[strlen(szFileName)-4] = 0;

	//выделить название файла
	ptr = strrchr(szFileName, '\\') + 1; //+1 чтобы отбросить символ \, инача будет \filename.exe

	//------------------------------------
	#ifdef _DEBUG
	SetWindowTextA("debug version");
	#else
	SetWindowTextA(m_csProgrammName);
	#endif

	//------------ установить язык интерфейса ---------------------
	//CMultilanguage* obj_MulyiLang = new CMultilanguage();
	CMultilanguage obj_MulyiLang;
	obj_MulyiLang.ReadGUILanguageFile(); //читаем языковые ресурсы из language.lng

	//delete obj_MulyiLang;
	//------------------------------------
	ModifyStyle(0, CS_DBLCLKS|WS_MINIMIZEBOX);	

	//------------------------------------
	CalcWindowSize();
	
	//------------------------------------
	CreateButtons();	

	//TRACE("OnCreate()\n");

	return 0;
}

//====================== создаем кнопки =============================
void CTimerDlg::CreateButtons()
{
	CRect m_recClient;
	GetClientRect(&m_recClient);

	int iYposition = 2;

	///////////// задаем расположение Экрана времени ////////////////
	CRect rc = CRect(m_recClient.left + 40, m_recClient.top + 22, m_recClient.right-25, m_recClient.bottom-49);

	//статик в котором будем отображать ход времени
	m_stcTime.Create("", WS_CHILD|WS_CHILD, rc, this, IDC_STATIC_TIME);
	m_stcTime.ShowWindow(TRUE);
	//--- устанавливаем цвета для цифр экрана --
	m_stcTime.SetColourFaded(RGB(49,49,80));
	m_stcTime.SetColourBackGround(RGB(49, 49, 49));
	m_stcTime.SetColourForeGround(RGB(100, 150, 220));		

	////////////////////////////////////////////////////////////////// кнопка установок //////////////////////////////////////
	m_buttonAdjustment.m_dibButtonStates.LoadBitmap(IDB_BUTTON_ADJUST);
	m_buttonAdjustment.CalcSize();

	//задаем координаты левого верхнего укла будущей кнопки
	CPoint origin = CPoint(m_recClient.right - 46, iYposition);
	//вычисляем размеры кнопки
	rc = CRect(origin.x, origin.y, origin.x + m_buttonAdjustment.m_szButton.cx, origin.y + m_buttonAdjustment.m_szButton.cy);
	m_buttonAdjustment.Create("", WS_CHILD|WS_VISIBLE, rc, this, IDC_SKIN_BUTTON_ADJUST);

	//создаем всплывающие подсказки
	m_wndToolTips = m_ToolTips.CreateToolTip(m_buttonAdjustment);
	m_ToolTips.AddTool(m_wndToolTips, m_buttonAdjustment, NULL, NULL,CMultilanguage::sLanguageText[6]);

	///////////////////////////////////////////////////////////////// кнопка закрытия окна ///////////////////////
	m_buttonClose.m_dibButtonStates.LoadBitmap(IDB_BUTTON_CLOSE);
	m_buttonClose.CalcSize();	

	origin = CPoint(m_recClient.right - 18, iYposition);

	rc = CRect(origin.x, origin.y, origin.x + m_buttonClose.m_szButton.cx, origin.y + m_buttonClose.m_szButton.cy);
	m_buttonClose.Create("", WS_CHILD|WS_VISIBLE, rc, this, IDC_SKIN_BUTTON_CLOSE);

	m_wndToolTips = m_ToolTips.CreateToolTip(m_buttonClose);
	m_ToolTips.AddTool(m_wndToolTips, m_buttonClose, NULL, NULL,CMultilanguage::sLanguageText[7]);

	///////////////////////////////////////////////////////////////// свернуть окно ///////////////////////////////
	m_buttonMinimize.m_dibButtonStates.LoadBitmap(IDB_BUTTON_MINIMIZE);
	m_buttonMinimize.CalcSize();	
	
	origin = CPoint(m_recClient.right - 32, iYposition);

	rc = CRect(origin.x, origin.y, origin.x + m_buttonMinimize.m_szButton.cx, origin.y + m_buttonMinimize.m_szButton.cy);
	m_buttonMinimize.Create("", WS_CHILD|WS_VISIBLE, rc, this, IDC_SKIN_BUTTON_MINIMIZE);

	m_wndToolTips = m_ToolTips.CreateToolTip(m_buttonMinimize);
	m_ToolTips.AddTool(m_wndToolTips, m_buttonMinimize, NULL, NULL,CMultilanguage::sLanguageText[8]);

	///////////////////////////////////////////////////////////////// кнопка Start\Stop ///////////////////////////
	iYposition = 87;
	
	m_buttonStartStop.m_dibButtonStates.LoadBitmap(IDB_BUTTON_STARTSTOP);
	m_buttonStartStop.CalcSize();	
	
	//origin = CPoint(148, 91);
	//вычисляем координаты левого верхнего укла кнопки
	//от правой стороны отнимаем 
	origin = CPoint(m_recClient.right - (m_buttonStartStop.m_szButton.cx * 2) - 25, iYposition);
	rc = CRect(origin.x, origin.y, origin.x + m_buttonStartStop.m_szButton.cx, origin.y + m_buttonStartStop.m_szButton.cy);
	m_buttonStartStop.Create("", WS_CHILD|WS_VISIBLE|WS_TABSTOP, rc, this, IDC_SKIN_BUTTON_STARTSTOP);

	//создаем всплывающие подсказки
	m_wndToolTips = m_ToolTips.CreateToolTip(m_buttonStartStop);
	m_ToolTips.AddTool(m_wndToolTips, m_buttonStartStop, NULL, NULL,CMultilanguage::sLanguageText[4]);

	/////////////////////////////////////////////////////////////////// кнопка Settings ///////////////////////////////
	m_buttonSettings.m_dibButtonStates.LoadBitmap(IDB_BUTTON_SETTINGS);
	m_buttonSettings.CalcSize();
	
	//origin = CPoint(248, 91);
	origin = CPoint(m_recClient.right - m_buttonSettings.m_szButton.cx - 18, iYposition);
	rc = CRect(origin.x, origin.y, origin.x + m_buttonSettings.m_szButton.cx, origin.y + m_buttonSettings.m_szButton.cy);
	m_buttonSettings.Create("", WS_CHILD|WS_VISIBLE|WS_TABSTOP, rc, this, IDC_SKIN_BUTTON_SETTINGS);

	//создаем всплывающие подсказки
	m_wndToolTips = m_ToolTips.CreateToolTip(m_buttonSettings);
	m_ToolTips.AddTool(m_wndToolTips, m_buttonSettings, NULL, NULL, CMultilanguage::sLanguageText[5]);
}

//====================== обрезаем окно по картинке ==================
void CTimerDlg::CreateCropRigion(CDC* pDC)
{
	COLORREF FilterColor;	/*color to filter out*/

	// load IDB_MAINWND from our resources
	CBitmap bmp;
	if (bmp.LoadBitmap(IDB_MAINWND))
	{
		// Get the info of the bitmap
		BITMAP bmpInfo;
		bmp.GetBitmap(&bmpInfo);

		// Create an in-memory DC compatible with the
		// display DC we're using to paint
		CDC dcMemory;
		dcMemory.CreateCompatibleDC(pDC);

		// Select the bitmap into the in-memory DC
		dcMemory.SelectObject(&bmp);

		/*The GetPixel method gets the color of a specified pixel in this bitmap*/
		FilterColor = dcMemory.GetPixel( 0, 0 ); //цвет пиксела который будем фильтровать

		//----------------------------------------------------------------------------------------
		HRGN hRgnTemp, hRgnFinal; //Handle to a region
		hRgnFinal = CreateRectRgn( 0,0,0,0 );	/*final must be initialized before being combined*/

		int iRow = 0, iStartPos = 0, iLeft = 0;

		//формируем ригион по которому будет вырезано изображение
		//перемещаемся по строкам
		for( iRow; iRow <= bmpInfo.bmHeight-1; iRow++)
		{	//перемещаемся по столбцам		
			while( iLeft < bmpInfo.bmWidth-1 )
			{	//пока цвет текущего пиксела совпадает с фильтруемым и не дошли до последнего столбца
				while( ( dcMemory.GetPixel( iLeft, iRow ) == FilterColor ) && ( iLeft < bmpInfo.bmWidth) )
				{	//смещаемся направо (к последнему столбцу)
					iLeft++;
				}
				//если не последний столбец
				if( iLeft != bmpInfo.bmWidth )
				{	//запоминаем начало региона
					iStartPos = iLeft;
					//пока цвет текущего пиксела не совпадает с фильтруемым и не дошли до последнего столбца
					while( ( dcMemory.GetPixel( iLeft, iRow) != FilterColor) && ( iLeft < bmpInfo.bmWidth ) )
					{	//смещаемся к последнему столбцу
						iLeft++;
					}
					//создаем регион с полученными координатами
					//Высота прямоугольника, описывающего одну строку, равна одному пикселу
					hRgnTemp = CreateRectRgn( iStartPos, iRow, iLeft, iRow+1 );

					//объединяем полученные регионы
					CombineRgn( hRgnFinal,		// handle to destination region
						hRgnFinal,		// handle to source region
						hRgnTemp,		// handle to source region
						RGN_XOR );	// region combining mode
				}
			}

			iLeft = 0;
		}
		//------------------------------------------------------------------------------------------------

		SetWindowRgn( hRgnFinal, TRUE);
		
		DeleteObject(hRgnTemp);	
		DeleteObject(hRgnFinal);
		dcMemory.DeleteDC();
		
		m_bFirstPaint = false;	
	}
}

void CTimerDlg::SetWindowBitmap(CDC* pDC)
{
	// load IDB_MAINWND from our resources
	CBitmap bMainWnd;
	if (bMainWnd.LoadBitmap(IDB_MAINWND))
	{
		if( m_bFirstPaint )	/*if its our first time painting, shape the window*/
		{
			ShowWindow( SW_HIDE );	/* window not visible until shaped*/
			//один раз создаем регион для обрезки изображения
			CreateCropRigion( pDC );
			ShowWindow( SW_SHOW );
		}

		CDC dcTemp;
		/*load and bitblt image to our hdc*/
		dcTemp.CreateCompatibleDC(pDC);

		dcTemp.SelectObject(&bMainWnd);

		//------------------------------------------------------
		//цвет шрифта
		dcTemp.SetTextColor(RGB(100, 150, 255));
		//режим отображения текста
		dcTemp.SetBkMode( TRANSPARENT);
		//------------------------------------------------------
		
		// Get the info of the bitmap
		BITMAP bmpInfo;
		bMainWnd.GetBitmap(&bmpInfo);

		pDC->BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcTemp, 0, 0, SRCCOPY);				
	
		dcTemp.DeleteDC();
	}
}

// ===================== Initialize the dialog box's controls =======
BOOL CTimerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{			
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);			
		}
		
		//убираем лишние элементы системного меню
		//pSysMenu->RemoveMenu(0, MF_BYPOSITION); //восстановить
		pSysMenu->RemoveMenu(4, MF_BYPOSITION); //развернуть
		pSysMenu->RemoveMenu(2, MF_BYPOSITION); //размер
		pSysMenu->RemoveMenu(1, MF_BYPOSITION); //переместить	
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//==============================================================
	//получить установки поведения окна 
	m_dlgAdjust.ReadAdjustments();
	
	//положение окна на экране
	RegReadWindowPosition();

	//================================================================
	CDefaulSettings oDefSettings;

	if (oDefSettings.IsDefaultSettings())
	{
		oDefSettings.ReadSettings(m_nHour, m_nMinute, m_nSeconds, m_iMode, m_iAction, m_iAdditionAction);
		
		//---- подготавливаем переменную с установленным временем срабатывания ---- 
		CString csHour, csMin, csSec;
		
		//--- форматируем часы ---
		if (m_nHour < 10)
			csHour.Format("0%d:", m_nHour);
		else
			csHour.Format("%d:", m_nHour);
		
		//--- форматируем минуты ---
		if (m_nMinute < 10)
			csMin.Format("0%d:", m_nMinute);
		else
			csMin.Format("%d:", m_nMinute);

		//--- форматируем секунды ---
		if (m_nSeconds < 10)
			csSec.Format("0%d", m_nSeconds);
		else
			csSec.Format("%d", m_nSeconds);
		
		m_csEstablishedTime = csHour + csMin + csSec;

		//отображаем время на табло главного окна
		m_stcTime.DisplayTime(m_csEstablishedTime);
	}
	
	//================================================================
	//--- делаем возможным запуск из командной строки ---
	RegWriteAppPath();	

	bool result = CommandLine();

	//постепенно показываем окно
	GraduallyShowHideWindow(true);
	
	//если есть командная строка 
	if ( result || m_dlgAdjust.IsImmediatelyStartTheTask())
	{
		//то запускаем задачу
		OnButtonStartStop();
	}	
		
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTimerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

// ===================== рисование ==================================================
void CTimerDlg::OnPaint()
{
	//IsIconic() Specifies whether CWnd is minimized (iconic).
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);		
	}
	else
	{
		//---------------------------------------------
		PAINTSTRUCT ps;
		CDC* pDC = BeginPaint(&ps);

		//натягиваем щкуру на окно
		SetWindowBitmap( pDC );

		EndPaint(&ps);
		//---------------------------------------------
		CDialog::OnPaint();
	}
}

BOOL CTimerDlg::OnEraseBkgnd(CDC* pDC)
{
	return true;//предотвращаем мерцание изображиния
}

//====================== мышиные сообщения ==========================
void CTimerDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//если можно закрывать двойным щелчком
	if ( m_dlgAdjust.m_structAdjust.bMainWndDoubleClickClose )
	{
		OnCancel();
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CTimerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bDragging)
	{
		m_bDragging = false;
		ReleaseCapture();
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CTimerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{	
	//Shuts down the WHOLE FMOD Sound System
	FSOUND_Close();

	MousePoint = point;

	m_bDragging = true;

	//Causes all subsequent mouse input to be sent to the current 
	//CWnd object regardless of the position of the cursor.
	SetCapture();

	CDialog::OnLButtonDown(nFlags, point);
}

void CTimerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	//point Specifies the x- and y-coordinate of the cursor. 
	if(m_bDragging)
	{
		//Copies the dimensions of the bounding rectangle of the 
		//CWnd object to the structure pointed to by lpRect
		GetWindowRect(&WndRect);
		
		WndRect.left = WndRect.left + (point.x - MousePoint.x);
		WndRect.top  = WndRect.top  + (point.y - MousePoint.y);
		
		//установить новые размеры окна
		SetWindowPos(this, WndRect.left, WndRect.top, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
		//TRACE("OnMouseMove()->SetWindowPos()\n");
	}
	CDialog::OnMouseMove(nFlags, point);
}

//====================== обрабатываем нажатия на кнопки =============
void CTimerDlg::OnButtonClose(void)
{
	OnCancel();
}

void CTimerDlg::OnOK()
{
	//предотвращаем закрытие окна при нажатии кнопки Энтер
	//CDialog::OnОК();
}

void CTimerDlg::OnCancel()
{
	if( m_dlgAdjust.m_structAdjust.bSaveMainWndPosition )
	{
		RegWriteWindowPosition();
	}

	KillTimer(ID_TIMER_ACTION);
	KillTimer(ID_TIMER_SECOND);

	GraduallyShowHideWindow(false);

	TrayIconDel();
	
	CDialog::OnCancel();
}

void CTimerDlg::OnButtonMinimize() 
{
	if (m_dlgAdjust.m_structAdjust.bMinimiseToSystemArea)
	{
		MinimizeToSystemTray();
	}
	else
	{
		ShowWindow(SW_MINIMIZE);
	}
}

void CTimerDlg::OnButtonStartStop(void)
{
	if ( !m_bSrartStop )
	{
		//проверять время выключения, оно не должно равно 00:00:00 если режим работы OM_TIMER
		if (m_iMode == ::OM_TIMER)
		{
			if ((m_csEstablishedTime.Compare( "00:00:00" ) == 0))
			{
				//показать сообщение об ошибке
				MessageBox("Не установлено время выполнения задачи.", "Error", MB_ICONERROR);
				return;
			}
		}

		CreateTimers();
		
		m_bSrartStop = true;
		//запретить педаль Settings
		m_buttonSettings.EnableWindow(false);
		
		//если установлено "сворачивать после запуска"
		if ( m_dlgAdjust.m_structAdjust.bMinimiseWhenStart)
		{
			if (m_dlgAdjust.m_structAdjust.bMinimiseToSystemArea)
			{
				CWinThread* pThread = 0;
				//запускаем процес сворачивания отдельным процессом, чтобы показать на главном окне 
				//установленное время срабатывания
				pThread = AfxBeginThread ((AFX_THREADPROC)&THREADPROCMinimizeToSystemTray, this, THREAD_PRIORITY_LOWEST);
			}
			else
			{
				Sleep(1000);
				ShowWindow(SW_MINIMIZE);
			}
		}
	}
	else
	{
		m_buttonSettings.EnableWindow(true);

		KillTimer(ID_TIMER_ACTION);
		KillTimer(ID_TIMER_SECOND);

		m_bSrartStop = false;
	}
}

UINT THREADPROCMinimizeToSystemTray(LPVOID pParam)
{
	CTimerDlg* dlgTimer = (CTimerDlg*)pParam;

	Sleep(1000);

	dlgTimer->MinimizeToSystemTray();

	return true;
}

void CTimerDlg::OnButtonSettings(void)
{	
	CDlgSettings* pDlgSettings = new CDlgSettings();
	//нажали кнопку OK?
	if ( pDlgSettings->DoModal() == IDOK)
	{
		//получить действие
		m_iAction = pDlgSettings->GetToDo();
		//получить режим работы
		m_iMode = pDlgSettings->GetOperetionMode();
		//получить дополнительное действие
		m_iAdditionAction = pDlgSettings->GetToDoAfter();	

		m_hwndWindowToFollow = pDlgSettings->GetWindowHwndToFollow();

		m_csWhatIsToBeDoneWithoutTime = pDlgSettings->GetWhatIsToBeDoneWithoutTime();
		m_csWhatIsToBeDone            = pDlgSettings->GetWhatIsToBeDone();

		if (m_hwndWindowToFollow)
		{
			::GetWindowTextA(m_hwndWindowToFollow, m_csWinToFollowCaption.GetBuffer(MAX_PATH), MAX_PATH);
			m_csWinToFollowCaption.ReleaseBuffer();
		}		
		
		//получить установленное время
		m_csEstablishedTime = pDlgSettings->GetEstablishedTime(m_nHour, m_nMinute, m_nSeconds);

		//показываем выбранное время
		m_stcTime.DisplayTime(m_csEstablishedTime);
		
		if ( m_dlgAdjust.m_structAdjust.bStartCountWhenSettingsWndClose )
		{
			OnButtonStartStop();
		}
	}

	delete pDlgSettings;
}

void CTimerDlg::OnButtonAdjust()
{
	//нажали кнопку OK?
	m_dlgAdjust.DoModal();	
}

// Get dll version number
ULONGLONG CTimerDlg::GetDllVersion(LPCTSTR lpszDllName)
{
	ULONGLONG ullVersion = 0;
	HINSTANCE hinstDll;
	hinstDll = LoadLibrary(lpszDllName);

	if(hinstDll)
	{
		DLLGETVERSIONPROC pDllGetVersion;
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, "DllGetVersion");
		
		if(pDllGetVersion)
		{
			DLLVERSIONINFO dvi;
			HRESULT hr;

			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);
			hr = (*pDllGetVersion)(&dvi);

			if(SUCCEEDED(hr))
				ullVersion = MAKEDLLVERULL(dvi.dwMajorVersion, dvi.dwMinorVersion,0,0);
		}
		FreeLibrary(hinstDll);
	}
	return ullVersion;
}

//====================== трэй ========================
void CTimerDlg::ShowTrayContextMenu(HWND hWnd)
{
	POINT pt;
	GetCursorPos(&pt);
	HMENU hMenu = CreatePopupMenu();

	if( hMenu )
	{
		if( ::IsWindowVisible(hWnd) )
			InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_HIDE, CMultilanguage::sLanguageText[1]); //скрыть
		else
			InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_SHOW, CMultilanguage::sLanguageText[2]);

		InsertMenu(hMenu, -1, MF_SEPARATOR, NULL, NULL);

		InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_EXIT, CMultilanguage::sLanguageText[3]);

		// note:	must set window to the foreground or the
		//			menu won't disappear when it should
		::SetForegroundWindow(hWnd);

		TrackPopupMenu(hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL );
		
		DestroyMenu(hMenu);
	}
}

//обрабатываем сообщения из трэя
LRESULT CTimerDlg::ProcessingTrayMessage(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

   // Handle message here.
   switch ( lParam )
   {
   case WM_LBUTTONDOWN:
	   //если защита паролем установлена
	   TrayMenuShow();
	   break;

   case WM_RBUTTONDOWN:
   case WM_CONTEXTMENU:
	   ShowTrayContextMenu(GetSafeHwnd());
	   break;
   }

   return 0;
}

void CTimerDlg::TrayIconAdd()
{
	//-------------------------------------------------------------
	// Fill the NOTIFYICONDATA structure and call Shell_NotifyIcon	
	
	// zero the structure - note:	Some Windows funtions require this but
	//								I can't be bothered which ones do and
	//								which ones don't.
	ZeroMemory(&niData, sizeof(NOTIFYICONDATA));

	// get Shell32 version number and set the size of the structure
	//		note:	the MSDN documentation about this is a little
	//				dubious and I'm not at all sure if the method
	//				bellow is correct
	ULONGLONG ullVersion = GetDllVersion(_T("Shell32.dll"));

	if(ullVersion >= MAKEDLLVERULL(5, 0,0,0))
		niData.cbSize = sizeof(NOTIFYICONDATA);
	else niData.cbSize = NOTIFYICONDATA_V2_SIZE;

	// the ID number can be anything you choose
	niData.uID = TRAYICONID;

	// state which structure members are valid
	niData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

	// load the icon
	niData.hIcon = (HICON)LoadImage(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME),
		IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);

	// the window to send messages to and the message to send
	//		note:	the message value should be in the
	//				range of WM_APP through 0xBFFF
	niData.hWnd = GetSafeHwnd();
	niData.uCallbackMessage = WM_TRAYMSG;

	// tooltip message
	//если таймер запущен, и есть иконка в трэе
	if ( m_bSrartStop )
	{
		lstrcpyn(niData.szTip, m_csWhatIsToBeDone, sizeof(niData.szTip)/sizeof(TCHAR));	
	}
	else //если НЕ запущен, но свернут в трэй
	{
		lstrcpyn(niData.szTip, _T("Timer\nУправление питанием компьютера"), sizeof(niData.szTip)/sizeof(TCHAR));
	}
	
	//Sends a message to the taskbar's status area
	Shell_NotifyIcon(NIM_ADD, &niData);

	// free icon handle
	if(niData.hIcon && DestroyIcon(niData.hIcon))
		niData.hIcon = NULL;
}

void CTimerDlg::TrayIconDel()
{
	Shell_NotifyIcon(NIM_DELETE, &niData);
}

void CTimerDlg::TrayIconModify()
{
		//-------------------------------------------------------------
	// Fill the NOTIFYICONDATA structure and call Shell_NotifyIcon	
	
	// zero the structure - note:	Some Windows funtions require this but
	//								I can't be bothered which ones do and
	//								which ones don't.
	ZeroMemory(&niData, sizeof(NOTIFYICONDATA));

	// get Shell32 version number and set the size of the structure
	//		note:	the MSDN documentation about this is a little
	//				dubious and I'm not at all sure if the method
	//				bellow is correct
	ULONGLONG ullVersion = GetDllVersion(_T("Shell32.dll"));

	if(ullVersion >= MAKEDLLVERULL(5, 0,0,0))
		niData.cbSize = sizeof(NOTIFYICONDATA);
	else niData.cbSize = NOTIFYICONDATA_V2_SIZE;

	// the ID number can be anything you choose
	niData.uID = TRAYICONID;

	// state which structure members are valid
	niData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

	// load the icon
	//niData.hIcon = (HICON)LoadImage(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	if (m_bIconToShow)
	{
		niData.hIcon = (HICON)LoadImage(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR );		
		m_bIconToShow = false;
	}
	else
	{
		niData.hIcon = (HICON)LoadImage(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_ICON_RUN), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR );
		m_bIconToShow = true;
	}

	// the window to send messages to and the message to send
	//		note:	the message value should be in the
	//				range of WM_APP through 0xBFFF
	niData.hWnd = GetSafeHwnd();
	niData.uCallbackMessage = WM_TRAYMSG;

	// tooltip message
	//если режим работы таймер, 
	if ( m_iMode == ::OM_TIMER )
	{
		//то показываем сколько времени осталось до выключения
		m_csWhatIsToBeDone = m_csWhatIsToBeDoneWithoutTime + m_csEstablishedTime;		
	}

	lstrcpyn(niData.szTip, m_csWhatIsToBeDone, sizeof(niData.szTip)/sizeof(TCHAR));
	
	//Sends a message to the taskbar's status area
	Shell_NotifyIcon(NIM_MODIFY, &niData);

	// free icon handle
	if(niData.hIcon && DestroyIcon(niData.hIcon))
		niData.hIcon = NULL;
}

//сформировать всплывающую подсказку
void CTimerDlg::MakeTrayToolTip()
{	
	CString csAction;

	//что сделать
	switch ( m_iAction )
	{
	case TODO_SHUTDOWN:
		csAction = "Выключить";
		break;

	case TODO_REBOOT:
		csAction = "Перезагрузить";
		break;

	case TODO_LOCK:
		csAction = "Заблокировать";
		break;

	case TODO_ALARM:
		csAction = "Подать сигнал";
		break;
	}
	//-----------------------------------------------
	//режим работы
	switch ( m_iMode )
	{
	case ::OM_TIMER:
		m_csWhatIsToBeDone.Format("%s через %s", csAction, m_csEstablishedTime);
		//m_csWhatIsToBeDoneWithoutTime нужна для формирования всплывающе подсказки
		//со временем оставшимся до срабатывания
		m_csWhatIsToBeDoneWithoutTime.Format("%s через ", csAction);
		break;

	case ::OM_ALARMCLOCK:

		m_csWhatIsToBeDone.Format("%s в %s", csAction, m_csEstablishedTime);

		if (m_iAdditionAction == TODOAFTER_CLOSEWINDOW)
		{
			m_csWhatIsToBeDone.Format("%s после закрытия выбраного окна", csAction);
		}
		else if(m_iAdditionAction == TODOAFTER_ENDPROCESS)
		{
			m_csWhatIsToBeDone.Format("%s после завершения выбраного процесса", csAction);			
		}
		
		break;
	}	
}


//===================== меню ===========================
void CTimerDlg::TrayMenuShow()
{
	//если защита паролем установлена
	if (IsPasswordProtect())
	{
		//проверяем провильность вводимого пароля
		if (IsPasswordCorrect())
		{
			//то показываем окно
			ShowWindow(SW_RESTORE);
			TrayIconDel();
			//показываем тень
			::ShowWindow(m_Shadow.GetHWND(), SW_RESTORE);
		}	 
	}
	else
	{
		//то показываем окно
		ShowWindow(SW_RESTORE);
		TrayIconDel();
		//показываем тень
		::ShowWindow(m_Shadow.GetHWND(), SW_RESTORE);
	}		   
}

void CTimerDlg::TrayMenuHide()
{
	ShowWindow(SW_HIDE);
}

void CTimerDlg::TrayMenuExit()
{
	//если защита паролем установлена
	if (IsPasswordProtect())
	{
		//проверяем провильность вводимого пароля
		if (IsPasswordCorrect())
		{
			OnCancel();
		}	 
	}
	else
	{
		OnCancel();
	}
}


//====================== таймер ====================================
void CTimerDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch ( nIDEvent )
	{
	case ID_TIMER_ACTION:
		KillTimer(ID_TIMER_ACTION);
		KillTimer(ID_TIMER_SECOND);
		
		m_stcTime.DisplayTime("00:00:00");
		//разрешаем кнопку Settings
		m_buttonSettings.EnableWindow(true);
		//выполняем указанное действие
		DoAction();
		break;

	case ID_TIMER_SECOND:
		//показываем ход времени
		ShowChengingTime();

		if (!IsWindowVisible())
			TrayIconModify();

		//если было выбрано какоето действие после закрытия определенного окна
		switch( m_iAdditionAction )
		{
		case ::TODOAFTER_CLOSEWINDOW:
			{
				//если окна с таким заголовком нет
				if (!TestWindow(m_hwndWindowToFollow))
				{
					//то убиваем таймер и выполняем действие
					KillTimer(ID_TIMER_SECOND);
					m_stcTime.DisplayTime("00:00:00");
					//разрешаем кнопку Settings
					m_buttonSettings.EnableWindow(true);					
					
					DoAction();
				}
			}
			break;

		case ::TODOAFTER_ENDPROCESS:
			break;
		}

		break;
	}

	CDialog::OnTimer(nIDEvent);
}

//создаем таймер с указанным временем срабатывания
bool CTimerDlg::CreateTimers()
{
	switch ( m_iMode )
	{
		//таймер
	case OM_TIMER:
		CreateTimerIfTimerMode();
		break;

		//будильник
	case OM_ALARMCLOCK:
		//создаем таймер только если не выбрано дополнительное действие
		if (m_iAdditionAction == ::TODOAFTER_NOTSET)
		{
			CreateTimerIfAlarmClockMode();
		}
		break;	
	}

	//for count seconds
	SetTimer( ID_TIMER_SECOND, 1000, NULL);
	
return false;
}

bool CTimerDlg::CreateTimerIfTimerMode()
{
	//преобразуем установленное время в миллисекунды
	int EstablishedTimeMSec = (m_nHour * 3600000) + (m_nMinute * 60000) + (m_nSeconds * 1000);

	//3)create timers by using the SetTimer function for shutdown	
	if ( !SetTimer( ID_TIMER_ACTION, EstablishedTimeMSec, NULL))
	{
		MessageBox("Error create timer IDT_TIMER_SHUTDOWN (StartIfTimer())");
		return false;
	}

return true;
}

bool CTimerDlg::CreateTimerIfAlarmClockMode()
{
	int ShutDown_Time = 0; //время выключения
	int Set_Time      = 0; //установленное время
	int System_Time   = 0; //системное время

	SYSTEMTIME st;
	//4)получить системное время
	GetLocalTime(&st);

	//время выключения хранится в strTime. 
	//3)преоразовать в миллисекунды 
	Set_Time = (m_nHour * 3600000)+(m_nMinute * 60000)+(m_nSeconds * 1000);		

	//конверт в миллисекунды
	System_Time = (st.wHour*3600000)+(st.wMinute*60000)+(st.wSecond*1000);

	//5)что больше устанвленное время или или системное
	if (Set_Time > System_Time)
	{
		ShutDown_Time = Set_Time - System_Time;
	}
	else
	{
		//время до полуночи + время выключения
		ShutDown_Time = (86400000-System_Time)+Set_Time;
	}

	//6)create timers by using the SetTimer function
	//for shutdown
	if ( !SetTimer( ID_TIMER_ACTION, ShutDown_Time, NULL))
	{
		MessageBox("Error create timer IDT_TIMER_SHUTDOWN");
		return false;
	}

return true;
}

//====================== размер и местоположение окна ==============
void CTimerDlg::GraduallyShowHideWindow(bool bShowHide) 
{
	//если true то показываем окно
	if (bShowHide)
	{
		//--- рисуем тень ---
		m_Shadow.Create(GetSafeHwnd());	
		m_Shadow.SetSize(-5);
		m_Shadow.SetPosition(15, 15);

		CTimerDlg::m_staticHwndWndShadow = m_Shadow.GetHWND();
				
		for(int c = 0; c <= 100; c += 5)
		{
			SetLayeredWindowAttributes( 0, (255 * c) / 100, LWA_ALPHA);
			m_Shadow.SetDarkness(c);
			
			::Sleep(LayeredWindowShowHideSpeed);
			UpdateWindow();
		}	
	}
	//если fase - скрываем
	else
	{
		for(int c = 100; c >= 0; c -= 5)
		{
			SetLayeredWindowAttributes( 0, (255 * c) / 100, LWA_ALPHA);
			
			m_Shadow.SetDarkness(c);

			::Sleep(LayeredWindowShowHideSpeed);
			UpdateWindow();	
		}	
	}
}

void CTimerDlg::MinimizeToSystemTray()
{
	MakeTrayToolTip();

	ShowWindow(SW_HIDE);

	//прячем окно с тенью
	::ShowWindow(m_Shadow.GetHWND(), SW_HIDE);
	TrayIconAdd();
}

void CTimerDlg::CalcWindowSize()
{
	// load IDB_MAINWND from our resources
	CBitmap bmp;
	if (bmp.LoadBitmap(IDB_MAINWND))
	{
		// Get the info of the bitmap
		BITMAP bmpInfo;
		bmp.GetBitmap(&bmpInfo);

		CRect winRect(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight);
		MoveWindow(winRect);
	}
}

//получаем координаты окна из реестра
void CTimerDlg::RegReadWindowPosition()
{
	/////////////// выделяем имя программы //////////////////
	char szFileName[MAX_PATH] = {'0'}; //название конфигурационного файла
	char* ptr; //нужен чтобы получить название экзешника

	//полный путь к исполняемому файлу
	GetModuleFileNameA( NULL, szFileName, MAX_PATH );
	//убираем расширение .exe
	szFileName[strlen(szFileName)-4] = 0;

	//выделить название файла
	ptr = strrchr(szFileName, '\\') + 1; //+1 чтобы отбросить символ \, инача будет \filename.exe

	CString csSubKey;
	csSubKey.Format("SOFTWARE\\%s", ptr);
	///////////////////////////////////////////////////////////

	HKEY hk = 0;	

	CString pszValue = "top";
	
	DWORD Top = 0, Left = 0;
	DWORD ValueSize = sizeof(DWORD);
	DWORD pdwType	= REG_DWORD;
	
	//---------------------------------------------------
	//читаем из реестра верхнее положение окна
	if(::SHGetValue( HKEY_CURRENT_USER, csSubKey, pszValue, &pdwType, &Top, &ValueSize) != ERROR_SUCCESS)
	{
		TRACE("Облом с получение координат окна\n");
	}
	//---------------------------------------------------
	//получаем из реестра левое положение окна
	pszValue = "left";
	ValueSize = sizeof(DWORD);

	if(::SHGetValue( HKEY_CURRENT_USER, csSubKey, pszValue, &pdwType, &Left, &ValueSize) != ERROR_SUCCESS)
	{
		TRACE("Облом с получение координат окна\n");
	}

	//----------------------------------------------------
	//если не удалось получить координаты или установлено "m_bKeepPosition"
	//то размещаем окно по средине экрана
	if (!Top || !Left || !m_dlgAdjust.m_structAdjust.bSaveMainWndPosition)
	{
		RECT rcDlg, rcDesktop;

		CWnd *wndDesktop = GetDesktopWindow(); 

		wndDesktop->GetWindowRect( &rcDesktop ); 
		GetWindowRect( &rcDlg ); 

		//смещаем координаты Desktop на ширину и высоту нашего окна
		//теперь наше окно будет точно по середине
		OffsetRect(&rcDesktop,  // rectangle
			-rcDlg.right,		// horizontal offset
			-rcDlg.bottom);		// vertical offset

		SetWindowPos(&CWnd::wndTop, rcDesktop.right / 2, rcDesktop.bottom / 2, 0, 0, SWP_NOSIZE); 
	}
	else
	{
		//размещаем окно там где было закрыто
		SetWindowPos(&CWnd::wndTop, Left, Top, 0, 0, SWP_NOSIZE); 
	}
}
//записываем координаты окна в реестр
void CTimerDlg::RegWriteWindowPosition()
{
	/////////////// выделяем имя программы //////////////////
	char szFileName[MAX_PATH] = {'0'}; //название конфигурационного файла
	char* ptr; //нужен чтобы получить название экзешника

	//полный путь к исполняемому файлу
	GetModuleFileNameA( NULL, szFileName, MAX_PATH );
	//убираем расширение .exe
	szFileName[strlen(szFileName)-4] = 0;

	//выделить название файла
	ptr = strrchr(szFileName, '\\') + 1; //+1 чтобы отбросить символ \, инача будет \filename.exe

	CString csSubKey;
	csSubKey.Format("SOFTWARE\\%s", ptr);
	///////////////////////////////////////////////////////////

	 WINDOWPLACEMENT lpwndpl;
	 ::ZeroMemory(&lpwndpl, sizeof(WINDOWPLACEMENT));

	::GetWindowPlacement(GetSafeHwnd(), &lpwndpl);

	//--- записать координаты окна в реестр ---
	HKEY hk; 
	DWORD dwDisp; 
	//char szBuf[MAX_PATH]; 
	//size_t cchSize = MAX_PATH;
	
	char *ValueName = "top";
	
	//StringCchPrintf(szBuf, cchSize, "SOFTWARE\\%s", ProgramName); 

	if (RegCreateKeyEx(HKEY_CURRENT_USER, csSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hk, &dwDisp)) 
	{
		TRACE("Could not create the registry key.\n"); 
		return;
	}

	// --- сохраняем координаты Top ---
	if (RegSetValueEx(hk,             // subkey handle 
		ValueName,				  // value name 
		0,                         // must be zero 
		REG_DWORD,             // value type 
		(LPBYTE) &lpwndpl.rcNormalPosition.top,          // pointer to value data 
		(DWORD) sizeof (DWORD))) // data size
	{
		TRACE("Could not set reg value Top.\n"); 
	}

	//--- сохраняем координаты Left ---
	ValueName = "left";

	if (RegSetValueEx(hk,             // subkey handle 
		ValueName,				  // value name 
		0,                         // must be zero 
		REG_DWORD,             // value type 
		(LPBYTE) &lpwndpl.rcNormalPosition.left,          // pointer to value data 
		(DWORD) sizeof (DWORD))) // data size
	{
		TRACE("Could not set reg value Left.\n"); 
	}

	RegCloseKey(hk);

	TRACE("CTimerDlg::RegWriteWindowPosition(); Top = %d, Left = %d\n", lpwndpl.rcNormalPosition.top, lpwndpl.rcNormalPosition.left);
}

//окно которое не тащит за собою кусок фона
void CTimerDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanged(lpwndpos);

	lpwndpos->flags |= SWP_NOCOPYBITS;
}

//прилипаем к краям
void CTimerDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanging(lpwndpos);	

	// ==================== прилипаем к араям ========================
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wndWorkArea, 0);
	GetWindowRect(wndRect);
	//уменьшаем рабочую область чтобы окно прилипало правильно
	//так как для размещения окна используем координаты левого верхнего угла
	wndWorkArea.right = wndWorkArea.right - wndRect.Width();
	wndWorkArea.bottom = wndWorkArea.bottom - wndRect.Height();

	//abs Calculate the absolute value.
	// если abs принимает отрицательное значение, то возвращает положительное
	if (abs(wndWorkArea.left - lpwndpos->x) <= StickAt)
	{
		lpwndpos->x = wndWorkArea.left;
	}

	if (abs(wndWorkArea.right - lpwndpos->x) <= StickAt)
	{
		lpwndpos->x = wndWorkArea.right;
	}

	if (abs(wndWorkArea.top - lpwndpos->y) <= StickAt)
	{
		lpwndpos->y = wndWorkArea.top;
	}

	if (abs(wndWorkArea.bottom - lpwndpos->y) <= StickAt)
	{
		lpwndpos->y = wndWorkArea.bottom;
	}
}

//====================== действия ===================================================
void CTimerDlg::DoAction()
{	
	m_bSrartStop = false;	
	
	if ( !IsWindowVisible() )
	{
		ShowWindow(SW_RESTORE);
		TrayIconDel();
		//показываем тень
		::ShowWindow(m_Shadow.GetHWND(), SW_RESTORE);
	}

	switch( m_iAction )
	{
	//Shotdown
	case TODO_SHUTDOWN:
		ShutDown();
		break;
	
	//Reboot
	case TODO_REBOOT:
		ReBoot();
		break;
	
	//lock
	case TODO_LOCK:
		LockWorkStation();
		break;

	case TODO_ALARM:
		PlayAlarmSignal();
		break;
	}


	//если есть установки по умолчания
	CDefaulSettings obDefaulSet;

	if (obDefaulSet.IsDefaultSettings())
	{
		//m_nHour, m_nMinute, m_nSeconds используются в CreateTimers - перечитываем настройки
		obDefaulSet.ReadSettings(m_nHour, m_nMinute, m_nSeconds, m_iMode, m_iAction, m_iAdditionAction);

		//-----форматируем значение часов-----
		CString csBuffer;

		csBuffer.Format("%d:", m_nHour);

		if (m_nHour < 10)
			csBuffer.Format("0%d:", m_nHour);

		m_csEstablishedTime = csBuffer;

		//-----форматируем значение минут-----
		csBuffer.Format("%d:", m_nMinute);

		if (m_nMinute < 10)
			csBuffer.Format("0%d:", m_nMinute);

		m_csEstablishedTime += csBuffer;

		//-----подготавливаем значение секунд-----
		csBuffer.Format("%d", m_nSeconds);

		if (m_nSeconds < 10)
			csBuffer.Format("0%d", m_nSeconds);

		m_csEstablishedTime += csBuffer;

		//то возвращаем установленное время на главное окно
		m_stcTime.DisplayTime(m_csEstablishedTime);
	}
	else
	{
		m_csEstablishedTime = "00:00:00";
		m_iMode = OM_TIMER;
		m_iAction = TODO_SHUTDOWN;
		m_iAdditionAction = TODOAFTER_NOTSET;
	}

}

void CTimerDlg::PlayAlarmSignal()
{	
	char szSubKey[MAX_PATH]; 
	
	sprintf(szSubKey, "SOFTWARE\\%s", m_csProgrammName);

	char chData[MAX_PATH];
	DWORD pdwType	= REG_SZ;
	DWORD pcbData	= MAX_PATH;
	
	SHGetValue(HKEY_CURRENT_USER, szSubKey, AlarmSignalPathName, &pdwType, chData, &pcbData );

	//-----------------------------------------------	
	//инициализация бииблиотеки воспроизведения звука
	FSOUND_Init(44100, 32, 0);

	FSOUND_STREAM *SongStream = FSOUND_Stream_Open(chData, FSOUND_2D, 0, 0);

	if ( !SongStream )
	{
		//в сообщении показывать какой файл не удалось проиграть
		MessageBox("Error open stream (FSOUND_Stream_Open().");
		return;
	}

	FSOUND_Stream_Play(0, SongStream);
}

//показываем изменение времени
void CTimerDlg::ShowChengingTime()
{
	switch ( m_iMode )
	{
	//режим таймера
	case ::OM_TIMER:
		//обратный отсчет
		ChengingTimeIfTimerMode();
		break;

	//режим будильника
	case ::OM_ALARMCLOCK: 
		//показываем текущее время
		ChengingTimeIfAlarmClockMode();
		break;
	}
}

void CTimerDlg::ChengingTimeIfTimerMode()
{
	//показываем обратный отсчет времени
	CString Hour, Minute, Seconds, csTimeNow;
	
	//нучно сформировать стороку m_csEstablishedTime с новым временем

	//1)уменьшить секунды на 1	
	m_nSeconds -= 1;

	//----- расчитываем значения часов, минут, секунд -----
	if (m_nSeconds == -1)//cek == 00 
	{
		m_nSeconds = 59; //следующие значение секунд

		m_nMinute -= 1; //уменьшаем на один минуты

		//если установлены часы (01:00:00)
		if (m_nHour != 0 && m_nMinute == -1)
		{
			m_nHour -= 1;
			m_nMinute = 59;
		}		
	}

	//----- обновляем часы ------
	Hour.Format("%d", m_nHour);

	//проверяем формат веремени, если Hour = "0"
	if (Hour.GetLength() == 1)
	{
		//то добавим в начало "0"
		Hour.Insert(0, "0");
	}

	//------ обновляем минуты -----
	Minute.Format("%d", m_nMinute);

	if (Minute.GetLength() == 1)
	{
		//то добавим в начало "0"
		Minute.Insert(0, "0");
	}

	//----- обновляем секунды -----
	Seconds.Format("%d", m_nSeconds);

	if (Seconds.GetLength() == 1)
	{
		//то добавим в начало "0"
		Seconds.Insert(0, "0");
	}	

	csTimeNow = Hour + ":" + Minute + ":" + Seconds;

	m_stcTime.DisplayTime(csTimeNow);
}

void CTimerDlg::ChengingTimeIfAlarmClockMode()
{
	//показываем текущее время
	m_stcTime.DisplayTime();
}


//====================== командная строка =========================
bool CTimerDlg::CommandLine()
{
	bool bResult = false;

	//для отладки командной строки
//#define COMMANDLINE_DEBUG

#ifdef COMMANDLINE_DEBUG
	wchar_t* szArglist[MAX_PATH] = {L"Timer.exe"
		, L"/h", L"20"	//часы
		, L"/m", L"43"	//минуты
		//, L"/m", L"00"	//минуты
		//, L"/m", L"yu"	//минуты
		//, L"/s", L"00"	//секунды
		, L"/a"			//режим работы будильник
		, L"/g"			//подать сигналл
		//, L"/r"			//перезагрузить
		//, L"/l"			//заблокировать
		//, L"/w"			//после закрытия окна
		//, L"/p"			//после завершения процесса
		//, L"/?"		//нелп
	};		
	int nArgs = 7; //число параметров
	#else
		wchar_t** szArglist; //указатель на массив указателей
		int nArgs;	

		(LPWSTR*)szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
#endif

	if( !szArglist )
	{
		AfxMessageBox("CommandLineToArgvW() failed.");
	}
	else if (nArgs > 1) //значит есть хотябы одна команда
	{
		char buffer[MAX_PATH] = {0};
		char szPath[MAX_PATH] = {0};

		CString H("00"), M("00"), S("00");

		//проходимся по командам
		for(int i = 1; i < nArgs; i++)
		{
			bResult = false;

			sprintf_s( buffer, sizeof(buffer), "%ws", szArglist[i] );
			_strlwr_s( buffer, sizeof(buffer) );

			//проверяем чтобы параметр командной строки был таким "/а"
			//а не просто "а" или "ааа"
			if ( strnlen_s( buffer, sizeof(buffer)) == 2)
			{
				TRACE(" %s %ws", buffer, szArglist[i+1]);
				//часы
				if( strcmp("/h", buffer) == 0 )
				{
					i++;
					m_nHour = _wtoi( szArglist[i] );
					H = szArglist[i];

					if ( H.GetLength() == 1 )
					{
						H.Insert(0, "0");
					}
					//если m_nHour равно нулю и Н не равно "00" заначит - ошибка
					//в m_nHour и Н должны быть одинаковые значения
					if (!m_nHour && H.Compare("00")!= 0)
					{
						ShowCommandLineHelp(true);
					}

					bResult = true;
					goto End;
				}
				//минуты
				if( strcmp("/m", buffer) == 0 )
				{
					i++;
					//если ошибка то _wtoi() возвращает 0
					m_nMinute = _wtoi( szArglist[i] );
					M = szArglist[i];

					if ( M.GetLength() == 1 )
					{
						M.Insert(0, "0");
					}
					//m_nMinute и M может равно 0 (20:00:00)
					if (!m_nMinute && M.Compare("00")!= 0)
					{
						ShowCommandLineHelp(true);
					}					

					bResult = true;
					goto End;
				}
				//секунды
				if( strcmp("/s", buffer) == 0 )
				{
					i++;
					m_nSeconds = _wtoi( szArglist[i] );
					S = szArglist[i];			

					if ( S.GetLength() == 1 )
					{
						S.Insert(0, "0");
					}
					
					if (!m_nSeconds && S.Compare("00")!= 0)
					{
						ShowCommandLineHelp(true);
					}

					bResult = true;
					goto End;
				}
				//режим работы будильник
				if( strcmp("/a", buffer) == 0 )
				{
					m_iMode = OM_ALARMCLOCK; //режим работы будильник
					bResult = true;
					goto End;
				}
				//подать сигнал
				if( strcmp("/g", buffer) == 0 )
				{				
					m_iAction = TODO_ALARM;
					bResult = true;
					goto End;
				}
				//перезагрузить
				if( strcmp("/r", buffer) == 0 )
				{
					m_iAction = TODO_REBOOT;
					bResult = true;
					goto End;
				}
				//заблокировать
				if( strcmp("/l", buffer) == 0 )
				{
					m_iAction = TODO_LOCK;
					bResult = true;
					goto End;
				}
				//выполнить после закрытия окна
				if( strcmp("/w", buffer) == 0 )
				{
					//показать диалог выбора окна
					CDlgWindowToFollow* pWindowToFollowDlg = new CDlgWindowToFollow();
					if ( pWindowToFollowDlg->DoModal() == IDOK)
					{
						m_iAdditionAction = TODOAFTER_CLOSEWINDOW;
						m_iMode = OM_ALARMCLOCK;

						//получить описатель нужного нам окна
						m_hwndWindowToFollow = pWindowToFollowDlg->GetWinToFollow();
					}
					else 
					{
						//если в диалоге выбора окна нажали педаль Cancel
						//то завершаем работу
						exit(EXIT_SUCCESS);
					}
					
					delete pWindowToFollowDlg;
					bResult = true;
					goto End;
				}
				//выполнить после завершения процесса
				if( strcmp("/p", buffer) == 0 )
				{
					//показать диалог выбора процесса
					bResult = true;
					goto End;
				}

				if ( strcmp("/?", buffer) == 0 )
				{
					ShowCommandLineHelp();
					bResult = true;
					goto End;
				}

				//если bResult нигде не изменился, зничит командная строка не правильная
				if ( !bResult )
					ShowCommandLineHelp( true );
				
			}//end if
			else 
			{
				//показываем сообщение об ошибке, а зетем хелп по командной строке
				ShowCommandLineHelp( true );
			}
End:; //ФИНИШ		
		} //end for

		m_csEstablishedTime = H + ":" + M + ":" + S;

		//показываем выбранное время
		m_stcTime.DisplayTime(m_csEstablishedTime);		
	}

#ifndef COMMANDLINE_DEBUG
	// Free memory allocated for CommandLineToArgvW arguments.
	LocalFree(szArglist);
#endif

	return bResult;
}

void CTimerDlg::ShowCommandLineHelp(bool bError)
{
	if (bError)
	{
		MessageBox("Не верный параметр командной строки."
			"Timer.exe /? для справки."
			,"Ошибка", MB_ICONERROR);
	}
	else
	{
		MessageBoxA("Timer command line:\n\n"
				" Hours \t\t\t/h\n"
				" Minutes \t\t\t/m\n"
				" Seconds \t\t\t/s\n"
				" Alarm mode \t\t/a\n"
				" To give the signal \t\t/g\n"
				" Reboot \t\t\t/r\n"
				" Lock \t\t\t/l\n"
				" When window close \t/w\n"
				" When end process \t\t/p\n"
				"\nFor example:\n\ttimer /a /g /h 22 /m 45 /s 59",
				"Timer command line", MB_OK|MB_ICONINFORMATION);
	}
	exit(EXIT_SUCCESS);
}

void CTimerDlg::RegWriteAppPath()
{
	//получить путь к программе 
	char szPath[MAX_PATH]; 
	
	GetModuleFileNameA( NULL, szPath, MAX_PATH );
	
	HKEY hKey;		
	LPCSTR lpSubKey = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Timer.exe";

	//и записать его в реестр для работы командной строки
	/*[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\Timer.exe]
	@="D:\\Visual Studio Projects\\Timer\\Release\\Timer.exe"
	"Path"="D:\\Visual Studio Projects\\Timer\\Release"*/
	
	//RegOpenKeyExA(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_READ, &hKey)	

	if (RegCreateKeyExA(HKEY_CURRENT_USER, lpSubKey, 0, NULL,
			REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL)== ERROR_SUCCESS)
	{	
		RegSetValueExA(hKey, "", 0, REG_SZ, (LPBYTE) szPath, strlen(szPath)+1);
		//нужно убрать из строки название программы
		char *pdest;
		int result;
		//найдем последнее размещение символа в строке
		pdest = strrchr( szPath, '\\' );
		result = (int)( pdest - szPath );
		szPath[result] = 0;

		RegSetValueExA(hKey, "Path", 0, REG_SZ, (LPBYTE) szPath, strlen(szPath)+1);
		
		RegCloseKey(hKey);		
	}
	else
	{
		TRACE("CTimerDlg::RegWriteAppPath(); Облом с записью в реестр.\n");
	}
}

bool CTimerDlg::TestWindow(HWND hwndWindowToTest)
{
	//если окна нет
	if (!IsWindow(hwndWindowToTest))
		return false;

	return true;
}

//есть защита паролем?
bool CTimerDlg::IsPasswordProtect()
{
	if (m_dlgAdjust.m_structAdjust.bPasswordProtect)
		return true;

	return false;	
}

//Правильно вводят пароль?
bool CTimerDlg::IsPasswordCorrect()
{
	bool result = false; //пароль неправильный

	//то, показать окно ввода пароля
	CDialog* pdlgGetPass = new CDlgGetPassword();

	INT_PTR nRet = -1;

	nRet = pdlgGetPass->DoModal();
	//если нажали ок и введенный пароль совпадает с установленным
	if ( nRet == IDOK )
	{
		result = true; //пароль правильно вводят
	}
	else
	{
		result = false; //вводят неправильный пароль или нажали кнопку отмена
	}

	delete pdlgGetPass;
	
	return result;
}

void CTimerDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	CAboutDlg* dlgAbout = new CAboutDlg();

	dlgAbout->DoModal();

	delete dlgAbout;

	CDialog::OnRButtonDown(nFlags, point);
}
