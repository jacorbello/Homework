#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	int parentPID, childPID, status;

	printf("This is the parent PID.\n");

	if((parentPID = fork()) == 0) {
		printf("This is the child PID. Waiting for 5 seconds...\n");
		sleep(5);
	} else {
		printf("Waiting for child to terminate.\n");
		childPID = wait(&status);
		printf("Back within the parent.\n");
	}
	exit(1);
}
