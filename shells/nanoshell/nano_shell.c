#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAX_READ 20000
#define MAX_SIZE 20000

extern char** environ;

char **new_argv;
int new_argc = 0;

int status = 0;

int local_var_num = 0;
int local_var_cap = 3;
char **local_var;

//Function Prototypes
void parse_commandline(char buffer[], int read_count);
int check_local_var(void);
int find_local_var(char* var, int var_len);

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		printf("Usage: the Shell doesn't have arguments, please run it again without arguments\n");
		exit(-1);
	}

	char input[MAX_READ];
	input[0] = 0;

	local_var = (char **)malloc(sizeof(char *) * (local_var_cap + 1));

	while(1)
	{	
		//Get command line arguments from user	
		printf("myshell command > ");
		fflush(stdout);
		if(fgets(input, MAX_READ,stdin) == NULL)
                        break;
		

		//Parse the command line into tokens
		parse_commandline(input, strlen(input));
			
		//Empty command
		if(strlen(new_argv[0]) == 0)
			continue;

		//Store local variables
		status = check_local_var();	
		
		//Incorrect command
		if(status == -1)
		{
			printf("Invalid command\n");
			continue;
		}
		//Successfully saved Local variable
		else if(status == 0)
		{
			continue;
		}
		//no variable enterned: contineu with loop

	
		//Echo Command
		if(strcmp(new_argv[0], "echo") == 0)
		{
			for(int i = 1; i < new_argc; i++)
			{
				if(i == new_argc - 1)
					printf("%s\n", new_argv[i]);
				else
					printf("%s ", new_argv[i]);

			}
		}
		//PWD command
		else if(strcmp(new_argv[0], "pwd") == 0)
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
		else if(strcmp(new_argv[0], "cd") == 0)
		{
			status = chdir(new_argv[1]);
			if(status != 0)
			{
				printf("cd: %s: No such file or directory\n", new_argv[1]);
				
			}
		}
		//Export Environemnt Variables
		else if(strcmp(new_argv[0], "export") == 0)
		{
			if(new_argc == 2)
			{
				int local_var_idx = find_local_var(new_argv[1], strlen(new_argv[1]));
				//Local variable not found
				if(local_var_idx < 0)
				{
					printf("Didn't find var :%s\n", new_argv[1]);
					status = -1;
				}
				else
				{
					status =  putenv(local_var[local_var_idx]);
					if(status != 0)
						printf("Failed to export %s\n", local_var[local_var_idx]);

				}
			}
			else
			{
				printf("Usage: export $local_variable_name\n");
				status = -1;
			}
		}
		//print all local variables
		else if(strcmp(new_argv[0], "local") == 0)
		{
			
		 	for(int i = 0; i < local_var_num; i++)
				printf("local_var[%d] : %s\n", i, local_var[i]);
		}
		//Exit command
		else if(strcmp(new_argv[0], "exit") == 0)
		{
			for (int i = 0; i < new_argc; i++)
				free(new_argv[i]);
			free(new_argv);

			for(int i = 0; i < local_var_num; i++)
				free(local_var[i]);
			free(local_var);
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
				//Construct the child command line argument for the fetched command from $PATH
				char *child_argv[new_argc+1];
				for(int i = 0; i < new_argc; i++)
				{
					child_argv[i] = new_argv[i]; 
				}
				child_argv[new_argc] = NULL;

				execvpe(new_argv[0], child_argv, environ);
			       	status = -1;	
				printf("%s: command not found\n", new_argv[0]);
				exit(status);
				

			}
			//parent process branch
			else if(pid > 0)
			{
				int child_status = 0;
				if(wait(&child_status) != pid)
				{
					status = child_status;
					printf("Failed to Wait for Child process: Child status error: %d\n", child_status);
					exit(status);
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
		for (int i = 0; i < new_argc; i++)
			free(new_argv[i]);
		free(new_argv);	
	}	
	return status;

}

int find_local_var(char* var, int var_len)
{
	for(int i = 0; i < local_var_num; i++)
	{
		if(strncmp(local_var[i], var, var_len) == 0)
			return i;
	}

	return -1;

}

char* get_special_arg(char* arg)
{
	char * dollar_pos = strstr(arg, "$");
	if(dollar_pos == NULL)
	{
		return arg;
	}
	//Found special Char 
	else
	{
		//start the key after the $ symbol
		char* name_start = dollar_pos + 1;

		//Calculating the length of variable
		int name_len = 0;
    		while (isalnum((unsigned char)name_start[name_len]) || name_start[name_len] == '_' || name_start[name_len] == '-')
        		name_len++;

		if (name_len == 0)
		{ 
			char *newline = malloc(1);
   			newline[0] = '\0';
   			return newline;
		}

		char *name_end = name_start + name_len;

		for(int i = 0; i < local_var_num; i++)
		{
			char* eq = strstr(local_var[i], "=");
			//Found key value
			if(eq != NULL)
			{
				int key_len = eq - local_var[i];
				if(key_len == name_len && strncmp(local_var[i], name_start, name_len) ==0)
				{
		        	    	char *value = eq + 1;
			           	int prefix_len = dollar_pos - arg;
	           			int value_len  = strlen(value);
	           			int suffix_len = strlen(name_end);
	           			int new_len = prefix_len + value_len + suffix_len + 1; // +1 for '\0'
	
				        char *new_arg = malloc(new_len);
	            			memcpy(new_arg, arg, prefix_len);
	            			memcpy(new_arg + prefix_len, value, value_len);
	            			memcpy(new_arg + prefix_len + value_len, name_end, suffix_len + 1); // copies '\0' too

	           		 	return new_arg;
        			}
			}
			else
				continue;	
		}
		//Didnt' Find key
		char *newline = malloc(1);
   		newline[0] = '\0';
   		return newline;

	}
	return arg;
}


void parse_commandline(char buffer[], int read_count)
{
	int alloc_cap = 8;
	//Initialy allocating memory for argv
	new_argv = (char**)malloc(sizeof(char*) * (alloc_cap + 1));
	
	int temp_index = 0;
	char *temp = strtok(buffer, " ");

	while(temp != NULL)
	{
		if(temp_index >= alloc_cap - 1)
		{
			alloc_cap *= 2;
			new_argv = (char **)realloc(new_argv, sizeof(char *) * alloc_cap);
		}
		//remove \n in the last token
		if(temp[strlen(temp) - 1] == '\n')
			temp[strlen(temp) - 1] = 0;
		
		temp = get_special_arg(temp);
				
		new_argv[temp_index] = malloc(strlen(temp) + 1);
		strcpy(new_argv[temp_index], temp);
		temp_index++;
		temp = strtok(NULL, " ");
	}
	new_argv[temp_index] = NULL;
	new_argc = temp_index;
}


int check_local_var(void)
{
	int exit_code = 0;
	int var_name_len = 0;
	char* found;
	for(int i = 0; i < new_argc; i++)
	{
		found = strstr(new_argv[i], "=");
		//Found
		if(found != NULL)
		{       
			if(new_argc == 1)
			{
				//Not an end or start character
				if((found !=  new_argv[i]) && ((found - new_argv[i] + 1) != strlen(new_argv[i])))
				{
					//set Variable name length
					var_name_len = found - new_argv[i] + 1;
					int stored_var_index = find_local_var(new_argv[i], var_name_len);
					
					//variable already exits
					if(stored_var_index != -1)
					{	
						local_var[stored_var_index] = (char*)realloc(local_var[stored_var_index], strlen(new_argv[i]+1));
						strcpy(local_var[stored_var_index], new_argv[i]);
					}
					else
					{
						if (local_var_num >= local_var_cap) 
						{
    							local_var_cap *= 2;
						    	local_var = (char **)realloc(local_var, sizeof(char *) * (local_var_cap + 1));
						}
						local_var[local_var_num] = malloc(strlen(new_argv[i]) + 1);
						strcpy(local_var[local_var_num], new_argv[i]);
						local_var_num++;
					}
					exit_code = 0;
					break;
				}
				else
				{
					exit_code = -1;
					break;
				}
			}
			//multiple agruments (invalid input)
			else
			{
				exit_code = -1;
				break;
			}
		}
		//Didn't find =
		else
		{
			exit_code = 1;
		}
	}
	return exit_code;
}
