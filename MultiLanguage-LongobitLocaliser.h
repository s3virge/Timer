#pragma once

class MultiLanguage
{
private:
	struct CMultiLangLang 
	{
		CString cCode;
		CString cLangName;
	};
	static int iSelectedLang;
	static UINT m_nMaxLangs;//, m_nStart;
	static HKEY m_hKey;
	static CString m_strEntry, m_strSection;
	
public:
	static CArray<CMultiLangLang, CMultiLangLang> m_arrNames;

	MultiLanguage(void);
	~MultiLanguage(void);

	static bool InitLocalization( /*UINT nStart, command id of first menu entry*/
		UINT nMaxLangs, //maximum number of languages
		HKEY hkey,
		LPCTSTR lpszSection,
		LPCTSTR lpszEntry);
	static void PopulateLangList();
	static CString ReadRegistryLang(HKEY hkey, LPCTSTR lpszSection, LPCTSTR lpszEntry);
	static bool WriteRegistryLang(HKEY hkey, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);
	static bool OnChangeLanguage(UINT nID);
	static CString GetCurLanguageCode();
	static bool MultiLangLoadLocalization();
	static bool FillLCIDInfo(DWORD lcid);
	static bool CheckPathLCID(LPCTSTR szLangCode, DWORD lcid);
	static bool GetLangFormatString(LPTSTR szFormat, HINSTANCE hInstance = 0);
	static bool IsUnique(CString szLangCode);
};
