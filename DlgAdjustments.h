#pragma once
#include "MultiLanguage.h"
#include <string>

class CDlgAdjustments : public CDialog
{
	DECLARE_DYNAMIC(CDlgAdjustments)

public:
	CDlgAdjustments(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAdjustments();

	struct Adjust
	{
		BOOL bMainWndDoubleClickClose;			// ��������� ������� ���� ������� �������
		BOOL bStartCountWhenSettingsWndClose;	// �������� ������ ����� ����� �������� ���� ���������
		BOOL bSaveMainWndPosition;				// ���������� ��������� ���� ��� ��������
		BOOL bMinimiseWhenStart;				// ����������� ��� �������
		BOOL bMinimiseToSystemArea;				// ����������� � ����
		BOOL bPasswordProtect;					// ������ ��������� �������
	} m_structAdjust; //��������� � ����������� ��������� ���������

	void ReadAdjustments();  //��������� ��������

private:
	CMultilanguage m_objMultiLang;
		
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_DLG_ADJUST };
	
	DECLARE_MESSAGE_MAP()
	
	//=== ������ ������ ===
	void WriteAdjustments(); //��������� ���������	

	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedPasswordProtection();
	
	CButton m_chekAutorun;

	afx_msg void OnCbnSelchangeComboUilanguage();

	BOOL m_ImmediatelyStartTheTask;
	CString m_csSubKey; //���� ������� � �����������

	static const string CloseDoubleClickName;
	static const string StartCloseName;
	static const string KeepPositionName;
	static const string MinimiseToSystemArea;
	static const string MinimiseWhenStart;
	
public:
	bool IsImmediatelyStartTheTask(void);
};
