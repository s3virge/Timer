// Timer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Timer.h"
#include "TimerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTimerApp

BEGIN_MESSAGE_MAP(CTimerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CTimerApp::CTimerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTimerApp object

CTimerApp theApp;

// CTimerApp initialization

BOOL CTimerApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	//--- предотвращаем множественный запуск ---
	HANDLE hMutex = CreateMutexA(NULL, true, "some_big_Timer_unique_mutex_name_Timer");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		// мы уже есть - ругаемся и выходим
		::MessageBoxA(NULL, "The program already run!", "Error", MB_ICONERROR|MB_OK);
		return false;
	}
	
	//------------------------------------------
	// Initialize GDI+.
	//GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	//------------------------------------------

	//будем рисовать тень для окна
	CWndShadow::Initialize(AfxGetInstanceHandle());

	CTimerDlg dlg;
	m_pMainWnd = &dlg;
	
	dlg.DoModal();	

	//----------------------------
	//GdiplusShutdown(gdiplusToken);

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
