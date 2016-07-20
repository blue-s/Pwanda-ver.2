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

	// [2] ���μ������� dll�� �ش��ϴ� �ּҸ� ������
	tNTQSI pNTQSI = (tNTQSI)GetProcAddress(GetModuleHandle(_T("NTDLL.DLL")), "NtQuerySystemInformation");

	// ���� �ּҸ� ������ ����
	if(pNTQSI != NULL){
		DWORD dwSize = sizeof(SYSTEM_HANDLE_INFORMATION);
			
		// [3] �ڵ��� �ʱ�ȭ ��Ŵ
		PSYSTEM_HANDLE_INFORMATION pHandleInfo = (PSYSTEM_HANDLE_INFORMATION) new BYTE[dwSize];
		NTSTATUS ntReturn = pNTQSI(SystemHandleInformation, pHandleInfo, dwSize, &dwSize);

		// ��û�� dwSize�� ���� ���� ��쿡 ���� ��û
		if(ntReturn == STATUS_INFO_LENGTH_MISMATCH){
			
			// �ٽ� �ڵ��� �����ϰ� �׿� �°� �޸� �Ҵ� ����
			delete pHandleInfo;
			pHandleInfo = (PSYSTEM_HANDLE_INFORMATION) new BYTE[dwSize];
			ntReturn = pNTQSI(SystemHandleInformation, pHandleInfo, dwSize, &dwSize);
		}

		// �ڵ� �ʱ�ȭ�� ������ ���
		if(ntReturn == STATUS_SUCCESS){
			
			printf(" Found %d Handles.\n\n", pHandleInfo->uCount);
			printf("  PID\tHandle\t%-16s%-18sHandle Name\n", "Type", "Process Name");	// ĭ�� �°� ���

			// [4] ���� �ڵ��� ����ŭ ���
			for(DWORD dwIdx = 0; dwIdx < pHandleInfo->uCount; dwIdx++)
			{
				// �ڵ�� ���õ� ���μ��� �ڵ� ����
				HANDLE hProcess = OpenProcess(PROCESS_DUP_HANDLE | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
					FALSE, pHandleInfo->Handles[dwIdx].uIdProcess);
				
				if(hProcess != INVALID_HANDLE_VALUE)
				{
					HANDLE hObject = NULL;

					if(DuplicateHandle(hProcess, (HANDLE)pHandleInfo->Handles[dwIdx].Handle,
						GetCurrentProcess(), &hObject, STANDARD_RIGHTS_REQUIRED, FALSE, 0) != FALSE)
					{
						// [5] Object ���� ������
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

							// test : �ڵ� �� ����� ����
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
