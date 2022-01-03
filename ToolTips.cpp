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
	�������� CreateToolTip( ) ���������� ��������� ��� ��������� ����, ��� �� ������ 
	��� �������. � ��� ���������� ������������� ��������� ���������� ����������� 
	��������� � �������� ���������� �������� ToolTip. ������ ��������� ��������� ���� 
	������� (���� ������� ���� ����������).
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
	������� FillInToolInfo( ) ������ ��������������� ���� ��� ���������� �������� 
	�������� �� ���������� TOOLINFO. ������ ��������� ������� ��������������� 
	������������� � �������� ����������� �������������� ������� ������ ��������� 
	(������� � MSDN ����� �������� tool) ������ ���� - �������� ��������� � ������, 
	���� � ��� ������� ������� ������������� nIDTool. � ������ ���������� �������� 
	nIDTool ����������� ��� ������ ���������� ������������ ������������ ��������.
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
	�������� ����� ������� ��������� ����� �������� AddTool( ). ������ ���������� 
	AddTool( ) ���������������, ��� ������� hTip ��� ��������� ���� ��������� ���������
	��������� � ������������ ���� �� ���� - ��������� ���������. ��� ����� ��� ��������
	������� ������������ ���� TTF_SUBCLASS. � ���� ������ ���������� �������� 
	������������� � �������������� ��������� TTM_RELAYEVENT. ���� TTF_TRANSPARENT 
	���������� � ��������, ��� ��������� ���� ��������� ����� ��������� ��� ������� 
	���������.

	���������� ����������� �������� ��������� ������ ��������� �� ����� ������� �����. 
	��� ����� ������ ���������� NULL-��������� � �������� ��������� �� ����� ���������.
	������ NULL � ToolTip ������� ����� �������� �������� LPSTR_TEXTCALLBACK, ��������� 
	��������, ��� ��� ������������� �� ������ �������� ����� ��������� ����������� 
	��������� ����������� (����� WM_NOTIFY) �������� TTN_GETDISPINFO (������������� ��� 
	TTN_NEEDTEXT).

	����� ���� AddTool( ) ��������������� ����������� ����������� �������������� 
	������� ���� (�� ������ ���� �������, �� � ���� ������ ��������) ���� ����������
	��������������� (���� ��������� �� ���� ����� NULL, ����� ������������ ��� 
	���������� ������� ����). ������, ��� ���������� ������� ��������� ����� �������� 
	������ ������������� ������� ��������� - ���� ��� �������� �� ������������� ������ 
	���� � �������� �������������� (���������� ���� TTF_IDISHWND), �� �������������� 
	�������� ���������� ��� ���������� ������� ���� - ��������, � ���������� 
	�������������� (���� ���� ��� ������� ����) ����� ��������������. ��� ����� �� 
	���������� ������� FillInToolInfo( ), ��� ����� ����������� ��� �������, ����� 
	nIDTool ����� ����.
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
	����� ����, ��� ������� ����������������, ����� ��������� �� ������� ����������� 
	UpdateTipText( ). ����� ��������, ��� � ��� ����� ���� ����������� ��� �� �������� 
	��������� ������ ������ ���������, ��� � � AddTool( ). �.�. � ��� ������, ���� 
	��������� lpszText ����� ���������� � NULL, �� ����� ������������ �������� 
	��������� ������ ������ ���������. � ��� �� ��������� � ������, ���� ����� ������ 
	���������� ����� �����-���� ����� ���������, ���� ��������� lpszText � NULL 
	����������� �������������� ������? � ���� ������ �����, ����� lpszText �������� �� 
	������ ������ "".
*/
void CToolTips::UpdateTipText(HWND hTip, HWND hWnd, UINT nIDTool, LPCSTR lpszText)
{	
	FillInToolInfo(&ti, hWnd, nIDTool);
	ti.lpszText = LPSTR( lpszText ? lpszText : LPSTR_TEXTCALLBACKA );
	SendMessageA(hTip, TTM_UPDATETIPTEXTA, 0, (LPARAM)&ti);
}

/*
	�������� ����� ���������� ��������� ����� ����������� GetTipText( ).
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
	��������/��������� ����� ���� ���������, ������������������ ������ tooltip-���������, 
	����� �������� EnableToolTip( ).
*/
void CToolTips::EnableToolTip(HWND hTip, BOOL activate)
{
	SendMessageA(hTip, TTM_ACTIVATE, activate, 0);
}
