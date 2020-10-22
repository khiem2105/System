#include "ls.h"

int main(int argc, char **argv) {
    //Par default, lister des fichiers de repertoire courant
    if(argc < 2) {
        if(print_stat_dir(".") == -1) {
            fprintf(stderr, "Impossible de lire .\n");
            return -1;
        }    
    }
    //Si non, lister des fichiers des repertoires et les informations de fichiers passés en arguments s
    else {
        for(int i = 1; i < argc; i++) {
            struct stat statut;
            if(lstat(argv[i], &statut) == -1) {
                fprintf(stderr, "%s: impossible d'obtenir le statut de %s", argv[0], argv[i]);
                continue;
            }
            //Si un repertoire, lister des informations des fichiers qu'il contient
            if(S_ISDIR(statut.st_mode)) {
                if(print_stat_dir(argv[i]) == -1) {
                    fprintf(stderr, "Impossible de lire %s", argv[i]);
                    return 0;
                }
            }
            //Si un fichier ou un lien, lister ses informations
            else print_stat(argv[i], &statut);
        }
        return 0;
    }
}    