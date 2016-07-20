#pragma once

// ������� ����
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>

// [2] �׽�Ʈ�� ���� �߰�
#include <locale.h>	
#include <shlobj.h>					// for IsUserAnAdmin() function
#pragma comment(lib, "shell32.lib")	// for IsUserAnAdmin() function
void checkState();			// ���� �Լ� �߰�

// [3] �׽�Ʈ�� ���� �߰�
#include "tlhelp32.h"

#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#define MAX_VALUE_NAME 16383
#define FILE_CHANGE_FLAGS FILE_NOTIFY_CHANGE_FILE_NAME |\
	FILE_NOTIFY_CHANGE_DIR_NAME |\
	FILE_NOTIFY_CHANGE_ATTRIBUTES |\
	FILE_NOTIFY_CHANGE_SIZE |\
	FILE_NOTIFY_CHANGE_CREATION |\
	FILE_NOTIFY_CHANGE_SECURITY

#define REG_CHANGE_FLAGS REG_NOTIFY_CHANGE_NAME |\
	REG_NOTIFY_CHANGE_LAST_SET


// RegFsNotify.cpp
extern HANDLE  g_hStopEvent;	// �̺�Ʈ �ٷ�� �ڵ�
extern HANDLE  g_hFile;			// ���� �ٷ�� �ڵ�
extern HANDLE  g_hRegWatch[2];

void Output_Console(USHORT Color, LPTSTR format, ... );
void Output_File(LPTSTR format, ...);
void StartFileMonitor(void);
void StartRegistryMonitor(void);

// file.cpp
#define MAX_DRIVES 24
#define MAX_VALUE_NAME 16383

// reg.cpp
extern TCHAR * resultBuffer;
static TCHAR	achValue[MAX_VALUE_NAME];	//regDelete.cpp

// extractProcessName.cpp
static int roamingNum = 0;
static int prefetchNum = 0;
static int runNum = 0;
static TCHAR* roamingList[100];
static TCHAR* prefetchList[100];
static TCHAR* runList[100];

static TCHAR* roamingPath[100];
static TCHAR* prefetchPath[100];

int compareFile(TCHAR* list[], int size, TCHAR* fileName);
void ExtractProcess(DWORD, TCHAR *, TCHAR*);

// processControl.cpp
void printProcessInformation(PROCESSENTRY32 pe);
DWORD FindProcessID(LPCTSTR szProcessName);
void ListProcessInfo(void);
BOOL KillProcess(TCHAR* TargetProcess);

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