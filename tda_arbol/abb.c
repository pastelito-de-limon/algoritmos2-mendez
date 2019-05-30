#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "abb.h"

nodo_t* crear_nodo(void* elemento) {
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (!nodo) return NULL;
	nodo->elemento = elemento;
	nodo->izquierda = NULL;
	nodo->derecha = NULL;
	return nodo;
}

nodo_t* _buscar(nodo_t* nodo, void* elemento, abb_comparador comparador) {
	if (!nodo) return NULL;
	int comparacion = comparador(elemento, nodo->elemento); 
	if (comparacion == 0) return nodo;
	if (comparacion < 0) return _buscar(nodo->izquierda, elemento, comparador);
	return _buscar(nodo->derecha, elemento, comparador);
}

nodo_t* buscar_padre(nodo_t* padre, nodo_t* nodo, void* elemento, abb_comparador comparador) {
	if (!nodo) return padre;
	if (!padre) padre = nodo;
	int comparacion = comparador(elemento, nodo->elemento); 
	if (comparacion == 0) return padre;
	if (comparacion < 0) return buscar_padre(nodo, nodo->izquierda, elemento, comparador);
	return buscar_padre(nodo, nodo->derecha, elemento, comparador);
}

void swap(nodo_t* nodo1, nodo_t* nodo2) {
	void* elemento = nodo1->elemento;
	nodo1->elemento = nodo2->elemento;
	nodo2->elemento = elemento;
}

abb_t* crear_arbol(abb_comparador comparador, abb_liberar_elemento destructor) {
	abb_t* arbol = malloc(sizeof(abb_t));
	if (!arbol) return NULL;
	arbol->nodo_raiz = NULL;
	arbol->comparador = comparador;
	arbol->destructor = destructor;
	return arbol;
}

int insertar(abb_t* arbol, void* elemento) {
	if (!arbol || !elemento || !arbol->comparador) return -1;
	nodo_t* padre = buscar_padre(NULL, arbol->nodo_raiz, elemento, arbol->comparador);
	nodo_t* nodo_preexistente = _buscar(padre, elemento, arbol->comparador);
	if (nodo_preexistente) return -1;
	nodo_t* nodo_nuevo = crear_nodo(elemento);
	if (!nodo_nuevo) return -1;
	
	if (!padre) {
		arbol->nodo_raiz = nodo_nuevo;
		return 0;
	}
	
	int comparacion = (arbol->comparador)(elemento, padre->elemento);
	if (comparacion == 0) arbol->nodo_raiz = nodo_nuevo;
	else if (comparacion < 0) padre->izquierda = nodo_nuevo;
	else padre->derecha = nodo_nuevo;
	return 0;
}

int borrar(abb_t* arbol, void* elemento) {
	if (!arbol || !elemento || !arbol->comparador) return -1;
	nodo_t* padre = buscar_padre(NULL, arbol->nodo_raiz, elemento, arbol->comparador);
	nodo_t* nodo_borrar = _buscar(padre, elemento, arbol->comparador);
	if (!padre || !nodo_borrar) return -1;
	int comparacion = (arbol->comparador)(elemento, padre->elemento);
	bool es_raiz = (comparacion == 0);

	if (nodo_borrar->derecha && nodo_borrar->izquierda) {
		nodo_t* nodo_reemplazo = nodo_borrar->derecha;
		padre = nodo_borrar;

		while (nodo_reemplazo->izquierda) {
			padre = nodo_reemplazo;
			nodo_reemplazo = nodo_reemplazo->izquierda;
		}

		comparacion = (arbol->comparador)(nodo_reemplazo->elemento, padre->elemento);
		es_raiz = false;
		swap(nodo_borrar, nodo_reemplazo);
		nodo_borrar = nodo_reemplazo;
	}

	if (!nodo_borrar->izquierda && !nodo_borrar->derecha) {
		if (es_raiz) arbol->nodo_raiz = NULL;
		else if (comparacion < 0) padre->izquierda = NULL;
		else padre->derecha = NULL;
	}

	if (nodo_borrar->derecha) {
		if (es_raiz) arbol->nodo_raiz = nodo_borrar->derecha;
		else if (comparacion < 0) padre->izquierda = nodo_borrar->derecha;
		else padre->derecha = nodo_borrar->derecha;
	
	} else if (nodo_borrar->izquierda) {
		if (es_raiz) arbol->nodo_raiz = nodo_borrar->izquierda;
		else if (comparacion < 0) padre->izquierda = nodo_borrar->izquierda;
		else padre->derecha = nodo_borrar->izquierda;
	}
	
	if (arbol->destructor) (arbol->destructor)(nodo_borrar->elemento);
	free(nodo_borrar);
	return 0;
}

