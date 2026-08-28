#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define STDIN_FILENO 0
#define STDOUT_FILENO 1

#define MAX_READ 1024
#define MAX_SIZE 100
char buffer[MAX_READ];
char femto_argv[MAX_SIZE][MAX_SIZE];
int femto_argc = 0;

void errExit(char* error_msg, int error_num)
{
        printf("%s", error_msg);
        exit(error_num);
}

int main(int argc, char* argv[])
{
	while(1)
	{	
		
		write(STDOUT_FILENO, "myshell command > ", strlen("myshell command > "));
		int read_count = read(STDIN_FILENO, buffer, MAX_READ);
		if (read_count < 0)
			 errExit("read failed\n", -2);

		int temp_index = 0;
		char temp[MAX_SIZE];
		memset(temp, 0, MAX_SIZE);
		for(int i = 0; i < read_count; i++)
		{		

		       	if(buffer[i] == ' ' || buffer[i] == '\n')
			{
				if(temp_index > 0)
				{
					temp[temp_index] = '\0';
					printf("Parsed input:%s\n", temp);
					strcpy(femto_argv[femto_argc], temp);
              	 		       	femto_argc++;
					temp_index = 0;
					memset(temp, 0, MAX_SIZE);
				}
				
			}
			else
			{
	
				 temp[temp_index] = buffer[i];
				 temp_index++;
				
			}

		}


		if(strcmp(femto_argv[0], "echo") == 0)
		{
			for(int i = 1; i < femto_argc; i++)
				printf("%s ", femto_argv[i]);
			printf("\n");
		}
		else if(strcmp(femto_argv[0], "exit") == 0)
		{
			printf("Good Bye :)\n");
			break;
		}
		else
			printf("Invalid command\n");
			


		for(int i = 0; i < femto_argc; i++)
			memset(femto_argv[i], 0, MAX_SIZE);
		femto_argc = 0;
	}	
	return 0;


}
