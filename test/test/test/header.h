#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include <tchar.h>
#pragma comment(lib, "psapi.lib") 
#define BUFSIZE 512


BOOL GetFileNameFromHandle(HANDLE hFile, TCHAR *pszFilename);

// testHandleList.cpp

#pragma comment(linker, "/opt:nowin98")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "shlwapi.lib")

#define _WIN32_WINNT	0x0501

//#include <winsock2.h>
#include <shlwapi.h>


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



void EnableDebugPrivilege();
LPWSTR GetObjectInfo(HANDLE hObject, OBJECT_INFORMATION_CLASS objInfoClass);
void OutputConnectionDetails(HANDLE hObject);