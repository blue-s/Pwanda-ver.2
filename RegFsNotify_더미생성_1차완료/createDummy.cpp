#include "mon.h"
#include <stdlib.h>
#include <time.h>

TCHAR path_C[]=_T("C:\\");
TCHAR path_Documents[]=_T("C:\\Users\\"); // C:\\Users\\[[kosta]]\\Documents

TCHAR temp_Users[]=_T("Users\\");
TCHAR temp_Documents[]=_T("\\Documents\\");
TCHAR extension[]=_T(".txt");
TCHAR back[]=_T("\\");

TCHAR Doc_white[100]=_T("\0");                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
TCHAR Doc_path[100]=_T("\0");

TCHAR dummyFileName[100]=_T("\0"); // path + resultName + extension 
TCHAR dummyFolderName[100]=_T("\0"); // path + resultName

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
TCHAR resultName[100]=_T("\0");		// 결과: 더미파일 이름

TCHAR * dummyWhite[6];
int dummyNum;

int makeDummyName(){

	dummyNum = 0;
	DWORD lpdwFlags[100];
	DWORD res;
	BOOL test;

	TCHAR fileData[] = _T("This is Dummy File");

	srand(time(NULL));

	for(int j=0; j<6; j++)
	{
		dummyWhite[j] = new TCHAR [BUFSIZ];
		firstIndex=rand()%5;
		resultName[0] = specialCh[firstIndex];

		for(int i=1; i<8; i++){
			remainIndex=rand()%67;
			resultName[i]=allCh[remainIndex];
		}

		printf("** 최종 더미 파일 이름 - ");
		printf("[%d] result name: %S\n",j, resultName);

		dummyNum++;
		printf("dummyNum: %d\n", dummyNum);

		printf("\n");

		if(j<3){
			_tcscpy(dummyWhite[j], resultName);
			_tcscat(dummyWhite[j], back); 
			//--> 이름\\,
			printf("dummyWhite C:\ : ------------->> %S \n", dummyWhite[j]);

			// 더미 폴더 이름 설정
			_tcscpy(dummyFolderName, path_C);		// dummyFolderName = C:\,
			_tcscat(dummyFolderName, resultName);	// dummyFolderName = C:\[랜덤이름]

			printf("%S\n ", dummyFolderName);		// dummyFolderName = C:\[랜덤이름]
			

			// 더미 폴더 생성
			res = CreateDirectory(dummyFolderName, NULL);
			printf("dummyFolderName: %S\n", dummyFolderName);


			// 더미 폴더 생성 에러 처리
			if (res == 0)
			{
				DWORD err = GetLastError();
				printf("err %d\n", err);
			}
			else
				printf("Create Folder SUCCESS!!!!!!! \n");

			// 더미 파일 이름 설정
			_tcscpy(dummyFileName, dummyFolderName);	// dummyFileName = C:\[랜덤이름]
			_tcscat(dummyFileName, back);				// dummyFileName = C:\[랜덤이름]\,
			_tcscat(dummyFileName, resultName);			// dummyFileName = C:\[랜덤이름]\[랜덤이름]
			_tcscat(dummyFileName, extension);			// dummyFileName = C:\[랜덤이름]\[랜덤이름].txt

			printf("최종 더미 파일이름과 경로: ");
			printf("dummyFileName: %S\n", dummyFileName);

			printf("\n");

		}
		else if(j>2){
			_tcscpy(dummyWhite[j], Doc_white);	// dummyWhite = Doc_white = Users\kosta\Documents\,
			_tcscat(dummyWhite[j], resultName);	// dummyWhite = Users\kosta\Documents\[랜덤명]
			_tcscat(dummyWhite[j], back);		// dummyWhite = Users\kosta\Documents\[랜덤명]\,
			//--> 이름\\,
			printf("dummyWhite Documents : -------------%S \n", dummyWhite[j]);

			// 더미 폴더 이름 설정
			_tcscpy(dummyFolderName, Doc_path);		// dummyFolderName = Doc_path = C:\Users\kosta\Documents\,
			_tcscat(dummyFolderName, resultName);	// dummyFolderName = C:\Users\kosta\Documents\[랜덤이름]

			printf("%S\n ", dummyFolderName);		// dummyFolderName = C:\Users\kosta\Documents\[랜덤이름]

			// 더미 폴더 생성
			res = CreateDirectory(dummyFolderName, NULL);
			printf("dummyFolderName: %S\n", dummyFolderName);


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
			_tcscat(dummyFileName, back);				// dummyFileName = C:\Users\kosta\Documents\[랜덤이름]\,
			_tcscat(dummyFileName, resultName);			// dummyFileName = C:\Users\kosta\Documents\[랜덤이름]\[랜덤이름]
			_tcscat(dummyFileName, extension);			// dummyFileName = C:\Users\kosta\Documents\[랜덤이름]\[랜덤이름].txt

			printf("최종 더미 파일이름과 경로: ");
			printf("dummyFileName: %S\n", dummyFileName);

			printf("\n");
		
		
		}

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
	}
	return 0;

}

int makeDummy()
{	
	makeDummyName();
	printf("\n\n\n\n");

	printf("%S\n", dummyWhite[0]);
	printf("%S\n", dummyWhite[1]);
	printf("%S\n", dummyWhite[2]);


	return 0;
}


void getUserName()
{

	TCHAR  infoBuf[MAX_VALUE_NAME];
	DWORD  bufCharCount = MAX_VALUE_NAME;

	bufCharCount = MAX_VALUE_NAME;
	if( !GetUserName( infoBuf, &bufCharCount ) )
		printf("GetUserName Error!!! \n"); 
	printf("\n----------------------- [User name] ----------------------->>  %S\n", infoBuf );

//TCHAR path_Documents[]=_T("C:\\Users\\"); 

//TCHAR temp_Users[]=_T("Users\\");
//TCHAR temp_Documents[]=_T("\\Documents\\");
//TCHAR extension[]=_T(".txt");
//TCHAR back[]=_T("\\");

//TCHAR Doc_white[100]=_T("\0");	// 목표 : \Users\kosta\Documents\[랜덤명]\
//TCHAR Doc_path[100]=_T("\0");		// 목표 : C:\\Users\\[[kosta]]\\Documents

	_tcscpy(Doc_white, temp_Users);				// Doc_white = Users\,
	_tcscat(Doc_white, infoBuf);				// Doc_white = Users\kosta
	_tcscat(Doc_white, temp_Documents);			// Doc_white = Users\kosta\Documents\,

	printf("<<<<<<<<<<<<<<<<<<<<<<< Doc_white <<<<<<<<<<<<<<<<<< %S \n", Doc_white);

	_tcscpy(Doc_path, path_Documents);			// Doc_path = C:\Users\,
	_tcscat(Doc_path, infoBuf);					// Doc_path = C:\Users\kosta
	_tcscat(Doc_path, temp_Documents);			// Doc_path = C:\Users\kosta\Documents\,

	printf("<<<<<<<<<<<<<<<<<<<<<<< Doc_path <<<<<<<<<<<<<<<<<< %S \n", Doc_path);
}
