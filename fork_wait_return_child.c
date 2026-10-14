#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    pid_t parent_current = getpid();

    pid_t pid = fork();
    if (pid < 0){
        perror("fork unsuccessfull!\n");
    }
    else if (pid == 0){
        printf("fork is successfull and returned child's PID!\n");
        printf("Child's PID: %d\n", getpid());
        printf("Parent's PID: %d\n", getppid());
    }

    else if(pid > 0){
        printf("fork is successful, child PID returned to parent: %d\n", pid);
        sleep(2); // keep parent alive 
        wait(NULL);
    }

    // If sleep() is not used?
    // Then a mismatch happens because the parent terminates (or is considered finish) before the child queries.
    // Once the parent is gone the OS reassigned
}