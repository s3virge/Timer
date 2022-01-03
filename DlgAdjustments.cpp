
#include "stdafx.h"
#include "Timer.h"
#include "DlgAdjustments.h"
#include "DlgPasswordProtect.h"
#include "afxwin.h"
//#include "Debug.h"
#include "Autorun.h"
//#include <string>



//название переменной для записи в реестр
const string CDlgAdjustments::StartCloseName		= "StartClose";
const string CDlgAdjustments::KeepPositionName		= "KeepPosition";
const string CDlgAdjustments::MinimiseToSystemArea	= "MinimizeToSystemArea";
const string CDlgAdjustments::MinimiseWhenStart		= "MinimizeWhenStart";
const string CDlgAdjustments::CloseDoubleClickName = "CloseDoubleClick";

// CDlgAdjustments dialog

IMPLEMENT_DYNAMIC(CDlgAdjustments, CDialog)

CDlgAdjustments::CDlgAdjustments(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAdjustments::IDD, pParent)
	, m_ImmediatelyStartTheTask(FALSE)
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
	
	m_csSubKey.Format("SOFTWARE\\%s", ptr);
	///////////////////////////////////////////////////////////	
}

CDlgAdjustments::~CDlgAdjustments()
{
}

void CDlgAdjustments::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CLOSEDOBLECLICK, m_structAdjust.bMainWndDoubleClickClose);
	DDX_Check(pDX, IDC_STARTCLOSE, m_structAdjust.bStartCountWhenSettingsWndClose);
	DDX_Check(pDX, IDC_KEEPPOSITION, m_structAdjust.bSaveMainWndPosition);
	DDX_Check(pDX, IDC_MINIMISETOSYSTEMAREA, m_structAdjust.bMinimiseToSystemArea);
	DDX_Check(pDX, IDC_MINIMISEWHENSTART, m_structAdjust.bMinimiseWhenStart);
	DDX_Check(pDX, IDC_PASSWORD_PROTECTION, m_structAdjust.bPasswordProtect);
	DDX_Control(pDX, IDC_AUTORUN, m_chekAutorun);
	DDX_Check(pDX, IDC_RUNTASK, m_ImmediatelyStartTheTask);
}

BEGIN_MESSAGE_MAP(CDlgAdjustments, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAdjustments::OnBnClickedOk)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_PASSWORD_PROTECTION, &CDlgAdjustments::OnBnClickedPasswordProtection)
	ON_CBN_SELCHANGE(IDC_COMBO_UILANGUAGE, &CDlgAdjustments::OnCbnSelchangeComboUilanguage)
END_MESSAGE_MAP()


// CDlgAdjustments message handlers

BOOL CDlgAdjustments::OnInitDialog()
{
	CDialog::OnInitDialog();

	CAutorun objAutorun;

	if (objAutorun.IsAutorunReg())
		m_chekAutorun.SetCheck(true);
	else
		m_chekAutorun.SetCheck(false);

	ReadAdjustments();
	UpdateData(FALSE);
		
	///////////////////////////////////////////////////////////////////

	/*заполняем комбокос имеющимися языками*/
	m_objMultiLang.FillLanguageComboBox(/*GetSafeHwnd()*/this, IDC_COMBO_UILANGUAGE);
	
	/* установить подписи к контролам на установленном языке */
	SetDlgItemText(IDC_CLOSEDOBLECLICK,		CMultilanguage::sLanguageText[10]);
	SetDlgItemText(IDC_STARTCLOSE,			CMultilanguage::sLanguageText[11]);
	SetDlgItemText(IDC_KEEPPOSITION,		CMultilanguage::sLanguageText[12]);
	SetDlgItemText(IDC_MINIMISETOSYSTEMAREA,CMultilanguage::sLanguageText[13]);
	SetDlgItemText(IDC_PASSWORD_PROTECTION, CMultilanguage::sLanguageText[14]);
	SetDlgItemText(IDC_AUTORUN,				CMultilanguage::sLanguageText[15]);
	SetDlgItemText(IDC_MINIMISEWHENSTART,	CMultilanguage::sLanguageText[16]);
	SetDlgItemText(IDC_LANGUAGE,			CMultilanguage::sLanguageText[17]);
	SetDlgItemText(IDC_RUNTASK,			    CMultilanguage::sLanguageText[56]);
	
	SetWindowText(CMultilanguage::sLanguageText[18]);

	SetDlgItemText(IDCANCEL,				CMultilanguage::sLanguageText[19]);
	
	return TRUE;
}

