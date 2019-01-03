#pragma once

/* Número máximo de caracteres por linea */
#define MAXCHAR  500 

/* Definiciones del indice de cada campo en el fichero CSV */
#define ATRASO_LLEGADA_AEROPUERTO 15
#define AEROPUERTO_ORIGEN 17
#define AEROPUERTO_DESTINO 18

#include "red-black-tree.h"
#include "linked-list.h"

void estadisticas_media_retardos(rb_tree *tree, char *origen);
void estadisticas_max_destinos(rb_tree *tree);


