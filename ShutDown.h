#pragma once
//#include "stdafx.h"

int GetOSVersion()
{
	int OSVersion = NULL;

   OSVERSIONINFOEX osvi;
   BOOL bOsVersionInfoEx;

   // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
   // If that fails, try using the OSVERSIONINFO structure.

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
   {
	  osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
	  if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
		 return FALSE;
   }

   switch (osvi.dwPlatformId)
   {
		   // Test for the Windows Me/98/95.
	  case VER_PLATFORM_WIN32_WINDOWS:
		 OSVersion = 1; //OSVersion = 1 если ОС от 95 до Ме
		 break;

	  // Test for the Windows NT product family.
	  case VER_PLATFORM_WIN32_NT:
		  OSVersion = 2; //OSVersion = 2 если ОС семейства NT
		  break;   
   }
   return OSVersion;

   /////////////////////////////////////////////////////////////////
   //BOOL CCompConfDlg::GetOSVersion()
   //{
	  // OSVERSIONINFOEX osvi;
	  // BOOL bOsVersionInfoEx;

	  // // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	  // // If that fails, try using the OSVERSIONINFO structure.

	  // ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	  // osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	  // if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	  // {
		 //  osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);

		 //  if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			//   return FALSE;
	  // }

	  // switch (osvi.dwPlatformId)
	  // {
		 //  // Test for the Windows NT product family.
	  // case VER_PLATFORM_WIN32_NT:

		 //  // Test for the specific product.
		 //  if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
			//   m_strWinName = "Microsoft Windows Server 2003 ";

		 //  if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
			//   m_strWinName = "Microsoft Windows XP ";

		 //  if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
			//   m_strWinName = "Microsoft Windows 2000 ";		

		 //  if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
			//   m_strWinName = "Microsoft Windows Vista ";

		 //  // Test for specific product on Windows NT 4.0 SP6 and later.
		 //  if( bOsVersionInfoEx )
		 //  {
			//   // Test for the workstation type.
			//   if ( osvi.wProductType == VER_NT_WORKSTATION )
			//   {
			//	   if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
			//		   m_strWinName += "Home Edition ";
			//	   else m_strWinName += "Professional ";
			//   }
		 //  }
	  // }

	  // m_strWinName += osvi.szCSDVersion;

	  // UpdateData(FALSE);

	  // //=======================================================
	  // return true;
   //}
}

BOOL ShutDown()
{
	int OSVersion = NULL;

	OSVersion = GetOSVersion();

	if ( !OSVersion )
		MessageBox(NULL,"!GetOSVersion()","ShutDown()", MB_OK|MB_ICONERROR);
		
	switch ( OSVersion )
	{
	case 1: //Windows Me/98/95.
		{
			if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0))
			{
				MessageBox(NULL,"!ExitWindowsEx()","ShutDown() case: Windows Me/98/95", MB_OK|MB_ICONERROR);
				return FALSE; 
			}
		}
		break;

	case 2: //windows XP
		{
			HANDLE hToken; 
			TOKEN_PRIVILEGES tkp; 
				 
			// Get a token for this process. 
				 
			if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
			{
				MessageBox(NULL,"!OpenProcessToken()","ShutDown()", MB_OK|MB_ICONERROR);
				return FALSE;
			}
				 
			// Get the LUID for the shutdown privilege.		 
			LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
				 
			tkp.PrivilegeCount = 1;  // one privilege to set    
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
				 
			// Get the shutdown privilege for this process. 		 
			AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 
				 
			if (GetLastError() != ERROR_SUCCESS) 
			{
				MessageBox(NULL,"!AdjustTokenPrivileges()","ShutDown()", MB_OK|MB_ICONERROR);
				return FALSE; 
			}
				 
			// Shut down the system and force all applications to close. 		 
			if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0))
			{
				MessageBox(NULL,"!ExitWindowsEx()","ShutDown() case: Windows NT family.", MB_OK|MB_ICONERROR);
				return FALSE; 
			}

			return TRUE;
		}
	}
return TRUE;
}

BOOL ReBoot()
{
	int OSVersion = NULL;

	OSVersion = GetOSVersion();

	if ( !OSVersion )
		MessageBox(NULL,"!GetOSVersion()","ShutDown()", MB_OK|MB_ICONERROR);
		
	switch ( OSVersion )
	{
	case 1: //Windows Me/98/95.
		{
			if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0))
			{
				MessageBox(NULL,"!ExitWindowsEx()","ShutDown() case: Windows Me/98/95", MB_OK|MB_ICONERROR);
				return FALSE; 
			}
		}
		break;

	case 2:
		{
			HANDLE hToken; 
			TOKEN_PRIVILEGES tkp; 
				 
			// Get a token for this process. 
				 
			if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
			{
				MessageBox(NULL,"!OpenProcessToken()","ShutDown()", MB_OK|MB_ICONERROR);
				return FALSE;
			}
				 
			// Get the LUID for the shutdown privilege.		 
			LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
				 
			tkp.PrivilegeCount = 1;  // one privilege to set    
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
				 
			// Get the shutdown privilege for this process. 		 
			AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 
				 
			if (GetLastError() != ERROR_SUCCESS) 
			{
				MessageBox(NULL,"!AdjustTokenPrivileges()","ShutDown()", MB_OK|MB_ICONERROR);
				return FALSE; 
			}
				 
			// Shut down the system and force all applications to close. 		 
			if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0))
			{
				MessageBox(NULL,"!ExitWindowsEx()","ShutDown() case: Windows NT family.", MB_OK|MB_ICONERROR);
				return FALSE; 
			}

			return TRUE;
		}
	}
return TRUE;
}

