#include "abb.h"
#include <stdio.h>

typedef struct cosa{
    int clave;
    char contenido[10];
}cosa;

cosa* crear_cosa(int clave){
    cosa* c = (cosa*)malloc(sizeof(cosa));
    if(c)
	c->clave = clave;
    return c;
}

void destruir_cosa(cosa* c){
    if(c)
	free(c);
}

int comparar_cosas(void* elemento1, void* elemento2){
    if(!elemento1 || !elemento2)
	return 0;

    if(((cosa*)elemento1)->clave>((cosa*)elemento2)->clave)
	return 1;
    if(((cosa*)elemento1)->clave<((cosa*)elemento2)->clave)
	return -1;
    return 0;
}

void destructor_de_cosas(void* elemento){
    if(!elemento)
	return;
    destruir_cosa((cosa*)elemento);
}

int main(){
    abb_t* arbol = crear_arbol(comparar_cosas, destructor_de_cosas);

    cosa* c1= crear_cosa(1);
    cosa* c2= crear_cosa(2);
    cosa* c3= crear_cosa(3);
    cosa* c4= crear_cosa(4);
    cosa* c5= crear_cosa(5);
    cosa* c6= crear_cosa(6);
    cosa* c7= crear_cosa(7);
    cosa* auxiliar = crear_cosa(0);
    
    insertar(arbol, c4);
    insertar(arbol, c2);
    insertar(arbol, c6);
    insertar(arbol, c1);
    insertar(arbol, c3);
    insertar(arbol, c5);
    insertar(arbol, c7);

    printf("El nodo raiz deberia ser 4: %s\n", ((cosa*)arbol->nodo_raiz->elemento)->clave==4?"SI":"NO");

    auxiliar->clave = 5;
    printf("Busco el elemento 5: %s\n", ((cosa*)buscar(arbol, auxiliar))->clave==5?"SI":"NO");

    auxiliar->clave = 7;
    printf("Borro nodo hoja (7): %s\n", (borrar(arbol, auxiliar))==0?"SI":"NO");

    auxiliar->clave = 6;
    printf("Borro nodo con un hijo (6): %s\n", (borrar(arbol, auxiliar))==0?"SI":"NO");

    auxiliar->clave = 2;
    printf("Borro nodo con dos hijos (2): %s\n", (borrar(arbol, auxiliar))==0?"SI":"NO");

    auxiliar->clave = 4;
    printf("Borro la raiz (4): %s\n", (borrar(arbol, auxiliar))==0?"SI":"NO");

    auxiliar->clave = 3;
    printf("Busco el elemento (3): %s\n", ((cosa*)buscar(arbol, auxiliar))->clave==3?"SI":"NO");

    cosa* elementos[10];

    printf("Recorrido inorden (deberian salir en orden 1 3 5): \n");
    int cantidad = recorrer_inorden(arbol, (void**)elementos, 10);
    printf("Cantidad: %d\n", cantidad);
    
    for(int i=0;i<cantidad;i++) {
	   printf("%i ", elementos[i]->clave);
    }

    printf("\n");
    
    printf("Recorrido preorden (deberian salir en orden 5 3 1): \n");
    cantidad = recorrer_preorden(arbol, (void**)elementos, 10);
    printf("Cantidad: %d\n", cantidad);

    for(int i=0;i<cantidad;i++) {
       printf("%i ", elementos[i]->clave);
    }

    printf("\n");

    printf("Recorrido postorden (deberian salir en orden 1 3 5): \n");
    cantidad = recorrer_postorden(arbol, (void**)elementos, 10);
    printf("Cantidad: %d\n", cantidad);

    for(int i=0;i<cantidad;i++) {
       printf("%i ", elementos[i]->clave);
    }

    printf("\n");    

    free(auxiliar);
    destruir_arbol(arbol);

    pruebas_micaela();
    
    return 0;
}
