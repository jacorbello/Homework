#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void printProcessInfo(pid) {
	printf("The PID of this instance is %d\n", pid);

        char str[40];
        sprintf(str, "/proc/%d/cmdline", pid);
        printf("The cmdline path for this PID is %s\n",str);
        FILE *file = fopen(str, "r");
        if (file != NULL) {
                char line[256];
                while (fgets(line, sizeof line, file) != NULL) {
                        printf("Cmdline is %s\n", line);
                }
        } else {
                printf("Guess I messed something up...\n");
        }
}

void printModulesInfo() {
	char mod[15] = "/proc/modules";
	FILE *file = fopen(mod, "r");
	if (file != NULL) {
                char line[256];
                while (fgets(line, sizeof line, file) != NULL) {
                        printf("%s", line);
                }
        } else {
                printf("Guess I messed something up...\n");
        }
}

int main(int argc, char **argv) {
	
	int processID = getpid();
	printf("***Beginning Process Evaluation***\n");
	printProcessInfo(processID);
	printf("\n***Ending Process Evaluation***\n");
	
	printf("***Beginning Module Evaluation***\n");
	printModulesInfo();
	printf("\n***Ending Module Evaluation***\n");

	

	exit(0);
}
