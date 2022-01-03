#pragma once
#include "afxwin.h"


// ���������� ���� CPasswordProtectDlg

class CDlgPasswordProtect : public CDialog
{
	DECLARE_DYNAMIC(CDlgPasswordProtect)

public:
	CDlgPasswordProtect(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CDlgPasswordProtect();

// ������ ����������� ����
	enum { IDD = IDD_DLG_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()

private:
	// ������ "���������� ������"
	CButton m_checkShowPassword;
	// ����� ������
	CEdit m_editNewPassword;
	// ������ ������
	CEdit m_editRepetPasswprd;
	//������ ���������� ������
	char m_oldPasswordChar;

	afx_msg void OnBnClickedShowPass();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void SavePassword(CString);
};
