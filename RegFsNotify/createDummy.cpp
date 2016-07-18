#include "mon.h"
#include <stdlib.h>
#include <time.h>

//TCHAR path[]=_T"Users\\kosta\\Documents\\+[랜덤명]" 
//TCHAR path[]=_T("C:\\Users\\kosta\\Documents\\");
TCHAR path[]=_T("C:\\");

TCHAR extension[]=_T(".txt");
TCHAR back[]=_T("\\");

TCHAR dummyFileName[100]=_T("\0"); // path + resultName + extension 
TCHAR dummyFolderName[100]=_T("\0"); // path + resultName
TCHAR * dummy_name;
//TCHAR* dummyWhite[6];

DWORD specialCh[]={33, 35, 36, 37, 64};
DWORD allCh[] = { 33,35,36,37,64,48,49,50,51,52,
	53,54,55,56,57,65,66,67,68,69,
	70,71,72,73,74,75,76,77,78,79,
	80,81,82,83,84,85,86,87,88,89,
	90,97,98,99,100,101,102,103,104,105,
	106,107,108,109,110,111,112,113,114,115,
	116,117,118,119,120,121,122};

DWORD firstIndex=0;					// rand 함수 --> 첫번째 글자 index
DWORD remainIndex=0;				// rand 함수 --> 나머지 7글자 index
TCHAR resultName[8]=_T("\0");		// 결과: 더미파일 이름

int makeDummyName(){

	dummyWhite[dummyNum] = new TCHAR [100];
	//memset(dummyWhite, 0, sizeof(dummyWhite));
	srand(time(NULL));
	// C:\ , $Recycle.Bin, Document
	
	for(int j=0; j<3; j++){
		//memset(resultName, 0, sizeof(resultName));
		firstIndex=rand()%5;
		resultName[0] = specialCh[firstIndex];
		printf("resultName[0]: %c\n", resultName[0]);

		for(int i=1; i<8; i++){
			remainIndex=rand()%67;
			printf("index: %d\n", remainIndex);
			resultName[i]=allCh[remainIndex];
			printf("resultName[%d]: %c\n", i,resultName[i]);
		}

		printf("최종 더미 파일 이름: ");
		for(int i=0; i<8; i++){
			printf("%c",resultName[i]);
		}

		_tcscpy(dummyWhite[j], resultName);
		_tcscat(dummyWhite[j], back);
		printf("<---------- %S ----------> \n", dummyWhite[j]);

		dummyNum++;

		printf("\n");
	}
	
	//Documents for문

	return 0;
}

int makeDummy()
{	
	makeDummyName();
	DWORD lpdwFlags[100];
	DWORD res;
	BOOL test;

	TCHAR fileData[] = _T("This is Dummy File");

	// 더미 폴더 이름 설정
	_tcscpy(dummyFolderName, path);			// dummyFolderName = C:\Users\kosta\Documents(\)
	_tcscat(dummyFolderName, resultName);	// dummyFolderName = C:\Users\kosta\Documents\[랜덤이름]

	// 더미 폴더 생성
	res = CreateDirectory(dummyFolderName, NULL);

	// 더미 폴더 생성 에러 처리
	if (res == 0)
	{
		DWORD err = GetLastError();
		printf("err %d\n", err);
	}
	else
		printf("Create Folder SUCCESS!!!!!!! \n");

	// 더미 파일 이름 설정
	_tcscpy(dummyFileName, dummyFolderName);	// dummyFileName = C:\Users\kosta\Documents\[랜덤이름]
	_tcscat(dummyFileName, back);				// dummyFileName = C:\Users\kosta\Documents\[랜덤이름](\)
	_tcscat(dummyFileName, resultName);			// dummyFileName = C:\Users\kosta\Documents\[랜덤이름]\[랜덤이름]
	_tcscat(dummyFileName, extension);			// dummyFileName = C:\Users\kosta\Documents\[랜덤이름]\[랜덤이름].txt

	printf("최종 더미 파일이름과 경로: ");
	for(int i=0; i<sizeof(dummyFileName)/sizeof(TCHAR); i++){
		printf("%c", dummyFileName[i]);
	}
	
	printf("\n");

	// 얘를 for 문으로 묶어서 3번 반복할 것임 
	// 더미 파일 생성
	HANDLE hFile = CreateFile( dummyFileName, 
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE,					// share for writing
		NULL,								// default security
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,				// 일반 파일 
		NULL);								// 기존에 존재하는 파일과 동일한 특성을 가지는 새 파일을 만들 때 사용되는 전달인자 

	if(hFile == INVALID_HANDLE_VALUE){
		DWORD err = GetLastError();
		printf("err %d\n", err);
		return 1;
	}
	else
	{
		printf("File's HANDLE is OK!\n");
		test = GetHandleInformation(hFile, lpdwFlags);
		printf("The return value is %d, error %d\n", test, GetLastError());
	}

	DWORD numOfByteWritten=0;
	WriteFile(
		hFile, fileData, sizeof(fileData), &numOfByteWritten, NULL
		);

	CloseHandle(hFile);


	return 0;
}

