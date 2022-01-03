// SettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Timer.h"
#include "DlgSettings.h"
#include "MultiLanguage.h"
#include "DlgWindowToFollow.h"
#include "DlgAlarmSignal.h"
#include "DefaultSettings.h"



//////////////////// CSettingsDlg dialog ///////////////////////////


IMPLEMENT_DYNAMIC(CDlgSettings, CDialog)

CDlgSettings::CDlgSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSettings::IDD, pParent)
	, m_csEstablishTime("00:00:00")
	, m_iHour(0)
	, m_iMinute(0)
	, m_iSeconds(0)		
{
	m_ToDo = TODO_SHUTDOWN;
	m_OperationMode = OM_TIMER;
	m_hwndWinToFollow = NULL;
}

CDlgSettings::~CDlgSettings()
{}

void CDlgSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_H, m_hwndHoursBox);
	DDX_Control(pDX, IDC_COMBO_M, m_hwndMinuteBox);
	DDX_Control(pDX, IDC_COMBO_S, m_hwndSecondsBox);
	DDX_Control(pDX, IDC_CURREENT_TIME, m_ctrlCurrentTime);
	DDX_Control(pDX, IDC_BTN_ACTION, m_btnToDo);
	DDX_Control(pDX, IDC_BTN_OPERETIONMODE, m_btnOperationMode);
	DDX_Control(pDX, IDC_BTN_ADDITIONACTION, m_btnToDoAfter);
	DDX_Text(pDX, IDC_FULL_COMMAND, m_csFullCommand);
}

BEGIN_MESSAGE_MAP(CDlgSettings, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	//ON_CBN_SELENDOK(IDC_COMBO_UILANGUAGE, &CSettingsDlg::OnCbnSelendokComboUilanguage)
	ON_BN_CLICKED(IDC_BTN_ACTION, &CDlgSettings::OnBnClickedAction)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_OPERETIONMODE, &CDlgSettings::OnBnClickedOperetionmode)
	ON_COMMAND(ID_MI_ACTION_SHOTDOWN, &CDlgSettings::OnMenuToDoShutdown)
	ON_COMMAND(ID_MI_ACTION_REBOOT, &CDlgSettings::OnMenuToDoReboot)
	ON_COMMAND(ID_MI_ACTION_LOCK, &CDlgSettings::OnMenuToDoLock)
	ON_COMMAND(ID_MI_OPERETIONMODE_TIMER, &CDlgSettings::OnMenuOperetionmodeTimer)
	ON_COMMAND(ID_MI_OPERETIONMODE_ALARM_CLOCK, &CDlgSettings::OnMenuOperetionmodeAlarmClock)
	ON_BN_CLICKED(IDOK, &CDlgSettings::OnBnClickedOk)
	ON_MENUXP_MESSAGES() //��������� ��������� ���� <--
	ON_BN_CLICKED(IDC_BTN_ADDITIONACTION, &CDlgSettings::OnBnClickedBtnAdditionAction)
	ON_COMMAND(ID_MI_ACTION_ALARM, &CDlgSettings::OnMenuToDoAlarm)
	ON_COMMAND(ID_ADDITION_CLOSEWINDOW, &CDlgSettings::OnMenuToDoAfterCloseWindow)
	ON_COMMAND(ID_ADDITION_ENDPROCESS, &CDlgSettings::OnMenuToDoAfterEndProcess)
	ON_CONTROL_RANGE(CBN_EDITUPDATE, IDC_COMBO_H, IDC_COMBO_S, &CDlgSettings::CbnEditUpdate)
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_COMBO_H, IDC_COMBO_S, &CDlgSettings::CbnEditUpdate)
	ON_BN_CLICKED(IDC_BTN_ALARMSIGNAL, &CDlgSettings::OnBnClickedBtnAlarmsignal)
	ON_BN_CLICKED(IDC_BTN_RESET, &CDlgSettings::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_SAVESETTINGSASDEFAULT, &CDlgSettings::OnBnClickedBtnSaveSettingsAsDefault)
END_MESSAGE_MAP()


//���������� ������������ ��������� ���� =====================
IMPLEMENT_MENUXP(CDlgSettings, CDialog);

