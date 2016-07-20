#include "mon.h"

// 파일 생성/수정/이름변경/삭제 감지 출력하려면
//Output_Console(FOREGROUND_BLUE, _T("[속성] %s%s\n"), g_szDrives[idx], szFile);
					

// 변수 선언
#define MAX_DRIVES 24
#define MAX_VALUE_NAME 16383

HANDLE  g_ChangeHandles[MAX_DRIVES];
HANDLE  g_DirHandles[MAX_DRIVES];
LPTSTR  g_szDrives[MAX_DRIVES];
DWORD   g_idx = 0;

TCHAR * file_name;				// 파일명
TCHAR * buffer_file_name;		// 파일명 저장하는 임시 변수
TCHAR * buffer_extension;		// 확장자 저장하는 임시 변수
	
const TCHAR * checkRoam =_T("Roaming");
const TCHAR * checkPoint = _T(".");
const TCHAR * checkEXE = _T(".exe");


void ProcessChange(int idx)
{
	BYTE buf[MAX_VALUE_NAME];
	TCHAR szFile[MAX_VALUE_NAME];
	PFILE_NOTIFY_INFORMATION pNotify;
	DWORD cb = 0;
	int offset = 0;
	int flag = -1;		// 화이트 리스트에 속하는지 여부 확인 플래그

	memset(buf, 0, sizeof(buf));

	if (ReadDirectoryChangesW(g_DirHandles[idx], buf, sizeof(buf), TRUE, FILE_CHANGE_FLAGS, &cb, NULL, NULL))
	{
		do {
			pNotify = (PFILE_NOTIFY_INFORMATION) &buf[offset];
			offset += pNotify->NextEntryOffset;

			memset(szFile, 0, sizeof(szFile));
			memcpy(szFile, pNotify->FileName, pNotify->FileNameLength);

			// 플래그에 따른 파일명 추출 및 비교
			// 1: Roaming, 4: Dummy, -1: Nothing
			flag = Whitelisted(szFile);

			// 화이트리스트에 속하지 않는 경우
			if(flag < 0){	continue; }			

			// 화이트리스트에 속하는경우
			switch (pNotify->Action)
			{
			case FILE_ACTION_ADDED:

				if(flag == 1)
				{
					buffer_file_name = _tcsstr(szFile, checkRoam) + 8;
					buffer_extension = _tcsstr(buffer_file_name, checkPoint);

					if(_tcsicmp(buffer_extension, checkEXE) != 0)	continue;
				}

				ExtractProcess(flag, buffer_file_name);					
				Output_Console(FOREGROUND_BLUE, _T("[ADDED] %s%s \n"), g_szDrives[idx], szFile);
				break;

			case FILE_ACTION_MODIFIED:
				if(flag==4){
					Output_Console(FOREGROUND_BLUE, _T("[MODIFIED] %s%s\n"), g_szDrives[idx], szFile);

					// 여기에 프로세스 추출.cpp 들어감 

					dbFlag=true;
					makeDummy();
				}
				else 
					continue;
				break;

			case FILE_ACTION_RENAMED_OLD_NAME:
				if(flag==4){
					Output_Console(FOREGROUND_BLUE, _T("\n[RENAMED (OLD)] %s%s \n"), g_szDrives[idx], szFile);

					// 여기에 프로세스 추출.cpp 들어감 

					dbFlag=true;
					makeDummy();
				}
				else 
					continue;
				break; 

			case FILE_ACTION_RENAMED_NEW_NAME:
				if(flag==4){
					Output_Console(FOREGROUND_BLUE, _T("\n[RENAMED (NEW)] %s%s \n"), g_szDrives[idx], szFile);

					// 여기에 프로세스 추출.cpp 들어감 

					dbFlag=true;
					makeDummy();
				}
				else 
					continue;
				break;

			case FILE_ACTION_REMOVED: 
				if(flag==4){
					Output_Console(FOREGROUND_BLUE, _T("\n [REMOVED] %s%s \n"), g_szDrives[idx], szFile);
					
					// getDummyDB(); //더미파일이 삭제되었으니까 db에 있는 파일들을 다시 끄집어와야 한다 
					// 이미 dummyWhite는 파일 삭제와 별개로 저장되어있는 전역변수 --> DB 역할을 해준다 
					// 따라서 전체 경로를 받은 dummyFileName을 포인터 배열 전역변수로 빼서 바로 파일생성이 가능하게 한다
					
					dbFlag=true;
					makeDummy();
					
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
