#include "minishell.h"
#include <stdio.h>
#include <unistd.h>

#define MAX_ARGS 64
#define MAX_ARG_LEN 16
#define MAX_LINE_LEN 80
#define WHITESPACE " .,\t\n"

struct command_t {
    char *name;
    int argc;
    char *argv[MAX_ARGS];
};

/* Function prototypes */
int parseCommand(char *, struct command_t *);

int main(int argc, char *argv[]) {
    int i;
    int pid, numChildren;
    int status;
    FILE *fid;
    char cmdLine[MAX_LINE_LEN];
    struct command_t command;

    /* Read the command line parameters */
    if(argc != 2) {
        fprintf(stderr, "Usage: launch <launch_set_filename>\n");
        exit(0);
    }
    
    /* Open a file that contains a set of commands */
    fid = fopen(argv[1], "r");

    /* Process each command in the launch file */
    numChildren = 0;
    while (fgets(cmdLine, MAX_LINE_LEN, fid) != NULL) {
        parseCommand(cmdLine, &command);
        command.argv[command.argc] = NULL;
        /* Create a child process to execute the command */
        if ((pid = fork()) == 0) {
            /* Child executing command */
            execvp(command.name, command.argv);
        }
        /* Parent continuing to the next command in the file */
        numChildren++;
    }
    printf("\n\nlaunch: Launched %d commands\n", numChildren);

    /* Terminate after all children have terminated */
    for(i = 0; i < numChildren; i++) {
        wait(&status);
        /* Should free dynamic storage in command data structure */
    }
    printf("\n\nlaunch: Terminating sucessfully\n");
    return 0;
}