/*
	1. ��� C:\\, C:\\Users\\[[UserName]]\\Documents ���� ���� / ���� ���� �ҽ��ڵ� ����
	2. ������ ���� / ���� ȭ��Ʈ����Ʈ�� �ѱ�°� ����
*/

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

static TCHAR * dummyFileName[6]; // path + resultName + extension 
TCHAR dummyFolderName[100]=_T("\0"); // path + resultName

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

int makeDummyName(){
	dummyNum = 0;
	DWORD lpdwFlags[100];
	DWORD res;
	BOOL test;

	TCHAR fileData[] = _T("This is Dummy File");

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

			//printf("** ���� ���� ���� �̸� - ");
			//printf("[%d] result name: %S\n",j, resultName);

			dummyNum++;
			printf("\n");


			if(j<3){

				_tcscpy(dummyWhite[j], resultName);
				_tcscat(dummyWhite[j], back); 
				//--> �̸�\\,
				//printf("dummyWhite C:\ : ------------->> %S \n", dummyWhite[j]);

				// ���� ���� �̸� ����
				_tcscpy(dummyFolderName, path_C);		// dummyFolderName = C:\,
				_tcscat(dummyFolderName, resultName);	// dummyFolderName = C:\[�����̸�]

				//printf("%S\n ", dummyFolderName);		// dummyFolderName = C:\[�����̸�]


				// ���� ���� ����
				res = CreateDirectory(dummyFolderName, NULL);
				//printf("dummyFolderName: %S\n", dummyFolderName);


				// ���� ���� ���� ���� ó��
				if (res == 0)
				{
					DWORD err = GetLastError();
					printf("err %d\n", err);
				}
				else
					//printf("Create Folder SUCCESS!!!!!!! \n");

				// ���� ���� �̸� ����
				_tcscpy(dummyFileName[j], dummyFolderName);	// dummyFileName = C:\[�����̸�]
				_tcscat(dummyFileName[j], back);				// dummyFileName = C:\[�����̸�]\,
				_tcscat(dummyFileName[j], resultName);			// dummyFileName = C:\[�����̸�]\[�����̸�]
				_tcscat(dummyFileName[j], extension);			// dummyFileName = C:\[�����̸�]\[�����̸�].txt

				printf("���� ���� �����̸��� ���: ");
				printf("dummyFileName: %S\n", dummyFileName[j]);

				printf("\n");

			}
			else if(j>2){
				_tcscpy(dummyWhite[j], Doc_white);	// dummyWhite = Doc_white = Users\kosta\Documents\,
				_tcscat(dummyWhite[j], resultName);	// dummyWhite = Users\kosta\Documents\[������]
				_tcscat(dummyWhite[j], back);		// dummyWhite = Users\kosta\Documents\[������]\,
				//--> �̸�\\,
				//printf("dummyWhite Documents : -------------%S \n", dummyWhite[j]);

				// ���� ���� �̸� ����
				_tcscpy(dummyFolderName, Doc_path);		// dummyFolderName = Doc_path = C:\Users\kosta\Documents\,
				_tcscat(dummyFolderName, resultName);	// dummyFolderName = C:\Users\kosta\Documents\[�����̸�]

				//printf("%S\n ", dummyFolderName);		// dummyFolderName = C:\Users\kosta\Documents\[�����̸�]

				// ���� ���� ����
				res = CreateDirectory(dummyFolderName, NULL);
				//printf("dummyFolderName: %S\n", dummyFolderName);


				// ���� ���� ���� ���� ó��
				if (res == 0)
				{
					DWORD err = GetLastError();
					printf("err %d\n", err);
				}
				else
					//printf("Create Folder SUCCESS!!!!!!! \n");

				// ���� ���� �̸� ����
				_tcscpy(dummyFileName[j], dummyFolderName);	// dummyFileName = C:\Users\kosta\Documents\[�����̸�]
				_tcscat(dummyFileName[j], back);				// dummyFileName = C:\Users\kosta\Documents\[�����̸�]\,
				_tcscat(dummyFileName[j], resultName);			// dummyFileName = C:\Users\kosta\Documents\[�����̸�]\[�����̸�]
				_tcscat(dummyFileName[j], extension);			// dummyFileName = C:\Users\kosta\Documents\[�����̸�]\[�����̸�].txt

				printf("���� ���� �����̸��� ���: ");
				printf("dummyFileName: %S\n", dummyFileName[j]);

				printf("\n");


			}


		} // ���̰� ó�� ������ ���� 

		hFile = CreateFile( dummyFileName[j], 
			GENERIC_READ | GENERIC_WRITE,
			NULL,//FILE_SHARE_WRITE,					// share for writing
			NULL,								// default security
			CREATE_ALWAYS, 
			FILE_ATTRIBUTE_NORMAL,				// �Ϲ� ���� 
			NULL);								// ������ �����ϴ� ���ϰ� ������ Ư���� ������ �� ������ ���� �� ���Ǵ� �������� 


		if(hFile == INVALID_HANDLE_VALUE){
			DWORD err = GetLastError();
			printf("err %d\n", err);
			return 1;
		}

		else
		{
			//printf("File's HANDLE is OK!\n");
			test = GetHandleInformation(hFile, lpdwFlags);
			//printf("The return value is %d, error %d\n", test, GetLastError());
			/*
			GetLastError �Լ��� �������� Ȯ���غ��� ������ ���� ������ ��쿣 0�� ��ȯ������ ������ �̹� �ִ� ��쿡�� ERROR_ALREADY_EXISTS(183L)�� ��ȯ�ȴ�.
			*/
		}

		writeDummy(); // ���� ���� ���뾲��

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

	//printf("\n----------------------- [User name] ----------------------->>  %S\n", infoBuf );

	_tcscpy(Doc_white, temp_Users);				// Doc_white = Users\,
	_tcscat(Doc_white, infoBuf);				// Doc_white = Users\kosta
	_tcscat(Doc_white, temp_Documents);			// Doc_white = Users\kosta\Documents\,

	//printf("<<<<<<<<<<<<<<<<<<<<<<< Doc_white <<<<<<<<<<<<<<<<<< %S \n", Doc_white);

	_tcscpy(Doc_path, path_Documents);			// Doc_path = C:\Users\,
	_tcscat(Doc_path, infoBuf);					// Doc_path = C:\Users\kosta
	_tcscat(Doc_path, temp_Documents);			// Doc_path = C:\Users\kosta\Documents\,

	//printf("<<<<<<<<<<<<<<<<<<<<<<< Doc_path <<<<<<<<<<<<<<<<<< %S \n", Doc_path);
}

void makeDummy()
{	
	getUserName();
	makeDummyName();

}