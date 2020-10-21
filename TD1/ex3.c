#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <string.h>

extern int errno;

int main() {
    int rep = syscall(STDOUT_FILENO, 6);    
    if(rep == -1) {
        char *err = strerror(errno);
        printf("%s", err);
    }
}