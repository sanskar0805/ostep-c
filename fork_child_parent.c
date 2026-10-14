#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]){
    pid_t parent_current = getpid();
    printf("Current running process PID: %d\n", parent_current);
    pid_t pid = fork();

    if (pid < 0){
        perror("Fork unsuccessfull!\n");
    }

    else if (pid == 0){
        printf("Fork is successfull!\n");
        printf("Child PID: %d\n", (int) getpid());
        printf("Parents PID: %d\n", (int) getppid());

        // lets check if the newly created child's parent process id matches with the running process pid
        if (getppid() == parent_current){
            printf("YES!!\n");
        }
    }

    // Currently this won't print as intended!
    // This is because the parent exited before the child is created so the parent of the child process is reassigned to some other integer.
    // In order to fix this, we will use <sys/wait.h> and the wait() function where the parent will wait() until the child process gets completed. 
    // Let's fix this in another program.
}