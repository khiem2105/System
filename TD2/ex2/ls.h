#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <math.h>

extern int errno;

void print_stat(const char *ref, struct stat *statut) {
    struct passwd *pw;
    struct group *grp;
    char type;

    #define LTEMPS 32

    char pws[10], grs[10], temps[LTEMPS];

    type = '?';
    int mode = statut->st_mode; // Mode de fichier
    //Trouver le type de fichier
    if(S_ISREG(mode)) type = '-';
    else if(S_ISDIR(mode)) type = 'd';
    else if(S_ISCHR(mode)) type = 'c';
    else if(S_ISBLK(mode)) type = 'b';
    else if(S_ISFIFO(mode)) type = 'p';
    else if(S_ISLNK(mode)) type = 'l';
    
    //Trouver le temps de derniere modification fichier
    strftime(temps, LTEMPS, "%a %e %h %Y %H:%M:%S", localtime(&(statut->st_mtime)));
    
    //L'information du proprietaire de fichier
    pw = getpwuid(statut->st_uid);
    if(pw != NULL) strcpy(pws, pw->pw_name);
    else sprintf(pws, "%8d", (int)statut->st_uid);

    //L'information du groupe de propietaire de fichier
    grp = getgrgid(statut->st_gid);
    if(grp != NULL) strcpy(grs, grp->gr_name);
    else sprintf(grs, "%8d", (int)statut->st_gid);

    //Imprimer toutes les informations deja trouvées et les droits d'acces à l'écran
    printf("%c%c%c%c%c%c%c%c%c%c %2d %9s %9s %9d %s %s", type,
           mode & S_IRUSR?'r':'-',
           mode & S_IWUSR?'w':'-',
           mode & S_IXUSR?'x':'-',
           mode & S_IRGRP?'r':'-',
           mode & S_IWGRP?'w':'-',
           mode & S_IXGRP?'r':'-',
           mode & S_IROTH?'r':'-',
           mode & S_IWOTH?'w':'-',
           mode & S_IXOTH?'x':'-',
           (int)statut->st_nlink, pws, grs, (int)statut->st_size, temps, ref);
}

//Fonction pour calculer le nombre total de bloc utilisé dans le repertoire
int nb_bloc_calculer(const char *dirname) {
    DIR *dirp;
    struct dirent *dp;
    int nb_bloc;
    //Ouvrir le repertoire dirname
    if((dirp = opendir(dirname)) == NULL) {
        perror("Could not open directory");
        return -1;
    }
    //Changer le repertoire courant à dirname
    if(chdir(dirname) == -1) {
        perror("Error changing directory");
        return -1;
    }
    while((dp = readdir(dirp))) {
        struct stat statut;
        if(dp->d_name[0] != '.') {
            if(lstat(dp->d_name, &statut) == -1) {
                fprintf(stderr, "Impossible d'obtenir le statut de %s\n", dp->d_name);
                continue;
                }
            nb_bloc += statut.st_blocks;
        }
    }
    nb_bloc = nb_bloc / 2; // Parce que le taille de bloc utilisée comme le unit of st_blocks est 512 octets
    return nb_bloc;
}

int print_stat_dir(const char *dirname) {
    DIR *dirp;
    struct dirent *dp;
    //Ouvrir le repertoire dirname
    if((dirp = opendir(dirname)) == NULL) {
        perror("Could not open directory");
        return -1;
    }
    //Changer le repertoire courant à dirname
    if(chdir(dirname) == -1) {
        perror("Error changing directory");
        return -1;
    }
    printf("Total:%d\n", nb_bloc_calculer(dirname));
    //Lire le repertoire dirname
    while((dp = readdir(dirp))) {
        struct stat statut;
        
        if(dp->d_name[0] != '.') { // Ne compte pas les fichiers commencés par .
            if(lstat(dp->d_name, &statut) == -1) {
                fprintf(stderr, "Impossible d'obtenir le statut de %s\n", dp->d_name);
                continue;
            }
            //Lister des fichiers grâce à la fonction print_stat déjà définie
            print_stat(dp->d_name, &statut);
            printf("\n");
        }
    }
    //Gestion des erreur
    if(errno != 0){
        perror("Error reading directory");
        return -1;
    }
    return 0;
}