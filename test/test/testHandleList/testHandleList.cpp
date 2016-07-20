/*
Ư�� ���μ����� ��� ���� ���� ��� ��ȸ�ϴ� ���
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

// [5] Object ���� ������
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

	// ������ ���Դ°�
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

	// [2] ���μ������� dll�� �ش��ϴ� �ּҸ� ������
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

	// ���� �ּҸ� ������ ����
	if(pNTQSI != NULL){
		DWORD dwSize = sizeof(SYSTEM_HANDLE_INFORMATION);
			/*
			*** (����� ����) SYSTEM_HANDLE_INFORMATION 
			�ڵ�� ����Ǿ� �ִ� �ý����� �ڵ���� ������ ����

			typedef struct _SYSTEM_HANDLE_INFORMATION {
				ULONG			uCount;
				SYSTEM_HANDLE	Handles[1];
			} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

			*** (����� ����) _SYSTEM_HANDLE
			�ڵ鿡 ���� ������ ��� ����

			typedef struct _SYSTEM_HANDLE {
				ULONG		uIdProcess;
				UCHAR		ObjectType;    // OB_TYPE_* (OB_TYPE_TYPE, etc.)
				UCHAR		Flags;         // HANDLE_FLAG_* (HANDLE_FLAG_INHERIT, etc.)
				USHORT		Handle;
				POBJECT		pObject;
				ACCESS_MASK	GrantedAccess;
			} SYSTEM_HANDLE, *PSYSTEM_HANDLE;
			*/

		// [3] �ڵ��� �ʱ�ȭ ��Ŵ
		PSYSTEM_HANDLE_INFORMATION pHandleInfo = (PSYSTEM_HANDLE_INFORMATION) new BYTE[dwSize];
		NTSTATUS ntReturn = pNTQSI(SystemHandleInformation, pHandleInfo, dwSize, &dwSize);

		// ��û�� dwSize�� ���� ���� ��쿡 ���� ��û
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

			// �ٽ� �ڵ��� �����ϰ� �׿� �°� �޸� �Ҵ� ����
			delete pHandleInfo;
			pHandleInfo = (PSYSTEM_HANDLE_INFORMATION) new BYTE[dwSize];
			ntReturn = pNTQSI(SystemHandleInformation, pHandleInfo, dwSize, &dwSize);
		}

		// �ڵ� �ʱ�ȭ�� ������ ���
		if(ntReturn == STATUS_SUCCESS){
			/*
			#define STATUS_SUCCESS					((NTSTATUS)0x00000000L)
			The operation completed successfully.
			*/

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
							/*
							*** DuplicateHandle
							Ŀ�� ��ü�� �ڵ��� ����
							������ �ڵ鰪�� �����Ͽ� ������ Ŀ�� ��ü�� �ٸ� ���μ����� ������ �� �ֵ��� �Ѵ�.

							WINBASEAPI BOOL WINAPI
							DuplicateHandle(
								__in        HANDLE hSourceProcessHandle,
												// ������ Ŀ�� ��ü�� ������ ���μ��� �ڵ��� ����
								__in        HANDLE hSourceHandle,
												// ������ Ŀ�� ��ü�� �ڵ��� ����
								__in        HANDLE hTargetProcessHandle,
												// ������ �ڵ��� ���޹��� ���μ����� �ڵ��� ����
								__deref_out LPHANDLE lpTargetHandle,
												// ������ Ŀ�� ��ü�� �ڵ� ���� ���� ���۸� ����
								__in        DWORD dwDesiredAccess,
												// Ŀ�� ��ü�� ���� ���� ��带 ����
								__in        BOOL bInheritHandle,
												// ������ �ڵ��� ��� ���� ����
												// TRUE : Ÿ�� ���μ����� ������ �ڽ� ���μ����� �Ͽ��� ������ �ڵ��� ��� �����ϰ� ����
												// FALSE : ����� ������� ����
								__in        DWORD dwOptions
												// �ڵ� ���� �� �����ϴ� �ɼ� �÷����� ��
												// DUPLICATE_CLOSE_SOURCE (0x01) : �� �Լ��� ȣ�� ���� ���οʹ� ������� �ҽ��� �ڵ��� ����
												// DUPLICATE_SAME_ACCESS (0x02) : dwDesiredAccess �Ű������� �����ϰ� �ҽ� �ڵ��� ���� ���Ѱ� ������ ������ �ο�
								);
							*/
					{
						// [5] Object Name ���� ������
						LPWSTR lpwsName = GetObjectInfo(hObject, ObjectNameInformation);

						if(lpwsName != NULL){
							// Object Type ���� ������
							LPWSTR lpwsType = GetObjectInfo(hObject, ObjectTypeInformation);
							// �޸� �ʱ�ȭ
							LPSTR lpszProcess = new CHAR[MAX_PATH];
							ZeroMemory(lpszProcess, MAX_PATH);

							// ��⿡�� ���� ���� ������
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
