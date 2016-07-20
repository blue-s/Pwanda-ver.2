#include <windows.h>
#include <stdio.h>
#include <Psapi.h>
#include <tchar.h>
#pragma comment(lib, "psapi.lib") 
#define BUFSIZE 512

BOOL GetFileNameFromHandle(HANDLE hFile, TCHAR *pszFilename) 
{
	BOOL bSuccess = FALSE;
	HANDLE hFileMap;

	// Get the file size.
	DWORD dwFileSizeHi = 0;
	DWORD dwFileSizeLo = GetFileSize(hFile, &dwFileSizeHi);

	if( dwFileSizeLo == 0 && dwFileSizeHi == 0 )
	{
		//OutputDebugString(L"Cannot map a file with a length of zero.\n");
		return FALSE;
	}

	// Create a file mapping object.
	hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 1, NULL);
		/*
		*** CreateFileMapping
		CreateFile�� ���� Ư�� ������ ���ų� �����ߴٸ�
		�� ������ ���� MMF Ŀ�� ��ü�� ������ CreateFilemapping �Լ��� ����Ѵ�.

		(1) MMF�� ����ϱ⸦ ���ϴ� ��ũ ���� ������ ���ų� ���� : CreateFile
		(2) �ý��ۿ��� ���� ũ��� ������ ��� �׼����� �������� �˸��� MMF Ŀ�� ��ü�� ����
			: CreateFileMapping �Ǵ� OpenFileMapping
		(3) �ý��ۿ��� MMF�� ��ü�� �κ��� ����� ���μ����� ���� �ּ� ������ ������ �� ���� ����
			: MapViewOfFile(Ex)
		(4) �ý��ۿ��� ���μ����� ���� �ּ� �������κ��� MMF�� ������ �����϶�� �����ϱ�
			: UnmapViewOfFile
		(5) MMF Ŀ�� ��ü �ݱ� : CloseHandle
		(6) ���� Ŀ�� ��ü �ݱ� : CloseHandle

		HANDLE WINAPI CreateFileMapping(
			_In_     HANDLE                hFile,		// CreateFile�� ���ؼ� ���ų� ������ ������ �ڵ�
			_In_opt_ LPSECURITY_ATTRIBUTES lpAttributes,	// ���� �Ӽ��� ����
			_In_     DWORD                 flProtect,		// ���� ��带 ����
			_In_     DWORD                 dwMaximumSizeHigh,	// 
			_In_     DWORD                 dwMaximumSizeLow,	// 
			_In_opt_ LPCTSTR               lpName
		);
		*/
	
	if (hFileMap) 
	{
		// Create a file mapping to get the file name.
		void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);
			
		printf("filename : %S\n", pszFilename);
		if (pMem) 
		{
			if (GetMappedFileName (GetCurrentProcess(), pMem, pszFilename, MAX_PATH))
					/*
					DWORD WINAPI GetMappedFileName(
					  _In_  HANDLE hProcess,
					  _In_  LPVOID lpv,
					  _Out_ LPTSTR lpFilename,
					  _In_  DWORD  nSize
					);
					*/
			{
				// Translate path with device name to drive letters.
				TCHAR szTemp[BUFSIZE];
				szTemp[0] = '\0';
				
				if (GetLogicalDriveStrings(BUFSIZE-1, szTemp)) 
				{
					TCHAR szName[MAX_PATH];
					TCHAR szDrive[3] = TEXT(" :");
					BOOL bFound = FALSE;
					TCHAR* p = szTemp;

					do 
					{
						// Copy the drive letter to the template string
						*szDrive = *p;

						// Look up each device name
						if (QueryDosDevice(szDrive, szName, MAX_PATH))
						{
							UINT uNameLen = _tcslen(szName);

							if (uNameLen < MAX_PATH) 
							{
								bFound = _tcsnicmp(pszFilename, szName, uNameLen) == 0;

								if (bFound) 
								{
									// Reconstruct pszFilename using szTempFile
									// Replace device path with DOS path
									TCHAR szTempFile[MAX_PATH];
									swprintf(szTempFile, L"%s%s", szDrive, pszFilename+uNameLen);
									_tcscpy(pszFilename, szTempFile);
								}
							}
						}

						// Go to the next NULL character.
						while (*p++);
					} while (!bFound && *p); // end of string
				}
			}
			bSuccess = TRUE;
			UnmapViewOfFile(pMem);
		}

		CloseHandle(hFileMap);
	}

	return(bSuccess);
}

void main(){
	TCHAR* filename;// = _T("testFile");
	HANDLE hFile = CreateFile(_T("testFile"), GENERIC_READ|GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,NULL);

	char wdata[20] = "Hello World";
	DWORD dwWrite;
	WriteFile(hFile, wdata, strlen(wdata), &dwWrite, NULL);
	
	if(GetFileNameFromHandle(hFile, filename))
		printf("%S\n", filename);
	printf("%S\n", filename);
}