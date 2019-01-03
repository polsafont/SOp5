#pragma once

/**
 *
 * Linked-list header 
 * 
 * Include this file in order to be able to call the 
 * functions available in linked-list.c. We include
 * here only those information we want to make visible
 * to other files.
 *
 * Lluis Garrido, 2018.
 *
 */

/**
 * 
 * The LIST_KEY_TYPE is used to define the type of the primary
 * key used to index data in the list. 
 *
 */

#define LIST_KEY_TYPE char *

/**
 *
 * This structure holds the information to be stored at each list item.  Change
 * this structure according to your needs.  In order to make this library work,
 * you also need to adapt the functions compEQ and freelist_data. For the
 * current implementation the "key" member is used search within the list. 
 *
 */

typedef struct list_data_ {
  // The variable used to index the list has to be called "key".
  LIST_KEY_TYPE key;

  // This is the additional information that will be stored
  // within the structure. This additional information is associated
  // to the key. You may include any field you may need useful.
  int numero_vuelos;
  int retardo_total;
} list_data;


/**
 * 
 * The item structure
 *
 */

typedef struct list_item_ {
  list_data *data;
  struct list_item_ *next;
} list_item;

/**
 * 
 * The list structure
 *
 */

typedef struct list_ {
  int num_items;
  list_item *first;
} list;

/**
 *
 * Function heders we want to make visible so that they
 * can be called from any other file.
 *
 */

void init_list(list *l);
void insert_list(list *l, list_data *data);
list_data *find_list(list *l, LIST_KEY_TYPE key);
void delete_first_list(list *l);
void delete_list(list *l);

