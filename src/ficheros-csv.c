
/* Codigo escrito por Lluis Garrido */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

#include "ficheros-csv.h"

// variables globals compartides

int w, r ,comptador, llegint;


pthread_mutex_t mutex;
pthread_cond_t condP, condC;

/**
 *
 * Esta funcion lee la lista de los aeropuertos y crea el arbol
 *
 */

void read_airports(rb_tree *tree, FILE *fp)
{
    int i, r, num_airports;
    char line[MAXCHAR];

    /*
     * eow es el caracter de fin de palabra
     */
    char eow = '\0';

    node_data *n_data;

    fgets(line, 100, fp);
    num_airports = atoi(line);

    i = 0;
    while (i < num_airports)
    {
        fgets(line, 100, fp);
        line[3] = eow;

        /* Reservamos memoria para el nodo */
        n_data = malloc(sizeof(node_data));

        /* Copiamos los datos al nodo */
        n_data->key = malloc(sizeof(char) * 4);
        strcpy(n_data->key, line);

        /* Inicializamos la lista */
        n_data->l = malloc(sizeof(list));
        init_list(n_data->l);

        //pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
        //n_data->mutex = mutex;
        //n_data->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_init(&n_data->mutex, NULL); 
        /* Suponemos que los nodos son unicos, no hace falta
         * comprobar si ya existen previamente.
         */
        insert_node(tree, n_data);

        i++;
    }
}

/**
 * Función que permite leer todos los campos de la línea de vuelo: origen,
 * destino, retardo.
 *
 */

static int extract_fields_airport(char *line, flight_information *fi) {

    /*Recorre la linea por caracteres*/
    char caracter;
    /* i sirve para recorrer la linea
     * iterator es para copiar el substring de la linea a char
     * coma_count es el contador de comas
     */
    int i, iterator, coma_count;
    /* start indica donde empieza el substring a copiar
     * end indica donde termina el substring a copiar
     * len indica la longitud del substring
     */
    int start, end, len;
    /* invalid nos permite saber si todos los campos son correctos
     * 1 hay error, 0 no hay error pero no hemos terminado
     */
    int invalid = 0;
    /*
     * eow es el caracter de fin de palabra
     */
    char eow = '\0';
    /*
     * contenedor del substring a copiar
     */
    char *word;
    /*
     * Inicializamos los valores de las variables
     */
    start = 0;
    end = -1;
    i = 0;
    coma_count = 0;
    /*
     * Empezamos a contar comas
     */
    do {
        caracter = line[i++];
        if (caracter == ',') {
            coma_count ++;
            /*
             * Cogemos el valor de end
             */
            end = i;
            /*
             * Si es uno de los campos que queremos procedemos a copiar el substring
             */
            if(coma_count == ATRASO_LLEGADA_AEROPUERTO ||
                    coma_count == AEROPUERTO_ORIGEN ||
                    coma_count == AEROPUERTO_DESTINO){
                /*
                 * Calculamos la longitud, si es mayor que 1 es que tenemos
                 * algo que copiar
                 */
                len = end - start;
                if (len > 1) {
                    /*
                     * Alojamos memoria y copiamos el substring
                     */
                    word =(char*)malloc(sizeof(char)*(len));
                    for(iterator = start; iterator < end-1; iterator ++){
                        word[iterator-start] = line[iterator];
                    }
                    /*
                     * Introducimos el caracter de fin de palabra
                     */
                    word[iterator-start] = eow;
                    /*
                     * Comprobamos que el campo no sea NA (Not Available)
                     */
                    if (strcmp("NA", word) == 0)
                        invalid = 1;
                    else {
                        switch (coma_count) {
                            case ATRASO_LLEGADA_AEROPUERTO:
                                fi->delay = atoi(word);
                                break;
                            case AEROPUERTO_ORIGEN:
                                strcpy(fi->origin, word);
                                break;
                            case AEROPUERTO_DESTINO:
                                strcpy(fi->destination, word);
                                break;
                            default:
                                printf("ERROR in coma_count\n");
                                exit(1);
                        }
                    }

                    free(word);

                } else {
                    /*
                     * Si el campo esta vacio invalidamos la linea entera
                     */

                    invalid = 1;
                }
            }
            start = end;
        }
    } while (caracter && invalid==0);

    return invalid;
}
/**
 * Funcion que permite añadir la informacion de los nodos de
 * las listas al arbol, bloqueando el nodo del arbol
 * hasta que termina la insercion
 *
 */

