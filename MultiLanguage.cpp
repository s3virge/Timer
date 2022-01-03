//#include "StdAfx.h"
#include "MultiLanguage.h"

/* глобальная статическая переменная содержит строки выбраного языка*/
CString CMultilanguage::sLanguageText[MAX_PATH]={"0"};

CMultilanguage::CMultilanguage()
{
	m_bGUILangChenged = false;
}

void CMultilanguage::ReadGUILanguageFile()
{	
	//обявление строка с дефолным языком English - например
	char *sDefaultLanguage[MAX_PATH]=
	{	
		"",				//0
		"Hide",			//1
		"Show",			//2
		"Exit",			//3
		"Start-Stop",	//4
		"Settings",		//5
		"Adjustments",	//6
		"Close",		//7
		"Minimize",		//8
		"Для изменения языка интерфейса, пожалуйста, перезапустите программу!",//9
		"Exit when double click", //10
		"Run task when close windows adjustments",//11
		"Remember windows place when close", //12
		"Minimize to notification area", //13
		"Password protect", //14
		"Run on Windows startup", //15
		"Minimize when task start", //16
		"Language",	//17
		"Adjustments", //18
		"Cancel", //19
		"Settings", //20
		"Current time",		//21
		"Reaction time",		//22
		"Hour",	//23
		"Min",	//24
		"Sec",	//25
		"Save the settings as default", //26
		"Operation mode",		//27
		"Timer",		//28
		"Alarm-clock",	//29
		"To do",		//30
		"Shutdown",		//31
		"Reboot",		//32
		"Lock",			//33
		"Alarm signal",	//34
		"After ...",	//35
		
		"Shutdown after ...", //36
		"Reboot after ...",		//37
		"Lock after ...",	//38
		"To give the alarm signal after ...", //39

		"Window close", //40
		"End process",	//41

		"Shutdown when windows close", //42
		"Reboot when windows close",	//43
		"Lock when windows close",	//44
		"Alarm signal when windows close", //45
		
		"Shutdown when end process", //46
		"Reboot when end process",	//47
		"Lock when end process",	//48
		"Alarm signal when end process", //49

		"Alarm signal",//50
		"Select signal",//51

		"Reset", //52

		"To do", //53

		"OK", //54
		"Cancel", //55
		
		"Immediately start the task" //56
	};

	//-------------------------------------------------------------
	char szCfgFile[MAX_PATH] = {'0'}; //название конфигурационного файла
	char szCurrentDir[MAX_PATH] = {'0'}; 
	char* ptr; //нужен чтобы получить название экзешника

	//полный путь к исполняемому файлу
	GetModuleFileNameA( NULL, szCfgFile, MAX_PATH );
	//убираем расширение .exe
	szCfgFile[strlen(szCfgFile)-4] = 0;

	strcpy(szCurrentDir, szCfgFile);

	strcat(szCfgFile, ".cfg");

	//выделить название файла
	ptr = strrchr(szCurrentDir, '\\') + 1; //+1 чтобы отбросить символ \, инача будет \filename.exe
	
	int result = (int)( ptr - szCurrentDir );
	szCurrentDir[result] = 0;
	//---------------------------------------------------------------
	char szLang[MAX_PATH], szTemp[512], Bufer[1024], szParam[32];

	// читаем из файла настроек установленный язык гуи
	GetPrivateProfileString("Common", "Language", "English", szLang, MAX_PATH, szCfgFile);

	//формируем полный путь к языковому файлу
	sprintf(szTemp, "%s\\%s.lng\0", szCurrentDir, szLang);

	//читаем все поля языкового файла
	for (int i = 0; i < MAX_PATH; i++) // Read all phrases to sText[] array
	{
		// Shape names for parameters, i.e. - lines numbers in the LNG-file
		sprintf(szParam, "%d\0", i);

		// Read next line in the LNG-file
		GetPrivateProfileString("Translations", szParam, sDefaultLanguage[i], Bufer, 512, szTemp);

		// As GetPrivateProfileString() function can't read
		// line data to the CString object directly, we have to 
		// perform additional determination:
		sLanguageText[i] = Bufer;

		// Change sublines "\t" and "\n" from the LNG-file
		//to standard C language format:
		sLanguageText[i].Replace("\\t", "\t");
		sLanguageText[i].Replace("\\n", "\n");
	}
}

