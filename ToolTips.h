#pragma once

#include "commctrl.h"

class CToolTips
{
private:
	TOOLINFOA ti;
	UINT nIDTool;
	BOOL activate;
	LPCTSTR szText;

public:
	CToolTips(void);
	~CToolTips(void);
	HWND CreateToolTip(HWND hWndParent); 
	void FillInToolInfo(TOOLINFOA* ti, HWND hWnd, UINT nIDTool); 
	bool AddTool(HWND hTip, HWND hWnd, RECT* pr, UINT nIDTool, LPCSTR szText); 
	void UpdateTipText(HWND hTip, HWND hWnd, UINT nIDTool, LPCSTR lpszText);
	void GetTipText(HWND hTip, HWND hWnd, UINT nIDTool, LPSTR szText);
	void EnableToolTip(HWND hTip, BOOL activate);
	
};
