/**
 *
 * Linked-list implementation. 
 * 
 * This is an implementation of a simple linked-list. A minimal
 * set of necessary functions have been included.
 *
 * Lluis Garrido, 2018.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * We include the linked-list.h header. Note the double
 * quotes.
 */

#include "linked-list.h"

/**
 *
 * Free data element. The user should adapt this function to their needs.  This
 * function is called internally by deleteList.
 *
 */

static void free_list_data(list_data *data)
{
  free(data->key);
  free(data); 
}

/**
 *
 * Compares if key1 is equal to key2. Should return 1 (true) if condition
 * is satisfied, 0 (false) otherwise.
 *
 */

static int compare_key1_equal_key2(LIST_KEY_TYPE key1, LIST_KEY_TYPE key2)
{
  int rc;

  rc = 0;

  if (strcmp(key1, key2) == 0)
    rc = 1;

  return rc;
}

/**
 * 
 * NO CANVIEU EL CODI QUE HI HA A SOTA LLEVAT QUE SAPIGUEU EL QUE ESTEU FENT.
 * PER FER LES PRACTIQUES NO CAL MODIFICAR EL CODI QUE HI HA A SOTA.
 *
 */

/**
 * 
 * Initialize an empty list
 *
 */

void init_list(list *l)
{
  l->num_items = 0;
  l->first = NULL;
}

/**
 * 
 * Insert data in the list.  This function does not perform a copy of data
 * when inserting it in the list, it rather creates a list item and makes
 * this item point to the data. Thus, the contents of data should not be
 * overwritten after calling this function.
 *
 */

void insert_list(list *l, list_data *data)
{
  list_item *tmp, *x;

  x = malloc(sizeof(list_item));

  if (x == 0) {
    printf("insufficient memory (insertItem)\n");
    exit(1);
  }

  /* Insert item at first position */

  tmp = l->first;
  l->first = x;
  x->next = tmp;

  /* Link data to inserted item */
  x->data = data;

  l->num_items++;
}

/**
 * 
 * Find item containing the specified key. Returns the data
 * that it points to (not the item itself).
 *
 */

list_data *find_list(list *l, LIST_KEY_TYPE key)
{
  list_item *current;

  current = l->first;

  while (current != NULL)
  {
    if (compare_key1_equal_key2(current->data->key, key))
      return (current->data);

    current = current->next;
  }

  return (NULL);
}

/**
 * 
 * Deletes the first item of the list. The data to which
 * the deleted item points to also is deleted.
 *
 */

void delete_first_list(list *l)
{
  list_item *tmp;

  tmp = l->first;

  if (tmp)
  {
    l->first = tmp->next;
    free_list_data(tmp->data);
    free(tmp);
    l->num_items--;
  }
}

/**
 * 
 * Deletes a list including the data to which their 
 * items point to.
 *
 */

void delete_list(list *l)
{
  list_item *current, *next;

  current = l->first;

  while (current != NULL)
  {
    next = current->next;
    free_list_data(current->data);
    free(current);
    current = next;
  }

  l->num_items = 0;
  l->first = NULL;
}