void fill_airports_data(struct parameters *par, flight_information *fi) {

    node_data *n_data;
    list_data *l_data;

    n_data = find_node(par->tree, fi->origin);

    if (n_data) {
        //si trobem node, bloquejem node
        pthread_mutex_lock(&n_data->mutex);
        l_data = find_list(n_data->l, fi->destination);

        if (l_data) {
            l_data->numero_vuelos += 1;
            l_data->retardo_total += fi->delay;
        } else {
            l_data = malloc(sizeof(list_data));

            l_data->key = malloc(sizeof(char) * 4);
            strncpy(l_data->key, fi->destination, 4);

            l_data->numero_vuelos = 1;
            l_data->retardo_total = fi->delay;

            insert_list(n_data->l, l_data);
        }
        // desbloquejem el node
        pthread_mutex_unlock(&n_data->mutex);

    } else {
        printf("ERROR: aeropuerto %s no encontrado en el arbol.\n", fi->origin);
        exit(EXIT_FAILURE);
    }

}

/**
 * Funcion d'entrada al thread productor
 * Lee bloques de N linias, guarda en buffer
 * mientras bloquea fitxero
 */

void *productor_fn(void *arg)
{
    //printf("producer thread  ID %d CREATE\n", pthread_self());
    char line[MAXCHAR];

    struct cell *productor;
    struct cell *tmp;

    productor = (struct cell *) malloc(sizeof(struct cell));
    productor->str = malloc(sizeof(char*)*N);
    productor->mida = 0;

    for (int i = 0; i < N; i++ ){
    	productor->str[i] = malloc(sizeof(char)*MAXCHAR);

    } 
    struct parameters *par = (struct parameters *) arg;
    int invalid;
    int dades = 1;

    while(dades){
        //generar dades

        // bloquejem fitxer per si volem afegir productors mes endavant
        // pthread_mutex_lock(&mutex_file);
        for(int i=0; i < N; i++){
            if( fgets(line, MAXCHAR, par->fp) ){
                strncpy(productor->str[i],line, sizeof(line));
                productor->mida += 1;
            }
            else{
                // Fi de lectura del fitxer de disc per part del fil.
                dades = 0;
                break;
            }
        }
        // desbloquejem fitxer en cas de multiples productors
        // pthread_mutex_unlock(&mutex_file);

        // bloquejem buffer mutex
        pthread_mutex_lock(&mutex);
        // cas que buffer sigui ple esperar a condP
        while(comptador == B ){
            pthread_cond_wait(&condP, &mutex);   
        }
        // copia dades a buffer[w]
        tmp = par->buffer->cell[w];
        par->buffer->cell[w] = productor;
        productor = tmp;
        // actualitzar w i comptador
        w = (w + 1) % B;

        comptador++;
  
        // cridem signal condC i desbloquejem  mutex
        pthread_cond_signal(&condC);
        pthread_mutex_unlock(&mutex);
    }
    // var global llegint a 0 per avisar a consumidors
    llegint = 0;
    
    // alliberar memoria
    for ( int i = 0; i < N; i++ ){
    	free( productor->str[i] );
	}
    free(productor->str);
    free(productor);

    return ((void *)0);
}

/**
 * Funcion d'entrada als threads consumidors
 */

void *consumidor_fn(void *arg)
{    
    //printf("consumer thread  ID %d CREATE\n", pthread_self());
    char line[MAXCHAR];

    struct cell *consumidor;
    struct cell *tmp;

    consumidor  = (struct cell*) malloc(sizeof(struct cell));
    //consumidor->str = (char**) malloc(sizeof(char*)*N);
    consumidor->str = malloc(sizeof(char*)*N);
    for (int i = 0; i < N; i++ ){
    	consumidor->str[i] = malloc(sizeof(char)*MAXCHAR);
    }

    struct parameters *par = (struct parameters *) arg;
    int invalid;
    flight_information fi;

    while( true ){
        pthread_mutex_lock(&mutex);
        // esperem nous blocs de dades 
        while((llegint == 1) & (comptador == 0)){   
            //printf("C WAIT, llegint: %d , comptador: %d\n", llegint, comptador);
            pthread_cond_wait(&condC, &mutex); 
        } 
        // si blocs = 0 i no llegim mes, finalitzem
        if( (llegint == 0) & (comptador == 0) ) break;

        //copia dades de buffer[w]
        tmp = consumidor;
        consumidor = par->buffer->cell[r];
        par->buffer->cell[r] = tmp;
        // actualitzem r i comptador
        r = (r + 1) % B;
        comptador--;
        // desbloquejem mutex i cridem signal a condP
        pthread_cond_signal(&condP);
        pthread_mutex_unlock(&mutex);

        //consumir dades, iterem buffer
        for(int i = 0; i < consumidor->mida; i++){
            // extract fields airports
            invalid = extract_fields_airport(consumidor->str[i], &fi);
            if(!invalid){
                fill_airports_data(par, &fi);
            }
        }
    }

    // alliberem memoria i seguent thread a condC 
    //pthread_cond_signal(&condC);
    //pthread_mutex_unlock(&mutex);
    
    //alliberem condC i mutex
    pthread_cond_broadcast(&condC);
    pthread_mutex_unlock(&mutex);

    // free memory
    for ( int i = 0; i < N; i++ ){
    	free(consumidor->str[i]);
	}
    free(consumidor->str);
    free(consumidor);

    return ((void *)0);
}


