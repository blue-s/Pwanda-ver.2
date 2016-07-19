#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#ifdef _WIN32
	#include <io.h>
#else
	#include <unistd.h>
	// http://pubs.opengroup.org/onlinepubs/7908799/xsh/unistd.h.html
#endif



int main()
{
	struct stat *buf;
	stat("C:\\setup.log", buf);
	
	if(S_ISREG(buf->st_mode))
		puts("regular file");
	else if(S_ISDIR(buf->st_mode))
		puts("directory");
	else
		puts("other file");

	return 0;
}