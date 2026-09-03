#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define MAX_READ 20000
#define MAX_SIZE 20000
char buffer[MAX_READ];
char femto_argv[MAX_SIZE][MAX_READ];
int femto_argc = 0;
int status = 0;


void errExit(char* error_msg, int error_num)
{
        printf("%s", error_msg);
        exit(error_num);
}

int main(int argc, char* argv[])
{
	while(1)
	{

		printf("myshell command > ");	
		if(fgets(buffer, MAX_READ,stdin) == 0)
			break;

		int temp_index = 0;
		char temp[MAX_SIZE];
		memset(temp, 0, MAX_SIZE);
		for(int i = 0; i < strlen(buffer); i++)
		{		

		       	if(buffer[i] == ' ' || buffer[i] == '\n')
			{
				if(temp_index > 0)
				{
					temp[temp_index] = '\0';
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
			{
				 if(i != femto_argc -1)
                                    printf("%s ", femto_argv[i]);
                                else
                                    printf("%s", femto_argv[i]);
			}
			printf("\n");
		}
		else if(strcmp(femto_argv[0], "exit") == 0)
		{
			printf("Good Bye :)\n");
			break;
		}
		else if(femto_argc == 0)
		{
			continue;
		}
		else
		{
			printf("Invalid command\n");
			status = -1;
		}	


		for(int i = 0; i < femto_argc; i++)
			memset(femto_argv[i], 0, MAX_SIZE);
		femto_argc = 0;
	}	
	return status;


}
