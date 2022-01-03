#pragma once
#include "StdAfx.h"

class CMultilanguage
{
public:
	CMultilanguage();
	//~CMultilanguage();

	//массив для хранения строк различных языков
	static CString sLanguageText[MAX_PATH];

	void ReadGUILanguageFile();
	void FillLanguageComboBox(/*HWND hwndDlg*/CWnd* , int );
	void WriteSettingsFile(CWnd* , int);
	void GetGUILanguage(char*);
	void SetGUILangChenged(bool);
	bool GetGUILangChenged();

private:
	char m_szCurrentLang[260];
	bool m_bGUILangChenged;
};