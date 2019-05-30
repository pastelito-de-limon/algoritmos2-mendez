/*******************************************************************
* Entrega: TP1
* Alumna: Micaela Juarez (102784)
* Algoritmos y Programación II - Méndez
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "tp1.h"
#include "ordenar.h"

#define TAM_BUFFER 200
#define SEPARADOR ";"

/*******************************************************************
*						DEFINICIÓN DE ESTRUCTURAS
********************************************************************/

typedef int (*comparar_t)(void*, void*);

typedef struct reino {
	region_t** regiones;
	int cant_regiones;
	int maximo_regiones;
}reino_t;

typedef struct region {
	char nombre [50];
	char lema [50];
	int ejercito ;
}region_t;

/*******************************************************************
*							FUNCIONES AUXILIARES
********************************************************************/

region_t* crear_region(char* linea, size_t largo_linea) {

	region_t* region = malloc(sizeof(region_t));
	if(!region) return NULL;
	char* separador = SEPARADOR;
	region->nombre = strtok(linea, separador);
	region->lema = strtok(NULL, separador);
	region->ejercito = atoi(strtok(NULL, separador));
	return region;
}

/*******************************************************************
*								FUNCIONES
********************************************************************/

reino_t* crear_reino(size_t maximo_regiones) {

	reino_t* reino = malloc(sizeof(reino_t));
	if (!reino) return NULL;
	region_t** regiones = malloc(sizeof(region_t*) * maximo_regiones);
	
	if (!regiones) {
		free(reino);
		return NULL;
	}

	for (int i = 0; i < maximo_regiones; i++) {
		regiones[i] = NULL;
	}

	reino->regiones = regiones;
	reino->cant_regiones = 0;
	reino->maximo_regiones = maximo_regiones;
	return reino;
}

int destruir_reino(reino_t* reino) {

	if (!reino) return 1;

	for (int i = 0; i < cant_regiones; i++) {
		if (reino->regiones[i]) free(reino->regiones[i]);
	}

	free(reino->regiones);
	free(reino);
	return 0;
}

int agregar_region(reino_t* reino, region_t* region) {
	
	if (!reino) return;
	FILE* archivo = fopen("reinos.txt", "r");
	char buffer[TAM_BUFFER];
	fscanf(archivo, "%s", buffer);
	int cant_lineas = atoi(buffer);
	if (cant_lineas > cant_regiones) cant_lineas = cant_regiones;

	for (int i = 0; i < cant_lineas; i++) {
		fscanf(archivo "%s", buffer);
		region_t* region = crear_region(buffer, TAM_BUFFER);
		
		if (!region) {
			fclose(archivo);
			return 1;
		}

		reino->regiones[i] = region;
		reino->cant_regiones++;
	}

	fclose(archivo);
	return 0;
}

void ordenar_reino(reino_t* reino , comparar_t comparar) {

	if (!reino) return;
	ordenar(reino->regiones, 0, reino->cant_regiones, comparar);
}

void listar_regiones(reino_t* reino) {

	if (!reino) return;

	for (int i = 0; i < reino->cant_regiones; i++) {
		printf("%s: '%s', %d/n", reino->regiones[i]->nombre, reino->regiones[i]->lema, reino->regiones[i]->ejercito);		
	}
}