#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#define PATH_DELIMITERS ":"

char *getPath(char **, char **);
int parseCommand(const char *, const char, char **);
int parsePath(char **);
void printPrompt();
void getCommand(char *);

int main(int argc, char *argv[], char * envp[])
{
    int pid = fork();
    char *commandLine;
    char **pathv;
    struct command_t command;

    printf("Welcome to the Shell\n");
    /* Shell initialization */
    /* ... */
    parsePath(pathv); /* Get directory paths from PATH */
    while (1)
    {
        printPrompt();

        /* Read the command line and parse it */
        getCommand(commandLine);
        /* ... */
        parseCommand(commandLine, PATH_DELIMITERS, &command);
        /* ... */

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

void getCommand(char *commandLine)
{
    fgets(commandLine, LINE_LEN, stdin);
}



int parseCommand(const char *s, const char delimiters, char **argvp){

    int error;
    int i;
    int numtokens;
    const char *snew;
    char *t;

    if ((s == NULL) || (delimiters == NULL) || (argvp == NULL)){
        errno = EINVAL;
        return -1;
    }
    argvp = NULL;

    snew = s + strspn(s,delimiters);
    if ((t=malloc(strlen(snew)+1))==NULL){
        return -1;
    }
    strcpy(t, snew);
    numtokens =0;
    if(strtok(t, delimiters) != NULL){
        for(numtokens =1; strtok(NULL, delimiters) != NULL; numtokens++);
    }
    if((argvp = malloc((numtokens + 1)*sizeof(char *))) == NULL){
        error = errno;
        free(t);
        errno = error;
        return -1;
    }
    if(numtokens == 0){
        free(t);
    }
    else{
        strcpy(t, snew);
        **argvp = strtok(t, delimiters);
        for(i=1; i<numtokens; i++){
            *((argvp)+i) = strtok(NULL, delimiters);
        }
    }
    *((argvp)+numtokens) = NULL;
    return numtokens;
}

int parsePath(char *pathv[])
{
    char *path;
    int i;
    char* pPath;
    pPath = getenv("PATH");

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


    //strcpy(path, pPath);
    /* split the PATH up into tokens (see http://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm) */
    char *token = strtok(pPath, ":");
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
    printf("%s: \n", prompt);
}

char *getPath(char **argv, char *dir[])
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