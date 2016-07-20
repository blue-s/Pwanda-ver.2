#include "mon.h"

void SetRegistyStartProgram(){

	HKEY   hKey;
	LONG   ret2;
	TCHAR value[1024]= _T("C:\\Windows\\System32\\notepad.exe");  // 노트 패드 경로 "C:\\Windows\\System32\\notepad.exe" // "C:\\Users\\kosta\\Desktop\\11111.exe"
	//C:\Users\kosta\Desktop
	ret2  = RegCreateKeyEx(HKEY_CURRENT_USER,  //HKEY_CURRENT_USER
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
		0, 
		NULL, 
		REG_OPTION_NON_VOLATILE, 
		KEY_ALL_ACCESS, 
		NULL, 
		&hKey, 
		NULL);

	if(ret2 == ERROR_SUCCESS) 
	{
		//Output_Console(FOREGROUND_BLUE, _T("(LPBYTE)value: %d ,  _tcslen(value): %d !\n", (LPBYTE)value, _tcslen(value)));
		RegSetValueEx(hKey, _T("RegFsNotify"), 0, REG_SZ, (LPBYTE)value, _tcslen(value)+31);
		Output_Console(FOREGROUND_BLUE, _T("autoRun Registry Success !\n"));
	}

}