void CMultilanguage::FillLanguageComboBox(/*HWND hwndDlg*/CWnd *wnd, int hwndDlgItem_UiLanguage)
{
	//------------------------------------------------------------
	char szCfgFile[MAX_PATH] = {'0'}; //название конфигурационного файла
	char szCurrentDir[MAX_PATH] = {'0'}; 
	char szExeFileName[MAX_PATH] = {'0'}; //название экзешника
	char* ptr; //нужен чтобы получить название экзешника

	//полный путь к исполняемому файлу
	GetModuleFileNameA( NULL, szCfgFile, MAX_PATH );
	//убираем расширение .exe
	szCfgFile[strlen(szCfgFile)-4] = 0;

	strcpy(szCurrentDir, szCfgFile);

	strcat(szCfgFile, ".cfg");

	//выделить название файла
	ptr = strrchr(szCurrentDir, '\\') + 1; //+1 чтобы отбросить символ \, инача будет \filename.exe
	
	int result = (int)( ptr - szCurrentDir );
	szCurrentDir[result] = 0;
	
	//---------------------------------------------------------------

	void *hSearch;
	WIN32_FIND_DATA wfd;
	char szFile[MAX_PATH] = {"0"};

	// искать языковой файл будем в тойже папке где экзешник
	SetCurrentDirectory(szCurrentDir);

	// Search all LNG-files in the current directory
	hSearch = FindFirstFile("*.lng", &wfd);

	// Read current interface language from actual program settings
	GetPrivateProfileString("Common", "Language", "English", m_szCurrentLang, 256, szCfgFile);

	if (hSearch != INVALID_HANDLE_VALUE)
	{
		do
		{
			strcpy(szFile, wfd.cFileName);

			// "Cut" file extension - ".LNG"
			szFile[strlen(szFile) - 4] = 0;

			// List LNG-file
			((CComboBox *)wnd->GetDlgItem(hwndDlgItem_UiLanguage))->AddString(szFile);
		}
		while (FindNextFile(hSearch, &wfd));
	}

	FindClose(hSearch);

	// Add default language
	if (((CComboBox *)wnd->GetDlgItem(hwndDlgItem_UiLanguage))->FindString(0, "English") == CB_ERR)
		((CComboBox *)wnd->GetDlgItem(hwndDlgItem_UiLanguage))->AddString("English");

	// Select current interface language
	int nLanguage = ((CComboBox *)wnd->GetDlgItem(hwndDlgItem_UiLanguage))->SelectString(0, m_szCurrentLang);

	// If it isn't listed, restore default language
	if (nLanguage == CB_ERR)
	{
		((CComboBox *)wnd->GetDlgItem( hwndDlgItem_UiLanguage))->SelectString(0, "English");
		nLanguage = 0;
	}
	///////////////////////////////////////////////////////////////////
}

void CMultilanguage::WriteSettingsFile(CWnd *wndDlg, int iDlgItemUiLanguage)
{
	//-------------------------------------------------------------
	char szCfgFileName[MAX_PATH];

	//полный путь к исполняемому файлу
	GetModuleFileNameA( NULL, szCfgFileName, MAX_PATH );
	//убираем расширение .exe
	szCfgFileName[strlen(szCfgFileName)-4] = 0;

	strcat(szCfgFileName, ".cfg");
	//---------------------------------------------------------------
	if (m_bGUILangChenged)
	{
		// Yes, it had
		CString csNewUiLanguage;
		wndDlg->GetDlgItemTextA(iDlgItemUiLanguage, csNewUiLanguage);

		// Save selected interface language
		WritePrivateProfileString("Common", "Language", csNewUiLanguage, szCfgFileName);
		
		m_bGUILangChenged = false;

		// Display the message to restart the program
		AfxMessageBox(sLanguageText[9]);
	}

}

void CMultilanguage::GetGUILanguage(char* chGUILanguage)
{
	strcpy(chGUILanguage, m_szCurrentLang);
}

void CMultilanguage::SetGUILangChenged(bool b)
{
	m_bGUILangChenged = b;
}

bool CMultilanguage::GetGUILangChenged()
{
	return m_bGUILangChenged;
}