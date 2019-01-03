 /**
 *
 * Practica 3 
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "red-black-tree.h"
#include "estadisticas.h"
#include "ficheros-csv.h"
#include "read-write-tree.h"

#define MAXLINE      200
#define MAGIC_NUMBER 0x0133C8F9

/**
 * 
 *  Menu
 * 
 */

int menu() 
{
    char str[5];
    int opcio;

    printf("\n\nMenu\n\n");
    printf(" 1 - Creacio de l'arbre\n");
    printf(" 2 - Emmagatzemar arbre a disc\n");
    printf(" 3 - Llegir arbre de disc\n");
    printf(" 4 - Consultar informacio de l'arbre\n");
    printf(" 5 - Sortir\n\n");
    printf("   Escull opcio: ");

    fgets(str, 5, stdin);
    opcio = atoi(str); 

    return opcio;
}

/**
 * 
 *  Main procedure
 *
 */

int main(int argc, char **argv)
{
    char str1[MAXLINE], str2[MAXLINE];
    int opcio;

    rb_tree *tree;

    tree = NULL;

    if (argc != 1)
        printf("Opcions de la linia de comandes ignorades\n");

    do {
        opcio = menu();
        printf("\n\n");

       /* Feu servir aquest codi com a pantilla */

        switch (opcio) {
            case 1:
                if (tree) {
                    printf("Alliberant arbre.\n");
                    delete_tree(tree);
                    free(tree);
                    tree = NULL;
                }

                printf("Introdueix fitxer que conte llistat d'aeroports: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;

                printf("Introdueix fitxer de dades: ");
                fgets(str2, MAXLINE, stdin);
                str2[strlen(str2)-1]=0;
                
                // creacio fil Productor i F fils consumidors

                //crida a create_tree
                tree = create_tree(str1, str2); 

                break;

            case 2:
                if (!tree) {
                    printf("No hi ha arbre creat.\n");
                    break;
                }

                printf("Introdueix el nom de fitxer en el qual es desara l'arbre: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;

                save_tree(tree, str1); 

                break;

            case 3:
                if (tree) {
                    printf("Alliberant arbre.\n");
                    delete_tree(tree);
                    free(tree);
                    tree = NULL;
                }
                



                printf("Introdueix nom del fitxer que conte l'arbre: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;

                tree = load_tree(str1); 

                break;

            case 4:
                if (!tree) {
                    printf("No hi ha arbre creat.\n");
                    break;
                }

                printf("Introdueix aeroport per cercar retard o polsa enter per saber l'aeroport amb mes destins: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;

                if (strlen(str1) > 0) {
                    printf("Media de retardos para %s\n", str1);
                    estadisticas_media_retardos(tree, str1);
                } else {
                    printf("\nAeropuerto con mas destinos\n");
                    estadisticas_max_destinos(tree);
                }

                break;

            case 5:
                if (!tree) {
                    printf("No hi ha arbre creat.\n");
                } else {
                    /* Liberamos memoria */
                    delete_tree(tree);
                    free(tree);
                }

                break;

            default:
                printf("Opcio no valida\n");

        } /* switch */
    }
    while (opcio != 5);

    return 0;
}

