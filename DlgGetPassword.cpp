// DlgGetPassword.cpp: файл реализации
//

#include "stdafx.h"
#include "Timer.h"
#include "DlgGetPassword.h"


// диалоговое окно CDlgGetPassword

IMPLEMENT_DYNAMIC(CDlgGetPassword, CDialog)

CDlgGetPassword::CDlgGetPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGetPassword::IDD, pParent)
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

CDlgGetPassword::~CDlgGetPassword()
{
}

void CDlgGetPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgGetPassword, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgGetPassword::OnBnClickedOk)
END_MESSAGE_MAP()


// обработчики сообщений CDlgGetPassword


BOOL CDlgGetPassword::OnInitDialog()
{
	CDialog::OnInitDialog();

	//получить сохраненный пароль для сравнения
	RegGetPassword();

	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}

void CDlgGetPassword::RegGetPassword()
{
	CString csSubKey = "SOFTWARE\\";
	csSubKey += m_csProgrammName;

	char pvData[MAX_PATH] = {0};
	DWORD pdwType = 0;
	DWORD pDataSize = MAX_PATH;

	SHGetValue(HKEY_CURRENT_USER, csSubKey, PasswordProtect, &pdwType, &pvData, &pDataSize);

	m_csPassword = pvData;
}


void CDlgGetPassword::OnBnClickedOk()
{
	CString csEnteredPasswod;
	
	CEdit* editPassword = (CEdit*) GetDlgItem(IDC_PASSWORD);
	
	editPassword->GetWindowText(csEnteredPasswod);

	if ( csEnteredPasswod != m_csPassword)
	{
		MessageBox("Неправильный пароль. Попробуйте еще раз.");

		editPassword->SetSel(0, -1);
		editPassword->Clear();

		return;
	}
	
	CDialog::OnOK();
}
