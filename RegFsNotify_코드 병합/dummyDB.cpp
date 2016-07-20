#include "mon.h"

void saveDummyToDB(){
	
	FILE *fp;
	
	fp = fopen("c:\\dummyList_by_Pwanda.txt", "w+"); // w+ : 파일이 없으면 생성 , 있으면 기존 파일에 덮어 쓰임  
	
	for(int i=0; i<6; i++){
		fprintf(fp, "%S\n", dummyWhite[i]);
	}
	fclose(fp);
	

}

