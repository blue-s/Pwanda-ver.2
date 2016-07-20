/*
특정 프로세스가 사용 중인 파일 목록 조회하는 방법
*/

#pragma comment(linker, "/opt:nowin98")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "shlwapi.lib")

#define _WIN32_WINNT	0x0501

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include <shlwapi.h>
#include <tchar.h>

// -------------------------------------------------------------------------
typedef LONG NTSTATUS;

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID Pointer;
    };
    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef void (WINAPI * PIO_APC_ROUTINE)(PVOID, PIO_STATUS_BLOCK, DWORD);

typedef LONG TDI_STATUS;
typedef PVOID CONNECTION_CONTEXT;       // connection context

typedef struct _TDI_REQUEST {
    union {
        HANDLE AddressHandle;     
        CONNECTION_CONTEXT ConnectionContext;
        HANDLE ControlChannel;
    } Handle;
	
    PVOID RequestNotifyObject;
    PVOID RequestContext;
    TDI_STATUS TdiStatus;
} TDI_REQUEST, *PTDI_REQUEST;

typedef struct _TDI_CONNECTION_INFORMATION {
    LONG UserDataLength;        // length of user data buffer
    PVOID UserData;             // pointer to user data buffer
    LONG OptionsLength;         // length of following buffer
    PVOID Options;              // pointer to buffer containing options
    LONG RemoteAddressLength;   // length of following buffer
    PVOID RemoteAddress;        // buffer containing the remote address
} TDI_CONNECTION_INFORMATION, *PTDI_CONNECTION_INFORMATION;

typedef struct _TDI_REQUEST_QUERY_INFORMATION {
    TDI_REQUEST Request;
    ULONG QueryType;                          // class of information to be queried.
    PTDI_CONNECTION_INFORMATION RequestConnectionInformation;
} TDI_REQUEST_QUERY_INFORMATION, *PTDI_REQUEST_QUERY_INFORMATION;

#define TDI_QUERY_ADDRESS_INFO			0x00000003
#define IOCTL_TDI_QUERY_INFORMATION		CTL_CODE(FILE_DEVICE_TRANSPORT, 4, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

typedef VOID *POBJECT;

