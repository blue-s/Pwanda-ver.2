#pragma once

// ��ġ�� ��

// ������� ����
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <tlhelp32.h>
#include <time.h>

// [2] �׽�Ʈ�� ���� �߰�
#include <locale.h>	
#include <shlobj.h>					// for IsUserAnAdmin() function
#pragma comment(lib, "shell32.lib")	// for IsUserAnAdmin() function
void checkState();			// ���� �Լ� �߰�

// [3] �׽�Ʈ�� ���� �߰�
#include "tlhelp32.h"
#define DEF_PROC_NAME	(L"RegFsNotify.exe")	//(L"svchost.exe")
#define DEF_DLL_NAME	(L"myhack.dll")
int setUser();				// ����� ���� �Լ� �߰�

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


// reg.cpp
extern TCHAR * resultBuffer;
static TCHAR achValue[MAX_VALUE_NAME];


// extractProcessName.cpp
static int roamingNum = 0;
static int prefetchNum = 0;
static int runNum = 0;
static TCHAR* roamingList[100];
static TCHAR* prefetchList[100];
static TCHAR* runList[100];
static int dummy_sense_count=0;  //�������� ��ȭ���� Ƚ��

void ListPrint(void);
BOOL compare(void);
void ExtractProcess(DWORD, TCHAR *);


// setUser.cpp
BOOL SetPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);	// setPrivilege


// processControl.cpp
DWORD FindProcessID(LPCTSTR szProcessName);			// ���ϴ� ���μ����� PID ������
void printProcessInformation(PROCESSENTRY32 pe);	// ���μ��� ���� ���
void ListProcessInfo(void);							// ���μ��� ��� ���
BOOL KillProcess(TCHAR* TargetProcess);				// ���μ��� ���̱�


// createDummy.cpp
int makeDummy();
void getUserName();
void writeDummy();

extern TCHAR * dummyWhite[6];
extern int dummyNum;
extern HANDLE hFile;


// autoRun.cpp
void SetRegistyStartProgram();


// deleteFileReg.cpp
BOOL regDel(TCHAR * reg);


// ******************* ȭ��Ʈ����Ʈ *************************

// ȭ��Ʈ ����Ʈ ���
static LPTSTR Roaming_szAllow[] = { 
	_T("Roaming\\"),
	_T("AppData\Roaming\\"),
	_T("Users\Default\AppData\Roaming\\"),

};
static LPTSTR Prefetch_szAllow[] = {  
	_T("Windows\Prefetch\\"),
	_T("Prefetch\\"),
};

// ȭ��Ʈ����Ʈ�� ���ϴ��� ���� Ȯ�� �Լ�
// return 1 : Roaming
// return 2 : Prefetch
// return 4 : Dummy File
// return -1 : Nothing
static int Whitelisted(LPTSTR szFile)              
{
	for(int i=0; i<sizeof(Roaming_szAllow)/sizeof(LPTSTR); i++)
	{
		if (_tcsstr(szFile, Roaming_szAllow[i]) != NULL)
			return 1;
	}

	for(int i=0; i<sizeof(Prefetch_szAllow)/sizeof(LPTSTR); i++)
	{
		if (_tcsstr(szFile, Prefetch_szAllow[i]) != NULL) 
			return 2;
	}

	for(int i=0; i<6; i++){
		if (_tcsstr(szFile, dummyWhite[i]) != NULL) 
		{
			return 4;
		}
	}
	return -1;
}
