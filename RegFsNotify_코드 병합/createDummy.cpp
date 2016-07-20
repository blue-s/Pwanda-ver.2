#include "mon.h"
#include <stdlib.h>
#include <time.h>

TCHAR path_C[]=_T("C:\\");
TCHAR path_Documents[]=_T("C:\\Users\\"); 

TCHAR temp_Users[]=_T("Users\\");
TCHAR temp_Documents[]=_T("\\Documents\\");
TCHAR extension[]=_T(".txt");
TCHAR back[]=_T("\\");

TCHAR Doc_white[100]=_T("\0");                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
TCHAR Doc_path[100]=_T("\0");

static TCHAR * dummyFileName[6];		// path + resultName + extension 
TCHAR dummyFolderName[100]=_T("\0");	// path + resultName

DWORD specialCh[]={33, 35, 36, 37, 64};
DWORD allCh[] = { 33,35,36,37,64,48,49,50,51,52,
	53,54,55,56,57,65,66,67,68,69,
	70,71,72,73,74,75,76,77,78,79,
	80,81,82,83,84,85,86,87,88,89,
	90,97,98,99,100,101,102,103,104,105,
	106,107,108,109,110,111,112,113,114,115,
	116,117,118,119,120,121,122};

DWORD firstIndex=0;					// rand �Լ� --> ù��° ���� index
DWORD remainIndex=0;				// rand �Լ� --> ������ 7���� index
TCHAR resultName[100]=_T("\0");		// ���: �������� �̸�

TCHAR * dummyWhite[6];
int dummyNum;

int makeDummy(){
	dummyNum = 0;
	DWORD lpdwFlags[100];
	DWORD res;
	BOOL test;

	srand(time(NULL));

	for(int j=0; j<6; j++)
	{

		if(!dbFlag){
			dummyWhite[j] = new TCHAR [BUFSIZ];
			dummyFileName[j] = new TCHAR [BUFSIZ];

			firstIndex=rand()%5;
			resultName[0] = specialCh[firstIndex];

			for(int i=1; i<8; i++){
				remainIndex=rand()%67;
				resultName[i]=allCh[remainIndex];
			}

			dummyNum++;

			if(j<3){

				_tcscpy(dummyWhite[j], resultName);
				_tcscat(dummyWhite[j], back); 
				
				// ���� ���� �̸� ����
				_tcscpy(dummyFolderName, path_C);		// dummyFolderName = C:\,
				_tcscat(dummyFolderName, resultName);	// dummyFolderName = C:\[�����̸�]


				// ���� ���� ����
				res = CreateDirectory(dummyFolderName, NULL);

				// ���� ���� ���� ���� ó��
				if (res == 0)
				{
					DWORD err = GetLastError();
					printf("err %d\n", err);
				}

				// ���� ���� �̸� ����
				_tcscpy(dummyFileName[j], dummyFolderName);		// dummyFileName = C:\[�����̸�]
				_tcscat(dummyFileName[j], back);				// dummyFileName = C:\[�����̸�]\,
				_tcscat(dummyFileName[j], resultName);			// dummyFileName = C:\[�����̸�]\[�����̸�]
				_tcscat(dummyFileName[j], extension);			// dummyFileName = C:\[�����̸�]\[�����̸�].txt

				printf("Path Dummy File : ");
				printf("dummyFileName: %S\n", dummyFileName[j]);

			}
			else if(j>2){
				_tcscpy(dummyWhite[j], Doc_white);	// dummyWhite = Doc_white = Users\kosta\Documents\,
				_tcscat(dummyWhite[j], resultName);	// dummyWhite = Users\kosta\Documents\[������]
				_tcscat(dummyWhite[j], back);		// dummyWhite = Users\kosta\Documents\[������]\,

				// ���� ���� �̸� ����
				_tcscpy(dummyFolderName, Doc_path);		// dummyFolderName = Doc_path = C:\Users\kosta\Documents\,
				_tcscat(dummyFolderName, resultName);	// dummyFolderName = C:\Users\kosta\Documents\[�����̸�]

				// ���� ���� ����
				res = CreateDirectory(dummyFolderName, NULL);

				// ���� ���� ���� ���� ó��
				if (res == 0)
				{
					DWORD err = GetLastError();
					printf("err %d\n", err);
				}

				// ���� ���� �̸� ����
				_tcscpy(dummyFileName[j], dummyFolderName);		// dummyFileName = C:\Users\kosta\Documents\[�����̸�]
				_tcscat(dummyFileName[j], back);				// dummyFileName = C:\Users\kosta\Documents\[�����̸�]\,
				_tcscat(dummyFileName[j], resultName);			// dummyFileName = C:\Users\kosta\Documents\[�����̸�]\[�����̸�]
				_tcscat(dummyFileName[j], extension);			// dummyFileName = C:\Users\kosta\Documents\[�����̸�]\[�����̸�].txt

				printf("Path Dummy File : ");
				printf("dummyFileName: %S\n", dummyFileName[j]);

			}


		} // ���̰� ó�� ������ ���� 

		hFile = CreateFile( dummyFileName[j], 
			GENERIC_READ | GENERIC_WRITE,
			NULL,		
			NULL,						
			CREATE_ALWAYS, 
			FILE_ATTRIBUTE_NORMAL,				
			NULL);								


		if(hFile == INVALID_HANDLE_VALUE){
			DWORD err = GetLastError();
			printf("err %d\n", err);
			return 1;
		}

		else
		{
			test = GetHandleInformation(hFile, lpdwFlags);
			/*
			GetLastError �Լ��� �������� Ȯ���غ��� ������ ���� ������ ��쿣 0�� ��ȯ������ ������ �̹� �ִ� ��쿡�� ERROR_ALREADY_EXISTS(183L)�� ��ȯ�ȴ�.
			*/
		}

		writeDummy(); // ���� ���� ���� ����

	}

	if(dbFlag==true){
		dbFlag=false;
	}

	return 0;
}

void writeDummy() 
{
	TCHAR fileData[] = {
		_T("This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! This is Dummy File !! Do Not Modify or Delete !!! ")
	};

	TCHAR * pFileData = fileData;

	DWORD numOfByteWritten=0;

	// ũ�� ������ ���� 20�� �ݺ�
	for(int i=0; i<20; i++)
	{
		WriteFile(
			hFile, pFileData, sizeof(fileData), &numOfByteWritten, NULL
			);
		i++;
	}

	CloseHandle(hFile); 
}

void getUserName()
{

	TCHAR  infoBuf[MAX_VALUE_NAME];
	DWORD  bufCharCount = MAX_VALUE_NAME;

	bufCharCount = MAX_VALUE_NAME;
	if( !GetUserName( infoBuf, &bufCharCount ) ){
		printf("GetUserName Error!!! \n"); 
	}

	_tcscpy(Doc_white, temp_Users);				// Doc_white = Users\,
	_tcscat(Doc_white, infoBuf);				// Doc_white = Users\kosta
	_tcscat(Doc_white, temp_Documents);			// Doc_white = Users\kosta\Documents\,

	_tcscpy(Doc_path, path_Documents);			// Doc_path = C:\Users\,
	_tcscat(Doc_path, infoBuf);					// Doc_path = C:\Users\kosta
	_tcscat(Doc_path, temp_Documents);			// Doc_path = C:\Users\kosta\Documents\,

}

void dummy_main()
{	
	getUserName();
	makeDummy();

}