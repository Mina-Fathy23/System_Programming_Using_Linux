#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define count 100
char buf[count];

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("Usage: %s file-name\n", argv[0]);
		exit(-1);
	}
	int fd = open(argv[1], O_RDONLY);
	if(fd  < 0)
	{
		printf("failed to open file\n");
		exit(-2);
	}
	int read_count = 0;
	while((read_count = read(fd, buf, count)) > 0)
	{
		if(write(1, buf, read_count) < 0)
		{
			printf("Faild to write to terminal");
			exit(-3);
		}
	}
	if(close(fd) < 0)
	{
		printf("Failed to close file");
		exit(-4);
	}
	return 0;
}
