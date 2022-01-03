#include "StdAfx.h"
#include "MultiLanguage.h"
#include <Shlwapi.h>
#include <locale.h>

int MultiLanguage::iSelectedLang = 0;
UINT MultiLanguage::m_nMaxLangs = 0;
HKEY MultiLanguage::m_hKey = 0;
CString MultiLanguage::m_strEntry = "";
CString MultiLanguage::m_strSection = "";
CArray<MultiLanguage::CMultiLangLang, MultiLanguage::CMultiLangLang> MultiLanguage::m_arrNames;

BOOL CALLBACK EnumLocalesProc( LPTSTR lpLocaleString /* locale identifier string */);

MultiLanguage::MultiLanguage(void)
{
}

MultiLanguage::~MultiLanguage(void)
{
}

bool MultiLanguage::InitLocalization(/*UINT nStart, command id of first menu entry*/
                              UINT nMaxLangs, //maximum number of languages
                              HKEY hkey,	  // в какой раздел реестра будет сохранена информация
                              LPCTSTR lpszSection, //
                              LPCTSTR lpszEntry) // название параметра
{
	ASSERT(nMaxLangs != 0);
	m_nMaxLangs = nMaxLangs;

	m_hKey = hkey;
	m_strEntry = lpszEntry;
	m_strSection = lpszSection;

	//узнаем какие языки доступны для нашего приложения
	PopulateLangList();

	CString cLang = ReadRegistryLang(m_hKey, m_strSection, m_strEntry);

	iSelectedLang = 0;

	//если получен из реестра код языка
	if(!cLang.IsEmpty())
	{
		for(int i = 1; i < m_arrNames.GetCount(); i++)
		{
			TRACE("Available language Dll -> %s\n", m_arrNames[i].cLangName);
			
			if(cLang == m_arrNames[i].cCode)
				iSelectedLang = i;
		}
	}

	//загружаем, соответствующему выбранному языку, библиотеку
	MultiLangLoadLocalization();

	return true;
}

void  MultiLanguage::PopulateLangList()
{	
  CMultiLangLang lang;
  
  //язык по умолчанию
  lang.cCode=_T("DEF");
  lang.cLangName=_T("English");
  
  m_arrNames.Add(lang);

  //Enumerates the locales that are either installed on or supported by an operating system
  EnumSystemLocales((LOCALE_ENUMPROC)EnumLocalesProc, LCID_SUPPORTED);
}

BOOL CALLBACK EnumLocalesProc( LPTSTR lpLocaleString   /*locale identifier string*/)
{
  DWORD lcid; //locale identifier - идентификатор территории

  //The sscanf function reads data from buffer into the location given by each argument
  //читаем информацию из lpLocaleString, приводим ее к соответствующему виду _T("%x"), сохраняем результат в &lcid
  _stscanf(lpLocaleString, _T("%x"), &lcid);
  
  /*TRACE("lcid = %d\n", lcid);
  TRACE("lpLocaleString = %s\n", lpLocaleString);*/

  MultiLanguage::FillLCIDInfo(lcid);

  return TRUE;
}

CString MultiLanguage::ReadRegistryLang(HKEY hkey, LPCTSTR lpszSection, LPCTSTR lpszEntry)
{
  ASSERT(lpszSection != NULL);
  ASSERT(lpszEntry != NULL);

  HKEY hSecKey = NULL;

  DWORD dw;
  if(RegCreateKeyEx(hkey, lpszSection, 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hSecKey, &dw)!=ERROR_SUCCESS)
    return _T("");

  CString strValue;
  DWORD dwType, dwCount;
  LONG lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType, NULL, &dwCount);
  
  if (lResult == ERROR_SUCCESS)
  {
    ASSERT(dwType == REG_SZ);
  
	lResult = RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
    strValue.ReleaseBuffer();
  }

  RegCloseKey(hSecKey);

  if (lResult == ERROR_SUCCESS)
  {
    ASSERT(dwType == REG_SZ);
    return strValue;
  }

  return _T("");
}

bool MultiLanguage::WriteRegistryLang(HKEY hkey, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
{
  ASSERT(lpszSection != NULL);
  ASSERT(lpszEntry != NULL);

  HKEY hSecKey = NULL;

  DWORD dw;
  if(RegCreateKeyEx(hkey, lpszSection, 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hSecKey, &dw)!=ERROR_SUCCESS)
    return FALSE;

  if (hSecKey == NULL)
    return FALSE;

  LONG lResult = RegSetValueEx(hSecKey, lpszEntry, NULL, REG_SZ, (LPBYTE)lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR));
  
  RegCloseKey(hSecKey);
  
  return lResult == ERROR_SUCCESS;
}

bool MultiLanguage::OnChangeLanguage(UINT nID)
{
	WriteRegistryLang(m_hKey, m_strSection, m_strEntry, m_arrNames[nID].cCode);

	return false;
}

