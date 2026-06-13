#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	char * buf = getcwd(NULL, 0);

	if(buf == NULL)
	{
		printf("Faild to Find Current Working directorys' path");	
		exit(-1);
	}
	else
		printf("%s\n", buf);
	return 0;
}
