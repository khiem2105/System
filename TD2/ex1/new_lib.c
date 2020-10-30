#include "new_lib.h"
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>

extern int errno;

char *buffer = NULL;
int ind = -1;

void* new_malloc(int taille) {
    return (void*)sbrk(taille);
}

void new_printf(char *input) {
    for(int i = 0; input[i] != '\0'; i++) {
        //Au premier appel, initialiser le tampon
        if(ind == -1) {
            if((buffer = new_malloc(BUF_SIZE)) != NULL) ind ++;
            else { 
                perror("Erreur initialiser le buffer");
                exit(EXIT_FAILURE);
            }    
        }
        //Placer la prochaine caractere dans le tampon
        buffer[ind] = input[i];
        ind ++;
        //Si la caractere est un saut de ligne ou le tampon est rempli, afficher le contenu de tampon à l'écran et vider le tampon
        if(input[i] == '\n' || ind == BUF_SIZE) {
            if(syscall(SYS_write, STDOUT_FILENO, buffer, ind) == -1) {
                perror("Erreur afficher le contenu du tampon");
                exit(EXIT_FAILURE);
            }
            ind = 0;
        }    
    }   
}

void new_exit() {
    //En quittant, afficher tous ceux qui restent dans le tampon à l'écran
    if(ind != 0) {
        if(syscall(SYS_write, STDOUT_FILENO, buffer, ind) == -1) {
                perror("Erreur afficher le contenu du tampon");
                exit(EXIT_FAILURE);
            }
    }
    exit(EXIT_SUCCESS);
}
