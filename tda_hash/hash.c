#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lista.h"
#define CONSTANTE_HASHING_1 16777619
#define CONSTANTE_HASHING_2 2166136261
#define VOLUMEN_INICIAL 200
#define FACTOR_CARGA 2
#define REDUCIR 4
#define	REDIMENSION 2


/* *****************************************************************
 *                    DEFINICIÓN DE ESTRUCTURAS
 * *****************************************************************/

typedef void (*hash_destruir_dato_t)(void *);

typedef struct hash {
	lista_t** tabla;
	size_t tamanio;
	size_t ocupado;
	hash_destruir_dato_t destruir_elemento;
}hash_t;

typedef struct campo_hash {
	char* clave;
	void* elemento;
} campo_hash_t;

typedef struct iter_hash {
	size_t posicion;
	const hash_t* hash;
	lista_iter_t* lista_iter;
}hash_iter_t;


/* *****************************************************************
 *                 	    FUNCIONES AUXILIARES
 * *****************************************************************/

/* Original: http://ctips.pbworks.com/w/page/7277591/FNV%20Hash */
size_t funcion_hashing(const char* clave) {
    size_t hashing = CONSTANTE_HASHING_2;
    size_t largo = strlen(clave);

    for(size_t i = 0; i < largo; i++){
        hashing ^= (size_t)(clave[i]);
        hashing *= CONSTANTE_HASHING_1;
    }

    return hashing;
}

char* duplicar(const char* org) {
	size_t longitud = strlen(org);
	char* dup = malloc(sizeof(char) * (longitud + 1));
	if (!dup) return NULL;
	strcpy(dup, org);
	return dup;
}

void rehash(hash_t* hash, lista_t** tabla_nueva, size_t tamanio_nuevo) {
	for (int i = 0; i < hash->tamanio; i++){
		lista_iter_t* iter = lista_iter_crear(hash->tabla[i]);
		
		while (!lista_iter_al_final(iter)){
			campo_hash_t* campo_hash = lista_iter_borrar(iter);
			lista_insertar_ultimo(tabla_nueva[funcion_hashing(campo_hash->clave) % tamanio_nuevo], campo_hash);
		}

		lista_destruir(hash->tabla[i], NULL);
		lista_iter_destruir(iter);
	}
}

lista_t** hash_tabla_crear (hash_t* hash, size_t tamanio_nuevo, bool redimensionar) {
	lista_t** tabla = malloc(sizeof(lista_t*) * tamanio_nuevo);

	if (!tabla) { 
		free(hash);
		return NULL;
	}

	for(size_t i = 0; i < tamanio_nuevo; i++) { 
		tabla[i] = lista_crear();
		if (!tabla[i]) return NULL;
	}

	if (redimensionar) {
		rehash(hash, tabla, tamanio_nuevo);
		free(hash->tabla);
	} else {
		hash->ocupado = 0;
	}

	hash->tabla = tabla;
	hash->tamanio = tamanio_nuevo;
	return tabla;
}

bool hash_redimensionar(hash_t* hash, size_t tamanio_nuevo) {
	lista_t** tabla_nueva = hash_tabla_crear(hash, tamanio_nuevo, true);
	if (!tabla_nueva) return false;
	return true;
}

campo_hash_t* campo_hash_crear(const char* clave, void *elemento) {
	campo_hash_t* campo_hash = malloc(sizeof(campo_hash_t));
	char * clave_dinamica = duplicar(clave);
	if (!campo_hash) return false;
	
	if (!clave_dinamica) { 
		free(campo_hash);
		return false;
	}
	
	campo_hash->clave = clave_dinamica;
	campo_hash->elemento = elemento;

	return campo_hash;
}

