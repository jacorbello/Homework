#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

char *lookupPath(char **, char **);
int parseCommand(char *, struct command_t *);
int parsePath(char **);
void printPrompt();
void readCommand(char *);

int main(int argc, char *argv[])
{
    int pid = fork();
    char *commandLine;
    char **pathv;
    struct command_t command;

    /* Shell initialization */
    /* ... */

    parsePath(pathv); /* Get directory paths from PATH */

    while (1)
    {
        printPrompt();

        /* Read the command line and parse it */
        readCommand(commandLine);
        /* ... */
        parseCommand(commandLine, &command);
        /* ... */

        /* Get the full pathname for the file */
        command.name = lookupPath(command.argv, pathv);
        if (command.name == NULL)
        {
            printf("ERROR: Path not found"); /* Report error */
            continue;
        }

        /* Create child and execute the command */
        //added fork stuff below stopping at exit(0)
        if (pid < 0)
        {
            printf("ERROR: fork error");
        }
        else if (pid == 0)
        {
            /*child process*/
            execv(command.name, command.argv);
        }
        else
        {
            /* parent process */
            /* Wait for the child to terminate */
            wait(0);
        }
    }

    /* Shell termination */
    exit(0);
}

void readCommand(char *commandLine)
{
    fgets(commandLine, LINE_LEN, stdin);
}

int parseCommand(char *commandLine, struct command_t *command)
{
    int argc = 0;
    char **commandLinePtr;

    commandLinePtr = &commandLine;
    command->argv[argc] = (char *)malloc(MAX_ARG_LEN);

    while (command->argv[argc] = strsep(commandLinePtr, WHITESPACE) != NULL)
    {
        command->argv[++argc] = (char *)malloc(MAX_ARG_LEN);
    }
    command->name = (char *)malloc(sizeof(command->argv[0]));
    strcpy(command->name, command->argv[0]);
    return 1;
}

int parsePath(char *pathv[])
{
    char *pathENV;
    char *path;
    int i;

    if (pathv == NULL)
    {
        return 0;
    }

    for (i = 0; i < MAX_PATHS; i++)
    {
        pathv[i] = malloc(MAX_PATH_LEN * sizeof(char));
    }

    if (pathv[i - 1] == NULL)
    {
        return 0;
    }

    pathENV = (char *)getenv("PATH");
    path = (char *)malloc(strlen(pathENV + 1));
    strcpy(path, pathENV);
    /* split the PATH up into tokens (see http://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm) */
    char *token = strtok(path, ":");
    int count = 0;
    while (token != NULL)
    {
        strcpy(pathv[count], token);
        token = strtok(NULL, ":");
        count++;
    }
    return 1;
}

void printPrompt()
{
    char prompt[MAX_PATH_LEN];
    getcwd(prompt, MAX_PATH_LEN);
    printf("%s: ", prompt);
}

char *lookupPath(char **argv, char *dir[])
{
}