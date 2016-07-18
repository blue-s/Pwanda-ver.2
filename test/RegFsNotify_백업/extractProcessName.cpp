#include "mon.h"

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
}

BOOL compare(void){  
	BOOL run = FALSE;
	BOOL prefetch = FALSE;

	for(int i=roamingNum-1; i>=0; i--)
	{
		for(int j=runNum-1; j>=0; j--)
		{
			if(_tcscmp(roamingList[i], runList[j]) == 0)
				return TRUE;
		}
	}

	for(int i=roamingNum-1; i>=0; i--)
	{
		for(int j=prefetchNum-1; j>=0; j--)
		{
			if(_tcscmp(roamingList[i], prefetchList[j]) == 0)
				return TRUE;
		}
	}

	return FALSE;
}

void ExtractProcess(DWORD flag, TCHAR *fileName){
	TCHAR buffer[100];
	_tcsncpy(buffer, fileName, 100);
	
	// Char·Î º¯È¯
	// flag : (1) Roaming, (2) Prefetch, (3) Run, (-1) Nothing
	switch(flag)
	{
	case 1:
		Output_Console(FOREGROUND_BLUE, _T("-----------------File> roaming -------------->>>>>>> %s\n"), fileName);
		roamingList[roamingNum] = new TCHAR [100];
		_tcsncpy(roamingList[roamingNum], buffer, 100);
		roamingNum += 1;
		break;
	case 2:
		Output_Console(FOREGROUND_BLUE, _T("-----------------File> prefetch -------------->>>>>>> %s\n"), fileName);
		prefetchList[prefetchNum] = new TCHAR [100];
		_tcsncpy(prefetchList[prefetchNum], buffer, 100);
		prefetchNum += 1;
		break;
	case 3:
		Output_Console(FOREGROUND_BLUE, _T("-----------------Registry> Run -------------->>>>>>> %s\n"), fileName);
		runList[runNum] = new TCHAR [100];
		_tcsncpy(runList[runNum], buffer, 100);
		runNum += 1;
		break;
	default:
		printf("Error!\n");
		break;
	}

	ListPrint();
	// ListProcessInfo();
	
	if(compare())
	{
		if(KillProcess(fileName)){
			Output_Console(FOREGROUND_RED, _T("*** KILL SUCCESS!!!\n\n"));
			return;
		}
	}
	Output_Console(FOREGROUND_RED, _T("*** Safe Process\n\n"));
}