lista_iter_t* hash_buscar(const hash_t* hash, const char* clave) {
	lista_iter_t* iter = lista_iter_crear(hash->tabla[funcion_hashing(clave) % hash->tamanio]);

	while (!lista_iter_al_final(iter)) {
		if (!strcmp(((campo_hash_t*)lista_iter_ver_actual(iter))->clave, clave)) {
			return iter;
		}

		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	return NULL;
}

/* *****************************************************************
 *                    PRIMITIVAS DEL HASH
 * *****************************************************************/

hash_t* crear_hash(hash_destruir_dato_t destruir_elemento) {
	hash_t* hash = malloc(sizeof(hash_t));
	if (!hash) return NULL;
	lista_t** tabla = hash_tabla_crear(hash, VOLUMEN_INICIAL, false);
	
	if (!tabla) { 
		free(hash);
		return NULL;
	}
	
	hash->destruir_elemento = destruir_elemento;
	return hash;
}

int guardar_elemento(hash_t *hash, const char* clave, void *elemento) {
	if (!hash || !clave || !elemento) return -1;
	
	if (hash->ocupado / hash->tamanio >= FACTOR_CARGA) {
		size_t tamanio_nuevo = hash->tamanio * REDIMENSION;
		if (!hash_redimensionar(hash, tamanio_nuevo)) return -1;
	}

	size_t posicion = funcion_hashing(clave) % hash->tamanio;
	lista_iter_t* iter;
	campo_hash_t* campo;

	if ((iter = hash_buscar(hash, clave))) {
		campo = lista_iter_ver_actual(iter);
		if (hash->destruir_elemento) (hash->destruir_elemento)(campo->elemento);
		campo->elemento = elemento;
		lista_iter_destruir(iter);
		return 0;
	}

	campo = campo_hash_crear(clave, elemento);

	if (!lista_insertar_ultimo(hash->tabla[posicion], campo)) {
		free(campo->clave);
		free(campo);
		return -1;
	}

	hash->ocupado++;
	return 0;
}

int quitar_elemento(hash_t *hash, const char *clave) {
	if (!hash || !clave) return -1;

	if ((hash->ocupado != 0) && (hash->ocupado / hash->tamanio <= FACTOR_CARGA / REDUCIR)) {
		size_t tamanio_nuevo = hash->tamanio / REDIMENSION;
		if (!hash_redimensionar(hash, tamanio_nuevo)) return -1;
	}

	lista_iter_t* iter = hash_buscar(hash, clave);
	if (!iter) return -1;
	campo_hash_t* campo_a_borrar = lista_iter_borrar(iter);
	if (hash->destruir_elemento) (hash->destruir_elemento)(campo_a_borrar->elemento);
	free(campo_a_borrar->clave);
	free(campo_a_borrar);
	lista_iter_destruir(iter);
	hash->ocupado--;
	return 0;
}

void *obtener_elemento(const hash_t *hash, const char *clave){
	if (!hash || !clave) return NULL;
	lista_iter_t* iter = hash_buscar(hash, clave);
	if (!iter) return NULL;
	void* elemento = ((campo_hash_t*)(lista_iter_ver_actual(iter)))->elemento;
	lista_iter_destruir(iter);
	return elemento;
}


bool existe_clave(const hash_t *hash, const char *clave){
	if (!hash || !clave) return false;
	lista_iter_t* iter = hash_buscar(hash, clave);
	if (!iter) return false;
	lista_iter_destruir(iter);
	return true;
}

size_t cantidad(const hash_t *hash) {
	if (!hash) return 0;
	return hash->ocupado;
}

int destruir_hash(hash_t *hash) {
	if (!hash) return -1;

	for (int i = 0; i < hash->tamanio; i++) {
		lista_iter_t* iter = lista_iter_crear(hash->tabla[i]);

		while (!lista_iter_al_final(iter)) {
			campo_hash_t* campo_a_borrar = lista_iter_borrar(iter);
			
			if (hash->destruir_elemento) {
				(hash->destruir_elemento)(campo_a_borrar->elemento);
			}

			free(campo_a_borrar->clave);
			free(campo_a_borrar);
		}

		lista_iter_destruir(iter);
		lista_destruir(hash->tabla[i], NULL);
	}

	free(hash->tabla);
	free(hash);
	return 0;
}

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

bool esta_al_final(const hash_iter_t *iter) {
	if (!iter) return false;
	if (!iter->hash->ocupado) return true;
	return iter->posicion == iter->hash->tamanio;
}

lista_iter_t* hash_iter_avanzar_lista(hash_iter_t* iter) {
	lista_iter_t* lista_iter = lista_iter_crear(iter->hash->tabla[iter->posicion]);

	while (!esta_al_final(iter)){
		if (!lista_iter_al_final(lista_iter)) {
			return lista_iter;
		}
		
		lista_iter_destruir(lista_iter);
		iter->posicion++;
		if (esta_al_final(iter)) return NULL;
		lista_iter = lista_iter_crear(iter->hash->tabla[iter->posicion]);
		if (!lista_iter) return NULL;
	}

	lista_iter_destruir(lista_iter);
	return NULL;
}

hash_iter_t *crear_iterador(const hash_t *hash) {
	if (!hash) return NULL;
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if(!iter) return NULL;
	iter->posicion = 0; 
	iter->hash = hash;
	iter->lista_iter = hash_iter_avanzar_lista(iter);
	return iter;
}

bool avanzar_iterador(hash_iter_t *iter) {
	if (!iter) return false;
	if (esta_al_final(iter)) return false;

	if (!lista_iter_al_final(iter->lista_iter)) {
		lista_iter_avanzar(iter->lista_iter);
		if (!lista_iter_al_final(iter->lista_iter)) return true;
	}

	lista_iter_destruir(iter->lista_iter);
	iter->posicion++;
	if (esta_al_final(iter)) return false;
	iter->lista_iter = hash_iter_avanzar_lista(iter);
	if (!iter->lista_iter) return false;
	return true;
}

const char *elemento_actual(const hash_iter_t *iter) {
	if (!iter) return NULL;
	if (esta_al_final(iter)) return NULL;
	return ((campo_hash_t*)lista_iter_ver_actual(iter->lista_iter))->clave;
}

int hash_iterador_destruir(hash_iter_t* iter) {
	if (!iter) return -1;
	if (!esta_al_final(iter)) lista_iter_destruir(iter->lista_iter);
	free(iter);
	return 0;
}