#include "mon.h"
#include <stdlib.h>
#include <time.h>

TCHAR path_C[]=_T("c:\\");
TCHAR path_Recycle[]=_T("C:\\$Recycle.Bin\\");
TCHAR white_Recycle[]=_T("$Recycle.Bin");

TCHAR extension[]=_T(".txt");
TCHAR back[]=_T("\\");

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
		dummyWhite[j] = new TCHAR [BUFSIZ];
		firstIndex=rand()%5;
		resultName[0] = specialCh[firstIndex];

		for(int i=1; i<8; i++){
			remainIndex=rand()%67;
			resultName[i]=allCh[remainIndex];
		}

		printf("** ���� ���� ���� �̸� - ");
		printf("[%d] result name: %S\n",j, resultName);

		_tcscpy(dummyWhite[j], resultName);
		_tcscat(dummyWhite[j], back); 
		//--> �̸�\\
		//printf("dummyWhite:-------------%S \n", dummyWhite[j]);


		dummyNum++;
		printf("dummyNum: %d\n", dummyNum);

		printf("\n");

		if(j<3){

			// ���� ���� �̸� ����
			_tcscpy(dummyFolderName, path_C);			// dummyFolderName = C:\Users\kosta\Documents(\)
			_tcscat(dummyFolderName, resultName);	// dummyFolderName = C:\Users\kosta\Documents\[�����̸�]

			printf("%S\n ", dummyFolderName);


			// ���� ���� ����
			res = CreateDirectory(dummyFolderName, NULL);
			printf("dummyFolderName: %S\n", dummyFolderName);


			// ���� ���� ���� ���� ó��
			if (res == 0)
			{
				DWORD err = GetLastError();
				printf("err %d\n", err);
			}
			else
				printf("Create Folder SUCCESS!!!!!!! \n");

			// ���� ���� �̸� ����
			_tcscpy(dummyFileName, dummyFolderName);	// dummyFileName = C:\Users\kosta\Documents\[�����̸�]
			_tcscat(dummyFileName, back);				// dummyFileName = C:\Users\kosta\Documents\[�����̸�](\)
			_tcscat(dummyFileName, resultName);			// dummyFileName = C:\Users\kosta\Documents\[�����̸�]\[�����̸�]
			_tcscat(dummyFileName, extension);			// dummyFileName = C:\Users\kosta\Documents\[�����̸�]\[�����̸�].txt

			printf("���� ���� �����̸��� ���: ");
			printf("dummyFileName: %S\n", dummyFileName);

			printf("\n");

		}
		else if(j>2){
			// ���� ���� �̸� ����
			_tcscpy(dummyFolderName, path_Recycle);			// dummyFolderName = C:\Users\kosta\Documents(\)
			_tcscat(dummyFolderName, resultName);	// dummyFolderName = C:\Users\kosta\Documents\[�����̸�]

			printf("%S\n ", dummyFolderName);


			// ���� ���� ����
			res = CreateDirectory(dummyFolderName, NULL);
			printf("dummyFolderName: %S\n", dummyFolderName);


			// ���� ���� ���� ���� ó��
			if (res == 0)
			{
				DWORD err = GetLastError();
				printf("err %d\n", err);
			}
			else
				printf("Create Folder SUCCESS!!!!!!! \n");

			// ���� ���� �̸� ����
			_tcscpy(dummyFileName, dummyFolderName);	// dummyFileName = C:\Users\kosta\Documents\[�����̸�]
			_tcscat(dummyFileName, back);				// dummyFileName = C:\Users\kosta\Documents\[�����̸�](\)
			_tcscat(dummyFileName, resultName);			// dummyFileName = C:\Users\kosta\Documents\[�����̸�]\[�����̸�]
			_tcscat(dummyFileName, extension);			// dummyFileName = C:\Users\kosta\Documents\[�����̸�]\[�����̸�].txt

			printf("���� ���� �����̸��� ���: ");
			printf("dummyFileName: %S\n", dummyFileName);

			printf("\n");
		
		
		}

		HANDLE hFile = CreateFile( dummyFileName, 
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_WRITE,					// share for writing
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

