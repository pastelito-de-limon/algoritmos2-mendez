#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"
#include "lista.h"
#define CANT_ELEMENTOS 8

void imprimir_prueba(char* cadena, bool funciona) {
	char* estado = "SI";
	if (!funciona) estado = "NO";
	printf("%s: %s\n", cadena, estado);
}

void prueba_hash_vacio() {
	printf("\n---INCIO DE PRUEBAS HASH VACÍO---\n\n");
	hash_t* hash = crear_hash(free);
	imprimir_prueba("Se creó el hash", hash);
	imprimir_prueba("Vacío es true", cantidad(hash) == 0);
	imprimir_prueba("Quitar elemento es -1", quitar_elemento(hash, "pato") == -1);
	imprimir_prueba("Existe clave es false", !existe_clave(hash, "pato"));
	imprimir_prueba("Obtener elemento es NULL", obtener_elemento(hash, "pato") == NULL);
	imprimir_prueba("Destruir es 0", destruir_hash(hash) == 0);
}

void prueba_hash_pocos_elementos() {
	printf("\n---INCIO DE PRUEBAS POCOS ELEMENTOS ESTÁTICOS---\n\n");
	hash_t* hash = crear_hash(NULL);
	imprimir_prueba("Se creó el hash", hash);
	char* elementos1[CANT_ELEMENTOS] = {"pato", "conejo", "pollito", "ciervo", "gato", "perro", "vaca", "pez"};

	for (int i = 0; i < CANT_ELEMENTOS; i++) {
		imprimir_prueba("Guardar es 0", guardar_elemento(hash, elementos1[i], elementos1[i]) == 0);
		imprimir_prueba("Obtener devuelve el elemento insertado", (char*)obtener_elemento(hash, elementos1[i]) == elementos1[i]);
		imprimir_prueba("Cantidad es el número correcto", cantidad(hash) == i + 1);
	}

	for (int i = 0; i < CANT_ELEMENTOS; i++) {
		imprimir_prueba("Quitar es 0", quitar_elemento(hash, elementos1[i]) == 0);
	}

	imprimir_prueba("Cantidad es 0", cantidad(hash) == 0);
	imprimir_prueba("Destruir es 0", destruir_hash(hash) == 0);

	printf("\n---INCIO DE PRUEBAS POCOS ELEMENTOS DINÁMICOS---\n\n");
	hash = crear_hash(free);
	imprimir_prueba("Se creó el hash", hash);
	char* e0 = malloc(sizeof(char*));
	char* e1 = malloc(sizeof(char*));
	char* e2 = malloc(sizeof(char*));
	char* e3 = malloc(sizeof(char*));
	char* e4 = malloc(sizeof(char*));
	char* e5 = malloc(sizeof(char*));
	char* e6 = malloc(sizeof(char*));
	char* e7 = malloc(sizeof(char*));
	char* elementos2[CANT_ELEMENTOS] = {e5, e2, e4, e0, e1, e7, e3, e6};

	for (int i = 0; i < CANT_ELEMENTOS; i++) {
		strcpy(elementos2[i], elementos1[i]);
	}

	for (int i = 0; i < CANT_ELEMENTOS; i++) {
		imprimir_prueba("Guardar es 0", guardar_elemento(hash, elementos1[i], elementos2[i]) == 0);
		imprimir_prueba("Obtener devuelve el elemento insertado", (char*)obtener_elemento(hash, elementos1[i]) == elementos2[i]);
		imprimir_prueba("Cantidad es el número correcto", cantidad(hash) == i + 1);
	}

	for (int i = 0; i < CANT_ELEMENTOS; i++) {
		imprimir_prueba("Quitar es 0", quitar_elemento(hash, elementos1[i]) == 0);
	}

	imprimir_prueba("Cantidad es 0", cantidad(hash) == 0);
	imprimir_prueba("Destruir es 0", destruir_hash(hash) == 0);
}

void prueba_hash_iterar() {
	printf("\n---INCIO DE PRUEBAS ITERADOR---\n\n");
	hash_t* hash = crear_hash(NULL);

	char* elementos1[CANT_ELEMENTOS] = {"pato", "conejo", "pollito", "ciervo", "gato", "perro", "vaca", "pez"};

	for (int i = 0; i < CANT_ELEMENTOS; i++) {
		imprimir_prueba("Guardar es 0", guardar_elemento(hash, elementos1[i], elementos1[i]) == 0);
		imprimir_prueba("Obtener devuelve el elemento insertado", (char*)obtener_elemento(hash, elementos1[i]) == elementos1[i]);
		imprimir_prueba("Cantidad es el número correcto", cantidad(hash) == i + 1);
	}

	hash_iter_t* iter = crear_iterador(hash);

	imprimir_prueba("Se creó el iterador", iter);

	for (int i = 0; i < CANT_ELEMENTOS - 1; i++) {
		printf("Elemento número %i de %i\n", i + 1, CANT_ELEMENTOS);
		imprimir_prueba("No está al final", !esta_al_final(iter));
		printf("Elemento actual: %s\n", (char*)elemento_actual(iter));
		imprimir_prueba("Avanzar es true", avanzar_iterador(iter));
	}

	printf("Elemento número %i de %i\n", CANT_ELEMENTOS, CANT_ELEMENTOS);
	imprimir_prueba("No está al final", !esta_al_final(iter));
	printf("Elemento actual: %s\n", (char*)elemento_actual(iter));
	imprimir_prueba("Avanzar es false", !avanzar_iterador(iter));
	imprimir_prueba("Está al final", esta_al_final(iter));
	imprimir_prueba("Elemento actual es NULL", elemento_actual(iter) == NULL);
	imprimir_prueba("Destruir iter es 0", hash_iterador_destruir(iter) == 0);
	imprimir_prueba("Destruir hash es 0", destruir_hash(hash) == 0);
}

void pruebas_micaela() {
	printf("\n------INCIO DE PRUEBAS MICAELA (=^-^=)------\n\n");
	prueba_hash_vacio();
	prueba_hash_pocos_elementos();
	prueba_hash_iterar();
}