void* buscar(abb_t* arbol, void* elemento) {
	if (!arbol || !elemento || !arbol->comparador) return NULL;
	nodo_t* nodo = _buscar(arbol->nodo_raiz, elemento, arbol->comparador);
	if (!nodo) return NULL;
	return nodo->elemento;
}

bool vacio(abb_t* arbol) {
	if (!arbol) return true;
	return arbol->nodo_raiz == NULL;
}

void _destruir_arbol(abb_liberar_elemento destructor, nodo_t* nodo){
	if (!nodo) return;
	if (nodo->izquierda) _destruir_arbol(destructor, nodo->izquierda);
	if (nodo->derecha) _destruir_arbol(destructor, nodo->derecha);
	if (destructor) destructor(nodo->elemento);
	free(nodo);
}

int destruir_arbol(abb_t* arbol) {
	if (!arbol) return -1;
	_destruir_arbol(arbol->destructor, arbol->nodo_raiz);
	free(arbol);
	return 0;
}

//IND
void _recorrer_inorden(nodo_t* nodo, void** array, int tamanio_array, int* cantidad_nodos) {
	if (!nodo) return;
	_recorrer_inorden(nodo->izquierda, array, tamanio_array, cantidad_nodos);
	if (tamanio_array <= *cantidad_nodos) return;
	array[*cantidad_nodos] = nodo->elemento;
	(*cantidad_nodos)++;
	_recorrer_inorden(nodo->derecha, array, tamanio_array, cantidad_nodos);
}

int recorrer_inorden(abb_t* arbol, void** array, int tamanio_array) {
	if (!arbol || !array || tamanio_array == 0) return 0;
	int cantidad_nodos = 0;
	_recorrer_inorden(arbol->nodo_raiz, array, tamanio_array, &cantidad_nodos);
	return cantidad_nodos;
}

//NID
void _recorrer_preorden(nodo_t* nodo, void** array, int tamanio_array, int* cantidad_nodos) {
	if (!nodo || tamanio_array <= *cantidad_nodos) return;
	array[*cantidad_nodos] = nodo->elemento;
	(*cantidad_nodos)++;
	_recorrer_preorden(nodo->izquierda, array, tamanio_array, cantidad_nodos);
	_recorrer_preorden(nodo->derecha, array, tamanio_array, cantidad_nodos);
}

int recorrer_preorden(abb_t* arbol, void** array, int tamanio_array) {
	if (!arbol || !array || tamanio_array == 0) return 0;
	int cantidad_nodos = 0;
	_recorrer_preorden(arbol->nodo_raiz, array, tamanio_array, &cantidad_nodos);
	return cantidad_nodos;
}

//IDN
void _recorrer_postorden(nodo_t* nodo, void** array, int tamanio_array, int* cantidad_nodos) {
	if (!nodo) return;
	_recorrer_postorden(nodo->izquierda, array, tamanio_array, cantidad_nodos);
	_recorrer_postorden(nodo->derecha, array, tamanio_array, cantidad_nodos);
	if (tamanio_array <= *cantidad_nodos) return;
	array[*cantidad_nodos] = nodo->elemento;
	(*cantidad_nodos)++;
}

int recorrer_postorden(abb_t* arbol, void** array, int tamanio_array) {
	if (!arbol || !array || tamanio_array == 0) return 0;
	int cantidad_nodos = 0;
	_recorrer_postorden(arbol->nodo_raiz, array, tamanio_array, &cantidad_nodos);
	return cantidad_nodos;
}