//////////////////// CSettingsDlg message handlers //////////////////
BOOL CDlgSettings::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	//--------------------------------------------------
	// Comment this line to deactivate the XP Look & Feel
    CMenuXP::SetXPLookNFeel(this);

	//--------------------------------------------------
	m_ctrlCurrentTime.SetColourBackGround(WindowColor);
	m_ctrlCurrentTime.SetColourFaded(WindowColor);
	m_ctrlCurrentTime.SetColourForeGround(ColorCurrTimeForeGround);
	
	//--------------------------------------------------
	CDefaulSettings objDefSettings;
	//���� ����������� ��������� �� ���������
	if (objDefSettings.IsDefaultSettings())
	{
		int iH, iM, iS, iOpMode, iToDo, iToDoAfter;
		//�������� ���������
		if (!objDefSettings.ReadSettings(iH, iM, iS, iOpMode, iToDo, iToDoAfter))
		{
			//MessageBox("�� ������� �������� ��������� �� ���������", "�����", MB_ICONERROR);
			return false;
		}
					
		//-----����������� �������� �����-----
		CString csBuff;

		csBuff.Format("%d", iH);

		if (iH < 10)
			csBuff.Format("0%d", iH);

		m_hwndHoursBox.SetWindowTextA(csBuff);

		//-----����������� �������� �����-----
		csBuff.Format("%d", iM);

		if (iM < 10)
			csBuff.Format("0%d", iM);

		m_hwndMinuteBox.SetWindowTextA(csBuff);

		//-----�������������� �������� ������-----
		csBuff.Format("%d", iS);

		if (iS < 10)
			csBuff.Format("0%d", iS);

		m_hwndSecondsBox.SetWindowTextA(csBuff);

		//-----------------------------------------
		//��������� ����� ������
		SwitchOperetionMode(iOpMode);
		//��������� ��� ������� � ��� ������� �����
		m_ToDoAfter = iToDoAfter;
		SwitchToDoNToDoAfter(iToDo);
	} 
	else
	{
		//����� �������� ��� ���������
		ResetSettings();		
	}

	//��������� ���������� ���������� �����, �����, ������
	FillHourMinSecComboBox();
	//----------------------------------------------------
	SetWindowText(CMultilanguage::sLanguageText[20]);
	SetDlgItemText(IDC_CURRENTTIME, CMultilanguage::sLanguageText[21]);
	SetDlgItemText(IDC_REACTIONTIME, CMultilanguage::sLanguageText[22]);
	SetDlgItemText(IDC_HOUR, CMultilanguage::sLanguageText[23]);
	SetDlgItemText(IDC_MINUTE, CMultilanguage::sLanguageText[24]);
	SetDlgItemText(IDC_SECOND, CMultilanguage::sLanguageText[25]);
	SetDlgItemText(IDC_BTN_SAVESETTINGSASDEFAULT, CMultilanguage::sLanguageText[26]);
	SetDlgItemText(IDC_OPERATION_MODE, CMultilanguage::sLanguageText[27]);
	SetDlgItemText(IDC_TODO, CMultilanguage::sLanguageText[30]);
	SetDlgItemText(IDC_TODO_AFTER, CMultilanguage::sLanguageText[35]);

	SetDlgItemText(IDC_ALARM_SIGNAL, CMultilanguage::sLanguageText[50]);
	SetDlgItemText(IDC_BTN_ALARMSIGNAL, CMultilanguage::sLanguageText[51]);
	SetDlgItemText(IDC_BTN_RESET, CMultilanguage::sLanguageText[52]);

	SetDlgItemText(IDC_TODO2, CMultilanguage::sLanguageText[53]);
	
	SetDlgItemText(IDOK, CMultilanguage::sLanguageText[54]);
	SetDlgItemText(IDCANCEL, CMultilanguage::sLanguageText[55]);
			
	//--------------------------------------------------
	SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//=== ���������� ���� �� ����� ������� ===
void CDlgSettings::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

