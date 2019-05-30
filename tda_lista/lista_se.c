#include "lista_se.h"
#include <stdbool.h>
#include <stdlib.h>

lista_se_t* crear_lista() {
	lista_se_t* lista = malloc(sizeof(lista_se_t));
	if (!lista) return NULL;
	lista->inicio = NULL;
	lista->cantidad = 0;
	return lista;
}

bool vacia(lista_se_t* lista) {
	if (!lista) return true;
	return lista->cantidad == 0;
}

size_t elementos(lista_se_t* lista) {
	if (!lista) return 0;
	return lista->cantidad;
}

void* elemento_en_posicion(lista_se_t* lista, int indice) {
	if (!lista || vacia(lista) || indice < 0 || indice >= lista->cantidad) return NULL;
	nodo_t* nodo = lista->inicio;

	for (int i = 0; i < indice; i++) {
		nodo = nodo->siguiente;
	}

	return nodo->elemento;
}

void* ultimo(lista_se_t* lista) {
	if (!lista || vacia(lista)) return NULL;
	return elemento_en_posicion(lista, ((int)lista->cantidad) - 1);

}

int insertar_en_posicion(lista_se_t* lista, void* elemento, int indice) {
	if (!lista || !elemento) return -1;
	nodo_t* a_insertar = malloc(sizeof(nodo_t));
	if(!a_insertar) return -1;
	a_insertar->elemento = elemento;
	nodo_t* actual = lista->inicio;

	if (vacia(lista) || indice == 0) {
		a_insertar->siguiente = actual;
		lista->inicio = a_insertar;
		lista->cantidad++;
		return 0;
	}

	if (indice > lista->cantidad || indice < 0) indice = (int)lista->cantidad;

	for (int i = 1; i < indice; i++) {
		actual = actual->siguiente;
	}

	a_insertar->siguiente = actual->siguiente;
	actual->siguiente = a_insertar;
	lista->cantidad++;
	return 0;
}

int insertar(lista_se_t* lista, void* elemento) {
	if (!lista || !elemento) return -1;
	return insertar_en_posicion(lista, elemento, (int)lista->cantidad);
}

int borrar_de_posicion(lista_se_t* lista, int indice) {
	if (!lista || vacia(lista)) return -1;
	if (indice >= lista->cantidad || indice < 0) indice = ((int)lista->cantidad) - 1;
	nodo_t* a_borrar = lista->inicio;

	if (indice == 0)  {
		lista->inicio = a_borrar->siguiente;
		free(a_borrar);
		lista->cantidad--;
		return 0;
	}

	nodo_t* anterior;

	for (int i = 0; i < indice; i++) {
		anterior = a_borrar;
		a_borrar = a_borrar->siguiente;
	}

	anterior->siguiente = a_borrar->siguiente;
	free(a_borrar);
	lista->cantidad--;
	return 0;

}

int borrar(lista_se_t* lista) {
	if (!lista || vacia(lista)) return -1;
	return borrar_de_posicion(lista, (int)lista->cantidad);
}

int destruir_lista(lista_se_t* lista) {
	if (!lista) return -1;

	while(!vacia(lista)) {
		borrar_de_posicion(lista, 0);
	}

	free(lista);
	return 0;
}