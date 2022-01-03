// DlgGetPassword.cpp: ���� ����������
//

#include "stdafx.h"
#include "Timer.h"
#include "DlgGetPassword.h"


// ���������� ���� CDlgGetPassword

IMPLEMENT_DYNAMIC(CDlgGetPassword, CDialog)

CDlgGetPassword::CDlgGetPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGetPassword::IDD, pParent)
{
	/////////////// �������� ��� ��������� //////////////////
	char szFileName[MAX_PATH] = {'0'}; //�������� ����������������� �����
	char* ptr; //����� ����� �������� �������� ���������

	//������ ���� � ������������ �����
	GetModuleFileNameA( NULL, szFileName, MAX_PATH );
	//������� ���������� .exe
	szFileName[strlen(szFileName)-4] = 0;

	//�������� �������� �����
	ptr = strrchr(szFileName, '\\') + 1; //+1 ����� ��������� ������ \, ����� ����� \filename.exe

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


// ����������� ��������� CDlgGetPassword


BOOL CDlgGetPassword::OnInitDialog()
{
	CDialog::OnInitDialog();

	//�������� ����������� ������ ��� ���������
	RegGetPassword();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����������: �������� ������� OCX ������ ���������� �������� FALSE
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
		MessageBox("������������ ������. ���������� ��� ���.");

		editPassword->SetSel(0, -1);
		editPassword->Clear();

		return;
	}
	
	CDialog::OnOK();
}
