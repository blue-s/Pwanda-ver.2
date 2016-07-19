#include <stdio.h>

int main(int argc, char** argv)
{
	if(argc <= 1)
	{
		printf("usage python use_virustotal.py [filename]\n");
	}
	else
		printf("success!\n");

	getchar();
	return 0;
}