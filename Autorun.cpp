#include "stdafx.h"
#include "Autorun.h"
#include "ShortCut.h"


CAutorun::CAutorun(void)
{
	m_RegRunSubKey = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	 
	//���������� ���� � ������������ �����
	GetModuleFileNameA( NULL, m_ExePath, MAX_PATH );
	//�������� �������� �����
	m_ExeFileName = strrchr(m_ExePath, '\\') + 1; //+1 ����� ��������� ������ \, ����� ����� \filename.exe
}


CAutorun::~CAutorun(void)
{
}


// �������� ���� � ������������
void CAutorun::AddReg(void)
{
	HKEY key;	
	
	//������� ������ � ������� 
	if (RegOpenKeyExA(HKEY_CURRENT_USER, m_RegRunSubKey, 0, KEY_SET_VALUE, &key)== ERROR_SUCCESS)
	{			
		//��������� � ������� ����� ���� � ���������
		RegSetValueExA(key, m_ExeFileName, 0, REG_SZ, (LPBYTE)m_ExePath, strlen(m_ExePath));
	}
	RegCloseKey(key);
}

// ������ ���� �� ������������
void CAutorun::RemoveReg(void)
{
	HKEY hKey;

	if (::RegOpenKeyEx(HKEY_CURRENT_USER, m_RegRunSubKey, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS)
	{
		RegDeleteValue(hKey, m_ExeFileName);
	}
	RegCloseKey(hKey);
}

// �������� ������������ ��� ���?
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

//������� ����� � ����� ������������
void CAutorun::Add(void)
{
	char* appData = getenv("APPDATA");
	
	//�������� ���� � ��������� 
	char szlnkFileName[MAX_PATH];
	char* ptr;

	//�������� ��� ��������� � �����������. 
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
		MessageBox(NULL, "����� � ��������� ������ � ����� Startup", "�����", MB_ICONERROR);
	}
}

bool CAutorun::IsAutorun(void)
{
	bool bResult = true;
	char* appData = getenv("APPDATA");

	//�������� ���� � ��������� 
	char szlnkFileName[MAX_PATH];
	char* ptr;

	//�������� ��� ��������� � �����������. 
	ptr = strrchr(m_ExePath, '\\')+1;

	strcpy(szlnkFileName, ptr);

	ptr = strrchr(szlnkFileName, '.');

	int result = (int)( ptr - szlnkFileName );
	szlnkFileName[result] = 0;

	strcat(szlnkFileName, ".lnk");

	CString csChortCut;
	csChortCut.Format("%s\\%s\\%s", appData, "Microsoft\\Windows\\Start Menu\\Programs\\Startup", szlnkFileName);

	//��������� ���� �� ����� � ����� ������������
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

	//�������� ���� � ��������� 
	char szlnkFileName[MAX_PATH];
	char* ptr;

	//�������� ��� ��������� � �����������. 
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
		TRACE("CAutorun::Remove(). ����� � �������� ������.\n");
	}
}
