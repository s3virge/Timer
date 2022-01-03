// TimerDlg.h : header file
//

#pragma once

#include "SkinBaseButton.h"
#include "StaticTime.h"
#include "WndShadow.h"
#include "DlgAdjustments.h"
#include "ToolTips.h"



#define IDC_STATIC_TIME				37834
#define IDC_SKIN_BUTTON_CLOSE		37836
#define IDC_SKIN_BUTTON_MINIMIZE	37837
#define IDC_SKIN_BUTTON_STARTSTOP	37838
#define IDC_SKIN_BUTTON_SETTINGS	37839
#define IDC_SKIN_BUTTON_ADJUST		37835


// CTimerDlg dialog
class CTimerDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
// Construction
public:
	CTimerDlg(CWnd* pParent = NULL);	// standard constructor
	// Dialog Data
	enum { IDD = IDD_TIMER_DIALOG };

	void OnButtonMinimize(void);
	CWndShadow m_Shadow; //����
	static HWND m_staticHwndWndShadow; //������ ����������� ����� ����� ����������� ��������� ����� �� ������ �������
	//����������� � ��������� ������� � ��������� �������
	void MinimizeToSystemTray();

private:
	bool m_bFirstPaint; //���� ��������� ���� �� ��������
	bool m_bDragging;	//���������; ���������, �����������
	HICON m_hIcon;
	CString m_csProgrammName; //��� ������������ �����

	//CSkinBaseButton 
	CSkinBaseButton m_buttonClose;
	CSkinBaseButton m_buttonMinimize;
	CSkinBaseButton m_buttonStartStop;
	CSkinBaseButton m_buttonSettings;
	CSkinBaseButton m_buttonAdjustment;

	CStaticTime	m_stcTime; //������� �����

	//��� ���������� � ����� ������
	CRect wndWorkArea;
	int StickAt;
	CRect wndRect;

	//��� ����������� ���� ����� �����
	CRect WndRect;
	CPoint MousePoint;

	//��������� ���� �� ������� ����� �������
	CString m_csWinToFollowCaption;
	//����� ���� �� ������� ����� ���������
	HWND m_hwndWindowToFollow;

	//����������� ���������
	HWND m_wndToolTips;
	CToolTips m_ToolTips;

		
	//����� ������������
	CString m_csEstablishedTime;
	CString m_csWhatIsToBeDone; //������ ������� ��� �������
	CString m_csWhatIsToBeDoneWithoutTime; //������� ��� �������
	
	int m_iAction; //�������� (��� �������)
	int m_iMode;   //����� ������
	int m_iAdditionAction; //�������������� ��������
	int m_nHour, m_nMinute, m_nSeconds;

	bool m_bSrartStop; //true - ������ �������

	CDlgAdjustments m_dlgAdjust;

	bool m_bIconToShow; //����� ���������� ������ � ���� � ������������� ��� ���

	NOTIFYICONDATA	niData;	// notify icon data

	static const int LayeredWindowShowHideSpeed = 30; //�������� ���������/������������ ����
	//timer identifier.
	static const int ID_TIMER_ACTION = 1;
	static const int ID_TIMER_SECOND = 2;

private:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
	// Generated message map functions
	virtual void OnOK(); //�������������� ����� ���� �� ����������� ��� ������� �� �����
	virtual void OnCancel(); //�������������� �������� ������� ESC
	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);	

	void CreateCropRigion(CDC* pDC);
	void SetWindowBitmap(CDC* pDC);
	void GraduallyShowHideWindow(bool bShowHide);
	
	void OnButtonClose(void);
	void OnButtonStartStop(void);
	void OnButtonSettings(void);
	void OnButtonAdjust();

	void CalcWindowSize(void);//��������� ������ �������� �������� ���� � ��������� ��� ���� ������ �������
	void CreateButtons();	//��� ������� ������
	void RegReadWindowPosition(); //��� ����� ��������� ����
	void RegWriteWindowPosition(); //�������� ���������� ���� � ��������� ��	

	//��������� ��������
	void DoAction();
	//��������� ���� �� ������������� �� ��������
	//bool TestWindow(CString chWindowCaption);
	//�� ������
	bool TestWindow(HWND hwndWindowToTest);
	//��������� �������� ����
	void PlayAlarmSignal();
		
	//������� ������� ��� ���������� ��������
	bool CreateTimers();
	bool CreateTimerIfTimerMode();
	bool CreateTimerIfAlarmClockMode();
	
	//���������� ��������� �������
	void ShowChengingTime();
	void ChengingTimeIfTimerMode();
	void ChengingTimeIfAlarmClockMode();

	//��������� ���������� ������
	bool CommandLine();
	void ShowCommandLineHelp(bool bError = false);
	
	//������ � ������ ���� � ��������� ��� ������� �� ��������� ������
	void RegWriteAppPath();
	
	ULONGLONG GetDllVersion(LPCTSTR lpszDllName);
	
	//��������� ������ � ����
	void TrayIconAdd();
	void TrayIconDel();
	void TrayIconModify();
	//������������ ��������� �� ����
	LRESULT ProcessingTrayMessage(WPARAM wParam, LPARAM lParam);
	void ShowTrayContextMenu(HWND hWnd);
	void TrayMenuShow();
	void TrayMenuHide();
	void TrayMenuExit();
	//������������ ����������� ���������
	void MakeTrayToolTip();	
	bool IsPasswordProtect();
	bool IsPasswordCorrect();

public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
