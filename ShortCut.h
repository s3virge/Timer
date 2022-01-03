#pragma once
#include "stdafx.h"

//--------------------------------------------------------------------- 
#define HOTKEY(modifier,key) ((((modifier)&0xff)<<8)|((key)&0xff)) 
//--------------------------------------------------------------------- 
// �������� ������ 
// ������� ���������: 
//  pwzShortCutFileName - ���� � ��� ������, ��������, "C:\\�������.lnk" 
//  ���� �� ������ ����, ����� ����� ������ � �����, ��������� � ��������� ���������. 
//  ����.: Windows ���� �� ��������� � ����� ���������� .lnk 
//  pszPathAndFileName  - ���� � ��� exe-�����, ��������, "C:\\Windows\\NotePad.Exe" 
//  pszWorkingDirectory - ������� �������, ��������, "C:\\Windows" 
//  pszArguments        - ��������� ��������� ������, ��������, "C:\\Doc\\Text.Txt" 
//  wHotKey             - ������� �������, ��������, ��� Ctrl+Alt+A     HOTKEY(HOTKEYF_ALT|HOTKEYF_CONTROL,'A') 
//  iCmdShow            - ��������� ���, ��������, SW_SHOWNORMAL 
//  pszIconFileName     - ���� � ��� �����, ����������� ������, ��������, "C:\\Windows\\NotePad.Exe" 
//  int iIconIndex      - ������ ������ � �����, ���������� � 0 

bool /*__fastcall*/CreateShortCut( 
	LPWSTR pwzShortCutFileName, 
	LPTSTR pszPathAndFileName, 
	LPTSTR pszWorkingDirectory, 
	LPTSTR pszArguments, 
	WORD wHotKey, 
	int iCmdShow, 
	LPTSTR pszIconFileName, 
	int iIconIndex) { 
		IShellLink * pSL; 
		IPersistFile * pPF; 
		HRESULT hRes; 
		// ��������� ���������� ���������� "�����" 
		hRes = CoCreateInstance(CLSID_ShellLink,
			0,
			CLSCTX_INPROC_SERVER,
			IID_IShellLink,
			(LPVOID *)&pSL); 
		if( SUCCEEDED(hRes) ) { 
			hRes = pSL->SetPath(pszPathAndFileName); 
			if( SUCCEEDED(hRes) ) { 
				hRes = pSL->SetArguments(pszArguments); 
				if( SUCCEEDED(hRes) ) { 
					hRes = pSL->SetWorkingDirectory(pszWorkingDirectory); 
					if( SUCCEEDED(hRes) ) { 
						hRes = pSL->SetIconLocation(pszIconFileName,iIconIndex); 
						if( SUCCEEDED(hRes) ) { 
							hRes = pSL->SetHotkey(wHotKey); 
							if( SUCCEEDED(hRes) ) { 
								hRes = pSL->SetShowCmd(iCmdShow); 
								if( SUCCEEDED(hRes) ) { 
									// ��������� ���������� ��������� ���������� 
									hRes = pSL->QueryInterface(IID_IPersistFile,(LPVOID *)&pPF); 
									if( SUCCEEDED(hRes) ) { 
										// ���������� ���������� ������ 
										hRes = pPF->Save(pwzShortCutFileName,TRUE); 
										pPF->Release(); 
									} 
								} 
							} 
						} 
					} 
				} 
			} 
			pSL->Release(); 
		} 
		return SUCCEEDED(hRes); 
}