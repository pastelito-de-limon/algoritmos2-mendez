#include "pila_vd.h"
#include <stdlib.h>
#define TAMANIO_PILA 20
#define TAMANIO_REDIMENSION 2
#define TAMANIO_REDUCCION 4

/* *****************************************************************
 *                    FUNCIONES AUXILARES
 * *****************************************************************/

int redimensionar(pila_t* pila, int tamanio) {
	void** elementos_nuevo = realloc(pila->elementos, (size_t)tamanio * sizeof(void*));
	if (!elementos_nuevo) return 1;
	pila->elementos = elementos_nuevo;
	pila->tamanio = tamanio;
	return 0;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t* crear_pila() {
	pila_t* pila = malloc(sizeof(pila_t));
	if (!pila) return NULL;
	pila->elementos = malloc(TAMANIO_PILA * sizeof(void*));

	if (!pila->elementos) {
		free(pila);
		return NULL;
	}

	pila->tope = 0;
	pila->tamanio = TAMANIO_PILA;
	return pila;
}

int apilar(pila_t* pila, void* elemento) {
	if (!pila) return 1;

	if (pila->tamanio == pila->tope) {
		if(redimensionar(pila, pila->tamanio * TAMANIO_REDIMENSION) != 0) {
			return 1;
		} 
	}

	pila->elementos[pila->tope++] = elemento;
	return 0;
}

int destruir_pila(pila_t* pila) {
	if (!pila) return 1;
	free(pila->elementos);
	free(pila);
	return 0;
}

bool vacia(pila_t* pila) {
	if (!pila) return false;
	return pila->tope == 0;
}

int desapilar(pila_t* pila) {
	if (!pila || vacia(pila)) return 1;
	
	if (pila->tope <= pila->tamanio / TAMANIO_REDUCCION) {
		if (redimensionar(pila, pila->tamanio / TAMANIO_REDIMENSION) != 0) {
			return 1;
		}
	}

	pila->tope--;
	return 0;
}

void* tope(pila_t* pila) {
	if (!pila || vacia(pila)) return NULL;
	return pila->elementos[pila->tope - 1];
}