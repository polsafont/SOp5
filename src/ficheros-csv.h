#pragma once

#include "linked-list.h"
#include "red-black-tree.h"

#define ATRASO_LLEGADA_AEROPUERTO 15
#define AEROPUERTO_ORIGEN 17
#define AEROPUERTO_DESTINO 18

#define MAXCHAR 500
#define N 100
#define B 100
#define THREADS 50

//B blocs amb N linies cada un

typedef struct flight_information {
    char origin[4];
    char destination[4];
    int delay;
} flight_information;

struct cell {
    //char str[N][MAXCHAR];
    char** str;
    int mida;
};

struct buffer {
    struct cell* cell[B];
    int mida;
};


struct parameters {
    FILE *fp;
    rb_tree *tree;
    struct buffer *buffer;
};


rb_tree *create_tree(char *str_airports, char *str_dades);
void read_airports(rb_tree *tree, FILE *fp);
void read_airports_data(rb_tree *tree, FILE *fp);