//=== ��������� ���������� ===
void CDlgSettings::FillHourMinSecComboBox()
{
	CString buf;
	int c = 0, iEndHour = 23, iEndMinute = 59, iEndSecons = 59;

	//�������� ������������� ������
	CString csHoure, csMinute, csSeconds;

	m_hwndHoursBox.GetWindowTextA(csHoure);
	m_hwndMinuteBox.GetWindowTextA(csMinute);
	m_hwndSecondsBox.GetWindowTextA(csSeconds);
	
	//������� ����������
	m_hwndHoursBox.SendMessage(CB_RESETCONTENT);
	m_hwndMinuteBox.SendMessage(CB_RESETCONTENT);
	m_hwndSecondsBox.SendMessage(CB_RESETCONTENT);
	
	//���������� �� ����� ��������� ����� ������������
	m_hwndHoursBox.SetWindowTextA(csHoure);
	m_hwndMinuteBox.SetWindowTextA(csMinute);
	m_hwndSecondsBox.SetWindowTextA(csSeconds);

	//----- ����� ������ ��������� -----
	if( m_OperationMode == OM_TIMER)
	{
		iEndHour = iEndMinute = iEndSecons = 99;
	}

	//----- ��������� ��������� ���� -----
	for(int c = 0; c <= iEndHour; c++ )
	{
		buf.Format("%d", c);
		
		if (c < 10)
			buf.Format("0%d", c);
		
		m_hwndHoursBox.AddString(buf);		
	}
	
	//----- ��������� ������ -----
	for(int c = 0; c <= iEndMinute; c++ )
	{
		buf.Format("%d", c);
		
		if (c < 10)
			buf.Format("0%d", c);
		
		m_hwndMinuteBox.AddString(buf);		
	}
	
	//----- ��������� ������� -----
	for(int c = 0; c <= iEndSecons; c++ )
	{
		buf.Format("%d", c);

		if (c < 10)
			buf.Format("0%d", c);

		m_hwndSecondsBox.AddString(buf);		
	}
}

//======== ������ ========
void CDlgSettings::OnBnClickedAction()
{
	ShowPopupMenu(IDR_MENU_BTN_ACTION, IDC_BTN_ACTION );
}

void CDlgSettings::OnBnClickedOperetionmode()
{
	ShowPopupMenu(IDR_MENU_BTN_OPERETIONMODE, IDC_BTN_OPERETIONMODE );
}

void CDlgSettings::OnBnClickedBtnAdditionAction()
{
	ShowPopupMenu(IDR_MENU_BTN_ADDITIONACTION, IDC_BTN_ADDITIONACTION );
}

void CDlgSettings::OnBnClickedOk()
{
	//��������� ���������� �� ������� � ������������ ��������� ������
	if ( CheckEstablishTime(true) )
		OnOK();
}

//��������� ������������ �������������� ������� � ����������� �� ������ ������
bool CDlgSettings::CheckEstablishTime(bool bCheckComboToEmpty)
{
	int iChechHour, iCheckMinute, iCheckSecond;
	//�������� ����� ������������
	CString csCheckEstablishTime = GetEstablishedTime(iChechHour, iCheckMinute, iCheckSecond);
	
	//���� ����� ������ ������, �� ������������� ����� �� ������ ��������� 00:00:00
	switch( m_OperationMode )
	{
	case ::OM_TIMER:
		if ( bCheckComboToEmpty )
		{
			if ( csCheckEstablishTime.Compare( "00:00:00" ) == 0 )
			{
				//�������� ��������� �� ������
				MessageBox("������ ����� ������ - ������.\n\n"
					"� ���� ������ ����� ������������ �� ����� ���� ����������� �� 00:00:00."
					,"����� ������������ �� �����������", MB_ICONERROR );

				m_hwndHoursBox.SetFocus();
				m_hwndHoursBox.SetWindowTextA("00");
				m_hwndHoursBox.SetEditSel(0, -1);
				return false;
			}
		}
		break;
	
	case ::OM_ALARMCLOCK:
		if (iChechHour < 0 || iChechHour > 23)
		{
			//�������� ��������� �� ������
			MessageBox("������ ����� ������ - ���������.\n\n"
				"� ���� ������ �������� ���� �� ����� ���� ������ 23 � ������ 00"
				,"������ ��������� ������� ������������", MB_ICONERROR );

			m_hwndHoursBox.SetFocus();
			m_hwndHoursBox.SetWindowTextA("00");
			m_hwndHoursBox.SetEditSel(0, -1);

			iChechHour = 0;
			m_csEstablishTime.Format("0%d:0%d:0%d", iChechHour, iCheckMinute, iCheckSecond);
			
			return false;
		}

		if (iCheckMinute < 0 || iCheckMinute > 59)
		{
			//�������� ��������� �� ������
			MessageBox("������ ����� ������ - ���������.\n\n"
				"� ���� ������ �������� ����� �� ����� ���� ������ 59 � ������ 00"
				,"������ ��������� ������� ������������", MB_ICONERROR );

			m_hwndMinuteBox.SetFocus();
			m_hwndMinuteBox.SetWindowTextA("00");
			m_hwndMinuteBox.SetEditSel(0, -1);  //�������� ������ � ��������

			iCheckMinute = 0;
			m_csEstablishTime.Format("0%d:0%d:0%d", iChechHour, iCheckMinute, iCheckSecond);
			
			return false;
		}

		if ( iCheckSecond < 0 || iCheckSecond > 59)
		{
			//�������� ��������� �� ������
			MessageBox("������ ����� ������ - ���������.\n\n"
				"� ���� ������ �������� ������ �� ����� ���� ������ 59 � ������ 00"
				,"������ ��������� ������� ������������", MB_ICONERROR );

			m_hwndSecondsBox.SetFocus();
			m_hwndSecondsBox.SetWindowTextA("00");
			m_hwndSecondsBox.SetEditSel(0, -1);  //�������� ������ � ��������
			
			iCheckSecond = 0;
			m_csEstablishTime.Format("0%d:0%d:0%d", iChechHour, iCheckMinute, iCheckSecond);
			
			return false;
		}

		break;		
	}
	
	return true;
}

