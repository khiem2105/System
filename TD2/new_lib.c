#include "new_lib.h"
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

char *buffer = NULL;
int ind = -1;

void* new_malloc(int taille) {
    return (void*)sbrk(taille);
}

void new_printf(char *input) {
    for(int i = 0; input[i] != '\0'; i++) {
        if(ind == -1) {
            buffer = new_malloc(BUF_SIZE);
            ind ++;
        }
        buffer[ind] = input[i];
        ind ++;
        if(input[i] == '\n' || ind == BUF_SIZE) {
            (void) syscall(SYS_write, STDOUT_FILENO, buffer, ind);
            ind = 0;
        }    
    }   
}

void new_exit() {
    if(ind != 0) {
        (void) syscall(SYS_write, STDOUT_FILENO, buffer, ind);
    }
    exit(EXIT_SUCCESS);
}
