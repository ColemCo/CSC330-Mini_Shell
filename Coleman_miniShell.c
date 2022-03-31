#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/wait.h>

int userInput(char* rawInput)
{
	char* buff;

	buff = readline("\n>>> ");
	if(strlen(buff) != 0)
	{
		strcpy(rawInput, buff);
		return 0;
	}
	else 
	{
		return 1;
	}
}

void printCWD()
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("\nCWD: %s", cwd);
}

int changeDir(char** path)
{
	int fail;
	fail = chdir(path[1]);
	if(fail == -1)	//chdir returns -1 when failed
	{
		printf("Changing directory failed");
	}
}

void parseCMD(char* rawInput, char** parsedCMD) //accepts the input and the char where the parsed input will be stored
{
	for(int i = 0; i < 100; i++)	//loops through the max amount of commands there could be (size of inputString)
	{
		parsedCMD[i] = strsep(&rawInput, " ");	//parses by a space

		if(parsedCMD[i] == NULL)	//if the first parsed command is NULL then exit for loop as there isnt a command
		{
			break;
		}

		if(strlen(parsedCMD[i]) == 0)	//if the size of the command in index i is 0 then retry
		{
			i--;
		}
	}
}

void execCMD(char** parsedCMD)
{
	pid_t pid = fork();	//creates fork

	if (pid == -1)	//-1 is fork error
	{
		printf("Fork Failed");
	}
	else if (pid == 0)	
	{
		if(execvp(parsedCMD[0], parsedCMD) == -1)	//execvp returns -1 if failed
		{
			printf("Command execution failed");
		}
		exit(0);	//exits child process
	}
	else
	{
		wait(NULL);	//makes parent process wait for child to finish
		return;
	}
}

void help()
{
	printf("\nMINI SHELL HELP");
	printf("\nList of Supported Commands");
	printf("\ncd\nls\nexit\nGeneral UNIX commands\n");
}

int main()
{
	char inputString[100];
	char* parsedCMD[100];

	while(1)
	{
		printCWD();
		userInput(inputString);
		parseCMD(inputString, parsedCMD);

		if(strcmp(parsedCMD[0], "cd") == 0)
		{
			changeDir(parsedCMD);
		}

		else if(strcmp(parsedCMD[0], "exit") == 0)
		{
			break;
		}

		else if(strcmp(parsedCMD[0], "help") == 0)
		{
			help();
		}
		else 
		{
			execCMD(parsedCMD);
		}

	}
	printf("Thank you for using my shell\n");
}
