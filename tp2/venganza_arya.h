#ifndef __VENGANZA_ARYA_H__
#define __VENGANZA_ARYA_H__

#include "venganza_arya.h"
#include "lista.h"
#include "pila.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#define MAX_NOMBRE 50
#define	MAX_DESCRIPCION 600
#define MAX_MENSAJE 300

typedef struct persona {
	char        nombre[MAX_NOMBRE];
	char        descripcion[MAX_DESCRIPCION];
	char        msj_muerte[MAX_MENSAJE];
	char        msj_perdon[MAX_MENSAJE];
	int         en_lista;           // 1 si está, 0 en caso contrario.
	int         culpable;           // 1 si lo es, 0 en caso contrario.
	int         danio;              // daño proporcionado por una persona inocente.
	int         beneficio;          // 1 aumenta vida, 2 elimina víctima, 3 llave del castillo.
} persona_t;

typedef struct ciudad {
	char        nombre[MAX_NOMBRE];
	persona_t   posible_victima;
}ciudad_t;

typedef struct jugador {
	int         vida;               //inicialmente 100
	bool        posee_llave;
	lista_t*    rostros;
	pila_t*     victimas;
} jugador_t;

/*
 * Crea un jugador y deja la lista de rostros preparada para ser utilizada.
 * Devuelve NULL si no pudo crear algo de lo necesario.
 */
jugador_t* nuevo_jugador();

/*
 * Carga las ciudades del archivo a la lista de ciudades a utilizar.
 * Devuelve 0 si la lista se cargo correctamente, -1 en caso contrario.
 */
int cargar_ciudades(char archivo[MAX_NOMBRE], lista_t* ciudades);

/*
 * Carga las victimas del archivo a la pila de víctimas a utilizar.
 * Devuelve 0 si la lista se cargo correctamente, -1 en caso contrario.
 */
int cargar_victimas(char archivo[MAX_NOMBRE], pila_t* victimas);

/*
 * Luego de asesinar una persona, se deben actualizar los registros según corresponda.
 * Recolectar el rostro, desapilar si es una víctima, actualizar el estado del jugador.
 * Devuelve 0 si se pudo actualizar correctamente o -1 en caso contrario.
 */

int actualizar_juego(jugador_t* jugador, persona_t persona, lista_t* ciudades, iterador_t* it_rostros);

/*
 * Pasa a la siguiente ciudad.
 * Devuelve 0 si se pudo avanzar o -1 en caso contrario.
 */
int avanzar_mapa(jugador_t* jugador, iterador_t* it_ciudades, ciudad_t* ultima_ciudad, int* ciudad_numero);

/*
 * Imprime por pantalla la ciudad actual y aquellas que no fueron visitadas.
 * Devuelve 0 si se pudo mostrar o -1 en caso contrario.
 */
void mostrar_mapa(lista_t* ciudades, iterador_t* it_ciudades, int* ciudad_numero);

/*
 * Imprime por pantalla los rostros colectados.
 * Devuelve 0 si se pudo mostrar o -1 en caso contrario.
 */
int mostrar_rostros_recolectados(lista_t* rostros, iterador_t* it_rostros);

/*
 * Imprime por pantalla el nombre de la próxima víctima.
 * Devuelve 0 si pudo mostrarlo o -1 en caso contrario.
 */
int mostrar_proxima_victima(pila_t* victimas);

/**/
int dar_datos_persona(jugador_t* jugador, lista_t* ciudades, iterador_t* it_ciudades, iterador_t* it_rostros, ciudad_t* ultima_ciudad, int* ciudad_numero);

/**/
int usuario_acepta(char entrada);

/**/
int ver_vida(jugador_t* jugador);

/**/
void modificar_vida(jugador_t* jugador, int cantidad);

/**/
lista_t* dar_lista_rostros(jugador_t* jugador);

/**/
pila_t* dar_pila_victimas(jugador_t* jugador);

/**/
bool tengo_llave(jugador_t* jugador);

#endif /* __VENGANZA_ARYA_H__ */