/**
 *
 * Esta funcion crea el arbol a partir de los datos de los aeropuertos y de los ficheros de retardo
 *
 */
rb_tree *create_tree(char *str_airports, char *str_dades)
{
    FILE *fp;
    rb_tree *tree;

    /* Part de la P4, fils */
    int err;

    // var globals
    w = 0;
	r = 0;
	comptador = 0;
	llegint = 1;
    // consumidors
    pthread_t ntid[THREADS];
    //productor
    pthread_t ptid;
    /* Reservamos memoria */
    tree = (rb_tree *) malloc(sizeof(rb_tree));

    /* Abrimos el fichero con la lista de aeropuertos */
    fp = fopen(str_airports, "r");
    if (!fp) {
        printf("Could not open file '%s'\n", str_airports);
        exit(EXIT_FAILURE);
    }

    /* Leemos los datos de ficheros de aeropuertos */
    init_tree(tree);
    read_airports(tree, fp);
    fclose(fp);

    /* Abrimos el fichero con los datos de los vuelos */
    fp = fopen(str_dades, "r");
    if (!fp) {
        printf("Could not open file '%s'\n", str_dades);
        exit(EXIT_FAILURE);
    }

    /* Leemos la cabecera del fichero i la ignoramos */
    char line[MAXCHAR];
    fgets(line, MAXCHAR, fp);

    /* Creacio del struct par amb el seu espai de memoria.
       El struct esta definit a "ficheros-csv.h", fem el import a dalt. */

    struct parameters *par;
    par = (struct parameters *) malloc(sizeof(struct parameters));

    // crear buffer circular
    



    struct buffer *buffer;
    buffer = malloc(sizeof(struct buffer));

    for(int i = 0; i < B; i++) {

        buffer->cell[i] = malloc(sizeof(struct cell));
        buffer->cell[i]->str =  malloc(sizeof(char*)*N);
        buffer->cell[i]->mida = 0;

        for (int j = 0; j < N; j++){
        	buffer->cell[i]->str[j] =  malloc(sizeof(char)*MAXCHAR);
        }
    }

    /* Copiem el fitxer de dades, buffer compartit i l'arbre a la estructura per passar com a parametres */
    par->fp = fp;
    par->tree = tree;
    par->buffer = buffer;

    /* iniciem contadors de temps */

    /* temps cronologic */
    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);
    /* Temps CPU */
    clock_t t1, t2;
    t1 = clock();

    //mutex del fitxer
    //pthread_mutex_init(&mutex_file, NULL);  
    // init mutex i cond
    pthread_mutex_init(&mutex, NULL); 
    pthread_cond_init(&condP, NULL);
    pthread_cond_init(&condC, NULL);


    /* Se leen los datos y se introducen en el arbol */
    /* Creació dels thread consumidors i productor, com a parametre passem la funció de llegir data i el struct amb els parametres */
    //productor
    err = pthread_create(&(ptid), NULL, productor_fn, (void *)par);
    if (err!=0) {
        printf("No es pot crear el thread productor.\n");
        exit(EXIT_FAILURE);
    }

    // consumidors
    for(int i = 0; i < THREADS; i++){
        err = pthread_create(&(ntid[i]), NULL, consumidor_fn, (void *)par);
        if (err!=0) {
            printf("No es poden crear els threads consumidors.\n");
            exit(EXIT_FAILURE);
        }
    }
    // wait producer and consumers
    err = pthread_join(ptid, NULL);
    if (err!=0) {
        printf("Error al pthread_join del productor.\n");
        exit(EXIT_FAILURE);
    }

    /* Esperem amb el joint fins que acabin els threads */
    for(int i=0; i < THREADS; i++){
        err = pthread_join(ntid[i], NULL);
        if (err!=0) {
            printf("Error al pthread_join dels consuumidors.\n");
            exit(EXIT_FAILURE);
        }
    }
    /* finalitzem contadors de temps */
    gettimeofday(&tv2, NULL);
    t2 = clock();
    printf("Temps cronologic: %f seconds\n", (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));
    printf("Temps de CPU: %f seconds\n", (double)(t2 - t1) / (double) CLOCKS_PER_SEC);
    printf("close file\n");
    // tanquem fitxer
    fclose(fp);

    // alliberem memòria
    for(int i = 0; i < B; i++) {
    	for (int j = 0; j < N; j++){
        	free( buffer->cell[i]->str[j] );
        }
        free(buffer->cell[i]->str);
        free(buffer->cell[i]);
    }
    
    free(buffer);
    free(par);
    return tree;
}
