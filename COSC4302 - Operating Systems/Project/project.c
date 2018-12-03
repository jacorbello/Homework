
#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

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

    while ((command->argv[argc] = strsep(commandLinePtr, WHITESPACE)) != NULL)
    {
        command->argv[argc] = (char *)malloc(MAX_ARG_LEN);
    }
    command->name = (char *)malloc(sizeof(command->argv[0]));
    strcpy(command->name, command->argv[0]);
    return 1;
}

int parsePath(char *dirs[])
{
    char *pathEnvVar;
    int i;
    char *path;

    for (i = 0; i < MAX_ARGS; i++)
    {
        dirs[i] = NULL;
    }
    pathEnvVar = (char *)getenv("PATH");
    strcpy(path, pathEnvVar);

    /* split the PATH up into tokens (see http://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm) */
    char *token = strtok(path, ":");
    int count = 0;
    while (token != NULL)
    {
        strcpy(dirs[count], token);
        token = strtok(NULL, ":");
        count++;
    }
    return 1;
}

void printPrompt()
{
    char prompt[MAX_PATH_LEN];
    getcwd(prompt, MAX_PATH_LEN);
    printf("%s: \n", prompt);
}

char *getPath(char **argv, char **dir)
{
    char *result;
    char pathName[MAX_PATH_LEN];

    if (*argv[0] == '/')
    {
        /* verify we can access the path (see https://www.gnu.org/software/libc/manual/html_node/Testing-File-Access.html) */
        if (access(argv[0], X_OK) == 0)
        {
            strcpy(result, argv[0]);
            return result;
        }
    }

    int count, i = 0;
    do
    {
        if (dir[count] == NULL)
        {
            i = -1;
            break;
        }
        strcpy(pathName, dir[count]);
        strcat(pathName, "/");
        strcat(pathName, argv[0]);

        if (access(pathName, X_OK) == 0)
        {
            result = (char *)malloc(strlen(pathName) + 1);
            strcpy(result, pathName);
            return result;
        }
    } while (i != -1);

    fprintf(stderr, "Cannot find command: %s\n", argv[0]);
    return NULL;
}
int main(int argc, char *argv[], char *envp[])
{
    char *commandLine;
    char **pathv = malloc(sizeof(char *) * MAX_PATHS);
    struct command_t command;
    int pid = fork();

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
        command.argv[command.argc] = NULL;
        /* Get the full pathname for the file */
        command.name = getPath(command.argv, pathv);
        if (command.name == NULL)
        {
            printf("ERROR: Path does not exist\n"); /* Report error */

            continue;
        }

        /* Create child and execute the command */
        /* added fork stuff below stopping at exit(0) */

        if (pid < 0)
        {
            printf("ERROR: Fork does not exist\n");
        }
        else if (pid == 0)
        {
            /*child process*/
            execv(command.name, command.argv);

            exit(0);
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
