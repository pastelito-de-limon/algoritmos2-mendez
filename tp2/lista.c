#include "lista.h"
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct nodo {
	void* dato;
	struct nodo* proximo;
} nodo_t;

struct lista {
	nodo_t* primero;
	nodo_t* ultimo;
	size_t largo;
};

struct lista_iter {
	nodo_t* anterior;
	nodo_t* actual;
	lista_t* lista;
};

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

nodo_t* crear_nodo(void* dato) {
	nodo_t* nodo = malloc(sizeof(nodo_t));

	if (!nodo) {
		return NULL;
	}

	nodo->dato = dato;
	nodo->proximo = NULL;
	return nodo;
}

lista_t *lista_crear(void) {
	lista_t* lista = malloc(sizeof(lista_t));

	if (!lista) {
		return NULL;
	}

	lista->primero = NULL;
	lista->ultimo = NULL;
	lista->largo = 0;
	return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
	return !lista->largo;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
	nodo_t* nodo = crear_nodo(dato);

	if (!nodo) {
		return false;
	}

	if (lista_esta_vacia(lista)) {
		lista->ultimo = nodo;
	} else {
		nodo->proximo = lista->primero;
	}

	lista->primero = nodo;
	lista->largo++;
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
	nodo_t* nodo = crear_nodo(dato);

	if (!nodo) {
		return false;
	}

	if (lista_esta_vacia(lista)) {
		lista->primero = nodo;
	} else {
		lista->ultimo->proximo = nodo;
	}

	lista->ultimo = nodo;
	lista->largo++;
	return true;
}

void *lista_borrar_primero(lista_t *lista) {
	if (lista_esta_vacia(lista)) {
		return NULL;
	}

	void* dato = lista->primero->dato;
	nodo_t* siguiente = lista->primero->proximo;
	free(lista->primero);
	lista->primero = siguiente;

	if (!siguiente) {
		lista->ultimo = NULL;
	}

	lista->largo--;
	return dato;
}

void *lista_ver_primero(const lista_t *lista) {
	if (lista_esta_vacia(lista)){
		return NULL;
	}

	return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista) {
	if (lista_esta_vacia(lista)) {
		return NULL;
	}

	return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista) {
	return lista->largo;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)) {
	while (!lista_esta_vacia(lista)) {
		void* dato = lista_borrar_primero(lista);
		if (destruir_dato) {
			destruir_dato(dato);
		}
	}

	free(lista);
}

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
	if (!visitar) {
		return;
	}

	nodo_t* actual = lista->primero;
	
	while(actual && visitar(actual->dato, extra)) {
		actual = actual->proximo;
	}
}

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t *lista_iter_crear(lista_t *lista) {
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));

	if (!iter) {
		return NULL;
	}

	iter->lista = lista;
	iter->actual = lista->primero;
	iter->anterior = NULL;

	return iter;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
	return !iter->actual;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
	if (lista_iter_al_final(iter)) {
		return false;
	}

	iter->anterior = iter->actual;
	iter->actual = iter->actual->proximo;
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
	if (!iter->actual) {
		return NULL;
	}

	return iter->actual->dato;
}

void lista_iter_destruir(lista_iter_t *iter) {
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
	nodo_t* nodo = crear_nodo(dato);
	
	if (!nodo) {
		return false;
	}

	if (!iter->anterior) {
		iter->lista->primero = nodo;
	} else {
		iter->anterior->proximo = nodo;
	}

	if (!iter->actual) {
		iter->lista->ultimo = nodo;
	}

	nodo->proximo = iter->actual;
	iter->actual = nodo;
	iter->lista->largo++;
	return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
	if (!iter->actual) {
		return NULL;
	}

	nodo_t* nodo = iter->actual->proximo;

	if (!iter->anterior) {
		iter->lista->primero = nodo;
	} else {
		iter->anterior->proximo = nodo;
	}

	if (!nodo) {
		iter->lista->ultimo = iter->anterior;
	}

	void* dato = iter->actual->dato;
	free(iter->actual);
	iter->actual = nodo;
	iter->lista->largo--;
	return dato;
}