//=== ������������� ���� ���� ===
BOOL CDlgSettings::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect( rect, WindowColor ); //���� ����
	
	//Nonzero if it erases the background; otherwise 0.
	return true; //������������� �������� �����������

	//return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgSettings::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	CBrush* m_pEditBkBrush;

	// Instantiate and initialize the background brush to black.
    m_pEditBkBrush = new CBrush( WindowColor );

	switch (nCtlColor) 
	{
	case CTLCOLOR_STATIC:
	//case CTLCOLOR_EDIT:
	//case CTLCOLOR_MSGBOX:
    case CTLCOLOR_BTN:
		// Set color to green on black and return the backgroundbrush.
		pDC->SetTextColor(TextColor);
		//pDC->SetBkColor(RGB(200, 200, 200));
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
	}

	// Return a different brush if the default is not desired
	return hbr;
}

//======== ������ ========
void CDlgSettings::OnTimer(UINT_PTR nIDEvent)
{
	m_ctrlCurrentTime.DisplayTime();

	CDialog::OnTimer(nIDEvent);
}

//======== ������ ���� =======
void CDlgSettings::ShowPopupMenu(UINT nIDMenuResource, int nIDDlgItem )
{
	CMenu menu, *m_PopupMenu;
	CRect rect;
    // ��������� ���� �� ��������
   	menu.LoadMenu(nIDMenuResource);
	
	//Retrieves the CMenu object of a pop-up menu.
	m_PopupMenu = menu.GetSubMenu(0);
	
	//���� ���������� ���� ��� ������ "��� �������"
	if ( nIDDlgItem == IDC_BTN_ACTION )
	{
		m_PopupMenu->ModifyMenu(ID_MI_ACTION_SHOTDOWN, MF_BYCOMMAND, ID_MI_ACTION_SHOTDOWN, CMultilanguage::sLanguageText[31]);
		m_PopupMenu->ModifyMenu(ID_MI_ACTION_REBOOT, MF_BYCOMMAND, ID_MI_ACTION_REBOOT, CMultilanguage::sLanguageText[32]);
		m_PopupMenu->ModifyMenu(ID_MI_ACTION_LOCK, MF_BYCOMMAND, ID_MI_ACTION_LOCK, CMultilanguage::sLanguageText[33]);
		m_PopupMenu->ModifyMenu(ID_MI_ACTION_ALARM, MF_BYCOMMAND, ID_MI_ACTION_ALARM, CMultilanguage::sLanguageText[34]);
	}

	//���� ���������� ���� ��� ������ "����� ������"
	if ( nIDDlgItem == IDC_BTN_OPERETIONMODE )
	{
		m_PopupMenu->ModifyMenu(ID_MI_OPERETIONMODE_TIMER, MF_BYCOMMAND, ID_MI_OPERETIONMODE_TIMER, CMultilanguage::sLanguageText[28]);
		m_PopupMenu->ModifyMenu(ID_MI_OPERETIONMODE_ALARM_CLOCK, MF_BYCOMMAND, ID_MI_OPERETIONMODE_ALARM_CLOCK, CMultilanguage::sLanguageText[29]);
	}

	//���� ���������� ���� ��� ������ "�������������� ��������"
	if ( nIDDlgItem == IDC_BTN_ADDITIONACTION )
	{
		m_PopupMenu->ModifyMenu(ID_ADDITION_CLOSEWINDOW, MF_BYCOMMAND, ID_ADDITION_CLOSEWINDOW, CMultilanguage::sLanguageText[40]);
		m_PopupMenu->ModifyMenu(ID_ADDITION_ENDPROCESS, MF_BYCOMMAND, ID_ADDITION_ENDPROCESS, CMultilanguage::sLanguageText[41]);
	}
    
	::GetWindowRect(GetDlgItem( nIDDlgItem )->GetSafeHwnd(), &rect);

	//���������� ������ �������� ��� ����
	CMenuXP::GetMenuParentWndSize(GetDlgItem( nIDDlgItem ));
    // ���������� ����
	//m_PopupMenu->TrackPopupMenu(TPM_LEFTALIGN, rect.right+1, rect.top , this); //������ �� ������
	m_PopupMenu->TrackPopupMenu(TPM_LEFTALIGN, rect.left, rect.bottom , this); //��������� ��� �������
	
    // ���������� ����
    m_PopupMenu->DestroyMenu();
	menu.DestroyMenu();
}