typedef struct _SYSTEM_HANDLE {
	ULONG		uIdProcess;
	UCHAR		ObjectType;    // OB_TYPE_* (OB_TYPE_TYPE, etc.)
	UCHAR		Flags;         // HANDLE_FLAG_* (HANDLE_FLAG_INHERIT, etc.)
	USHORT		Handle;
	POBJECT		pObject;
	ACCESS_MASK	GrantedAccess;
} SYSTEM_HANDLE, *PSYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION {
	ULONG			uCount;
	SYSTEM_HANDLE	Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;

typedef UNICODE_STRING OBJECT_NAME_INFORMATION;
typedef UNICODE_STRING *POBJECT_NAME_INFORMATION;

#define SystemHandleInformation			16

typedef enum _OBJECT_INFORMATION_CLASS{
	ObjectBasicInformation,
	ObjectNameInformation,
	ObjectTypeInformation,
	ObjectAllTypesInformation,
	ObjectHandleInformation
} OBJECT_INFORMATION_CLASS;

#define STATUS_SUCCESS					((NTSTATUS)0x00000000L)
#define STATUS_INFO_LENGTH_MISMATCH		((NTSTATUS)0xC0000004L)
#define STATUS_BUFFER_OVERFLOW			((NTSTATUS)0x80000005L)
// -------------------------------------------------------------------------


typedef NTSTATUS (WINAPI *tNTQSI)(DWORD SystemInformationClass, PVOID SystemInformation,
								  DWORD SystemInformationLength, PDWORD ReturnLength);
typedef NTSTATUS (WINAPI *tNTQO)(HANDLE ObjectHandle, OBJECT_INFORMATION_CLASS ObjectInformationClass, PVOID ObjectInformation,
								 DWORD Length, PDWORD ResultLength);
typedef NTSTATUS (WINAPI *tNTDIOCF)(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext,
							   PIO_STATUS_BLOCK IoStatusBlock, DWORD IoControlCode,
							   PVOID InputBuffer, DWORD InputBufferLength,
							   PVOID OutputBuffer, DWORD OutputBufferLength);

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
		/*
		*** OBJECT_INFORMATION_CLASS
		The OBJECT_INFORMATION_CLASS enumeration type represents the type of information to supply about an object.

		typedef enum _OBJECT_INFORMATION_CLASS{
			ObjectBasicInformation,
			ObjectNameInformation,
			ObjectTypeInformation,
			ObjectAllTypesInformation,
			ObjectHandleInformation
		} OBJECT_INFORMATION_CLASS;
		*/
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

void OutputConnectionDetails(HANDLE hObject)
{
	tNTDIOCF pNTDIOCF = (tNTDIOCF)GetProcAddress(GetModuleHandle(_T("NTDLL.DLL")), "NtDeviceIoControlFile");
	if(pNTDIOCF != NULL){
		IO_STATUS_BLOCK IoStatusBlock;
		TDI_REQUEST_QUERY_INFORMATION tdiRequestAddress = {{0}, TDI_QUERY_ADDRESS_INFO};
		BYTE tdiAddress[128];
		
		HANDLE hEvent2 = CreateEvent(NULL, TRUE, FALSE, NULL);
		NTSTATUS ntReturn2 = pNTDIOCF(hObject, hEvent2, NULL, NULL, &IoStatusBlock, IOCTL_TDI_QUERY_INFORMATION,
			&tdiRequestAddress, sizeof(tdiRequestAddress), &tdiAddress, sizeof(tdiAddress));
		if(hEvent2) CloseHandle(hEvent2);

		if(ntReturn2 == STATUS_SUCCESS){
			struct in_addr *pAddr = (struct in_addr *)&tdiAddress[14];
			printf("@%s:%d", inet_ntoa(*pAddr), ntohs(*(PUSHORT)&tdiAddress[12]));
		}
	}
	printf("\n");
}


/***************************** main*******************/

int main(int argc, char *argv[])
{
	printf("TCP/UDP Handle List\n");
	printf("===============================\n\n");


	// [1] SetPrivilege
	EnableDebugPrivilege();

	// [2] 프로세스에서 dll에 해당하는 주소를 가져옴
	tNTQSI pNTQSI = (tNTQSI)GetProcAddress(GetModuleHandle(_T("NTDLL.DLL")), "NtQuerySystemInformation");
		/*
		*** tNTQSI
		typedef NTSTATUS (WINAPI *tNTQSI)(DWORD SystemInformationClass, PVOID SystemInformation,
				DWORD SystemInformationLength, PDWORD ReturnLength);

		*** GetProcAddress
		Retrieves the address of an exported function or variable from the specified dynamic-link library (DLL).

		WINBASEAPI FARPROC WINAPI
		GetProcAddress (
			__in HMODULE hModule,
					// A handle to the DLL module that contains the function or variable.
					// The LoadLibrary, LoadLibraryEx, LoadPackagedLibrary,
					// or GetModuleHandle function returns this handle.
					// The GetProcAddress function does not retrieve addresses from modules
					// that were loaded using the LOAD_LIBRARY_AS_DATAFILE flag.
			__in LPCSTR lpProcName
					// The function or variable name, or the function's ordinal value.
					// If this parameter is an ordinal value,
					// it must be in the low-order word; the high-order word must be zero.
			);

		*** GetModuleHandle
		Retrieves a module handle for the specified module.
		The module must have been loaded by the calling process.

		WINBASEAPI __out_opt HMODULE WINAPI
		GetModuleHandleW(
			__in_opt LPCWSTR lpModuleName
			);
		*/

	// 현재 주소를 가져온 상태
	if(pNTQSI != NULL){
		DWORD dwSize = sizeof(SYSTEM_HANDLE_INFORMATION);
			/*
			*** (사용자 정의) SYSTEM_HANDLE_INFORMATION 
			핸들과 연결되어 있는 시스템의 핸들들의 정보를 담음

			typedef struct _SYSTEM_HANDLE_INFORMATION {
				ULONG			uCount;
				SYSTEM_HANDLE	Handles[1];
			} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

			*** (사용자 정의) _SYSTEM_HANDLE
			핸들에 대한 정보를 담고 있음

			typedef struct _SYSTEM_HANDLE {
				ULONG		uIdProcess;
				UCHAR		ObjectType;    // OB_TYPE_* (OB_TYPE_TYPE, etc.)
				UCHAR		Flags;         // HANDLE_FLAG_* (HANDLE_FLAG_INHERIT, etc.)
				USHORT		Handle;
				POBJECT		pObject;
				ACCESS_MASK	GrantedAccess;
			} SYSTEM_HANDLE, *PSYSTEM_HANDLE;
			*/

		// [3] 핸들을 초기화 시킴
		PSYSTEM_HANDLE_INFORMATION pHandleInfo = (PSYSTEM_HANDLE_INFORMATION) new BYTE[dwSize];
		NTSTATUS ntReturn = pNTQSI(SystemHandleInformation, pHandleInfo, dwSize, &dwSize);

		// 요청한 dwSize와 맞지 않은 경우에 대한 요청
		if(ntReturn == STATUS_INFO_LENGTH_MISMATCH){
			/*
			#define STATUS_INFO_LENGTH_MISMATCH		((NTSTATUS)0xC0000004L)
			
			The specified information record length does not match the length
			that is required for the specified information class.

			An unusual aspect of calling NtQuerySystemInformation with SystemHandleInformation is that if you supply a buffer which is too small,
			it returns STATUS_INFO_LENGTH_MISMATCH (0xc0000004) instead of giving you the correct buffer size in ReturnLength.
			This means you will have to guess the buffer size.
			A common technique is to call NtQuerySystemInformation in a loop until it succeeds with STATUS_SUCCESS (0),
			reallocating and doubling the buffer size each time it fails with STATUS_INFO_LENGTH_MISMATCH.
			*/

			// 다시 핸들을 생성하고 그에 맞게 메모리 할당 해줌
			delete pHandleInfo;
			pHandleInfo = (PSYSTEM_HANDLE_INFORMATION) new BYTE[dwSize];
			ntReturn = pNTQSI(SystemHandleInformation, pHandleInfo, dwSize, &dwSize);
		}

		// 핸들 초기화에 성공한 경우
		if(ntReturn == STATUS_SUCCESS){
			/*
			#define STATUS_SUCCESS					((NTSTATUS)0x00000000L)
			The operation completed successfully.
			*/

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
							/*
							*** DuplicateHandle
							커널 객체의 핸들을 복제
							복제된 핸들값을 전달하여 동일한 커널 객체를 다른 프로세스와 공유할 수 있도록 한다.

							WINBASEAPI BOOL WINAPI
							DuplicateHandle(
								__in        HANDLE hSourceProcessHandle,
												// 복제할 커널 객체를 생성한 프로세스 핸들을 전달
								__in        HANDLE hSourceHandle,
												// 복제할 커널 객체의 핸들을 전달
								__in        HANDLE hTargetProcessHandle,
												// 복제될 핸들을 전달받을 프로세스의 핸들을 전달
								__deref_out LPHANDLE lpTargetHandle,
												// 복제될 커널 객체의 핸들 값을 위한 버퍼를 전달
								__in        DWORD dwDesiredAccess,
												// 커널 객체에 대한 접근 모드를 지정
								__in        BOOL bInheritHandle,
												// 복제될 핸들의 상속 여부 지정
												// TRUE : 타깃 프로세스가 생성할 자식 프로세스로 하여금 복제된 핸들을 상속 가능하게 해줌
												// FALSE : 상속을 허용하지 않음
								__in        DWORD dwOptions
												// 핸들 복제 시 지정하는 옵션 플래그의 값
												// DUPLICATE_CLOSE_SOURCE (0x01) : 이 함수의 호출 성공 여부와는 상관없이 소스의 핸들을 닫음
												// DUPLICATE_SAME_ACCESS (0x02) : dwDesiredAccess 매개변수를 무시하고 소스 핸들의 접근 권한과 동일한 권한을 부여
								);
							*/
					{
						// [5] Object Name 정보 얻어오기
						LPWSTR lpwsName = GetObjectInfo(hObject, ObjectNameInformation);

						if(lpwsName != NULL){
							// Object Type 정보 얻어오기
							LPWSTR lpwsType = GetObjectInfo(hObject, ObjectTypeInformation);
							// 메모리 초기화
							LPSTR lpszProcess = new CHAR[MAX_PATH];
							ZeroMemory(lpszProcess, MAX_PATH);

							// 모듈에서 파일 정보 얻어오기
							GetModuleFileNameEx(hProcess, NULL, (LPWSTR)lpszProcess, MAX_PATH);
								/*
								*** GetModuleFileNameEx
								Retrieves the fully qualified path for the file
								containing the specified module.

								DWORD WINAPI GetModuleFileNameEx(
									_In_     HANDLE  hProcess,
									_In_opt_ HMODULE hModule,
									_Out_    LPTSTR  lpFilename,
									_In_     DWORD   nSize
								);
								*/
							
							printf("%5d\t%6d\t%-16ws%-18s%ws",
								pHandleInfo->Handles[dwIdx].uIdProcess, pHandleInfo->Handles[dwIdx].Handle, lpwsType,
								((lstrlen((LPWSTR)lpszProcess) > 0)?PathFindFileName((LPWSTR)lpszProcess):_T("[System]")), lpwsName);
							
							if(!wcscmp(lpwsName, L"\\Device\\Tcp") || !wcscmp(lpwsName, L"\\Device\\Udp"))
								OutputConnectionDetails(hObject);
							else
								printf("\n");
							delete lpwsName, lpwsType, lpszProcess;
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
