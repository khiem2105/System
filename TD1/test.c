#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("%ld\n", sysconf(_SC_CHILD_MAX));
}
