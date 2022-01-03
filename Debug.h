#pragma once

#include "stdafx.h"

//полезная функция при отладке
void ErrorExit(LPSTR lpszFunction) 
{ 
	char szBuf[200]; 
	LPVOID lpMsgBuf;
	DWORD dw = GetLastError(); 

	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR) &lpMsgBuf,
		0, NULL );

	wsprintfA(szBuf, "%s failed with error %d: %s", lpszFunction, dw, lpMsgBuf); 

	MessageBoxA(NULL, szBuf, "Error", MB_ICONERROR); 
	TRACE(szBuf);

	LocalFree(lpMsgBuf);
	//ExitProcess(dw); 
}
