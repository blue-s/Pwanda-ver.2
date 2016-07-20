#include "mon.h"

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

TCHAR * resultBuffer;
ULARGE_INTEGER g_tmStart;

typedef struct REGMON { 
	HKEY   hMainKey;
	LPTSTR szSubkey;
} REGMON, *PREGMON;

typedef struct _KEY_NAME_INFORMATION {
	ULONG NameLength;
	WCHAR Name[4096];
} KEY_NAME_INFORMATION, *PKEY_NAME_INFORMATION;

typedef enum _KEY_INFORMATION_CLASS {
	KeyBasicInformation            = 0,
	KeyNodeInformation             = 1,
	KeyFullInformation             = 2,
	KeyNameInformation             = 3,
	KeyCachedInformation           = 4,
	KeyFlagsInformation            = 5,
	KeyVirtualizationInformation   = 6,
	KeyHandleTagsInformation       = 7,
	MaxKeyInfoClass                = 8 
} KEY_INFORMATION_CLASS;

typedef NTSTATUS (WINAPI *ZWQUERYKEY)(
	HANDLE, 
	KEY_INFORMATION_CLASS, 
	PVOID, 
	ULONG, 
	PULONG);

ZWQUERYKEY ZwQueryKey;

// [4] ������Ʈ�� ��ȭ�� Ž���ϰ� Ű ���� �����͸� �����´� 
void QueryKey(HKEY hKey){

	TCHAR    achKey[MAX_KEY_LENGTH];	
	DWORD    cbName;					
	TCHAR    achClass[MAX_PATH] = _T("");	
	DWORD    cchClassName = MAX_PATH;	
	DWORD    cSubKeys=0;				
	DWORD    cbMaxSubKey;				
	DWORD    cchMaxClass;		
	DWORD    cValues;					
	DWORD    cchMaxValue;			
	DWORD    cbMaxValueData;		
	DWORD    cbSecurityDescriptor;		

	FILETIME ftWrite;
	ULARGE_INTEGER tmWrite;

	DWORD	i, retCode;
	HKEY	hNewKey;
	DWORD	cchValue = MAX_VALUE_NAME;

	TCHAR	achData[MAX_VALUE_NAME];
	DWORD	cchData = MAX_VALUE_NAME;

	TCHAR	path[MAX_VALUE_NAME] = _T("");
	TCHAR	search[MAX_VALUE_NAME] = _T("Roaming"); 
	TCHAR*	result = _T("");

	LPSTR lpresult = "";

	// ������Ʈ���� ���� ��, ������, ����Ű �̸��� ���� ���� �������� �Լ� 
	retCode = RegQueryInfoKey(
		hKey,						
		achClass,					
		&cchClassName,			
		NULL,					
		&cSubKeys,					
		&cbMaxSubKey,			
		&cchMaxClass,				
		&cValues,						
		&cchMaxValue,				
		&cbMaxValueData,				
		&cbSecurityDescriptor,			
		&ftWrite
		);				

	if(retCode == ERROR_SUCCESS)  
	{ 
		tmWrite.HighPart = ftWrite.dwHighDateTime;
		tmWrite.LowPart  = ftWrite.dwLowDateTime;

		// ���� �ֱ� ���� �ð��� start���� ũ�� ����� �� 
		if (tmWrite.QuadPart > g_tmStart.QuadPart)
		{
			// ������ ������Ʈ�� Ű�� ����(Values) ����ŭ
			for(i=0; i<cValues; i++) 
			{
				cchValue = MAX_VALUE_NAME;	// Values �ִ� ũ�� 
				achValue[0] = '\0';			// �� �ʱ�ȭ 
				cchData = MAX_VALUE_NAME;	// Data �ִ� ũ�� 
				achData[0] = '\0';			// ������ �ʱ�ȭ 

				// ������ ������Ʈ�� Ű�� ������ �� �����ϴ� �Լ� 
				retCode = RegEnumValue(
					hKey,					
					i,                      
					achValue, 
					&cchValue,					
					NULL,					
					NULL,					
					(LPBYTE)achData,	 				
					&cchData
					);					

				// ������Ʈ�� Ű�� Value �� Data �� ��� 
				if(retCode == ERROR_SUCCESS)
				{ 
					// achData�� �������� ���μ��� ��ΰ� ���������Ƿ� path�� ���� 
					memcpy(path, achData, MAX_VALUE_NAME);	

					// path ���ڿ����� search ���ڿ��� �ִ��� �˻� 
					if(result = _tcsstr(path, search)){
						resultBuffer = result + 8;
						ExtractProcess(3, resultBuffer);

						Output_Console(FOREGROUND_BLUE, _T("[REG_ADDED] %s \n"), result);
					}
				}
			}
		} // end for

		retCode = RegOpenKeyEx( hKey, achKey, 0, KEY_ALL_ACCESS, &hNewKey);

		if(retCode == ERROR_SUCCESS) 
		{ 
			RegCloseKey(hNewKey);
		}
	}
}

// [3] �ð� ������Ʈ
void UpdateTime(void)
{
	SYSTEMTIME st;
	FILETIME   ft;

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);

	g_tmStart.HighPart = ft.dwHighDateTime;
	g_tmStart.LowPart  = ft.dwLowDateTime;
}

// [2] ������ ����
DWORD WatchKey(PREGMON p)
{
	HANDLE hEvent;
	HKEY   hKey;
	LONG   ret;

	Output_Console(0, _T("Monitoring HKEY %x\\%s\n"), p->hMainKey, p->szSubkey); 

	// ������Ʈ�� Ű ���� (���� ���� ������ ����� �ڵ��� �����´�)
	ret = RegOpenKeyEx(
		p->hMainKey, 
		p->szSubkey, 
		0,
		KEY_ALL_ACCESS,
		&hKey
		);

	if (ret != ERROR_SUCCESS)	return -1;

	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (hEvent == NULL)			return -1;

	// WAIT_OBJECT_0: ���� 
	// ������ ���ᰡ ���� ������ 
	while(WaitForSingleObject(g_hStopEvent, 1) != WAIT_OBJECT_0)
	{
		// ���� ���� �ð��� �ý��� �ð����� ����
		UpdateTime();	

		ret = RegNotifyChangeKeyValue(hKey, 
			TRUE, 
			REG_CHANGE_FLAGS, 
			hEvent, 
			TRUE
			);

		// ���� ������ ��� 
		if (ret != ERROR_SUCCESS)
		{
			_tprintf(_T("Registry Open Failed!! \n"));
			break;
		}

		// ������ ������ ��ٷ��� �ϸ� �������´� 
		if (WaitForSingleObject(hEvent, INFINITE) == WAIT_FAILED)	break;

		QueryKey(hKey); // [4] Ű ���� Data�� ����
	}

	RegCloseKey(hKey);
	CloseHandle(hEvent);
	return 0;
}

// [1] ������Ʈ�� ����� ���� 
void StartRegistryMonitor(void)
{
	HMODULE hNtdll = GetModuleHandle(_T("ntdll.dll"));
	ZwQueryKey = (ZWQUERYKEY)GetProcAddress(hNtdll, "NtQueryKey");

	PREGMON p[1];

	p[0] = new REGMON;
	p[0]->hMainKey = HKEY_CURRENT_USER;
	p[0]->szSubkey = _T("SOFTWARE\\\Microsoft\\Windows\\CurrentVersion\\Run"); 

	g_hRegWatch[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WatchKey, p[0], 0, NULL);
}