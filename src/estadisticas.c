/* Codigo escrito por Lluis Garrido, 2018 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estadisticas.h"

/**
 * Calcula la media de retardo para los destinos de un
 * determinado aeropuerto y las imprime por pantalla.
 *
 */

void estadisticas_media_retardos(rb_tree *tree, char *origen)
{
    node_data *n_data;

    list_item *l_item;
    list_data *l_data;
    float media;

    /* Buscamos el aeropuerto origen en el árbol*/
    n_data = find_node(tree, origen);

    if (n_data == NULL) {
        printf("El aeropuerto %s no existe en arbol\n", origen);
        return;
    }

    printf("Retardos para el aeropuerto: %s\n", origen);

    /* Recorremos la lista de destinos */
    l_item = n_data->l->first;
    while (l_item) {
        l_data = l_item->data;
        media = (float) l_data->retardo_total / (float) l_data->numero_vuelos;
        printf("   %s -- %7.3f minutos\n", l_data->key, media);
        l_item = l_item->next;
    }
}

/**
 *  
 *  Funcion recursiva para encontrar el aeropuerto con mas destinos.
 */

void estadisticas_max_destinos_recursive(node *x, char **origen, int *num_destinos)
{
    node_data *n_data;

    list *l; 

    /* Cogemos los datos asociados al nodo del arbol */
    n_data = x->data;

    /* Ahora cogemos la lista asociada a los datos */
    l = n_data->l;

    /* Miramos el numero de elementos que tiene la lista */
    if (l->num_items > *num_destinos) {
        *num_destinos = l->num_items;
        strcpy(*origen, n_data->key);
    }

    if (x->right != NIL)
        estadisticas_max_destinos_recursive(x->right, origen, num_destinos);

    if (x->left != NIL)
        estadisticas_max_destinos_recursive(x->left, origen, num_destinos); 
}

/**
 *
 * Funcion que permite encontrar el aeropuerto con mas destinos. Imprime
 * el resultado por pantalla.
 *
 */

void estadisticas_max_destinos(rb_tree *tree)
{
    char *origen;
    int num_destinos;

    origen = malloc(sizeof(char) * 4);
    num_destinos = 0;

    if (tree->root != NIL)
        estadisticas_max_destinos_recursive(tree->root, &origen, &num_destinos);

    printf("Aeropuerto con mas destinos: %s, destinos: %d\n", origen, num_destinos);

    free(origen);
}


