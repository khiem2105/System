#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    char *env = getenv(argv[1]);
    printf("%s\n", env);
}