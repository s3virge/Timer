#include "StdAfx.h"
#include "ToolTips.h"

#pragma comment(lib, "comctl32.lib")

CToolTips::CToolTips(void)
{
}

CToolTips::~CToolTips(void)
{
}

/*
	Название CreateToolTip( ) достаточно прозрачно для понимания того, что же делает 
	эта функция. В ней происходит инициализация системной библиотеки управляющих 
	элементов и создание собственно контрола ToolTip. Обычно родителем выступает окно 
	диалога (либо главное окно приложения).
*/
HWND CToolTips::CreateToolTip(HWND hWndParent)
{
	InitCommonControls();

	HWND hTip = CreateWindowExA(
		0,TOOLTIPS_CLASSA,
		0,0,0,0,0,0,
		hWndParent,0,0,0);
	return hTip;
}

/*
	Функция FillInToolInfo( ) играет вспомогательную роль для выполнения рутинных 
	операций со структурой TOOLINFO. Логика поведения функции предусматривает 
	использование в качестве уникального идентификатора области вывода подсказки 
	(которая в MSDN носит название tool) хэндла окна - носителя подсказки в случае, 
	если в нее передан нулевой идентификатор nIDTool. В случае ненулевого значения 
	nIDTool программист сам должен обеспечить уникальность передаваемых значений.
*/
void CToolTips::FillInToolInfo(TOOLINFOA* ti, HWND hWnd, UINT nIDTool)
{
	ZeroMemory(ti,sizeof(TOOLINFOA));
	ti->cbSize = sizeof(TOOLINFOA);

	if(!nIDTool)
	{
		ti->hwnd   = GetParent(hWnd);
		//Indicates that the uId member is the window handle to the tool. 
		//If this flag is not set, uId is the tool's identifier. 
		ti->uFlags = TTF_IDISHWND;
		ti->uId    = (UINT)hWnd;
	}
	else
	{
		ti->hwnd   = hWnd;
		ti->uFlags = 0;
		ti->uId    = nIDTool;
	}
}

/*
	Добавить новую область подсказки можно функцией AddTool( ). Данная реализация 
	AddTool( ) предусматривает, что контрол hTip сам обеспечит себе получение системных
	сообщений о передвижении мыши от окон - носителей подсказки. Для этого при создании
	области выставляется флаг TTF_SUBCLASS. В этом случае совершенно отпадает 
	необходимость в использованиии механизма TTM_RELAYEVENT. Флаг TTF_TRANSPARENT 
	опционален и означает, что выводимые окна подсказки будут прозрачны для мышиных 
	сообщений.

	Существует возможность отложить установку текста подсказки на более позднее время. 
	Для этого просто передается NULL-указатель в качестве указателя на текст подсказки.
	Вместо NULL в ToolTip контрол будет передано значение LPSTR_TEXTCALLBACK, говорящее 
	контролу, что при необходимости он сможет получить текст подсказки посредством 
	механизма нотификации (через WM_NOTIFY) посылкой TTN_GETDISPINFO (эквивалентное ему 
	TTN_NEEDTEXT).

	Кроме того AddTool( ) предусматривает возможность ограничения чувствительной 
	области окна (не только окна диалога, но и окна любого контрола) явно задаваемым
	прямоугольником (если указатель на него равен NULL, будет использована вся 
	клиентская область окна). Однако, при добавлении области подсказки имеет значение 
	способ идентификации области подсказки - если она основана на использовании хэндла 
	окна в качестве идентификатора (установлен флаг TTF_IDISHWND), то чувствительной 
	областью становится вся клиентская область окна - носителя, а координаты 
	прямоугольника (даже если они указаны явно) будут игнорироваться. Как видно из 
	реализации функции FillInToolInfo( ), это будет происходить для случаев, когда 
	nIDTool равен нулю.
*/
bool CToolTips::AddTool(HWND hTip, HWND hWnd, RECT* pr, UINT nIDTool, LPCSTR szText)
{	
	RECT r;// = {0,0,0,0};

	FillInToolInfo(&ti, hWnd, nIDTool);

	ti.hinst  = (HINSTANCE)GetModuleHandleA(NULL);
	ti.uFlags |= TTF_SUBCLASS | TTF_TRANSPARENT;
	ti.lpszText = LPSTR( szText ? szText : LPSTR_TEXTCALLBACKA );

	if(!(ti.uFlags & TTF_IDISHWND))
	{
		if(!pr)
		{
			pr = &r;
			GetClientRect(hWnd, pr);
		}
		memcpy(&ti.rect, pr, sizeof(RECT));
	}
	BOOL res = SendMessageA(hTip, // handle to destination control 
		TTM_ADDTOOLA,			  // message ID 
		0,						  // = 0; not used, must be zero
		(LPARAM)&ti);			  // = (LPARAM) (LPTOOLINFO) lpti;
	return res;
}

/*
	После того, как область зарегистрирована, можно управлять ее текстом посредством 
	UpdateTipText( ). Можно заметить, что в ней может быть использован тот же механизм 
	обратного вызова текста подсказки, что и в AddTool( ). Т.е. в том случае, если 
	указатель lpszText будет установлен в NULL, то будет задействован механизм 
	обратного вызова текста подсказки. А как же поступить в случае, если нужно просто 
	прекратить вывод какой-либо одной подсказки, если установка lpszText в NULL 
	задействует альтернативный способ? В этом случае нужно, чтобы lpszText указывал на 
	пустую строку "".
*/
void CToolTips::UpdateTipText(HWND hTip, HWND hWnd, UINT nIDTool, LPCSTR lpszText)
{	
	FillInToolInfo(&ti, hWnd, nIDTool);
	ti.lpszText = LPSTR( lpszText ? lpszText : LPSTR_TEXTCALLBACKA );
	SendMessageA(hTip, TTM_UPDATETIPTEXTA, 0, (LPARAM)&ti);
}

/*
	Получить текст конкретной подсказки можно посредством GetTipText( ).
*/
void CToolTips::GetTipText(HWND hTip, HWND hWnd, UINT nIDTool, LPSTR szText)
{
	// TOOLINFO ti;
	if(!szText)
		return;
	*szText = 0;
	FillInToolInfo(&ti, hWnd, nIDTool);
	ti.lpszText = szText;
	SendMessage(hTip, TTM_GETTEXT, 0, (LPARAM)&ti);
}

/*
	Включить/выключить вывод всех подсказок, зарегистрированных данным tooltip-контролом, 
	можно функцией EnableToolTip( ).
*/
void CToolTips::EnableToolTip(HWND hTip, BOOL activate)
{
	SendMessageA(hTip, TTM_ACTIVATE, activate, 0);
}
