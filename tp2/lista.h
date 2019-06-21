#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/*Lista compuesta de punteros genéricos.*/
typedef struct lista lista_t;

/*Iterador de lista.*/
typedef struct lista_iter iterador_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

/*Crea una lista vacía.
Post: devuelve la lista, de no poder crearla, devuelve NULL.*/
lista_t *lista_crear(void);

/*Informa si está vacía la lista.
Pre: la lista existe.
Post: devuelve true si está vacía, false de lo contrario.*/
bool lista_esta_vacia(const lista_t *lista);

/*Inserta el dato recibido por parámetro en la primera posición de la lista.
Pre: la lista existe.
Post: devuelve true de insertar con éxito, false de lo contrario.*/
bool lista_insertar_primero(lista_t *lista, void *dato);

/*Inserta el dato recibido por parámetro en la última posición de la lista.
Pre: la lista existe.
Post: devuelve true de insertar con éxito, false de lo contrario.*/
bool lista_insertar_ultimo(lista_t *lista, void *dato);

/*Elimina el primer elemento de la lista.
Pre: la lista existe.
Post: devuelve el dato eliminado, NULL si la lista está vacía.*/
void *lista_borrar_primero(lista_t *lista);

/*Muestra el primer elemento.
Pre: la lista existe.
Post: devuelve el primer dato de la lista, NULL si la lista está vacía.*/
void *lista_ver_primero(const lista_t *lista);

/*Muestra el último elemento.
Pre: la lista existe.
Post: devuelve el ultimo dato de la lista, NULL si la lista está vacía.*/
void *lista_ver_ultimo(const lista_t* lista);

/*Informa el largo de la lista.
Pre: la lista existe.
Post: devuelve el size_t del largo de la lista.*/
size_t lista_largo(const lista_t *lista);

/*Destruye la lista. Si se le pasa por parámetro una función, además destruye los datos
de la misma; de lo contrario, si recibe NULL, no actúa sobre dichos elementos.
Pre: la lista existe y la función pasada por parámetro es capaz de destruir los elementos.
Post: la lista y sus elementos fueron destruídos.*/
void lista_destruir(lista_t *lista, void destruir_dato(void *));

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

/*Aplica la funcion recibida por parámetro a cada uno de los datos de la lista.
Además recibe un puntero genérico extra en caso de ser necesario para dicha función.
Pre: la lista existe y la función recibida es capaz de manejar los datos.
Post: ejecutó una llamada a visitar para cada dato de la lista.*/
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

/*Crea el iterador de lista.
Pre: la lista existe.
Post: el iterador de lista fue creado y se encuentra apuntando al primer elemento de la lista.
Si la lista está vacía, el iterador apunta a NULL.*/
iterador_t *lista_iter_crear(lista_t *lista);

/*Avanza el iterador para que apunte al siguiente elemento de la lista.
Pre: el iterador existe.
Post: devuelve true si logra avanzar el iterador. False en caso de alcanzar el final de la lista.*/
bool lista_iter_avanzar(iterador_t *iter);

/*Devuelve el dato de la lista al cual apunta actualmente.
Pre: el iterador existe.
Post: devuelve dicho dato, de no existir, devuelve NULL.*/
void *lista_iter_ver_actual(const iterador_t *iter);

/*Informa si el iterador ha alcanzado el final de la lista y se encuentra apuntando a NULL.
Pre: el iterador existe.
Post: devuelve true de estar al final, false de lo contrario.*/
bool lista_iter_al_final(const iterador_t *iter);

/*Destruye el iterador.
Pre: el iterador existe.
Post: el iterador fue destruído.*/
void lista_iter_destruir(iterador_t *iter);

/*Inserta el dato recibido por parámetro en la lista pertinente al iterador. El dato pasa a ocupar
la posición apuntada.
Pre: el iterador existe.
Post: fue insertado el dato en la lista y el iterador apunta al mismo.*/
bool lista_iter_insertar(iterador_t *iter, void *dato);

/*Elimina el elemento de la lista al cual apunta el iterador.
Pre: el iterador existe.
Post: de lograr eliminarlo, lo devuelve. Si el iterador apuntaba al final, devuelve NULL.*/
void *lista_iter_borrar(iterador_t *iter);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

/*Ejecución de las pruebas.*/
void pruebas_lista_alumno(void);

#endif