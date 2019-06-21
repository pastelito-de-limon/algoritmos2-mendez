#define _GNU_SOURCE
#include "venganza_arya.h"
#include "lista.h"
#include "pila.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/*
 * Crea un jugador y deja la lista de rostros preparada para ser utilizada.
 * Devuelve NULL si no pudo crear algo de lo necesario.
 */
jugador_t* nuevo_jugador() {
	jugador_t* jugador = malloc(sizeof(jugador_t));
	if (!jugador) return NULL;
	jugador->vida = 100;
	jugador->posee_llave = false;
	jugador->rostros = lista_crear();
	
	if (!jugador->rostros) {
		free(jugador);
		return NULL;
	}

	jugador->victimas = pila_crear();

	if (!jugador->victimas) {
		free(jugador->rostros);
		free(jugador);
		return NULL;
	}

	if (cargar_victimas("victimas.txt", jugador->victimas) != 0) {
		free(jugador->victimas);
		free(jugador->rostros);
		free(jugador);
		return NULL;
	}

	return jugador;
}

/*
 * Carga las ciudades del archivo a la lista de ciudades a utilizar.
 * Devuelve 0 si la lista se cargo correctamente, -1 en caso contrario.
 */
int cargar_ciudades(char archivo[MAX_NOMBRE], lista_t* ciudades) {
	FILE* archivo_ciudades = fopen(archivo, "r");
	if (!archivo_ciudades) return -1;
	ciudad_t* ciudad;
	size_t tamanio = sizeof(ciudad_t);

	while(!feof(archivo_ciudades)){
		ciudad = malloc(sizeof(ciudad_t));

		if (!ciudad) {
			fclose(archivo_ciudades);
			return -1;
		}

		fread(ciudad, tamanio, 1, archivo_ciudades);

		if (!lista_insertar_ultimo(ciudades, ciudad)) {
			free(ciudad);
			fclose(archivo_ciudades);
			return -1;
		}	
	}

	fclose(archivo_ciudades);
	return 0;
}

/*
 * Carga las victimas del archivo a la pila de víctimas a utilizar.
 * Devuelve 0 si la lista se cargo correctamente, -1 en caso contrario.
 */
int cargar_victimas(char archivo[MAX_NOMBRE], pila_t* victimas) {
	FILE* archivo_victimas = fopen(archivo, "r");
	if (!archivo_victimas) return -1;
	char* linea = NULL;
	ssize_t leido;
	size_t tam;
	char* victima;

	while((leido = getline(&linea, &tam, archivo_victimas)) != -1){

		victima = malloc(sizeof(char) * (size_t)leido);
		
		if (!victima) {
			fclose(archivo_victimas);
			return -1;
		}

		strcpy(victima, linea);
		victima[leido - 1] = '\0';

		if (!pila_apilar(victimas, victima)) {
			free(victima);
			fclose(archivo_victimas);
			return -1;
		}
	}

	free(linea);
	fclose(archivo_victimas);
	return 0;
}

/*void matar_victima(jugador_t* jugador, persona_t persona) {
	lista_insertar_ultimo(jugador->rostros, persona);
	pila_desapilar(jugador->victimas);
}
*/
/*
 * Luego de asesinar una persona, se deben actualizar los registros según corresponda.
 * Recolectar el rostro, desapilar si es una víctima, actualizar el estado del jugador.
 * Devuelve 0 si se pudo actualizar correctamente o -1 en caso contrario.
 */
/*int actualizar_juego(jugador_t* jugador, persona_t persona) {
	if (persona->en_lista == 1) {
		if (pila_ver_tope(jugador->victimas) != persona) { //LA PILA DE VICITMAS TIENE CHAR* ADENTROOOOO
			jugador->vida = 0;
			return -1; //hay una victima anterior que no fue eliminada
		}
		
		matar_victima(jugador, persona);
		return 0;
	}

	if (persona->culpable == 1) {
		if (persona->beneficio == 1) modificar_vida(jugador, AUMENTO_VIDA);
		else if (persona->beneficio == 2) matar_victima(jugador, pila_ver_tope(jugador->victimas));
		else jugador->posee_llave = true;
	} else {
		modificar_vida(jugador, persona->danio);
	}

	return 0;
}
*/
/*
 * Pasa a la siguiente ciudad.
 * Devuelve 0 si se pudo avanzar o -1 en caso contrario.
 */
