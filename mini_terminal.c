#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define MAX_SIZE 1000
#define PROMPT '$'
#define END_TERMINAL "Leave"
#define DELIM_PATH ":"
#define DELIM_COMMANDS " "

void getParams(char *command, char** params)
{
    char *str;
    strcpy(str, command);
    char *token, *nextToken;
    token = strtok_r(str, DELIM_COMMANDS, &nextToken);
    int i = 0;
    while (token != NULL)
    {
        params[i++] = token;
        token = strtok_r(NULL, DELIM_COMMANDS, &nextToken);
    }
    params[i] = NULL;
}

void getFullPath(char *token, char*fullPath, char**params){
    strcpy(fullPath, token);
    strcat(fullPath, "/");
    strcat(fullPath, params[0]);
}

void main()
{
    int pid, stat;
    char path[MAX_SIZE];
    strcpy(path, getenv("PATH")); // Getting the PATH param value and copying its value to another string
    char command[MAX_SIZE];
    char *params[MAX_SIZE] = {NULL};
    char fullPath[MAX_SIZE];
    char *token, *nextToken;

    putchar(PROMPT);
    fgets(command, MAX_SIZE, stdin);
    command[strlen(command)-1] = '\0';
    while (strcasecmp(command, END_TERMINAL))
    {
        getParams(command, params); // Getting the params of the current command
        if ((pid = fork()) == 0)
        {
            token = strtok_r(path, DELIM_PATH, &nextToken);
            while (token != NULL) // Trying to execute the command using all the paths. If one of the options succeeded/we're out of options - we will exit the loop
            {
                getFullPath(token, fullPath, params);
                execv(fullPath, params);
                token = strtok_r(NULL, DELIM_PATH, &nextToken);
            }
            printf("$ Failed executing the command \n");
            exit(1);
        }
        else // In father process after executing the command, getting a new command..
        {
            wait(&stat);
            putchar(PROMPT);
            fgets(command, MAX_SIZE, stdin);
            command[strlen(command)-1] = '\0';
        }
    }
}


