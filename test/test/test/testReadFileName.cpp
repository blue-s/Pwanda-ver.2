#include "header.h"

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

	if (hFileMap) 
	{
		// Create a file mapping to get the file name.
		void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);

		if (pMem) 
		{
			if (GetMappedFileName (GetCurrentProcess(), pMem, pszFilename, MAX_PATH)) 
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