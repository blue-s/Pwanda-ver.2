/*
	1. ���ǽ� ���� (� ������ �޾ƿ� ���ǽĿ� �����Ͽ�, ���μ��� Kill �ϴ���)
	2. ���μ��� Kill �ϰ�, deleteFileReg.cpp ���� ���� �Լ� ȣ���Ͽ� �ܿ� ���� ���� ����
		-> ������ �ѱ� ��, ȣ�� ��ġ
*/

#include "mon.h"

BOOL condition = FALSE;
TCHAR * same_Name;

void ListPrint(void){
	printf("*** Roaming List ***\n");
	for(int i=0; i<roamingNum; i++){
		printf("%S\n", roamingList[i]);
	}

	printf("*** Prefetch List ***\n");
	for(int i=0; i<prefetchNum; i++){
		printf("%S\n", prefetchList[i]);
	}

	printf("*** Run List ***\n");
	for(int i=0; i<runNum; i++){
		printf("%S\n", runList[i]);
	}
	printf("\n");
}

BOOL compare(void){  


	for(int i=roamingNum-1; i>=0; i--)
	{
		for(int j=runNum-1; j>=0; j--)
		{
			if(_tcsicmp(roamingList[i], runList[j]) == 0){
				same_Name=roamingList[i];
				return TRUE;
			}
				
		}
	}

	/*for(int i=roamingNum-1; i>=0; i--)
	{
		for(int j=prefetchNum-1; j>=0; j--)
		{
			if(_tcsicmp(roamingList[i], prefetchList[j]) == 0)
				return TRUE;
		}
	}
	*/

	return FALSE;
}

void ExtractProcess(DWORD flag, TCHAR *fileName){
	TCHAR buffer_filename[BUFSIZ];
	
	
	_tcsncpy(buffer_filename, fileName, BUFSIZ);
	
	// Char�� ��ȯ
	// flag : (1) Roaming, (2) Prefetch, (3) Run, (4) dummyfilecount (-1) Nothing
	switch(flag)
	{
	case 1:
		Output_Console(FOREGROUND_BLUE, _T("----------------- File > roaming -------------->>>>>>> %s\n"), fileName);
		roamingList[roamingNum] = new TCHAR [BUFSIZ];
		_tcsncpy(roamingList[roamingNum], buffer_filename, BUFSIZ);
		roamingNum += 1;
		break;
	case 2:
		Output_Console(FOREGROUND_BLUE, _T("----------------- File > prefetch -------------->>>>>>> %s\n"), fileName);
		prefetchList[prefetchNum] = new TCHAR [BUFSIZ];
		_tcsncpy(prefetchList[prefetchNum], buffer_filename, BUFSIZ);
		prefetchNum += 1;
		break;
	case 3:
		Output_Console(FOREGROUND_BLUE, _T("----------------- Registry > Run -------------->>>>>>> %s\n"), fileName);
		runList[runNum] = new TCHAR [BUFSIZ];
		_tcsncpy(runList[runNum], buffer_filename, BUFSIZ);
		runNum += 1;
		break;
	case 4:
		dummy_sense_count+= 1;
		Output_Console(FOREGROUND_BLUE, _T("----------------- File > dummyfile -------------->>>>>>> %d\n"), dummy_sense_count);
		break;
	default:
		printf("Error!\n");
		break;
	}


	//ListPrint();
	//ListProcessInfo();
	
	condition=compare();
	printf("##### condition output: %s #####\n", condition?"true":"false");
	printf("------------------------------ fileName : %S -----------------------------\n", same_Name);


	// Roaming ������ Run�� �ö�� ���μ��� �̸��� �����鼭, ������ ������ ���� 2�� �̻��� ��
	// Process Kill
	if(compare()&&(dummy_sense_count>1)) 
	{
		if(KillProcess(same_Name)){
			printf("process name : %S\n", same_Name);
			Output_Console(FOREGROUND_RED, _T("*** KILL SUCCESS !!!\n\n"));
			return;
		}
		
	}else{
		Output_Console(FOREGROUND_RED, _T("*** Safe Process\n\n"));
	}
}