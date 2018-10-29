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

int main() {

    /* ... */
    /* Shell initialization */
    /* ... */
    
    parsePath(pathv); /* Get directory paths from PATH */

    while(TRUE) {
        printPrompt();

        /* Read the command line and parse it */
        readCommand(commandLine);
        /* ... */
        parseCommand(commandLine, &command);
        /* ... */

        /* Get the full pathname for the file */
        command.name = lookupPath(command.argv, pathv);
        if(command.name == NULL) {
            /* Report error */
            continue;
        }

        /* Create child and execute the command */
        /* ... */

        /* Wait for the child to terminate */
        /* ... */
    }

    /* Shell termination */
    /* ... */
}

void readCommand(char *commandLine) {
    fgets(commandLine, LINE_LEN, stdin);
}

int parseCommand(char *commandLine, struct command_t *command) {
    
}
