#include "mon.h"

HANDLE g_hStopEvent;
HANDLE g_hFile;		// 파일 다루는 핸들
HANDLE g_hRegWatch[2];
bool dbFlag;

USHORT GetConsoleTextAttribute(HANDLE hConsole)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	return(csbi.wAttributes);
}

// 콘솔로 출력 함수
void Output_Console(USHORT Color, LPTSTR format, ... )
{
	va_list args;
	int len;
	DWORD cb;
	LPTSTR buffer;

	va_start(args, format);
	len = _vsctprintf(format, args) + sizeof(TCHAR); 
	buffer = new TCHAR[len * sizeof(TCHAR)];

	if (!buffer) { 
		return;
	}

	_vstprintf_s(buffer, len, format, args);

	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (Color) 
	{ 
		SetConsoleTextAttribute(
			Handle, Color | FOREGROUND_INTENSITY);
	} 

	_tprintf(buffer);

	SetConsoleTextAttribute(Handle, 
		FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

	delete[] buffer;
}

// 텍스트 파일로 출력 함수
void Output_File(LPTSTR format, ...)
{
	va_list args;
	int len;
	DWORD cb;
	LPTSTR buffer;

	va_start(args, format);
	len = _vsctprintf(format, args) + sizeof(TCHAR); 
	buffer = new TCHAR[len * sizeof(TCHAR)];

	if (!buffer){	return; }

	_vstprintf_s(buffer, len, format, args);

	if (g_hFile != INVALID_HANDLE_VALUE) {
#ifdef _UNICODE
		LPSTR str = new CHAR[len + 1];
		if (str) { 
			memset(str, 0, len + 1);
			WideCharToMultiByte(CP_ACP, 0, buffer, -1, str, len, NULL, NULL);
			WriteFile(g_hFile, str, strlen(str), &cb, NULL);
			delete[] str;
		}
#else 
		WriteFile(g_hFile_Prefetch, buffer, strlen(buffer), &cb, NULL);
#endif
	}

	delete[] buffer;
}

//프로그램 종료 시 사용될 부분이라 신경안써도 됨.
BOOL CtrlHandler(DWORD fdwCtrlType) 
{ 
	switch(fdwCtrlType) 
	{ 
	case CTRL_C_EVENT: 
		SetEvent(g_hStopEvent);
		return TRUE;
	case CTRL_CLOSE_EVENT: 
		SetEvent(g_hStopEvent);
		return TRUE; 
	case CTRL_BREAK_EVENT: 
		return FALSE; 
	case CTRL_LOGOFF_EVENT: 
		return FALSE; 
	case CTRL_SHUTDOWN_EVENT: 
		return FALSE; 
	default: 
		return FALSE; 
	} 
}

void _tmain(int argc, TCHAR *argv[])
{
	checkState();	// [2] 테스트 함수

	// Privilege Setting
	if( !SetPrivilege(SE_DEBUG_NAME, TRUE) )
        return;

	// 시작할 때 더미파일을 생성한다 --> dummyWhite[6] 에 저장이 된다 
	// saveToDB함수를 불러서 txt파일로 저장한다 이름은 fix , --> C:\\dummyList_by_Pwanda.txt --> 숨김 파일로 만들 수 있으면 만들기 
	// fileMonitoring에서 더미를 flag 4로 받았다. 이 때 더미 화이트 리스트를 기반으로 탐지를 수행한다
	// 이 때 수정(이름이나 확장자가 변경되면 DB가 업데이트 되어야 하니까) 혹은 삭제되면 makedummy()를 불러서 6개를 다시 만들고
	// 오류1. --> 프로그램이 종료되지 않은채로 makedummy를 다시만들면 파일이 계속 쌓이게 될 것 (파일은 프로세스 종료시에 삭제되니까)
	// 그래서 dummyList_by_Pwanda.txt 에 있는 것을 가져와서 다시 makedummy해야한다 
	
	
	// 전체를 다시 덮어씌운다 // 이 때 txt 파일 생성할 때 파일이 있으면 항상 덮어쓰기 옵션으로 지정해줘야 함 -> 'w+'
	// 사용자가 하나라도 삭제하면 다시 만들고 하기 때문에 더미파일 6개가 항상 있을 수 밖에 없다 
	// 프로그램 종료를 사용자가 원하면 dummyWhite 리스트를 불러와서 이 경로에 있는 파일들을 다 삭제한다 

	dbFlag=false;
	makeDummy();
	saveDummyToDB();
	

	SetRegistyStartProgram();
	

	_tprintf(_T("\n"));

	g_hStopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);

	// 파일 생성 : 현재 우리는 파일 출력을 할 필요가 없기 때문에 완전한 결과값 하나만 출력하도록 함
	g_hFile = CreateFile(_T("KillProcessList.txt"), GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, NULL);

	// 쓰레드 생성 및 활동
	HANDLE hThread[2];

	hThread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartFileMonitor, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartRegistryMonitor, NULL, 0, NULL);
	
	WaitForMultipleObjects(2, (const HANDLE*)&hThread, TRUE, INFINITE); 

	// 열려 있는 핸들 종료
	CloseHandle(g_hStopEvent);
	CloseHandle(g_hFile);

	_tprintf(_T("Program terminating.\n"));
}