/*int avanzar_mapa(iterador_t* it_ciudades) {
	if (!lista_iter_avanzar(it_ciudades)) return -1;
	return 0;
}
*/
/*BIEN: 0, ERROR: -1*/
/*int dar_datos_persona(jugador_t* jugador, iterador_t* it_ciudades) {
	persona_t* persona = it_ciudades->actual->posible_victima;
	printf("%s\n", persona->descripcion);
	char opcion[2];
	printf("Sale ese asesinato loquillo? (S/N): \n");
	fgets(opcion, 2, stdin);
	int aceptado = usuario_acepta(opcion[0]);

	while(aceptado > 0) {
		printf("Escribí bien\n");
		modificar_vida(jugador, DESCUENTO_VIDA);
		aceptado = usuario_acepta(opcion[0]);
	}

	if (aceptado < 0) {
		printf("%s\n", persona->msj_perdon);
		return 0;
	}

	printf("%s\n", persona->msj_muerte);
	if (actualizar_juego(jugador, persona) != 0) return -1;
	avanzar_mapa(it_ciudades); //eliminar persona de ciudad y luego chequear NULLs... (!)
	//...o si no eliminar ciudad pero asegurar que quede la ciudad anterior apuntando a la siguiente (!)
	return 0;
}
*/
/*
 * Imprime por pantalla la ciudad actual y aquellas que no fueron visitadas.
 * Devuelve 0 si se pudo mostrar o -1 en caso contrario.
 */
int mostrar_mapa(iterador_t* it_ciudades) {
	char* ciudad = (char*)lista_iter_ver_actual(it_ciudades);
	printf("Ciudad actual: \n%s\n", ciudad);
	if (lista_iter_al_final(it_ciudades)) return 0;
	lista_iter_avanzar(it_ciudades);
	printf("Mapa:\n");

	while (!lista_iter_al_final(it_ciudades)) {
		ciudad = (char*)lista_iter_ver_actual(it_ciudades);
		printf("%s\n", ciudad);
		lista_iter_avanzar(it_ciudades);
	}

	//hay que eliminar el iter al final actual y rehacerlo

	return 0;
}

/*
 * Imprime por pantalla los rostros colectados.
 * Devuelve 0 si se pudo mostrar o -1 en caso contrario.
 */

int mostrar_rostros_recolectados(iterador_t* it_rostros) {
	if (lista_iter_al_final(it_rostros)) {
		printf("Aún no tenés rostros\n");
		return 0;
	}

	char* rostro;

	while (!lista_iter_al_final(it_rostros)) {
		rostro =  (char*)lista_iter_ver_actual(it_rostros);
		printf("%s\n", rostro);
		lista_iter_avanzar(it_rostros);
	}

	//hay que eliminar el iter al final actual y rehacerlo

	return 0;
}

/*
 * Imprime por pantalla el nombre de la próxima víctima.
 * Devuelve 0 si pudo mostrarlo o -1 en caso contrario.
 */

int mostrar_proxima_victima(pila_t* victimas) {
	if (!pila_ver_tope(victimas)) return -1;
	char* proxima_victima = (char*)pila_ver_tope(victimas);
	printf("La próxima vez que sangre sea derramada,\nTendrá %s una muerte desalmada.\n", proxima_victima);
	return 0;
}

int ver_vida(jugador_t* jugador) {
	return jugador->vida;
}

void modificar_vida(jugador_t* jugador, int cantidad) {
	jugador->vida += cantidad;
}

lista_t* dar_lista_rostros(jugador_t* jugador) {
	return jugador->rostros;
}

pila_t* dar_pila_victimas(jugador_t* jugador) {
	return jugador->victimas;
}

/* ACEPTA: 0, RECHAZA: -1, BASURA: 1 */
int usuario_acepta(char entrada) {
	if (entrada == 'S' || entrada == 's') return 0;
	if (entrada == 'N' || entrada == 'n') return -1;
	return 1;
}

bool tengo_llave(jugador_t* jugador) {
	return jugador->posee_llave;
}