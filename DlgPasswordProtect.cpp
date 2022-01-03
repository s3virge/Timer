// PasswordProtectDlg.cpp: ���� ����������
//

#include "stdafx.h"
#include "Timer.h"
#include "DlgPasswordProtect.h"
//#include "afxdialogex.h"


// ���������� ���� CPasswordProtectDlg

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


// ����������� ��������� CPasswordProtectDlg


void CDlgPasswordProtect::OnBnClickedShowPass()
{	
	//���� ������ ����������
	if (m_checkShowPassword.GetCheck())
	{
		//�� ���������� �������� �������
		m_editNewPassword.SetPasswordChar(0);		 
		m_editRepetPasswprd.SetPasswordChar(0);	
		TRACE("\n������ ���������� ������: %d (%c)", m_oldPasswordChar, m_oldPasswordChar);	
	}
	else
	{		
		//�� ���������� �������� �������
		m_editNewPassword.SetPasswordChar(m_oldPasswordChar);
		m_editRepetPasswprd.SetPasswordChar(m_oldPasswordChar);
		TRACE("\n������ ���������� ������: %d (%c)", m_oldPasswordChar, m_oldPasswordChar);
	}

	Invalidate(TRUE);
}


BOOL CDlgPasswordProtect::OnInitDialog()
{
	CDialog::OnInitDialog();

	//��������� ������ ������ ���������� ������
	//m_oldPasswordChar = m_editNewPassword.GetPasswordChar();
	m_oldPasswordChar = 42;
	TRACE("\n������ ���������� ������: %d (%c)", m_oldPasswordChar, m_oldPasswordChar);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����������: �������� ������� OCX ������ ���������� �������� FALSE
}


void CDlgPasswordProtect::OnBnClickedOk()
{
	CString csNewPassword, csRepetPassword;
	//���������� ��������� ������
	m_editNewPassword.GetWindowText(csNewPassword);
	m_editRepetPasswprd.GetWindowText(csRepetPassword);

	if (csNewPassword != csRepetPassword)
	{
		MessageBox("������ �� ���������, ���������� ��� ���.", "������ �� ���������", MB_ICONEXCLAMATION);
		
		m_editNewPassword.SetSel(0, -1);
		m_editNewPassword.Clear();
		m_editNewPassword.SetFocus();
	
		m_editRepetPasswprd.SetSel(0, -1);
		m_editRepetPasswprd.Clear();	
		
		return;
	}

	//������ ������ - ����
	if (csNewPassword.GetLength()== 0 && csRepetPassword.GetLength()==0)
	{
		MessageBox("������ ������ ������������ ������. ������� ������", "������ �� ������.", MB_ICONEXCLAMATION);
		m_editNewPassword.SetFocus();
		
		return;
	}
	
	//��������� ������
	TRACE("\n��������� ������");

	//������� ������ � ������
	SavePassword(csNewPassword);
	
	CDialog::OnOK();
}

//��������� ������ � ������
void CDlgPasswordProtect::SavePassword(CString csPassword)
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

	///////////////////////////////////////////////////////////
	//������� ������ � ������ =======================
	CString csSubKey;
	csSubKey.Format("SOFTWARE\\%s", ptr);

	SHSetValue(HKEY_CURRENT_USER, csSubKey, PasswordProtect, REG_SZ, csPassword.GetString(), csPassword.GetLength() );	
}

//=== ���������� ���� �� ����� ������� ===
void CDlgPasswordProtect::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CDialog::OnLButtonDown(nFlags, point);
}