CString MultiLanguage::GetCurLanguageCode()
{
	return ReadRegistryLang(m_hKey, m_strSection, m_strEntry);
}
bool MultiLanguage::GetLangFormatString(LPTSTR szFormat, HINSTANCE hInstance)
{
  LPTSTR pszExtension;

  //Retrieves the fully-qualified path for the file that contains the specified module. 
  //The module must have been loaded by the current process.
  /*
  hInstance -  A handle to the loaded module whose path is being requested. 
  If this parameter is NULL, GetModuleFileName retrieves the path
  of the executable file of the current process.
  */
  int ret = ::GetModuleFileName(hInstance, szFormat, MAX_PATH);
  /*  после выполнения GetModuleFileName(), szFormat содержит полный путь к запущенному текущему процессу  */
  
  if(ret == 0 || ret == MAX_PATH)
  {
    ASSERT(FALSE);
    return false;
  }
  
  //Searches a path for an extension
  //находим в пути к процессу его расширение
  //PathFindExtension() Returns the address of the "."
  pszExtension = PathFindExtension(szFormat);

  TCHAR szExt[] = _T(".%s");
  
  //if((pszExtension - szFormat + _countof(szExt)) > MAX_PATH)
  /*{
    ASSERT(FALSE);
    return false;
  }*/

  //This function copies a string to a buffer.
  //копируем по адресу pszExtension новое расширение szExt
  lstrcpy(pszExtension, szExt);
  return true;
}

//проверяем нет ли повторений в массиве m_arrNames
bool MultiLanguage::IsUnique(CString szLangCode)
{
  int iLang;
  
  for (iLang = 0; iLang < m_arrNames.GetCount(); iLang++)
  {
    if(m_arrNames[iLang].cCode == szLangCode)
      return false;
  }
  return true;
}

bool MultiLanguage::FillLCIDInfo(DWORD lcid)
{
  TCHAR szLangCode[4]; //Pointer to a buffer in which GetLocaleInfo() retrieves the requested locale information
  int nResult;

  //Retrieves information about a locale specified by identifier. 
  //locale identifier - идентификатор территории
  //LOCALE_SABBREVLANGNAME - Abbreviated name of the language
  //узнаем абривиатуру языка
  nResult = ::GetLocaleInfo(lcid, LOCALE_SABBREVLANGNAME , szLangCode, 4);

  if ( !nResult )
    return false;
  
  ASSERT( nResult == 4 );

  //проверяем наличие языковай библиотеки с расширение из трех символов после точки (MyApp.RUS)
  CheckPathLCID(szLangCode, lcid);

  szLangCode[2]=L'\0';

  //проверяем наличие языковай библиотеки с расширение из двух символов после точки (MyApp.RU)
  CheckPathLCID(szLangCode, lcid);

  return TRUE;
}

bool MultiLanguage::CheckPathLCID(LPCTSTR szLangCode, DWORD lcid)
{
	//абривиатура языка не должна повторяться в массиве m_arrNames
  if(!IsUnique(szLangCode))
    return FALSE;

  int nResult;
  TCHAR szLangDLL[_MAX_PATH+14];

  TCHAR szFormat[MAX_PATH] = {0};
  //получаем путь к текущему процессу и заменяем расширение на ".%s"
  GetLangFormatString(szFormat, NULL);

  //Writes formatted data to a string
  //форматируем строку
  int ret = _sntprintf(szLangDLL, _countof(szLangDLL), szFormat, szLangCode);
  
  if(ret == -1 || ret >= _countof(szLangDLL))
  {
    ASSERT(FALSE);
    return NULL;
  }

  TRACE("проверяем наличие языковой библиотеки здесь -> %s\n", szLangDLL);

  //если такой файл существует, то продолжаем работать
  if(::PathFileExists(szLangDLL))
  {
    CMultiLangLang mlang;
    mlang.cCode = szLangCode;

	TRACE("нашли языковую библиотеку -> %s\n", szLangDLL);

	//nResult - содержит количество символов в названии языка
	nResult = ::GetLocaleInfo(lcid, LOCALE_SENGLANGUAGE, NULL, 0);

	TCHAR* szLangName = new TCHAR[nResult];
	//узнаем полное английское имя языка
	/* LOCALE_SENGLANGUAGE - Full English name of the language from the International Organization for Standardization (ISO) */
    nResult = ::GetLocaleInfo(lcid, LOCALE_SENGLANGUAGE, szLangName, nResult);

	/* LOCALE_SENGCOUNTRY - Full English name of the country/region */
    nResult = ::GetLocaleInfo(lcid, LOCALE_SENGCOUNTRY, NULL, 0);
    TCHAR* szCountryName = new TCHAR[nResult];
    nResult = ::GetLocaleInfo(lcid, LOCALE_SENGCOUNTRY, szCountryName, nResult);

	/*_tcslen - Get the length of a string, using the current locale or a specified locale. */
    if(_tcslen(szLangCode) != 2)
      mlang.cLangName.Format(_T("%s (%s)"), szLangName, szCountryName);
    else
      mlang.cLangName.Format(_T("%s"), szLangName);

	//такая языковая библиотека существует, сохраняем ее в массиве доступных языков
    m_arrNames.Add(mlang);

    delete[] szLangName;
    delete[] szCountryName;     
  }

  return TRUE;
}

bool MultiLanguage::MultiLangLoadLocalization()
{
  TCHAR szLangDLL[_MAX_PATH+14];

  if(!iSelectedLang)
    return true;

  CString cLangCode = m_arrNames[iSelectedLang].cCode;

  TCHAR szFormat[MAX_PATH];
  GetLangFormatString(szFormat);

  int ret = _sntprintf(szLangDLL,_countof(szLangDLL),szFormat,cLangCode);

  if(ret == -1 || ret >= _countof(szLangDLL))
  {
    ASSERT(false);
    return false;
  }

  HINSTANCE hInstance;
  hInstance = ::LoadLibrary(szLangDLL);

  if(hInstance)
  {
    AfxSetResourceHandle(hInstance);
    return true;
  }

  return false;
}