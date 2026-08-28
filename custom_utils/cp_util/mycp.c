#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define count 100
char buf[count];

void errExit(char* error_msg, int error_num)
{
	printf("%s", error_msg);
	exit(error_num);
}


int main(int argc, char* argv[])
{
	if((argc != 3) || strcmp(argv[1], "--help") == 0)
	{
		printf("Usage: %s file-name\n", argv[0]);
		exit(-1);
	}

	int openfalg = 0644;
	int fdinput = open(argv[1], O_RDONLY);
	
	if(fdinput  < 0)
		errExit("Faild to open destination file\n", -2);


	int fdoutput = open(argv[2], O_RDWR | O_TRUNC | O_CREAT, openfalg); 
	if(fdoutput < 0)
		errExit("faild to open destination file\n", -2);

	int read_count = 0;
	while((read_count = read(fdinput, buf, count)) > 0)
	{
		if(write(fdoutput, buf, read_count) < 0)
			errExit("Faild to write to destination file\n", -3);
	}

	if(close(fdinput) < 0)
		errExit("Failed to close source file\n", -4);

	
	if(close(fdoutput) < 0)
		errExit("Failed to close destination  file\n", -4);

	return 0;
}
