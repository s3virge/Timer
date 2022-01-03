// AlarmSignalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Timer.h"
#include "DlgAlarmSignal.h"


bool CDlgAlarmSignal::m_bPlayStop = true;

// CAlarmSignalDlg dialog
IMPLEMENT_DYNAMIC(CDlgAlarmSignal, CDialog)

CDlgAlarmSignal::CDlgAlarmSignal(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarmSignal::IDD, pParent)
	//, m_bPlayStop(true)
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

	m_csProgrammName.Format("%s", ptr);
	///////////////////////////////////////////////////////////
}

CDlgAlarmSignal::~CDlgAlarmSignal()
{
}

void CDlgAlarmSignal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAlarmSignal, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_ADD, &CDlgAlarmSignal::OnBnClickedBrows)
	ON_BN_CLICKED(IDOK, &CDlgAlarmSignal::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_PLAY, &CDlgAlarmSignal::OnBnClickedBtnPlay)
END_MESSAGE_MAP()


// CAlarmSignalDlg message handlers
BOOL CDlgAlarmSignal::OnInitDialog()
{
	CDialog::OnInitDialog();

	//если все удачно
	if (RegGetAlarmSignal(m_csAlarmSignal, m_csAlarmSignalPath))
	{
		//то показываем Название сигнала
		GetDlgItem(IDC_COMBO)->SetWindowText( m_csAlarmSignal );
	}
	else
	{
		GetDlgItem(IDC_COMBO)->SetWindowText( "Выберите сигнал" );
	}

	return TRUE;
}

//=== устанавливаем цвет окна ===
BOOL CDlgAlarmSignal::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect( rect, WindowColor ); //цвет окна
	
	//Nonzero if it erases the background; otherwise 0.
	return true; //предотвращаем мерцание изображиния

	//return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgAlarmSignal::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

//=== перемещаем окно за любую область ===
void CDlgAlarmSignal::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgAlarmSignal::OnBnClickedBrows()
{
	OPENFILENAMEA ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name and path
	char szFileName[260];	//buffer for file name
	//HWND hwnd;              // owner window
	//HANDLE hf = NULL;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetSafeHwnd();
	ofn.lpstrFile = szFile;
	ofn.lpstrFileTitle = szFileName;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All supported types\0*.mp3;*.wav;*.ogg;*.wma;*.aac\0"
		"MPEG Audio Files(*.mp3,*.aac)\0*.mp3;*.aac\0"
		"Windows Media Audio Files(*.wma)\0*.wma\0"
		"Windows waveform(*.wav)\0*.wav\0"
		"Ogg files(*.ogg)\0*.ogg\0";
	ofn.nFilterIndex = 1;
	//ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = sizeof(szFileName);
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	//ofn.FlagsEx = OFN_EX_NOPLACESBAR;
	
	// Display the Open dialog box.
	if ( GetOpenFileNameA(&ofn) ) 
	{
		//GetDlgItem(IDC_COMBO)->SendMessageA( CB_INSERTSTRING, -1, (LPARAM) ofn.lpstrFile );
		GetDlgItem(IDC_COMBO)->SendMessageA( CB_INSERTSTRING, -1, (LPARAM) ofn.lpstrFileTitle );
		//показать название мелодии в комбобокс
		//SetDlgItemTextA( IDC_COMBO, ofn.lpstrFile );
		m_csAlarmSignal = ofn.lpstrFileTitle;
		m_csAlarmSignalPath = ofn.lpstrFile;
		
		SetDlgItemTextA( IDC_COMBO, m_csAlarmSignal );		
		
		RegSaveAlarmSignal( m_csAlarmSignal, m_csAlarmSignalPath );
	}
}

