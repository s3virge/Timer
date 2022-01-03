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
		BOOL bMainWndDoubleClickClose;			// закрывать главное окно двойным щелчком
		BOOL bStartCountWhenSettingsWndClose;	// начинать отсчет сразу после закрытия окна настройки
		BOOL bSaveMainWndPosition;				// запоминать польжение окна при закрытии
		BOOL bMinimiseWhenStart;				// сворачивать при запуске
		BOOL bMinimiseToSystemArea;				// сворачивать в трэй
		BOOL bPasswordProtect;					// защита программы паролем
	} m_structAdjust; //структура с настройками поведения программы

	void ReadAdjustments();  //прочитать настойки

private:
	CMultilanguage m_objMultiLang;
		
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_DLG_ADJUST };
	
	DECLARE_MESSAGE_MAP()
	
	//=== методы класса ===
	void WriteAdjustments(); //сохранить настройки	

	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedPasswordProtection();
	
	CButton m_chekAutorun;

	afx_msg void OnCbnSelchangeComboUilanguage();

	BOOL m_ImmediatelyStartTheTask;
	CString m_csSubKey; //куст реестра с параметрами

	static const string CloseDoubleClickName;
	static const string StartCloseName;
	static const string KeepPositionName;
	static const string MinimiseToSystemArea;
	static const string MinimiseWhenStart;
	
public:
	bool IsImmediatelyStartTheTask(void);
};
