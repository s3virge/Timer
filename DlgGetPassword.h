#pragma once
#include "afxwin.h"


// ���������� ���� CDlgGetPassword

class CDlgGetPassword : public CDialog
{
	DECLARE_DYNAMIC(CDlgGetPassword)

public:
	CDlgGetPassword(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CDlgGetPassword();

// ������ ����������� ����
	enum { IDD = IDD_DLG_GETPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	void RegGetPassword(); //�������� ���������� ������	
public:
	afx_msg void OnBnClickedOk();

private:
	CString m_csPassword;
	CString m_csProgrammName; //��� ������������ �����
};
