#include "ls.h"

int main(int argc, char **argv) {
    struct stat statut;
    int i;

    if(argc < 2) {
        if(fstat(STDIN_FILENO, &statut) == -1) {
            fprintf(stderr, "%s: impossible d'obtenir le statut de %s",
                    argv[0], "<STDIN>");
            exit(EXIT_FAILURE);
        }
        print_stat("<STDIN>", &statut);
    }
    else {
        for(i = 1; i < argc; i++) {
            //Utiliser lstat au lieu de stat pour la gestion des fichier de type lien
            if(lstat(argv[i], &statut) == -1) {
                fprintf(stderr, "%s: impossible d'obtenir le statut de %s",
                        argv[0], argv[i]);
                continue;
            }
            print_stat(argv[i], &statut);
        }
        exit(EXIT_SUCCESS);
    }
}