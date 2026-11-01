#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    printf("Hello world! (pid: %d", (int) getpid());
    pid_t rc = fork();

    
}