#include "lista_se.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define POSICION_FUERA_DE_RANGO 7000
#define CANT_ELEMENTOS 5
#define VOLUMEN 1000

void imprimir_prueba(char* cadena, bool funciona) {
	char* estado = "SI";
	if (!funciona) estado = "NO";
	printf("%s: %s\n", cadena, estado);
}

bool aux_insertar_en_posicion(lista_se_t* lista, int argc, void** argv) {
	bool no_hay_errores = true;
	size_t indice = (size_t)(rand() % ((int)elementos(lista)));

	for (int n = 0; n < argc; n++) {
		no_hay_errores &= ((insertar_en_posicion(lista, argv[n], (int)indice) == 0) && (elemento_en_posicion(lista, (int)indice) == argv[n]) && (elementos(lista) == n + 1));
		indice = (size_t)(rand() % ((int)elementos(lista)));
	}

	return no_hay_errores;
}

bool aux_insertar_ultimo(lista_se_t* lista, int argc, void** argv) {
	bool no_hay_errores = true;

	for (int n = 0; n < argc; n++) {
		no_hay_errores &= ((insertar(lista, argv[n]) == 0) && (ultimo(lista) == argv[n]) && (elementos(lista) == n + 1));
	}

	return no_hay_errores;
}

bool aux_borrar_de_posicion(lista_se_t* lista, int argc, void** argv) {
	bool no_hay_errores = true;
	size_t indice = (size_t)(rand() % ((int)elementos(lista) - 1));

	for (int n = 0; n < argc; n++) {
		no_hay_errores &= ((borrar_de_posicion(lista, (int)indice) == 0) && (elementos(lista) == argc - n - 1));
		indice = (size_t)(rand() % ((int)elementos(lista) - 1));
	}
	
	return no_hay_errores;
}

bool aux_borrar_ultimo(lista_se_t* lista, int argc, void** argv) {
	bool no_hay_errores = true;

	for (int n = 0; n < argc; n++) {
		no_hay_errores &= ((borrar(lista) == 0) && (elementos(lista) == argc - n - 1));
	}

	return no_hay_errores;
}

void pruebas_lista_vacia() {
	printf("---INCIO DE PRUEBAS LISTA VACIA---\n");
	
	lista_se_t* lista = crear_lista();
	imprimir_prueba("Se creo la lista", lista != NULL);
	imprimir_prueba("Esta vacia es true", vacia(lista));
	imprimir_prueba("Largo es 0", elementos(lista) == 0);
	imprimir_prueba("Ver primero es null", !elemento_en_posicion(lista, POSICION_FUERA_DE_RANGO));
	imprimir_prueba("Ver ultimo es null", !ultimo(lista));
	imprimir_prueba("Borrar devuelve -1", borrar(lista) == -1);
	imprimir_prueba("Borrar de posicion devuelve -1", borrar_de_posicion(lista, POSICION_FUERA_DE_RANGO) == -1);
	imprimir_prueba("Se destruyo la lista", destruir_lista(lista) == 0);	
}

void pruebas_lista_con_algunos_elementos() {
	printf("---INCIO DE PRUEBAS LISTA CON ALGUNOS ELEMENTOS---\n");
	
	lista_se_t* lista = crear_lista();
	int a = 1;
	float b = 2;
	double c = 3;
	int d[] = {4};
	char* e = "JEJE";
	void* elementos1[CANT_ELEMENTOS] = {&a, &b, &c, d, e};

	//imprimir_prueba("Insertar en posicion, elemento en posicion y elementos funcionan bien", aux_insertar_en_posicion(lista, CANT_ELEMENTOS, elementos1));
	//imprimir_prueba("Borrar de posicion y elementos funcionan bien", aux_borrar_de_posicion(lista, CANT_ELEMENTOS, elementos1));
	imprimir_prueba("Insertar, ultimo y elementos funcionan bien", aux_insertar_ultimo(lista, CANT_ELEMENTOS, elementos1));
	imprimir_prueba("Borrar y elementos funcionan bien", aux_borrar_ultimo(lista, CANT_ELEMENTOS, elementos1));
	imprimir_prueba("Esta vacia es true", vacia(lista));
	imprimir_prueba("Largo es 0", elementos(lista) == 0);
	imprimir_prueba("Ver primero es null", !elemento_en_posicion(lista, POSICION_FUERA_DE_RANGO));
	imprimir_prueba("Ver ultimo es null", !ultimo(lista));
	imprimir_prueba("Borrar devuelve -1", borrar(lista) == -1);
	imprimir_prueba("Borrar de posicion devuelve -1", borrar_de_posicion(lista, POSICION_FUERA_DE_RANGO) == -1);
	imprimir_prueba("Se destruyo la lista", destruir_lista(lista) == 0);
}

void pruebas_lista_con_muchos_elementos() {
	printf("---INCIO DE PRUEBAS LISTA CON MUCHOS ELEMENTOS---\n");

	lista_se_t* lista = crear_lista();
	void* elementos1[VOLUMEN];

	for (int n = 0; n < VOLUMEN; n += CANT_ELEMENTOS) {
		int a = 1;
		float b = 2;
		double c = 3;
		int d[] = {4};
		char* e = "JOJO";

		elementos1[n] = &a;
		elementos1[n + 1] = &b;
		elementos1[n + 2] = &c;
		elementos1[n + 3] = d;
		elementos1[n + 4] = e;
	}

	//imprimir_prueba("Insertar en posicion, elemento en posicion y elementos funcionan bien", aux_insertar_en_posicion(lista, CANT_ELEMENTOS, elementos1));
	//imprimir_prueba("Borrar de posicion y elementos funcionan bien", aux_borrar_de_posicion(lista, CANT_ELEMENTOS, elementos1));
	imprimir_prueba("Insertar, ultimo y elementos funcionan bien", aux_insertar_ultimo(lista, CANT_ELEMENTOS, elementos1));
	imprimir_prueba("Borrar y elementos funcionan bien", aux_borrar_ultimo(lista, CANT_ELEMENTOS, elementos1));
	imprimir_prueba("Esta vacia es true", vacia(lista));
	imprimir_prueba("Largo es 0", elementos(lista) == 0);
	imprimir_prueba("Ver primero es null", !elemento_en_posicion(lista, POSICION_FUERA_DE_RANGO));
	imprimir_prueba("Ver ultimo es null", !ultimo(lista));
	imprimir_prueba("Borrar devuelve -1", borrar(lista) == -1);
	imprimir_prueba("Borrar de posicion devuelve -1", borrar_de_posicion(lista, POSICION_FUERA_DE_RANGO) == -1);
	imprimir_prueba("Se destruyo la lista", destruir_lista(lista) == 0);
}

void pruebas_micaela() {
	srand ((unsigned int)time(NULL));
	pruebas_lista_vacia();
	pruebas_lista_con_algunos_elementos();
	pruebas_lista_con_muchos_elementos();
}