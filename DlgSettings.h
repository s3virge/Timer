#pragma once

#include "afxwin.h"
#include "StaticTime.h"
#include "MenuXP.h"

// CSettingsDlg dialog

class CDlgSettings : public CDialog
{
	DECLARE_DYNAMIC(CDlgSettings) //Adds the ability to access run-time information about an object's class when deriving a class from CObject
	DECLARE_MESSAGE_MAP()
	DECLARE_MENUXP();

public:
	CDlgSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettings();
	// Dialog Data
	enum { IDD = IDD_DLG_SETTINGS };
	
	int GetToDo();
	int GetOperetionMode();
	int GetToDoAfter();
	
	//получить установленное время
	CString GetEstablishedTime(int& Hour, int& Min, int& Sec);
	
	////установить значение часов
	//void SetHour(int H = 0);
	////установить заначение минут
	//void SetMinutes(int M = 0);
	////установить значение секунд
	//void SetSeconds(int S = 0);
		
	HWND GetWindowHwndToFollow();
	CString GetWhatIsToBeDone();
	CString GetWhatIsToBeDoneWithoutTime();	

private:
	CComboBox m_hwndHoursBox, m_hwndMinuteBox, m_hwndSecondsBox;
	
	int m_iHour, m_iMinute, m_iSeconds;
	CStaticTime m_ctrlCurrentTime;
	
	//кнопки
	CButton m_btnToDo;
	CButton m_btnOperationMode;
	CButton m_btnToDoAfter;
		
	//заголовок окна за торым будем следить
	CString m_csWinToFollowText;
	HWND m_hwndWinToFollow; //описатель окна за которым следим
	//полный текст команды
	CString m_csFullCommand;
	CString m_csCommandWithoutTime; //команда без времени

	//доступные действия
	int m_ToDo;
	//режим работы
	int m_OperationMode;
	//дополнительное действие
	int m_ToDoAfter;

	//установленное время срабатывания
	CString m_csEstablishTime;	

private:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnCbnSelendokComboUilanguage();
	afx_msg void OnBnClickedAction();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOperetionmode();
	afx_msg void OnMenuToDoShutdown();
	afx_msg void OnMenuToDoReboot();
	afx_msg void OnMenuToDoLock();
	afx_msg void OnMenuOperetionmodeTimer();	
	afx_msg void OnMenuOperetionmodeAlarmClock();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnAdditionAction();
	afx_msg void OnMenuToDoAlarm();
	afx_msg void OnMenuToDoAfterCloseWindow();
	afx_msg void OnMenuToDoAfterEndProcess();
	afx_msg void OnBnClickedBtnAlarmsignal();
	afx_msg void OnBnClickedBtnReset();
	void FillHourMinSecComboBox();
	void SwitchOperetionMode(int OperationMode);
	void SwitchToDoNToDoAfter(int iToDo);
	void CbnEditUpdate(UINT p_nCurrentID);

	//устанавливаем значения по умолчанию
	void ResetSettings();
	//показываем всплывающее окно
	void ShowPopupMenu(UINT nIDMenuResource, int nIDDlgItem );
	//проверяем установленное время
	bool CheckEstablishTime(bool bCheckComboToEmpty = false);	
	void MakeFullCommandString();

	afx_msg void OnBnClickedBtnSaveSettingsAsDefault();
};
