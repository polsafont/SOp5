#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_CELLS 100

struct cell {
    char str[100];
    int mida;
};


struct buffer {
    int mida;
    struct cell *cell[NUM_CELLS];
};


int main(void)
{
    int i;
    
    struct cell *productor;
    struct cell *consumidor;
    struct cell *tmp;
    
    struct buffer *buffer;
    
    /* Tenim una cel-la pel productor i una pel consumidor */
    
    productor  = malloc(sizeof(struct cell));
    consumidor = malloc(sizeof(struct cell));
    
    /* Inicialitzem cel-la productor */
    
    strcpy(productor->str, "productor");
    productor->mida = 153;
    
    /* Initializem cel-la consumidor */
    
    strcpy(consumidor->str, "cel-la processada");
    consumidor->mida = -167;
    
    /* Reservem memoria pel buffer. Observar que 'str' de la cell no s'inicialitza */
    
    buffer = malloc(sizeof(struct buffer));
    for(i = 0; i < NUM_CELLS; i++) 
        buffer->cell[i] = malloc(sizeof(struct cell));
    
    /* Ara copiem de forma eficient la cell del productor al buffer. Per aixo agafem una cell
     * del buffer (que estigui disponible) */
    
    tmp = buffer->cell[0];
    buffer->cell[0] = productor;
    productor = tmp;
    
    /* Ara el consumidor agafa la cel-la que ha posat el productor. El consumidor posa al buffer
     * la cel-la que te buida */
    
   tmp = consumidor;
   consumidor = buffer->cell[0];
   buffer->cell[0] = tmp;
   
   /* Imprimim */
   
   printf("Consumidor ha rebut del productor '%s' i %d.\n", consumidor->str, consumidor->mida);
   
   /* Alliberem memoria */
   
   free(productor);
   free(consumidor);
   
   for(i = 0; i < NUM_CELLS; i++)
       free(buffer->cell[i]);
   free(buffer);
 
   /* Sortim del programa */
   
   return 0;
}
    