void CDlgAdjustments::ReadAdjustments()
{	
	HKEY hKey = 0;

	memset(&m_structAdjust, 0, sizeof m_structAdjust);

	//открываем или создаем ключ
	if (::RegOpenKeyEx(HKEY_CURRENT_USER, m_csSubKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{		
		if(::RegQueryValueEx(hKey, CloseDoubleClickName.c_str(), 0, 0, 0, 0) == ERROR_SUCCESS)
			m_structAdjust.bMainWndDoubleClickClose = true;

		if(::RegQueryValueEx(hKey, StartCloseName.c_str(), 0, 0, 0, 0) == ERROR_SUCCESS)
			m_structAdjust.bStartCountWhenSettingsWndClose = true;
		
		if(::RegQueryValueEx(hKey, KeepPositionName.c_str(), 0, 0, 0, 0) == ERROR_SUCCESS)
			m_structAdjust.bSaveMainWndPosition = true;
	
		if(::RegQueryValueEx(hKey, MinimiseToSystemArea.c_str(), 0, 0, 0, 0) == ERROR_SUCCESS)
			m_structAdjust.bMinimiseToSystemArea = true;

		if(::RegQueryValueEx(hKey, MinimiseWhenStart.c_str(), 0, 0, 0, 0) == ERROR_SUCCESS)
			m_structAdjust.bMinimiseWhenStart = true;
		
		if (::RegQueryValueEx(hKey, PasswordProtect, 0, 0, 0, 0) == ERROR_SUCCESS)
			m_structAdjust.bPasswordProtect = true;

		if (::RegQueryValueEx(hKey, "ImmediatelyStartTheTask", 0, 0, 0, 0) == ERROR_SUCCESS)
			m_ImmediatelyStartTheTask = true;

		::RegCloseKey(hKey);		
	}
	else
	{TRACE("CDlgAdjustments::ReadAdjustments(); Получить настройки не удалось.\n");}
}

void CDlgAdjustments::WriteAdjustments()
{	
	HKEY hkey = 0;

	// ============ автозагрузка ============
	CAutorun objAutorun;

	if (m_chekAutorun.GetCheck())
	{
		//objAutorun.RegAdd();
		objAutorun.AddReg();
	} 
	else
	{
		objAutorun.RemoveReg();
	}

	//получить состояние чекбоксов
	UpdateData(TRUE);

	//===  открываем или создаем ключ  ===
	if (RegCreateKeyEx(HKEY_CURRENT_USER, m_csSubKey, 0,	0, REG_OPTION_NON_VOLATILE,	KEY_WRITE, NULL, &hkey,	0) == ERROR_SUCCESS)
	{
		//если флажок установлен
		if (m_structAdjust.bMainWndDoubleClickClose != 0)
			RegSetValueEx(hkey, CloseDoubleClickName.c_str(), 0, REG_BINARY, 0, 0);
		else
			RegDeleteValue(hkey, CloseDoubleClickName.c_str());
		
		if (m_structAdjust.bStartCountWhenSettingsWndClose != 0)
			RegSetValueEx(hkey, StartCloseName.c_str(), 0, REG_BINARY, 0, 0);
		else
			RegDeleteValue(hkey,StartCloseName.c_str());

		if (m_structAdjust.bSaveMainWndPosition != 0)
			RegSetValueEx(hkey, KeepPositionName.c_str(), 0, REG_BINARY, 0, 0);
		else
			RegDeleteValue(hkey, KeepPositionName.c_str());

		if (m_structAdjust.bMinimiseToSystemArea != 0)
			RegSetValueEx(hkey, MinimiseToSystemArea.c_str(), 0, REG_BINARY, 0, 0);
		else
			RegDeleteValue(hkey, MinimiseToSystemArea.c_str());

		if (m_structAdjust.bMinimiseWhenStart != 0)
			RegSetValueEx(hkey, MinimiseWhenStart.c_str(), 0, REG_BINARY, 0, 0);
		else
			RegDeleteValue(hkey, MinimiseWhenStart.c_str());

		if (m_structAdjust.bPasswordProtect != 0)
			SHDeleteValue(HKEY_CURRENT_USER, m_csSubKey, PasswordProtect);

		if (m_ImmediatelyStartTheTask)
			RegSetValueEx(hkey, "ImmediatelyStartTheTask", 0, REG_BINARY, 0, 0);
		else
			RegDeleteValue(hkey, "ImmediatelyStartTheTask");
		
		::RegCloseKey(hkey);
	}
	else
	{TRACE("CDlgAdjustments::WriteAdjustments(); Сохранить настройки не удалось.\n");}
}

void CDlgAdjustments::OnBnClickedOk()
{
	WriteAdjustments();
	
	m_objMultiLang.WriteSettingsFile(this, IDC_COMBO_UILANGUAGE);

	OnOK();
}

//=== устанавливаем цвет окна ===
BOOL CDlgAdjustments::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect( rect, WindowColor ); //цвет окна
	
	//Nonzero if it erases the background; otherwise 0.
	return true; //предотвращаем мерцание изображиния

	//return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgAdjustments::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
	case CTLCOLOR_BTN:
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
void CDlgAdjustments::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgAdjustments::OnBnClickedPasswordProtection()
{	
	//если флажок установлен
	CButton* pbtnPasswordProtect = (CButton*) GetDlgItem(IDC_PASSWORD_PROTECTION);

	if (pbtnPasswordProtect->GetCheck())
	{
		//то показываем диалог управления паролем
		CDlgPasswordProtect *pDlgPasswordProtection = new CDlgPasswordProtect();

		//если в диалоге не была нажата кнопка ОК
		if (pDlgPasswordProtection->DoModal() != IDOK)
		{
			//снимаем флажок
			pbtnPasswordProtect->SetCheck(false);
		}
		
		delete pDlgPasswordProtection;
	}
	else
	{
		//иначе делаем недействительным
		m_structAdjust.bPasswordProtect = false;
	}
}

void CDlgAdjustments::OnCbnSelchangeComboUilanguage()
{
	char cGUILanguage[MAX_PATH]= {'0'};

	m_objMultiLang.GetGUILanguage(cGUILanguage);

	char cCurrentGuiLang[MAX_PATH] = {'0'};
	//получить текст текущего выделенного пункта
	(CComboBox*)GetDlgItem(IDC_COMBO_UILANGUAGE)->GetWindowTextA(cCurrentGuiLang, sizeof (cCurrentGuiLang) );
	
	//сравнить с тем который был раньше
	int result = strcmp(cCurrentGuiLang, cGUILanguage);

	//если отличный от старого языка
	if (result != 0)
	{
		//устанавливаем флаг изменения языка
		m_objMultiLang.SetGUILangChenged(true);

		TRACE("Изменился язык интерфейса c %s на %s\n", cGUILanguage, cCurrentGuiLang);
	}
	else
	{
		//устанавливаем флаг изменения языка
		m_objMultiLang.SetGUILangChenged(false);
		TRACE("Язык интерфейса не изменися\n");
	}
}

bool CDlgAdjustments::IsImmediatelyStartTheTask(void)
{
	HKEY hKey = 0;
	
	//открываем или создаем ключ
	if (::RegOpenKeyEx(HKEY_CURRENT_USER, m_csSubKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		if (::RegQueryValueEx(hKey, "ImmediatelyStartTheTask", 0, 0, 0, 0) == ERROR_SUCCESS)
			return true;
	}
	else
	{
		TRACE("CDlgAdjustments::IsImmediatelyStartTheTask(); Не удалось прочитать ключ реестра.\n");
	}

	return false;
}
