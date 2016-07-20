#include "header.h"

/***************************** main*******************/

int main(int argc, char *argv[])
{
	printf("TCP/UDP Handle List\n");
	printf("===============================\n\n");

	TCHAR* filename;// = _T("testFile");
	HANDLE hFile = CreateFile(_T("testFile"), GENERIC_READ|GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,NULL);
	printf("file handle : %d\n", hFile);

	// [1] SetPrivilege
	EnableDebugPrivilege();

	// [2] 프로세스에서 dll에 해당하는 주소를 가져옴
	tNTQSI pNTQSI = (tNTQSI)GetProcAddress(GetModuleHandle(_T("NTDLL.DLL")), "NtQuerySystemInformation");

	// 현재 주소를 가져온 상태
	if(pNTQSI != NULL){
		DWORD dwSize = sizeof(SYSTEM_HANDLE_INFORMATION);
			
		// [3] 핸들을 초기화 시킴
		PSYSTEM_HANDLE_INFORMATION pHandleInfo = (PSYSTEM_HANDLE_INFORMATION) new BYTE[dwSize];
		NTSTATUS ntReturn = pNTQSI(SystemHandleInformation, pHandleInfo, dwSize, &dwSize);

		// 요청한 dwSize와 맞지 않은 경우에 대한 요청
		if(ntReturn == STATUS_INFO_LENGTH_MISMATCH){
			
			// 다시 핸들을 생성하고 그에 맞게 메모리 할당 해줌
			delete pHandleInfo;
			pHandleInfo = (PSYSTEM_HANDLE_INFORMATION) new BYTE[dwSize];
			ntReturn = pNTQSI(SystemHandleInformation, pHandleInfo, dwSize, &dwSize);
		}

		// 핸들 초기화에 성공한 경우
		if(ntReturn == STATUS_SUCCESS){
			
			printf(" Found %d Handles.\n\n", pHandleInfo->uCount);
			printf("  PID\tHandle\t%-16s%-18sHandle Name\n", "Type", "Process Name");	// 칸에 맞게 출력

			// [4] 현재 핸들의 수만큼 출력
			for(DWORD dwIdx = 0; dwIdx < pHandleInfo->uCount; dwIdx++)
			{
				// 핸들과 관련된 프로세스 핸들 얻어옴
				HANDLE hProcess = OpenProcess(PROCESS_DUP_HANDLE | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
					FALSE, pHandleInfo->Handles[dwIdx].uIdProcess);
				
				if(hProcess != INVALID_HANDLE_VALUE)
				{
					HANDLE hObject = NULL;

					if(DuplicateHandle(hProcess, (HANDLE)pHandleInfo->Handles[dwIdx].Handle,
						GetCurrentProcess(), &hObject, STANDARD_RIGHTS_REQUIRED, FALSE, 0) != FALSE)
					{
						// [5] Object 정보 얻어오기
						LPWSTR lpwsName = GetObjectInfo(hObject, ObjectNameInformation);

						if(lpwsName != NULL){
							LPWSTR lpwsType = GetObjectInfo(hObject, ObjectTypeInformation);
							LPSTR lpszProcess = new CHAR[MAX_PATH];
							ZeroMemory(lpszProcess, MAX_PATH);
							GetModuleFileNameEx(hProcess, NULL, (LPWSTR)lpszProcess, MAX_PATH);
							printf("%5d\t%6d\t%-16ws%-18s%ws",
								pHandleInfo->Handles[dwIdx].uIdProcess, pHandleInfo->Handles[dwIdx].Handle, lpwsType,
								((lstrlen((LPWSTR)lpszProcess) > 0)?PathFindFileName((LPWSTR)lpszProcess):_T("[System]"))
								, GetObjectInfo(hObject, ObjectHandleInformation));//, lpwsName);
							/*if(!wcscmp(lpwsName, L"\\Device\\Tcp") || !wcscmp(lpwsName, L"\\Device\\Udp"))
								OutputConnectionDetails(hObject);
							else
								printf("\n");
							delete lpwsName, lpwsType, lpszProcess;*/
							printf("\n");

							// test : 핸들 값 출력해 보기
							/*TCHAR* filename;
							GetFileNameFromHandle((HANDLE)pHandleInfo->Handles[dwIdx].Handle, filename);
							printf("filename : %S\n", filename);*/
						}
						CloseHandle(hObject);
					}
					CloseHandle(hProcess);
				}	
			}
			printf("\n\n");
		}else{
			printf("Error while trying to allocate memory for System Handle Information.\n");
		}
		delete pHandleInfo;
	}else{
		printf("Cannot find NtQuerySystemInformation API... Is this system not Win2K and above?");
	}
	
	return 0;
}
