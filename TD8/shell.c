#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// test git

#define BUFLEN 512

int read_command(char *command, char *args[], char *fdest) {
    int nb_args = 1;
    char c;
    char *argument;

    //read the command
    scanf("%s", command);
    if(strcmp(command, "exit") == 0) 
        return -1;
    
    //read the arguments and the output file name
    while(getchar() != '\n') {
        // read the arguments
        argument = malloc((BUFLEN-1) * sizeof(char));
        scanf("%s", argument);

        // read the output file name
        if(strcmp(argument, ">") == 0) {
            scanf("%s", fdest);
            break;
        }

        args[nb_args++] = argument;
    }

    return nb_args;
}

void execute_command(char *command, char *args[], char *fdest) {
    pid_t status;
    int fd;

    status = fork();
    switch (status)
    {
    case -1:
        perror("Creation of processus");
        exit(EXIT_FAILURE);
        break;
    case 0:
        if(strlen(fdest) > 0) {
            fd = open(fdest, O_WRONLY|O_CREAT|O_TRUNC, 0666);
            dup2(fd, STDOUT_FILENO);
        }

        if(execvp(command, args) == -1) {
            perror("Error");
            exit(EXIT_FAILURE);
            break;
        }

        if(strlen(fdest) > 0)
            close(fd); 
    default:
        // wait for the child processus to terminate
        wait(&status);
        if(WIFEXITED(status))
            printf("%s terminates with no error\n", command);
        else
            printf("%s terminates with errors\n", command);
        break;
    }
}

int main() {
    char command[BUFLEN], fdest[BUFLEN];
    char *args[BUFLEN];
    int nb_args, i;

    while(1) {
        fdest[0] = '\0';
        // read the command to execute
        nb_args = read_command(command, args, fdest);
        if(nb_args == -1) {
            printf("Exit");
            exit(EXIT_SUCCESS);
        }

        args[0] = command;
        execute_command(command, args, fdest);
    }
}