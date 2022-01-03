#include "stdafx.h"
#include "DefaultSettings.h"


CDefaulSettings::CDefaulSettings(void):
csDefSettingsHour( "Hour"),
csDefSettingsMinute ("Minutes"),
csDefSettingsSeconds ("Seconds"),
csDefSettingsOperationMode ("OperationMode"),
csDefSettingsToDo ("ToDo"),
csDefSettingsToDoAfter ("ToDoAfter"),
csDefSettingsRegSubKey ("SettingsAsDefault")
{
	/////////////// выделяем имя программы //////////////////
	char szFileName[MAX_PATH] = {'0'}; //название конфигурационного файла
	char* ptr; //нужен чтобы получить название экзешника

	//полный путь к исполняемому файлу
	GetModuleFileNameA( NULL, szFileName, MAX_PATH );
	//убираем расширение .exe
	szFileName[strlen(szFileName)-4] = 0;

	//выделить название файла
	ptr = strrchr(szFileName, '\\') + 1; //+1 чтобы отбросить символ \, инача будет \filename.exe
	///////////////////////////////////////////////////////////

	m_RegSubKey.Format("SOFTWARE\\%s\\%s", ptr, csDefSettingsRegSubKey);
	
	//нельзя в конструкторе инициализировать константные члены
	//можно только в списке инициализации	
}


CDefaulSettings::~CDefaulSettings(void)
{
}


//true - установлены настройки по умолчанию, false - настроек нет
bool CDefaulSettings::IsDefaultSettings(void)
{
	HKEY defHkey;
	//проверяем в реестре наличие ключа csDefSettingsRegKeyName
	//если ключ есть
	if (RegOpenKey(HKEY_CURRENT_USER, m_RegSubKey, &defHkey)== ERROR_SUCCESS)
	{		
		RegCloseKey(defHkey);
		return true;
	}
	//иначе нет
	return false;

}

bool CDefaulSettings::WriteSettings(int iHours, int iMin, int iSec, int iOperetionMode, int iToDo, int iToDoAfter)
{
	/*
	//тут будем сохранять все параметры в реестр
	//установленное время срабатывания
	//режим работы
	//что сделать
	//что сделать после...
	*/

	HKEY key;	

	//сделать запись в реестре 
	if (RegCreateKeyEx(HKEY_CURRENT_USER, m_RegSubKey, 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, NULL)== ERROR_SUCCESS)
	{			
		//сохраняем в реестре 
		RegSetValueEx(key, csDefSettingsHour, 0, REG_DWORD, (BYTE*)&iHours, (DWORD)sizeof(int));
		RegSetValueEx(key, csDefSettingsMinute, 0, REG_DWORD, (BYTE*)&iMin, (DWORD)sizeof(int));
		RegSetValueEx(key, csDefSettingsSeconds, 0, REG_DWORD, (BYTE*)&iSec, (DWORD)sizeof(int));
		RegSetValueEx(key, csDefSettingsOperationMode, 0, REG_DWORD, (BYTE*)&iOperetionMode, (DWORD)sizeof(int));
		RegSetValueEx(key, csDefSettingsToDo, 0, REG_DWORD, (BYTE*)&iToDo, (DWORD)sizeof(int));
		RegSetValueEx(key, csDefSettingsToDoAfter, 0, REG_DWORD, (BYTE*)&iToDoAfter, (DWORD)sizeof(int));
					
		RegCloseKey(key);
		
		return true;
	}	

	return false;
}

bool CDefaulSettings::ReadSettings(int& outHour, int& outMinute, int& outSeconds, int& outOperationMode, int& outToDo, int& outToDoAfter)
{
	DWORD dwBuffSize = sizeof(int);
	DWORD pdwType	= REG_DWORD;

	//получаем значение часов
	if (SHGetValue(HKEY_CURRENT_USER, m_RegSubKey, csDefSettingsHour,			&pdwType, (void*) &outHour, (DWORD*) &dwBuffSize)!=ERROR_SUCCESS)
		goto error_exit;
	
	//получаем значение минут
	if (SHGetValue(HKEY_CURRENT_USER, m_RegSubKey, csDefSettingsMinute,		&pdwType, (void*) &outMinute, (DWORD*) &dwBuffSize)!=ERROR_SUCCESS)
		goto error_exit;

	//получаем значение секунд
	if (SHGetValue(HKEY_CURRENT_USER, m_RegSubKey, csDefSettingsSeconds,		&pdwType, (void*) &outSeconds, (DWORD*) &dwBuffSize)!=ERROR_SUCCESS)
		goto error_exit;

	//режим работы
	if (SHGetValue(HKEY_CURRENT_USER, m_RegSubKey, csDefSettingsOperationMode,	&pdwType, (void*) &outOperationMode, (DWORD*) &dwBuffSize)!=ERROR_SUCCESS)
		goto error_exit;

	//что сделать
	if (SHGetValue(HKEY_CURRENT_USER, m_RegSubKey, csDefSettingsToDo,			&pdwType, (void*) &outToDo, (DWORD*) &dwBuffSize)!=ERROR_SUCCESS)
		goto error_exit;

	//что сделать после
	if (SHGetValue(HKEY_CURRENT_USER, m_RegSubKey, csDefSettingsToDoAfter,		&pdwType, (void*) &outToDoAfter, (DWORD*) &dwBuffSize)!=ERROR_SUCCESS)
		goto error_exit;

	//все в порядке
	return true;

	error_exit:
	MessageBox(NULL, "Error SHGetValue() in CDefaulSettings::ReadSettings()", ERROR, MB_ICONERROR);
	return false;	
}

void CDefaulSettings::ResetSettings()
{
	RegDeleteKey(HKEY_CURRENT_USER, m_RegSubKey);
}