void CDlgSettings::OnMenuToDoShutdown()
{
	SwitchToDoNToDoAfter(::TODO_SHUTDOWN);
}

void CDlgSettings::OnMenuToDoReboot()
{
	SwitchToDoNToDoAfter(::TODO_REBOOT);	
}

void CDlgSettings::OnMenuToDoLock()
{
	SwitchToDoNToDoAfter(::TODO_LOCK);
}

void CDlgSettings::OnMenuToDoAlarm()
{
	SwitchToDoNToDoAfter(::TODO_ALARM);
}
//��������� ����� �������� ����
void CDlgSettings::OnMenuToDoAfterCloseWindow()
{
	CDlgWindowToFollow* pWindowToFollowDlg = new CDlgWindowToFollow();

	//���������� ������
	if (pWindowToFollowDlg->DoModal() == IDOK)
	{
		//m_btnOperetionMode.SetWindowTextA("���������");
		m_btnOperationMode.EnableWindow(FALSE);

		//��������� ����������
		m_hwndHoursBox.EnableWindow(FALSE);
		m_hwndMinuteBox.EnableWindow(FALSE);
		m_hwndSecondsBox.EnableWindow(FALSE);

		//������������� ����� ������ � ��� ������� �����
		m_ToDoAfter = ::TODOAFTER_CLOSEWINDOW;
		m_OperationMode = ::OM_ALARMCLOCK;

		//�������� ��������� ������� ��� ����
		m_hwndWinToFollow = pWindowToFollowDlg->GetWinToFollow();
		//�������� ����� ����
		::GetWindowText(m_hwndWinToFollow, m_csWinToFollowText.GetBuffer(MAX_PATH), MAX_PATH);
		m_csWinToFollowText.ReleaseBuffer();

		//-------------------------------------------------------------
		CString csAction;
		switch(m_ToDo)
		{
		case TODO_SHUTDOWN:
			csAction = CMultilanguage::sLanguageText[31];		
			break;

		case TODO_REBOOT:
			csAction = CMultilanguage::sLanguageText[32];
			break;

		case TODO_LOCK:
			csAction = CMultilanguage::sLanguageText[33];
			break;

		case TODO_ALARM:
			csAction = CMultilanguage::sLanguageText[34];
			break;
		}	

		//���������� ����� ������ "�����..."
		CString csWinText;

		if ( m_ToDoAfter != TODOAFTER_NOTSET )
		{
			switch ( m_ToDoAfter )
			{
			case TODOAFTER_CLOSEWINDOW:
				switch(m_ToDo)
				{
				case TODO_SHUTDOWN:
					m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[42]);
					break;

				case TODO_REBOOT:
					m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[43]);
					break;

				case TODO_LOCK:
					m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[44]);
					break;

				case TODO_ALARM:
					m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[45]);
					break;
				}
				break;

			case TODOAFTER_ENDPROCESS:
				switch(m_ToDo)
				{
				case TODO_SHUTDOWN:
					m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[46]);
					break;

				case TODO_REBOOT:
					m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[47]);
					break;

				case TODO_LOCK:
					m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[48]);
					break;

				case TODO_ALARM:
					m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[49]);
					break;
				}
				break;
			}		
		}
		else
		{
			//������������� ����� ������ ���� �������� + ����� ...
			switch(m_ToDo)
			{
			case TODO_SHUTDOWN:
				m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[36]);
				break;

			case TODO_REBOOT:
				m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[37]);
				break;

			case TODO_LOCK:
				m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[38]);
				break;

			case TODO_ALARM:
				m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[39]);
				break;
			}
		}

		//-------------------------------------------------------------
		MakeFullCommandString();
	}

	delete pWindowToFollowDlg;
}

