#include "abb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define CANT_ELEMENTOS 8
#define CANT_VOLUMEN 1000
#define CONST_MODULO 20
#define TAMANIO_CADENA 10

void imprimir_prueba(char* cadena, bool funciona) {
	char* estado = "SI";
	if (!funciona) estado = "NO";
	printf("%s: %s\n", cadena, estado);
}

void imprimir_cadenas_inorden(nodo_t* nodo) {
	if (!nodo) return;
	if (nodo->izquierda) imprimir_cadenas_inorden(nodo->izquierda);
	printf("%s\n", (char*)nodo->elemento);
	if (nodo->derecha) imprimir_cadenas_inorden(nodo->derecha);
}

void imprimir_numeros_inorden(nodo_t* nodo) {
	if (!nodo) return;
	if (nodo->izquierda) imprimir_numeros_inorden(nodo->izquierda);
	printf("%d\n", *(int*)nodo->elemento);
	if (nodo->derecha) imprimir_numeros_inorden(nodo->derecha);
}

int comparar_numeros(void* elemento1, void* elemento2) {
	if (elemento1 > elemento2) return 1;
	if (elemento1 < elemento2) return -1;
	return 0;
}

int comparar_cadenas(void* elemento1, void* elemento2){
	return strcmp((char*)elemento1, (char*)elemento2);
}

void swap_cadenas(char* cadena1, char* cadena2) {
	char* aux = cadena1;
	cadena1 = cadena2;
	cadena2 = aux;
}

void prueba_arbol_vacio() {
	printf("\n---INCIO DE PRUEBAS ÁRBOL VACÍO---\n\n");
	abb_t* abb = crear_arbol(comparar_cadenas, NULL);
	imprimir_prueba("Se creó el árbol", abb);
	imprimir_prueba("Vacío es true", vacio(abb));
	imprimir_prueba("Borrar es -1", borrar(abb, "pato") == -1);
	imprimir_prueba("Buscar es NULL", buscar(abb, "pato") == NULL);
	imprimir_prueba("Destruir es 0", destruir_arbol(abb) == 0);
}

void prueba_pocos_elementos() {
	printf("\n---INCIO DE PRUEBAS POCOS ELEMENTOS ESTÁTICOS---\n\n");
	abb_t* abb = crear_arbol(comparar_cadenas, NULL);
	imprimir_prueba("Se creó el árbol", abb);
	char* elementos1[CANT_ELEMENTOS] = {"pato", "conejo", "pollito", "ciervo", "gato", "perro", "vaca", "pez"};

	for (int i = 0; i < CANT_ELEMENTOS; i++) {
		imprimir_prueba("Insertar es 0", insertar(abb, elementos1[i]) == 0);
		imprimir_prueba("Buscar devuelve el elemento insertado", (char*)buscar(abb, elementos1[i]) == elementos1[i]);
		imprimir_prueba("Vacío devuelve false", !vacio(abb));
	}

	for (int i = 0; i < CANT_ELEMENTOS; i++) {
		imprimir_prueba("Borrar es 0", borrar(abb, elementos1[i]) == 0);
	}

	imprimir_prueba("Vacío es true", vacio(abb));
	imprimir_prueba("Destruir es 0", destruir_arbol(abb) == 0);

	printf("\n---INCIO DE PRUEBAS POCOS ELEMENTOS DINÁMICOS---\n\n");
	abb = crear_arbol(comparar_numeros, free);
	imprimir_prueba("Se creó el árbol", abb);
	int* e0 = malloc(sizeof(int));
	int* e1 = malloc(sizeof(int));
	int* e2 = malloc(sizeof(int));
	int* e3 = malloc(sizeof(int));
	int* e4 = malloc(sizeof(int));
	int* e5 = malloc(sizeof(int));
	int* e6 = malloc(sizeof(int));
	int* e7 = malloc(sizeof(int));
	*e0 = 0;
	*e1 = 1;
	*e2 = 2;
	*e3 = 3;
	*e4 = 4;
	*e5 = 5;
	*e6 = 6;
	*e7 = 7;
	int* elementos2[CANT_ELEMENTOS] = {e5, e2, e4, e0, e1, e7, e3, e6};

	for (int i = 0; i < CANT_ELEMENTOS; i++) {
		imprimir_prueba("Insertar es 0", insertar(abb, elementos2[i]) == 0);
		imprimir_prueba("Buscar devuelve el elemento insertado", (int*)buscar(abb, elementos2[i]) == elementos2[i]);
		imprimir_prueba("Vacío devuelve false", !vacio(abb));
	}

	for (int i = 0; i < CANT_ELEMENTOS; i++) {
		imprimir_prueba("Borrar es 0", borrar(abb, elementos2[i]) == 0);
	}

	imprimir_prueba("Vacío es true", vacio(abb));
	imprimir_prueba("Destruir es 0", destruir_arbol(abb) == 0);
}

void prueba_volumen() {
	printf("\n---INCIO DE PRUEBAS VOLUMEN---\n\n");
	abb_t* abb = crear_arbol(comparar_numeros, NULL);
	imprimir_prueba("Se creó el árbol", abb);
	char (*elementos)[TAMANIO_CADENA] = malloc(CANT_VOLUMEN * TAMANIO_CADENA);

	for (unsigned i = 0; i < CANT_VOLUMEN; i++) {
		sprintf(elementos[i], "%08d", i);
	}

	for (int i = sizeof(elementos) / sizeof(char*) - 1; i > 0; i--) {
		swap_cadenas(elementos[i], elementos[rand() % (i + 1)]);
	}

	bool ok = true;

	for (int i = 0; i < CANT_VOLUMEN; i++) {
		ok &= ((insertar(abb, elementos[i]) == 0) && ((char*)buscar(abb, elementos[i]) == elementos[i]));
	}

	imprimir_prueba("Insertar funciona bien", ok);
	ok = true;

	for (int i = 0; i < CANT_VOLUMEN; i++) {
		ok &= (borrar(abb, elementos[i]) == 0);
	}

	imprimir_prueba("Borrar funciona bien", ok);
	imprimir_prueba("Vacío es true", vacio(abb));
	imprimir_prueba("Destruir es 0", destruir_arbol(abb) == 0);
	free(elementos);
}

void pruebas_micaela() {
	printf("\n------INCIO DE PRUEBAS MICAELA (=^-^=)------\n\n");
	srand((unsigned int)time(NULL));
	prueba_arbol_vacio();
	prueba_pocos_elementos();
	prueba_volumen();
}