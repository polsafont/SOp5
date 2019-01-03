#pragma once

#include "linked-list.h"
#include <pthread.h>

/**
 *
 * Red-black tree header 
 *
 *
 */

#define RBTREE_KEY_TYPE char *

/**
 *
 * This structure holds the information to be stored at each node. Change this
 * structure according to your needs.  In order to make this library work, you
 * also need to adapt the functions comp_key1_less_than_key2,
 * comp_key1_equal_to_key2, and free_node_data. For the current implementation
 * the "key" member is used to index data within the tree. 
 *
 */

typedef struct node_data_
{
    // The variable used to index the tree has to be called "key".
    // The type may be any you want (float, char *, etc)
    RBTREE_KEY_TYPE key;     

    // This is the additional information that will be stored
    // within the structure. You may adapt it to your needs:
    // add or remove fields as you need.
    list *l;   
    
    pthread_mutex_t mutex;
    
} node_data; 

/**
 *
 * The node structure. NO ES NECESSARI MODIFICAR AQUESTA ESTRUCTURA
 * PER A FER LES PRACTIQUES.
 *
 */

typedef enum { BLACK, RED } node_color;

typedef struct node_ {
    /* For internal use of the structure. Do not change. */
    struct node_ *left;         /* left child */
    struct node_ *right;        /* right child */
    struct node_ *parent;       /* parent */
    node_color color;            /* node color (BLACK, RED) */

    /* Data to be stored at each node */
    node_data *data; /* data stored in node */
} node;


/**
 * 
 * Some definitions
 * 
 */

#define NIL &sentinel  /* all leafs are sentinels */
extern node sentinel;  /* declared in file red-black-tree.c */

/**
 *
 * The tree structure. It just contains the root node, from
 * which we may go through all the nodes of the binary tree.
 * Add any field you may find useful.
 *
 */

typedef struct rb_tree_ {
    node *root;                   /* root of Red-Black tree */
} rb_tree;

/*
 * Function headers. Note that not all the functions of
 * red-black-tree.c have been included here.
 */

void init_tree(rb_tree *tree);
void insert_node(rb_tree *tree, node_data *d);
node_data *find_node(rb_tree *tree, RBTREE_KEY_TYPE key); 
void delete_tree(rb_tree *tree);

