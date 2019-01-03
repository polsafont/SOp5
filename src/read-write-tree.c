#include <stdio.h>
#include <stdlib.h>

#include "red-black-tree.h"
#include "linked-list.h"
#include "read-write-tree.h"

/**
 *
 *  Contar el numero de nodos del arbol 
 *
 */

int count_nodes_recursive(node *x)
{
    int nodes;

    nodes = 0;

    /* Analyze the children */
    if (x->right != NIL)
        nodes += count_nodes_recursive(x->right);

    if (x->left != NIL)
        nodes += count_nodes_recursive(x->left);

    /* Take into account the node itself */
    nodes += 1;

    return nodes;
}

/**
 *
 *  Contar el numero de nodos del arbol 
 *
 */

int count_nodes(rb_tree *tree)
{
    int nodes;

    nodes = count_nodes_recursive(tree->root);

    return nodes;
} 

/**
 *
 *  Almacena un nodo en disco. Ver fichero save_tree. 
 *
 */

void save_tree_recursive(node *x, FILE *fp)
{
    int i, value1, value2;
    list_item *element;

    if (x->right != NIL)
        save_tree_recursive(x->right, fp);

    if (x->left != NIL)
        save_tree_recursive(x->left, fp);

    /* Se supone que el origen tiene 3 caracteres */
    fwrite(x->data->key, sizeof(char), 3, fp);

    /* Guardamos el numero de elementos de la lista */ 
    value1 = x->data->l->num_items;
    fwrite(&value1, sizeof(int), 1, fp);

    element = x->data->l->first; 

    /* Guardamos los elementos de la lista */
    for(i = 0; i < value1; i++) {
         fwrite(element->data->key, sizeof(char), 3, fp);

         value2 = element->data->numero_vuelos;
         fwrite(&value2, sizeof(int), 1, fp);

         value2 = element->data->retardo_total;
         fwrite(&value2, sizeof(int), 1, fp);

         element = element->next;
    }
}

/**
 *
 *  Funcion que guarda a disco un arbol
 *
 */

void save_tree(rb_tree *tree, char *filename)
{
    FILE *fp;

    int magic, nodes;

    fp = fopen(filename, "w");
    if (!fp) {
        printf("ERROR: no he pogut obrir el fitxer per escriptura.\n");
        return;
    }

    magic = MAGIC_NUMBER;
    fwrite(&magic, sizeof(int), 1, fp);

    nodes = count_nodes(tree);
    fwrite(&nodes, sizeof(int), 1, fp);

    save_tree_recursive(tree->root, fp);

    fclose(fp);  
}

/**
 *
 *  Load tree from disc
 *
 */

rb_tree *load_tree(char *filename)
{
    FILE *fp;
    rb_tree *tree;
    node_data *n_data;
    list_data *l_data;

    int i, j, magic, nodes, num_dest, value1, value2;
    char *str;

    fp = fopen(filename, "r");
    if (!fp) {
        printf("ERROR: No se ha podido abrir el fichero.\n");
        return NULL;
    }

    /* Leemos el numero magico */
    fread(&magic, sizeof(int), 1, fp);
    if (magic != MAGIC_NUMBER) {
        printf("ERROR: el numero magico no es correcto.\n");
        return NULL;
    }

    /* Leemos el numero de nodos del arbol */
    fread(&nodes, sizeof(int), 1, fp);

    /* Reservamos memoria para el arbol */
    tree = (rb_tree *) malloc(sizeof(rb_tree));

    /* Inicializamos el arbol */
    init_tree(tree);

    /* Leemos el arbol. */ 
    for(i = 0; i < nodes; i++)
    {
        /* Leemos el origen */
        str = malloc(sizeof(char) * 4);
        fread(str, sizeof(char), 3, fp);

        /* Marcamos el final de la palabra */
        str[3] = 0;

        /* Leemos el numero de destinos */
        fread(&num_dest, sizeof(int), 1, fp);
        
        /* Preparamos el nodo */
        n_data = malloc(sizeof(node_data));

        n_data->key = str;

        /* Inicializamos la lista */
        n_data->l = malloc(sizeof(list));
        init_list(n_data->l); 

        /* Leemos los datos asociados a los destinos */
        for(j = 0; j < num_dest; j++) {
            l_data = malloc(sizeof(list_data));

            /* Leemos destino */
            str = malloc(sizeof(char) * 4);
            fread(str, sizeof(char), 3, fp);

            /* Marcamos el final de la palabra */
            str[3] = 0;

            /* Leemos numero de vuelos y retardo */
            fread(&value1, sizeof(int), 1, fp);
            fread(&value2, sizeof(int), 1, fp);

            /* Creamos elemento de la lista */
            l_data->key = str;
            l_data->numero_vuelos = value1;
            l_data->retardo_total = value2;

            /* Insertemos en la lista */
            insert_list(n_data->l, l_data);
        }

        /* Insertamos el nodo en el arbol */
        insert_node(tree, n_data);
    }

    fclose(fp);

    return tree;
}

