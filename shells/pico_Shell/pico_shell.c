#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>


#define MAX_READ 20000
#define MAX_SIZE 20000
char pico_argv[MAX_SIZE][MAX_READ];
int pico_argc = 0;
int status = 0;

//Function Prototypes
void errExit(char* error_msg, int error_num);
void parse_commandline(char buffer[], int read_count);


int main(int argc, char* argv[])
{

	char buffer[MAX_READ];
	while(1)
	{	
		//Get command line arguments from user	
		printf("myshell command > ");
		fflush(stdout);
		if(fgets(buffer, MAX_READ,stdin) == NULL)
                        break;

		//Parse the command line into tokens
		parse_commandline(buffer, strlen(buffer));
	
		//Empty command
		if(strlen(pico_argv[0]) == 0)
			continue;
	
		//Echo Command
		if(strcmp(pico_argv[0], "echo") == 0)
		{
			for(int i = 1; i < pico_argc; i++)
			{
				if(i == pico_argc - 1)
					printf("%s\n", pico_argv[i]);
				else
					printf("%s ", pico_argv[i]);

			}
		}
		//PWD command
		else if(strcmp(pico_argv[0], "pwd") == 0)
		{
			char* cwd = getcwd(NULL, 0);
			if(cwd == NULL)
			{
				printf("Failed to Find working directory\n");
				status = -1;	
			}
			else
			{
				printf("%s\n", cwd);
			}
		}
		//cd Command
		else if(strcmp(pico_argv[0], "cd") == 0)
		{
			status = chdir(pico_argv[1]);
			if(status != 0)
			{
				printf("cd: %s: No such file or directory\n", pico_argv[1]);
				
			}
		}
		//Exit command
		else if(strcmp(pico_argv[0], "exit") == 0)
		{
			printf("Good Bye :)\n");
			break;
		}
		//search for command in $PATH
		else
		{

			pid_t pid = 0;
			pid = fork();
			//Child process branch
			if(pid == 0)
			{
				//Construct the new command line argument for the fetched command from $PATH
				char *new_argv[pico_argc+1];
				for(int i = 0; i < pico_argc; i++)
				{
					new_argv[i] = pico_argv[i]; 
				}
				new_argv[pico_argc] = NULL;

				execvp(pico_argv[0], new_argv);
			       	status = -1;	
				printf("%s: command not found\n", pico_argv[0]);
				exit(status);
				

			}
			//parent process branch
			else if(pid > 0)
			{
				int child_status = 0;
				if(wait(&child_status) != pid)
				{
					status = child_status;
					errExit("Failed to Wait for Child process: Child status error: %d\n", child_status);
				}
				else if (WIFEXITED(child_status))
       					status = WEXITSTATUS(child_status);
    				else
        				status = -1;   // killed by signal or abnormal termination

			}
			//Failed to create new process
			else
			{
				status = -1;
				printf("Faild to Create new Process with PID: %d\n", pid);
			}
		}	


		//Reset command arguments
		for(int i = 0; i < pico_argc; i++)
			memset(pico_argv[i], 0, MAX_SIZE);
		pico_argc = 0;
	}	
	return status;


}

void errExit(char* error_msg, int error_num)
{
        printf("%s", error_msg);
        exit(error_num);
}

void parse_commandline(char buffer[], int read_count)
{
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
				strcpy(pico_argv[pico_argc], temp);
				pico_argc++;
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
}
