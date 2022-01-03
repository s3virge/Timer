#pragma once
#include "afxwin.h"


// диалоговое окно CDlgGetPassword

class CDlgGetPassword : public CDialog
{
	DECLARE_DYNAMIC(CDlgGetPassword)

public:
	CDlgGetPassword(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CDlgGetPassword();

// Данные диалогового окна
	enum { IDD = IDD_DLG_GETPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	void RegGetPassword(); //получить сохраненый пароль	
public:
	afx_msg void OnBnClickedOk();

private:
	CString m_csPassword;
	CString m_csProgrammName; //имя исполняемого файла
};
