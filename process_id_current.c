#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]){
    //current process id that is running the program.
    printf("Process id of the running program: %d", getpid());
}