void CDlgSettings::OnMenuToDoAfterEndProcess()
{
	m_ToDoAfter = TODOAFTER_ENDPROCESS;
	m_OperationMode = OM_ALARMCLOCK;

	//������������� ����� ������
	m_btnOperationMode.SetWindowTextA(CMultilanguage::sLanguageText[29]);
	//��������� ������
	m_btnOperationMode.EnableWindow(FALSE);
	
	//������������� ����� ������ ToDoAfter
	switch(m_ToDo)
	{
	case TODO_SHUTDOWN:
		m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[46]);
		break;

	case TODO_REBOOT:
		m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[47]);
		break;

	case TODO_LOCK:
		m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[48]);
		break;

	case TODO_ALARM:
		m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[49]);
		break;
	}

	MakeFullCommandString();
}

void CDlgSettings::OnMenuOperetionmodeTimer()
{
	SwitchOperetionMode(OM_TIMER);	
}

void CDlgSettings::OnMenuOperetionmodeAlarmClock()
{
	SwitchOperetionMode(OM_ALARMCLOCK);
}

//����������� ��������
void CDlgSettings::SwitchToDoNToDoAfter(int iToDo)
{
	//������������� ����� ������	
	m_ToDo = iToDo;

	//�������� ����� ������ "��� �������"
	CString csAction;
	switch(m_ToDo)
	{
	case TODO_SHUTDOWN:
		csAction = CMultilanguage::sLanguageText[31];		
		break;

	case TODO_REBOOT:
		csAction = CMultilanguage::sLanguageText[32];
		break;

	case TODO_LOCK:
		csAction = CMultilanguage::sLanguageText[33];
		break;

	case TODO_ALARM:
		csAction = CMultilanguage::sLanguageText[34];
		GetDlgItem(IDC_BTN_ALARMSIGNAL)->EnableWindow(TRUE);
		break;
	}	
	
	//������������� ���� ������ ToDo
	m_btnToDo.SetWindowText(csAction);
		
	////////////////////////////////////////////////////////////////////////////
	//���������� ����� ������ "�����..."
	CString csWinText;

	//���� ������ �����
	if ( m_ToDoAfter != TODOAFTER_NOTSET )
	{
		switch ( m_ToDoAfter )
		{
		case TODOAFTER_CLOSEWINDOW:
			//������������� ����� ������ ToDoAfter
			switch(m_ToDo)
			{
			case TODO_SHUTDOWN:
				m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[42]);
				break;

			case TODO_REBOOT:
				m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[43]);
				break;

			case TODO_LOCK:
				m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[44]);
				break;

			case TODO_ALARM:
				m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[45]);
				break;
			}
			break;

		case TODOAFTER_ENDPROCESS:
			switch(m_ToDo)
			{
			case TODO_SHUTDOWN:
				m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[46]);
				break;

			case TODO_REBOOT:
				m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[47]);
				break;

			case TODO_LOCK:
				m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[48]);
				break;

			case TODO_ALARM:
				m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[49]);
				break;
			}
			break;
		}		
	}
	else
	{
		//������������� ����� ������ ���� �������� + ����� ...
		switch(m_ToDo)
		{
		case TODO_SHUTDOWN:
			m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[36]);
			break;

		case TODO_REBOOT:
			m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[37]);
			break;

		case TODO_LOCK:
			m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[38]);
			break;

		case TODO_ALARM:
			m_btnToDoAfter.SetWindowText(CMultilanguage::sLanguageText[39]);
			break;
		}
	}
	
	//=== ��������� ������ ������ ������� ===
	MakeFullCommandString();	
}
//����������� ����� ������
void CDlgSettings::SwitchOperetionMode(int iOperationMode)
{
	CString csBtnCaption; //����� �� ������ � ����������� �� ������ ������

	switch(iOperationMode)
	{
		case OM_TIMER:
			csBtnCaption = CMultilanguage::sLanguageText[28];
		break;

		case OM_ALARMCLOCK:
			csBtnCaption = CMultilanguage::sLanguageText[29];
			break;
	}

	//�������� ����� �� ������
	m_btnOperationMode.SetWindowTextA(csBtnCaption);

	//�������� GetEstablishedTime ��� ������������ m_csEstablishTime
	int iHr = 0, iMi = 0, iSe = 0;
	GetEstablishedTime(iHr, iMi, iSe);
		
	//���������� ����� ������
	m_OperationMode = iOperationMode;
	
	FillHourMinSecComboBox();

	//=== ������������ ����� ������� ������� ������� �� m_iToDo ===
	MakeFullCommandString();
}

//=== ��������� ������ ������ � �������� ===
HWND CDlgSettings::GetWindowHwndToFollow()
{
	if (m_hwndWinToFollow)
		return m_hwndWinToFollow;

	return NULL;
}

int CDlgSettings::GetToDo()
{
	return m_ToDo;
}

