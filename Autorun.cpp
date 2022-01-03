#include "stdafx.h"
#include "Autorun.h"
#include "ShortCut.h"


CAutorun::CAutorun(void)
{
	m_RegRunSubKey = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	 
	//определить путь к исполн€емому файлу
	GetModuleFileNameA( NULL, m_ExePath, MAX_PATH );
	//выделить название файла
	m_ExeFileName = strrchr(m_ExePath, '\\') + 1; //+1 чтобы отбросить символ \, инача будет \filename.exe
}


CAutorun::~CAutorun(void)
{
}


// добавить себ€ в автозагрузку
void CAutorun::AddReg(void)
{
	HKEY key;	
	
	//сделать запись в реестре 
	if (RegOpenKeyExA(HKEY_CURRENT_USER, m_RegRunSubKey, 0, KEY_SET_VALUE, &key)== ERROR_SUCCESS)
	{			
		//сохран€ем в реестре новый путь к экзешнику
		RegSetValueExA(key, m_ExeFileName, 0, REG_SZ, (LPBYTE)m_ExePath, strlen(m_ExePath));
	}
	RegCloseKey(key);
}

// убрать себ€ из автозагрузки
void CAutorun::RemoveReg(void)
{
	HKEY hKey;

	if (::RegOpenKeyEx(HKEY_CURRENT_USER, m_RegRunSubKey, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS)
	{
		RegDeleteValue(hKey, m_ExeFileName);
	}
	RegCloseKey(hKey);
}

// включена автозагрузка или нет?
bool CAutorun::IsAutorunReg(void)
{
	HKEY hKey;

	if (RegOpenKeyExA(HKEY_CURRENT_USER, m_RegRunSubKey, 0, KEY_READ, &hKey)== ERROR_SUCCESS)
	{	
		if (RegQueryValueExA(hKey, m_ExeFileName, 0, NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			return true;
		}
	}

	return false;
}

//создаем €рлык в папке автозагрузка
void CAutorun::Add(void)
{
	char* appData = getenv("APPDATA");
	
	//получить путь к программе 
	char szlnkFileName[MAX_PATH];
	char* ptr;

	//выдел€ем им€ программы с расширением. 
	ptr = strrchr(m_ExePath, '\\')+1;

	strcpy(szlnkFileName, ptr);

	ptr = strrchr(szlnkFileName, '.');

	int result = (int)( ptr - szlnkFileName );
	szlnkFileName[result] = 0;

	strcat(szlnkFileName, ".lnk");

	CStringW csChortCut;
	csChortCut.Format(L"%S\\%S\\%S\0", appData, "Microsoft\\Windows\\Start Menu\\Programs\\Startup", szlnkFileName);
		
	bool res = CreateShortCut(csChortCut.GetBuffer(), m_ExePath, NULL, NULL, NULL, SW_NORMAL, NULL, NULL);

	if (!res)
	{
		MessageBox(NULL, "ќблом с созданием €рлыка в папке Startup", "ќблом", MB_ICONERROR);
	}
}

bool CAutorun::IsAutorun(void)
{
	bool bResult = true;
	char* appData = getenv("APPDATA");

	//получить путь к программе 
	char szlnkFileName[MAX_PATH];
	char* ptr;

	//выдел€ем им€ программы с расширением. 
	ptr = strrchr(m_ExePath, '\\')+1;

	strcpy(szlnkFileName, ptr);

	ptr = strrchr(szlnkFileName, '.');

	int result = (int)( ptr - szlnkFileName );
	szlnkFileName[result] = 0;

	strcat(szlnkFileName, ".lnk");

	CString csChortCut;
	csChortCut.Format("%s\\%s\\%s", appData, "Microsoft\\Windows\\Start Menu\\Programs\\Startup", szlnkFileName);

	//проверить есть ли €рлык в папке јвтозагрузка
	HANDLE hFile = CreateFile(csChortCut.GetBuffer(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	
	if (GetLastError() == ERROR_FILE_NOT_FOUND)
	{
		bResult = false;
	}

	CloseHandle(hFile);

	return bResult;
}

void CAutorun::Remove(void)
{
	bool bResult = true;
	char* appData = getenv("APPDATA");

	//получить путь к программе 
	char szlnkFileName[MAX_PATH];
	char* ptr;

	//выдел€ем им€ программы с расширением. 
	ptr = strrchr(m_ExePath, '\\')+1;

	strcpy(szlnkFileName, ptr);

	ptr = strrchr(szlnkFileName, '.');

	int result = (int)( ptr - szlnkFileName );
	szlnkFileName[result] = 0;

	strcat(szlnkFileName, ".lnk");

	CString csChortCut;
	csChortCut.Format("%s\\%s\\%s", appData, "Microsoft\\Windows\\Start Menu\\Programs\\Startup", szlnkFileName);

	if (DeleteFile(csChortCut) == 0)
	{
		TRACE("CAutorun::Remove(). ќблом с удаление €рлыка.\n");
	}
}
