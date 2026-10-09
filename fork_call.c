#include <sys/types.h>   // for pid_t
#include <unistd.h>      // for fork(), getpid(), getppid()
#include <stdio.h>       // for printf(), perror()
#include <stdlib.h>      // for exit()
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // Show the PID before fork
    pid_t pid = getpid();
    printf("Current executing Process ID: %d\n", (int)pid);

    // Perform fork
    pid = fork();

    if (pid < 0) {
        perror("fork failed!");
        exit(1); 
    }
    else if (pid == 0) {
        // Child branch
        printf("fork successful, return value: %d\n", (int)pid); // always 0 in child
        printf("Child process id: %d\n", (int)getpid());
        printf("Parent process id: %d\n", (int)getppid());
    }
    else {
        // Parent branch
        printf("fork successful with return value %d (child PID).\n", (int)pid);
        printf("Parent's PID: %d\n", (int)getpid());
        printf("PID of the process that launched our program: %d\n", (int)getppid());

        // Synchronize: wait for child to finish to avoid jumbled output
        wait(NULL);
    }

    return 0;
}
