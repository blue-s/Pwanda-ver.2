

#include "header.h"

// [1] SetPrivilege
void EnableDebugPrivilege()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tokenPriv;
	LUID luidDebug;
	if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken) != FALSE) {
		if(LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luidDebug) != FALSE)
		{
			tokenPriv.PrivilegeCount           = 1;
			tokenPriv.Privileges[0].Luid       = luidDebug;
			tokenPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			AdjustTokenPrivileges(hToken, FALSE, &tokenPriv, sizeof(tokenPriv), NULL, NULL);
		}
	}
}

// [5] Object 정보 얻어오기
LPWSTR GetObjectInfo(HANDLE hObject, OBJECT_INFORMATION_CLASS objInfoClass)
{
	LPWSTR lpwsReturn = NULL;
	tNTQO pNTQO = (tNTQO)GetProcAddress(GetModuleHandle(_T("NTDLL.DLL")), "NtQueryObject");

	// 정보를 얻어왔는가
	if(pNTQO != NULL){
		DWORD dwSize = sizeof(OBJECT_NAME_INFORMATION);
		POBJECT_NAME_INFORMATION pObjectInfo = (POBJECT_NAME_INFORMATION) new BYTE[dwSize];
		NTSTATUS ntReturn = pNTQO(hObject, objInfoClass, pObjectInfo, dwSize, &dwSize);

		if((ntReturn == STATUS_BUFFER_OVERFLOW) || (ntReturn == STATUS_INFO_LENGTH_MISMATCH)){
			delete pObjectInfo;
			pObjectInfo = (POBJECT_NAME_INFORMATION) new BYTE[dwSize];
			ntReturn = pNTQO(hObject, objInfoClass, pObjectInfo, dwSize, &dwSize);
		}
		if((ntReturn >= STATUS_SUCCESS) && (pObjectInfo->Buffer != NULL))
		{
			lpwsReturn = (LPWSTR) new BYTE[pObjectInfo->Length + sizeof(WCHAR)];
			ZeroMemory(lpwsReturn, pObjectInfo->Length + sizeof(WCHAR));
			CopyMemory(lpwsReturn, pObjectInfo->Buffer, pObjectInfo->Length);
		}
		delete pObjectInfo;
	}
	return lpwsReturn;
}

//void OutputConnectionDetails(HANDLE hObject)
//{
//	tNTDIOCF pNTDIOCF = (tNTDIOCF)GetProcAddress(GetModuleHandle(_T("NTDLL.DLL")), "NtDeviceIoControlFile");
//	if(pNTDIOCF != NULL){
//		IO_STATUS_BLOCK IoStatusBlock;
//		TDI_REQUEST_QUERY_INFORMATION tdiRequestAddress = {{0}, TDI_QUERY_ADDRESS_INFO};
//		BYTE tdiAddress[128];
//		
//		HANDLE hEvent2 = CreateEvent(NULL, TRUE, FALSE, NULL);
//		NTSTATUS ntReturn2 = pNTDIOCF(hObject, hEvent2, NULL, NULL, &IoStatusBlock, IOCTL_TDI_QUERY_INFORMATION,
//			&tdiRequestAddress, sizeof(tdiRequestAddress), &tdiAddress, sizeof(tdiAddress));
//		if(hEvent2) CloseHandle(hEvent2);
//
//		if(ntReturn2 == STATUS_SUCCESS){
//			struct in_addr *pAddr = (struct in_addr *)&tdiAddress[14];
//			printf("@%s:%d", inet_ntoa(*pAddr), ntohs(*(PUSHORT)&tdiAddress[12]));
//		}
//	}
//	printf("\n");
//}