//получение установленой мелодии будильника
bool CDlgAlarmSignal::RegGetAlarmSignal(CString &csAlarmSignal, CString &csAlarmSignalPath)
{
	//сохранять в реестре путь к файлу и имя файла
	char szSubKey[MAX_PATH]; 
	size_t SubKeySize = MAX_PATH;

	sprintf(szSubKey, "SOFTWARE\\%s", m_csProgrammName);

	char chData[MAX_PATH] = {"0"};
	DWORD pdwType	= REG_SZ;
	DWORD pcbData	= MAX_PATH;
	//если все путем, то продолжаем
	if (SHGetValue(HKEY_CURRENT_USER, szSubKey, AlarmSignalName, &pdwType, chData, &pcbData ) == ERROR_SUCCESS)
	{
		csAlarmSignal = chData;

		pdwType	= REG_SZ;
		pcbData	= MAX_PATH;

		if (SHGetValue(HKEY_CURRENT_USER, szSubKey, AlarmSignalPathName, &pdwType, chData, &pcbData) == ERROR_SUCCESS)
		{
			csAlarmSignalPath = chData;
			//тут нужно проверять существует ли файл
		}
		else
		{
			//возвращаем "облом"
			return false;
		}

		return true;
	}

	//возвращаем "облом"
	return false;
}

//сохраним в реестре название мелодии будильника
void CDlgAlarmSignal::RegSaveAlarmSignal(CString csAlarmSignal, CString csAlarmSignalPath)
{
	//сохранять в реестре путь к файлу и имя файла
	char szSubKey[MAX_PATH]; 
	size_t SubKeySize = MAX_PATH;
	
	sprintf_s(szSubKey, SubKeySize, "SOFTWARE\\%s", m_csProgrammName); 

	SHSetValue(HKEY_CURRENT_USER, szSubKey, AlarmSignalName, REG_SZ, csAlarmSignal.GetString(), csAlarmSignal.GetLength() );
	SHSetValue(HKEY_CURRENT_USER, szSubKey, AlarmSignalPathName, REG_SZ, csAlarmSignalPath.GetString(), csAlarmSignalPath.GetLength() );
}
//сохранить список сигналов
void CDlgAlarmSignal::RegSaveAlarmSignalList()
{

}

void CDlgAlarmSignal::OnBnClickedOk()
{
	if ( !m_bPlayStop )
	{
		FSOUND_Close();
	}

	OnOK();
}

void CDlgAlarmSignal::OnCancel()
{
	if ( !m_bPlayStop )
	{
		FSOUND_Close();
	}

	CDialog::OnCancel();
}

signed char F_CALLBACKAPI Endcallback(FSOUND_STREAM *stream, void *buff, int len, void *param)
{
	// end of stream callback doesnt have a 'buff' value, if it doesnt it could be a synch point.
	if ( !buff )
	{
		SetWindowTextA( GetDlgItem((HWND)param, IDC_BTN_PLAY), "P l a y");
		CDlgAlarmSignal::m_bPlayStop = true;
	}
return TRUE;
}

void CDlgAlarmSignal::OnBnClickedBtnPlay()
{
	if ( m_bPlayStop )
	{
		//инициализация бииблиотеки воспроизведения звука
		FSOUND_Init(44100, 32, 0);

		//установить новое значение переменной Alarm
		//GetDlgItemTextA(hDlg, IDC_COMBO, m_csAlarmSignalPath, MAX_PATH);
		RegGetAlarmSignal(m_csAlarmSignal, m_csAlarmSignalPath);

		m_bPlayStop = false;

		FSOUND_STREAM *SongStream = FSOUND_Stream_Open(m_csAlarmSignalPath, FSOUND_2D, 0, 0);

		if ( !SongStream )
		{
			MessageBox("Error open stream (FSOUND_Stream_Open().");
			m_bPlayStop = true;
			return;
		}

		FSOUND_Stream_SetEndCallback(SongStream, Endcallback, GetSafeHwnd());

		FSOUND_Stream_Play(0, SongStream);

		GetDlgItem(IDC_BTN_PLAY)->SetWindowText( "S t o p");
	}
	else
	{
		m_bPlayStop = true;
		
		FSOUND_Close();
		
		GetDlgItem(IDC_BTN_PLAY)->SetWindowText("P l a y");
	}
}

CString CDlgAlarmSignal::GetAlarmSignal()
{
	return m_csAlarmSignal;
}