int CDlgSettings::GetOperetionMode()
{
	return m_OperationMode;
}

int CDlgSettings::GetToDoAfter()
{
	return m_ToDoAfter;
}

CString CDlgSettings::GetWhatIsToBeDone()
{
	return m_csFullCommand;
}

CString CDlgSettings::GetWhatIsToBeDoneWithoutTime()
{
	return m_csCommandWithoutTime;
}

CString CDlgSettings::GetEstablishedTime(int& iHour, int& iMin, int& iSec)
{	
	CString csHour = "00", csMinute = "00", csSeconds = "00";

	//m_csEstablishTime = "00:00:00";

	int iLbCurSel = NULL;

	//--------------------------------------
	if ( IsWindow(m_hwndHoursBox.GetSafeHwnd()) )
	{
		iLbCurSel = m_hwndHoursBox.GetCurSel();

		if (iLbCurSel >= 0)
		{m_hwndHoursBox.GetLBText(iLbCurSel, csHour);}
		else
		{
			//���� ��������� �� ������, ����� csHour = "00"
			if ( m_hwndHoursBox.GetWindowTextLengthA() )
			{
				//�� �������� ��� �����
				m_hwndHoursBox.GetWindowTextA(csHour);
			}			
		}

		//���� ������� ���� �����, �� ��������� ����� ��� 0
		if (csHour.GetLength() == 1)
		{
			//�� ������� � ������ "0"
			csHour.Insert(0, "0");
		}

		//��������� ������ � ����� ������ SettingsDlg
		m_iHour	= atoi(csHour.GetString());
		//iHour = (m_iHour > 0) ? m_iHour : 0;
	}

	//--------------------------------------
	if ( IsWindow(m_hwndMinuteBox.GetSafeHwnd()) )
	{
		iLbCurSel = m_hwndMinuteBox.GetCurSel();

		if (iLbCurSel >= 0)
		{m_hwndMinuteBox.GetLBText(iLbCurSel, csMinute);}
		else
		{
			if ( m_hwndMinuteBox.GetWindowTextLengthA() )
				m_hwndMinuteBox.GetWindowTextA(csMinute);
		}

		if (csMinute.GetLength() == 1)
		{
			//�� ������� � ������ "0"
			csMinute.Insert(0, "0");
		}

		//��������� ������ � ����� ������ SettingsDlg
		m_iMinute	= atoi(csMinute.GetString());
		//iMin  = (m_iMinute > 0) ? m_iMinute : 0;
	}

	//--------------------------------------
	if ( IsWindow(m_hwndSecondsBox.GetSafeHwnd()) )
	{
		iLbCurSel = m_hwndSecondsBox.GetCurSel();

		if (iLbCurSel >= 0)
		{m_hwndSecondsBox.GetLBText(iLbCurSel, csSeconds);}
		else
		{
			if (m_hwndSecondsBox.GetWindowTextLengthA())
			{
				m_hwndSecondsBox.GetWindowTextA(csSeconds);
			}
		}

		//���� ������� ���� �����
		if (csSeconds.GetLength() == 1)
		{
			//�� ������� � ������ "0"
			csSeconds.Insert(0, "0");
		}	

		m_iSeconds	= atoi(csSeconds.GetString());
		//iSec = (m_iSeconds > 0) ? m_iSeconds : 0;
	}
	
	//���� ������� ���������� �� �������, �� ��������� �������� � ���� ������ m_csEstablishTime
	if ( IsWindow( this->GetSafeHwnd() ) )
	{
		m_csEstablishTime = csHour + ":" + csMinute + ":" + csSeconds;
	}
	
	//��� �������� ���������� ������, ���� ���� ��� ��� ���
	iHour = (m_iHour > 0) ? m_iHour : 0;
	iMin  = (m_iMinute > 0) ? m_iMinute : 0;
	iSec = (m_iSeconds > 0) ? m_iSeconds : 0;
	
	//� ���� �� �� �������, �� ���������� ����� ����������� ��������
	return m_csEstablishTime;
}

