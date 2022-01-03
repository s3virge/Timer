#pragma once
#include "afxwin.h"


// диалоговое окно CPasswordProtectDlg

class CDlgPasswordProtect : public CDialog
{
	DECLARE_DYNAMIC(CDlgPasswordProtect)

public:
	CDlgPasswordProtect(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CDlgPasswordProtect();

// Данные диалогового окна
	enum { IDD = IDD_DLG_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()

private:
	// кнопка "показывать пароль"
	CButton m_checkShowPassword;
	// новый пароль
	CEdit m_editNewPassword;
	// повтор пароля
	CEdit m_editRepetPasswprd;
	//символ скрывающий пароль
	char m_oldPasswordChar;

	afx_msg void OnBnClickedShowPass();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void SavePassword(CString);
};
