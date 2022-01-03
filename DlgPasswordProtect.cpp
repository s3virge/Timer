// PasswordProtectDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "Timer.h"
#include "DlgPasswordProtect.h"
//#include "afxdialogex.h"


// диалоговое окно CPasswordProtectDlg

IMPLEMENT_DYNAMIC(CDlgPasswordProtect, CDialog)

CDlgPasswordProtect::CDlgPasswordProtect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPasswordProtect::IDD, pParent)
{

}

CDlgPasswordProtect::~CDlgPasswordProtect()
{
}

void CDlgPasswordProtect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOW_PASS, m_checkShowPassword);
	DDX_Control(pDX, IDC_NEW_PASSWORD, m_editNewPassword);
	DDX_Control(pDX, IDC_REPET_PASSWORD, m_editRepetPasswprd);
}


BEGIN_MESSAGE_MAP(CDlgPasswordProtect, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_SHOW_PASS, &CDlgPasswordProtect::OnBnClickedShowPass)
	ON_BN_CLICKED(IDOK, &CDlgPasswordProtect::OnBnClickedOk)
END_MESSAGE_MAP()


// обработчики сообщений CPasswordProtectDlg


void CDlgPasswordProtect::OnBnClickedShowPass()
{	
	//если флажок установлен
	if (m_checkShowPassword.GetCheck())
	{
		//то показываем вводимые символы
		m_editNewPassword.SetPasswordChar(0);		 
		m_editRepetPasswprd.SetPasswordChar(0);	
		TRACE("\nСимвол скрывающий пароль: %d (%c)", m_oldPasswordChar, m_oldPasswordChar);	
	}
	else
	{		
		//то показываем вводимые символы
		m_editNewPassword.SetPasswordChar(m_oldPasswordChar);
		m_editRepetPasswprd.SetPasswordChar(m_oldPasswordChar);
		TRACE("\nСимвол скрывающий пароль: %d (%c)", m_oldPasswordChar, m_oldPasswordChar);
	}

	Invalidate(TRUE);
}


BOOL CDlgPasswordProtect::OnInitDialog()
{
	CDialog::OnInitDialog();

	//сохраняем старый символ скрывающий пароль
	//m_oldPasswordChar = m_editNewPassword.GetPasswordChar();
	m_oldPasswordChar = 42;
	TRACE("\nСимвол скрывающий пароль: %d (%c)", m_oldPasswordChar, m_oldPasswordChar);

	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


void CDlgPasswordProtect::OnBnClickedOk()
{
	CString csNewPassword, csRepetPassword;
	//сравниваем введенные пароли
	m_editNewPassword.GetWindowText(csNewPassword);
	m_editRepetPasswprd.GetWindowText(csRepetPassword);

	if (csNewPassword != csRepetPassword)
	{
		MessageBox("Пароли не совпадают, попробуйте еще раз.", "Пароли не совпадают", MB_ICONEXCLAMATION);
		
		m_editNewPassword.SetSel(0, -1);
		m_editNewPassword.Clear();
		m_editNewPassword.SetFocus();
	
		m_editRepetPasswprd.SetSel(0, -1);
		m_editRepetPasswprd.Clear();	
		
		return;
	}

	//пустой пароль - низя
	if (csNewPassword.GetLength()== 0 && csRepetPassword.GetLength()==0)
	{
		MessageBox("Пустой пароль использовать нельзя. Введите пароль", "Пароль не введен.", MB_ICONEXCLAMATION);
		m_editNewPassword.SetFocus();
		
		return;
	}
	
	//сохраняем пароль
	TRACE("\nСохраняем пароль");

	//запишим пароль в реестр
	SavePassword(csNewPassword);
	
	CDialog::OnOK();
}

//сохраняем пароль в реестр
void CDlgPasswordProtect::SavePassword(CString csPassword)
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

	///////////////////////////////////////////////////////////
	//сделать запись в реестр =======================
	CString csSubKey;
	csSubKey.Format("SOFTWARE\\%s", ptr);

	SHSetValue(HKEY_CURRENT_USER, csSubKey, PasswordProtect, REG_SZ, csPassword.GetString(), csPassword.GetLength() );	
}

//=== перемещаем окно за любую область ===
void CDlgPasswordProtect::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CDialog::OnLButtonDown(nFlags, point);
}