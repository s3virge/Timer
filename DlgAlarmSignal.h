#pragma once


// CAlarmSignalDlg dialog

class CDlgAlarmSignal : public CDialog
{
	DECLARE_DYNAMIC(CDlgAlarmSignal)
	DECLARE_MESSAGE_MAP()

public:
	CDlgAlarmSignal(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAlarmSignal();

// Dialog Data
	enum { IDD = IDD_DLG_ALARMSIGNAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	

public:
	static bool m_bPlayStop;

private:	
	CString m_csAlarmSignal, m_csAlarmSignalPath;
	CString m_csProgrammName;

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	bool RegGetAlarmSignal(CString& csAlarmSignal, CString& csAlarmSignalPath);
	//сохраним в реестре название мелодии будильника
	void RegSaveAlarmSignal(CString csAlarmSignal, CString csAlarmSignalPath);
	void RegSaveAlarmSignalList();
	afx_msg void OnBnClickedBrows();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnPlay();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

public:
	CString GetAlarmSignal();
};
