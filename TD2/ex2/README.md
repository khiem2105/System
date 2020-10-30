- ls_file.c contient la gestion d'un fichier normal
- ls.link.c est modifié pour adapter à la gestion des fichier de type lien
- Dernierement, ls.c ajoute la fonctionnalité de gestion de la liste des fichiers d'un repertoire. Il peut fonctionner similairement avec la commande ls -l n'import quel type de fichier
- ls.h contient deux fonctions étant utilisées régulièrement dans tous les 3 exercise:
 + print_stat(const char* ref, struct stat* statut) afficher des informations d'un fichier(y compris tous les types de fichier)
 + print_stat_dir(const char* dirname) lister des informations des fichiers dans un repertoire