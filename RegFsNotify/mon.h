#pragma once

#include <windows.h>
#include <tchar.h>

#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)

#define FILE_CHANGE_FLAGS FILE_NOTIFY_CHANGE_FILE_NAME |\
	FILE_NOTIFY_CHANGE_DIR_NAME |\
	FILE_NOTIFY_CHANGE_ATTRIBUTES |\
	FILE_NOTIFY_CHANGE_SIZE |\
	FILE_NOTIFY_CHANGE_CREATION |\
	FILE_NOTIFY_CHANGE_SECURITY

#define REG_CHANGE_FLAGS REG_NOTIFY_CHANGE_NAME |\
	REG_NOTIFY_CHANGE_LAST_SET
void Output_Roaming(USHORT Color, LPTSTR format, ... );
void Output_Prefetch(USHORT Color, LPTSTR format, ... );
void StartFileMonitor(void);
void StartRegistryMonitor(void);

extern HANDLE  g_hStopEvent;
extern HANDLE  g_hFile;
extern HANDLE  g_hRegWatch[2];

// whitelisted filenames or paths
//ȭ��Ʈ����Ʈ
static LPTSTR Roaming_szAllow[] = {       //asdf_szAllow -->Roaming_szAllow[] 
	_T("Roaming\\"),
	_T("AppData\Roaming\\"),

};

static LPTSTR Prefetch_szAllow[] = {      //qwer_szAllow --> Prefetch_szAllow[]
	_T("Windows\Prefetch\\"),
	_T("Prefetch\\"),
};

// return true if szFile is in the g_szAllow list

static BOOL RoamingWhitelisted(LPTSTR szFile)              
{
	for(int i=0; i<sizeof(Roaming_szAllow)/sizeof(LPTSTR); i++)
	{
		if (_tcsstr(szFile, Roaming_szAllow[i]) != NULL) //ȭ��Ʈ ����Ʈ�� ��ġ�ϸ� ���� �־, NULL�� �ƴϴϱ� ��
			return TRUE;							// ��ġ���� ������, NULL�̴ϱ� ����.
	}
	return FALSE;
}

static BOOL PrefetchWhitelisted(LPTSTR szFile)                
{
	for(int i=0; i<sizeof(Prefetch_szAllow)/sizeof(LPTSTR); i++)
	{
		if (_tcsstr(szFile, Prefetch_szAllow[i]) != NULL) 
			return TRUE;
	}
	return FALSE;
}