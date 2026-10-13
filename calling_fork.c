#include <stdio.h> // IO
#include <sys/types.h> // typedef for pid
#include <unistd.h> // fork()

int main(int argc, char * argv[]){
    printf("Hello, World (pid: %d)\n", (int) getpid());
    int rc = fork();
    if (rc < 0){ // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0){ // child (new process)
        printf("Hello I am child (pid: %d)\n", (int) getpid)
    }
    else { // parent goes down this path
        printf("hello, I am parent of %d (pid: %d)\n", rc, (int) getpid());
    }
    return 0;
}