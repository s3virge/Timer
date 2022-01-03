// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

/*отключаем "Предупреждение 1 warning C4996: 'wctomb': This function or variable may be unsafe. Consider using wctomb_s instead. */
#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif



#include <strsafe.h> //для использования StringCchPrintf()
#include "shlwapi.h" //для использования SHGetValue()

//#include <gdiplus.h>
//using namespace Gdiplus;
//#pragma comment(lib, "gdiplus.lib")

using namespace std;

//подключаем звук
#include "fmod.h"  
#pragma comment(lib, "fmodvc.lib") 

//действия
enum TODO { TODO_SHUTDOWN, TODO_REBOOT, TODO_LOCK, TODO_ALARM };
//режим работы
enum OPERETIONMODE { OM_TIMER, OM_ALARMCLOCK };
//дополнительные настройки
enum TODOAFTER { TODOAFTER_NOTSET, TODOAFTER_CLOSEWINDOW, TODOAFTER_ENDPROCESS};

//название программы 
#define AlarmSignalPathName		"AlarmSignalPath"
#define CloseWhenWindowSelect	"CloseWhenWindowSelect"
#define PasswordProtect			"Password"
#define AlarmSignalName			"AlarmSignal"

// ===== цвета =====
const COLORREF WindowColor = RGB(220, 220, 220);
const COLORREF TextColor   = RGB(0, 0, /*140*/0) ;
const COLORREF ColorCurrTimeForeGround    = RGB(100, 100, 100);

//#pragma warning(disable:4995);
