#include "mon.h"

HANDLE g_hStopEvent;
HANDLE g_hFile;		// ���� �ٷ�� �ڵ�
HANDLE g_hRegWatch[2];
bool dbFlag;

USHORT GetConsoleTextAttribute(HANDLE hConsole)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	return(csbi.wAttributes);
}

// �ַܼ� ��� �Լ�
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

// �ؽ�Ʈ ���Ϸ� ��� �Լ�
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

//���α׷� ���� �� ���� �κ��̶� �Ű�Ƚᵵ ��.
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
	checkState();	// [2] �׽�Ʈ �Լ�

	// Privilege Setting
	if( !SetPrivilege(SE_DEBUG_NAME, TRUE) )
        return;

	// ������ �� ���������� �����Ѵ� --> dummyWhite[6] �� ������ �ȴ� 
	// saveToDB�Լ��� �ҷ��� txt���Ϸ� �����Ѵ� �̸��� fix , --> C:\\dummyList_by_Pwanda.txt --> ���� ���Ϸ� ���� �� ������ ����� 
	// fileMonitoring���� ���̸� flag 4�� �޾Ҵ�. �� �� ���� ȭ��Ʈ ����Ʈ�� ������� Ž���� �����Ѵ�
	// �� �� ����(�̸��̳� Ȯ���ڰ� ����Ǹ� DB�� ������Ʈ �Ǿ�� �ϴϱ�) Ȥ�� �����Ǹ� makedummy()�� �ҷ��� 6���� �ٽ� �����
	// ����1. --> ���α׷��� ������� ����ä�� makedummy�� �ٽø���� ������ ��� ���̰� �� �� (������ ���μ��� ����ÿ� �����Ǵϱ�)
	// �׷��� dummyList_by_Pwanda.txt �� �ִ� ���� �����ͼ� �ٽ� makedummy�ؾ��Ѵ� 
	
	
	// ��ü�� �ٽ� ������ // �� �� txt ���� ������ �� ������ ������ �׻� ����� �ɼ����� ��������� �� -> 'w+'
	// ����ڰ� �ϳ��� �����ϸ� �ٽ� ����� �ϱ� ������ �������� 6���� �׻� ���� �� �ۿ� ���� 
	// ���α׷� ���Ḧ ����ڰ� ���ϸ� dummyWhite ����Ʈ�� �ҷ��ͼ� �� ��ο� �ִ� ���ϵ��� �� �����Ѵ� 

	dbFlag=false;
	makeDummy();
	saveDummyToDB();
	

	SetRegistyStartProgram();
	

	_tprintf(_T("\n"));

	g_hStopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);

	// ���� ���� : ���� �츮�� ���� ����� �� �ʿ䰡 ���� ������ ������ ����� �ϳ��� ����ϵ��� ��
	g_hFile = CreateFile(_T("KillProcessList.txt"), GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, NULL);

	// ������ ���� �� Ȱ��
	HANDLE hThread[2];

	hThread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartFileMonitor, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartRegistryMonitor, NULL, 0, NULL);
	
	WaitForMultipleObjects(2, (const HANDLE*)&hThread, TRUE, INFINITE); 

	// ���� �ִ� �ڵ� ����
	CloseHandle(g_hStopEvent);
	CloseHandle(g_hFile);

	_tprintf(_T("Program terminating.\n"));
}