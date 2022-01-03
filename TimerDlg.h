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
	CWndShadow m_Shadow; //тень
	static HWND m_staticHwndWndShadow; //делаем статической чтобы иметь возможность управлять тенью из других классов
	//сворачиваем в системную область в отдельном прцессе
	void MinimizeToSystemTray();

private:
	bool m_bFirstPaint; //флаг обрезания окна по картинке
	bool m_bDragging;	//тянущийся; отстающий, волочащийся
	HICON m_hIcon;
	CString m_csProgrammName; //имя исполняемого файла

	//CSkinBaseButton 
	CSkinBaseButton m_buttonClose;
	CSkinBaseButton m_buttonMinimize;
	CSkinBaseButton m_buttonStartStop;
	CSkinBaseButton m_buttonSettings;
	CSkinBaseButton m_buttonAdjustment;

	CStaticTime	m_stcTime; //часовой экран

	//для прилипания к краям экрана
	CRect wndWorkArea;
	int StickAt;
	CRect wndRect;

	//для перемешения окна левой мышью
	CRect WndRect;
	CPoint MousePoint;

	//заголовок окна за которым будем следить
	CString m_csWinToFollowCaption;
	//хэндл окна за которым будем наблюдать
	HWND m_hwndWindowToFollow;

	//всплывающие подсказки
	HWND m_wndToolTips;
	CToolTips m_ToolTips;

		
	//время срабатывания
	CString m_csEstablishedTime;
	CString m_csWhatIsToBeDone; //полная команда что сделать
	CString m_csWhatIsToBeDoneWithoutTime; //команда без времени
	
	int m_iAction; //действие (что сделать)
	int m_iMode;   //режим работы
	int m_iAdditionAction; //дополнительное действие
	int m_nHour, m_nMinute, m_nSeconds;

	bool m_bSrartStop; //true - таймер запущен

	CDlgAdjustments m_dlgAdjust;

	bool m_bIconToShow; //какой показывать значек в трэе с треугольником или без

	NOTIFYICONDATA	niData;	// notify icon data

	static const int LayeredWindowShowHideSpeed = 30; //скорость появления/исчезновения окна
	//timer identifier.
	static const int ID_TIMER_ACTION = 1;
	static const int ID_TIMER_SECOND = 2;

private:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
	// Generated message map functions
	virtual void OnOK(); //переопределяем чтобы окно не закрывалось при нажатии на Энтер
	virtual void OnCancel(); //предотваращаем закрытие кнопкой ESC
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

	void CalcWindowSize(void);//вычисляем размер картинки главного окна и подгоняем под него размер диалога
	void CreateButtons();	//тут создаем кнопки
	void RegReadWindowPosition(); //где будет появлятся окно
	void RegWriteWindowPosition(); //получить координаты окна и сохранить их	

	//выполнить операцию
	void DoAction();
	//проверяем окно на существование по названию
	//bool TestWindow(CString chWindowCaption);
	//по хэндлу
	bool TestWindow(HWND hwndWindowToTest);
	//проиграть звуковой файл
	void PlayAlarmSignal();
		
	//создаем таймеры для выполнения действия
	bool CreateTimers();
	bool CreateTimerIfTimerMode();
	bool CreateTimerIfAlarmClockMode();
	
	//показываем изменение времени
	void ShowChengingTime();
	void ChengingTimeIfTimerMode();
	void ChengingTimeIfAlarmClockMode();

	//выполнить комадндную строку
	bool CommandLine();
	void ShowCommandLineHelp(bool bError = false);
	
	//запись в реестр пути к программе для запуска из командной строки
	void RegWriteAppPath();
	
	ULONGLONG GetDllVersion(LPCTSTR lpszDllName);
	
	//добавляем иконку в трэй
	void TrayIconAdd();
	void TrayIconDel();
	void TrayIconModify();
	//обрабатываем сообщения из трэя
	LRESULT ProcessingTrayMessage(WPARAM wParam, LPARAM lParam);
	void ShowTrayContextMenu(HWND hWnd);
	void TrayMenuShow();
	void TrayMenuHide();
	void TrayMenuExit();
	//сформировать всплывающую подсказку
	void MakeTrayToolTip();	
	bool IsPasswordProtect();
	bool IsPasswordCorrect();

public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
