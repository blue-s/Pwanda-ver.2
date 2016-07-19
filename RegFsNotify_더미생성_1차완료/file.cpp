#include "mon.h"

// ���� ����
#define MAX_DRIVES 24
#define MAX_VALUE_NAME 16383

HANDLE  g_ChangeHandles[MAX_DRIVES];
HANDLE  g_DirHandles[MAX_DRIVES];
LPTSTR  g_szDrives[MAX_DRIVES];
DWORD   g_idx = 0;

TCHAR * file_name;				// ���ϸ�
TCHAR * buffer_file_name;		// ���ϸ� �����ϴ� �ӽ� ����
TCHAR * buffer_extension;		// Ȯ���� �����ϴ� �ӽ� ����

const TCHAR * checkRoam =_T("Roaming");
const TCHAR * findPre =_T("Prefetch");
const TCHAR * checkPoint = _T(".");
const TCHAR * checkEXE = _T(".exe");


void ProcessChange(int idx)
{
	BYTE buf[MAX_VALUE_NAME];
	TCHAR szFile[MAX_VALUE_NAME];
	PFILE_NOTIFY_INFORMATION pNotify;
	DWORD cb = 0;
	int offset = 0;
	int flag = -1;		// ȭ��Ʈ ����Ʈ�� ���ϴ��� ���� Ȯ�� �÷���

	memset(buf, 0, sizeof(buf));

	if (ReadDirectoryChangesW(g_DirHandles[idx], buf, sizeof(buf), TRUE, FILE_CHANGE_FLAGS, &cb, NULL, NULL))
	{
		do {
			pNotify = (PFILE_NOTIFY_INFORMATION) &buf[offset];
			offset += pNotify->NextEntryOffset;

			memset(szFile, 0, sizeof(szFile));
			memcpy(szFile, pNotify->FileName, pNotify->FileNameLength);

			// �÷��׿� ���� ���ϸ� ���� �� ��
			// 1: Roaming, 2: Prefetch, -1: Nothing
			flag = Whitelisted(szFile);

			// ȭ��Ʈ����Ʈ�� ������ �ʴ� ���
			if(flag < 0){	continue; }			

			// ȭ��Ʈ����Ʈ�� ���ϴ°��
			switch (pNotify->Action)
			{
			case FILE_ACTION_ADDED:
				// Roaming�� �ش��ϴ� ���, ���� �� ����
				if(flag == 1)
				{
					buffer_file_name = _tcsstr(szFile, checkRoam) + 8;
					buffer_extension = _tcsstr(buffer_file_name, checkPoint);

					//Ȯ���ڰ� exe�� �ƴ� ��� 
					if(_tcsicmp(buffer_extension, checkEXE) != 0)	continue;
				}
				else if(flag == 2)
				{
					buffer_file_name = _tcsstr(szFile, findPre) + 9;
				}

				//ExtractProcess(flag, buffer_file_name);					
				Output_Console(FOREGROUND_GREEN, _T("[ADDED] %s%s \n"), g_szDrives[idx], szFile);
				break;

			case FILE_ACTION_MODIFIED:
				if(flag==4){
					Output_Console(FOREGROUND_BLUE, _T("[MODIFIED] %s%s\n"), g_szDrives[idx], szFile);
					
				}
				else 
					continue;
				break;

			case FILE_ACTION_RENAMED_OLD_NAME:
				if(flag==4){
					Output_Console(0, _T("\n[RENAMED (OLD)] %s%s \n"), g_szDrives[idx], szFile);
				}
				else 
					continue;
				break; 

			case FILE_ACTION_RENAMED_NEW_NAME:
				if(flag==4){
					Output_Console(0, _T("\n[RENAMED (NEW)] %s%s \n"), g_szDrives[idx], szFile);

				}
				else 
					continue;
				break;

				// ����ڰ� ���� �� ���� -> ��� �޼��� ���
			case FILE_ACTION_REMOVED: 
				if(flag==4){
					Output_Console(FOREGROUND_RED, _T("\n [REMOVED] %s%s \n"), g_szDrives[idx], szFile);
					// �����ϸ� dummyNum �����ϵ��� �ؾ���  
				}
				else 
					continue;
				break;

			default:
				break;
			}
			flag = -1;
		} while (pNotify->NextEntryOffset != 0);
	}
}    

void StartFileMonitor(void)
{
	DWORD dwWaitStatus;
	BOOL  bOK = FALSE;
	TCHAR   pszList[1024];
	DWORD   ddType;
	LPTSTR  pStart = NULL;
	HANDLE  hChange, hDir;

	memset(pszList, 0, sizeof(pszList));
	GetLogicalDriveStrings(sizeof(pszList), pszList);

	pStart = pszList;
	while(_tcslen(pStart)) 
	{
		ddType = GetDriveType(pStart);

		if ((ddType == DRIVE_FIXED || ddType == DRIVE_REMOVABLE) && _tcscmp(pStart, _T("A:\\")) != 0)
		{
			hChange = FindFirstChangeNotification(pStart,
				TRUE,
				FILE_CHANGE_FLAGS);

			if (hChange == INVALID_HANDLE_VALUE)
				continue;

			hDir = CreateFile(pStart, 
				FILE_LIST_DIRECTORY, 
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				NULL, 
				OPEN_EXISTING, 
				FILE_FLAG_BACKUP_SEMANTICS,
				NULL);

			if (hDir == INVALID_HANDLE_VALUE) {
				FindCloseChangeNotification(hChange);
				continue;
			}

			_tprintf(_T("Monitoring %s\n"), pStart);

			g_szDrives[g_idx]      = _tcsdup(pStart);
			g_DirHandles[g_idx]    = hDir;
			g_ChangeHandles[g_idx] = hChange;
			g_idx++;
		}

		pStart += wcslen(pStart) + 1;
	}

	while(WaitForSingleObject(g_hStopEvent, 1) != WAIT_OBJECT_0) 
	{
		dwWaitStatus = WaitForMultipleObjects( g_idx, g_ChangeHandles, FALSE, INFINITE); 

		bOK = FALSE;

		for(int i=0; i < g_idx; i++)
		{
			if (dwWaitStatus == WAIT_OBJECT_0 + i) 
			{
				bOK = TRUE;
				ProcessChange(i);
				if (!FindNextChangeNotification(g_ChangeHandles[i])) 
				{
					SetEvent(g_hStopEvent);
					break;
				}
				break;
			}
		}

		if (!bOK) break;
	}
	_tprintf(_T("Got stop event...\n"));

	for(int i=0; i < g_idx; i++)
	{
		_tprintf(_T("Closing handle for %s\n"), g_szDrives[i]);
		CloseHandle(g_DirHandles[i]);
		FindCloseChangeNotification(g_ChangeHandles[i]);
	}
}
