#include "mon.h"

void saveDummyToDB(){
	
	FILE *fp;
	
	fp = fopen("c:\\dummyList_by_Pwanda.txt", "w+"); // w+ : ������ ������ ���� , ������ ���� ���Ͽ� ���� ����  
	
	for(int i=0; i<6; i++){
		fprintf(fp, "%S\n", dummyWhite[i]);
	}
	fclose(fp);
	

}