void CDlgSettings::ResetSettings()
{
	m_csFullCommand = "��������� ����� 00:00:00";

	m_csEstablishTime = "00:00:00";

	//����� ������� �� ��������� ��� ����� ������
	m_ToDo = TODO_SHUTDOWN;
	m_OperationMode = OM_TIMER;
	m_ToDoAfter = TODOAFTER_NOTSET;

	//������������ ���� � �����������
	m_hwndHoursBox.SetWindowTextA("00");
	m_hwndMinuteBox.SetWindowTextA("00");
	m_hwndSecondsBox.SetWindowTextA("00");

	//������� ������
	m_btnOperationMode.SetWindowTextA(CMultilanguage::sLanguageText[28]);
	m_btnToDo.SetWindowTextA(CMultilanguage::sLanguageText[31]);	
	m_btnToDoAfter.SetWindowTextA(CMultilanguage::sLanguageText[36]);

	//��������� ������
	m_btnOperationMode.EnableWindow(true);
	GetDlgItem(IDC_BTN_ALARMSIGNAL)->EnableWindow(false);

	//��������� ����������
	m_hwndHoursBox.EnableWindow(true);
	m_hwndMinuteBox.EnableWindow(true);
	m_hwndSecondsBox.EnableWindow(true);
	
	UpdateData(FALSE);

	//----------------------------------------------
	CDefaulSettings oDefSet;
	//���� ����������� ��������� �� ���������
	if (oDefSet.IsDefaultSettings())
	{
		oDefSet.ResetSettings();
	}
}

//=== ��������� �� ����������� ===
void CDlgSettings::CbnEditUpdate(UINT p_nCurrentID)
{
	/*��� ��������� ��������� ���������� ON_CONTROL_RANGE*/

	//��������� ������������ ������� ������������	
	if (!CheckEstablishTime())
		return;

	MakeFullCommandString();
}

void CDlgSettings::OnBnClickedBtnAlarmsignal()
{
	CDlgAlarmSignal* pDlgAlarmSignalDlg = new CDlgAlarmSignal();
	
	if ( pDlgAlarmSignalDlg->DoModal()== IDOK )
	{
		GetDlgItem( IDC_BTN_ALARMSIGNAL )->SetWindowTextA( pDlgAlarmSignalDlg->GetAlarmSignal() );
	}
	
	delete pDlgAlarmSignalDlg;
}

//��������� ������ � ��������
void CDlgSettings::MakeFullCommandString()
{
	//������������ ����� ������� ������� ������� �� m_iToDo
	CString csAction;
	switch(m_ToDo)
	{
	case TODO_SHUTDOWN:
		csAction = CMultilanguage::sLanguageText[31];		
		break;

	case TODO_REBOOT:
		csAction = CMultilanguage::sLanguageText[32];
		break;

	case TODO_LOCK:
		csAction = CMultilanguage::sLanguageText[33];
		break;

	case TODO_ALARM:
		csAction = CMultilanguage::sLanguageText[34];
		break;
	}	

	//------------------------------------------------
	switch(m_OperationMode)
	{
	case ::OM_TIMER:
		m_csFullCommand.Format("%s after %s", csAction, m_csEstablishTime);
		break;

	case ::OM_ALARMCLOCK:
		m_csFullCommand.Format("%s in %s", csAction, m_csEstablishTime);
		break;
	}

	//------------------------------------------------
	if ( m_ToDoAfter != TODOAFTER_NOTSET )
	{
		switch ( m_ToDoAfter )
		{
		case TODOAFTER_CLOSEWINDOW:
			//��������� ����� ������ �������
			if (m_csWinToFollowText.IsEmpty())
			{
				m_csFullCommand.Format("%s ����� �������� ���� c ���������� %d", csAction, m_hwndWinToFollow);
			}
			else
			{
				m_csFullCommand.Format("%s ����� �������� ���� \"%s\" (���������: %d)", csAction, m_csWinToFollowText, m_hwndWinToFollow);
			}
			break;
			
		case TODOAFTER_ENDPROCESS:
			if (m_csWinToFollowText.IsEmpty())
			{
				m_csFullCommand.Format("%s ����� ���������� �������� ���������� %d", csAction, m_hwndWinToFollow);
			}
			else
			{
				m_csFullCommand.Format("%s ����� ���������� �������� \"%s\" (���������: %d)", csAction, m_csWinToFollowText, m_hwndWinToFollow);
			}
			break;
		}
	}

	//�������� ������
	UpdateData(FALSE);
}

void CDlgSettings::OnBnClickedBtnReset()
{
	ResetSettings();
}

void CDlgSettings::OnBnClickedBtnSaveSettingsAsDefault()
{
	CDefaulSettings objSeveSetAsDef = CDefaulSettings();

	//����� ������ m_iHour, m_iMinute, m_iSeconds
	//�������� �������� ��� ��������� ��������� �� ���������� CbnEditUpdate
	objSeveSetAsDef.WriteSettings(m_iHour, m_iMinute, m_iSeconds, m_OperationMode, m_ToDo, m_ToDoAfter);
}
