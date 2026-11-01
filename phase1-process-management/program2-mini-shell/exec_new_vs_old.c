// This code demonstrates how exec command replaces the code block completely with the new executable.

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    printf("Before exec!\n");

    execl("/bin/ls", "ls", "-l", NULL);

    printf("After exec\n")
}

// "After exec" will never appear, because after a successfull exec() the original program that never exist.