#pragma once
#include "stdafx.h"

//--------------------------------------------------------------------- 
#define HOTKEY(modifier,key) ((((modifier)&0xff)<<8)|((key)&0xff)) 
//--------------------------------------------------------------------- 
// Создание ярлыка 
// Входные параметры: 
//  pwzShortCutFileName - путь и имя ярлыка, например, "C:\\Блокнот.lnk" 
//  Если не указан путь, ярлык будет создан в папке, указанной в следующем параметре. 
//  Прим.: Windows сама НЕ добавляет к имени расширение .lnk 
//  pszPathAndFileName  - путь и имя exe-файла, например, "C:\\Windows\\NotePad.Exe" 
//  pszWorkingDirectory - рабочий каталог, например, "C:\\Windows" 
//  pszArguments        - аргументы командной строки, например, "C:\\Doc\\Text.Txt" 
//  wHotKey             - горячая клавиша, например, для Ctrl+Alt+A     HOTKEY(HOTKEYF_ALT|HOTKEYF_CONTROL,'A') 
//  iCmdShow            - начальный вид, например, SW_SHOWNORMAL 
//  pszIconFileName     - путь и имя файла, содержащего иконку, например, "C:\\Windows\\NotePad.Exe" 
//  int iIconIndex      - индекс иконки в файле, нумеруется с 0 

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
		// Получение экземпляра компонента "Ярлык" 
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
									// Получение компонента хранилища параметров 
									hRes = pSL->QueryInterface(IID_IPersistFile,(LPVOID *)&pPF); 
									if( SUCCEEDED(hRes) ) { 
										// Сохранение созданного ярлыка 
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