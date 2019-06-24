#include "venganza_arya.h"
#include "lista.h"
#include "pila.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#define CANT_OPCIONES 5
#define TAM_ENTRADA_USUARIO 3
#define DESCUENTO_VIDA -20
#define AUMENTO_VIDA 5

/*
Intro: explicacion juego, explicacion menú, prompt comenzar, ...
UI: vida, llave, ciudad actual, cant ciudades restantes, cant victimas restantes, menú, ...
Menú:
	I: Información. 	Datos de persona en ciudad actual.
	M: Mapa. 			Nombra ciudad actual y restantes en orden.
	A: Avanzar. 		Avanza a la próxima ciudad.
	P: Próximo. 		Imprime próxima víctima.
	C: Colección. 		Imprime todos los rostros actuales.
...
*/

void destruir_pila_y_elementos(pila_t* victimas) {
	while (!pila_esta_vacia(victimas)) {
		free(pila_desapilar(victimas));
	}
	
	pila_destruir(victimas);
}

void destruir_estructuras(jugador_t* jugador, pila_t* victimas, lista_t* rostros, iterador_t* it_rostros, lista_t* ciudades, iterador_t* it_ciudades, iterador_t* it_mapa) {
	if (it_rostros) lista_iter_destruir(it_rostros);
	if (rostros) lista_destruir(rostros, free);
	if (it_ciudades) lista_iter_destruir(it_ciudades);
	if (it_mapa) lista_iter_destruir(it_mapa);
	if (ciudades) lista_destruir(ciudades, free);
	if (victimas) destruir_pila_y_elementos(victimas);
	if (jugador) free(jugador);
}

/*
Pierdo si:
	vida <= 0
	última ciudad pero sin llave 
	(proxima victima no es asesinada entonces vida sea hace 0)
	
Gano si:
	pila_esta_vacia(victimas) == true
*/

/* GANAR/PERDER: 1, SEGUIR: 0 */
/*
int chequear_fin_juego(pila_t* victimas, jugador_t* jugador, iterador_t* it_ciudades, ciudad_t* ultima_ciudad) {
	if (pila_esta_vacia(victimas)) { //Si gano, con el print mensaje ganar 
		printf("Has aniquilado a tus enemigos finalmente\n y han quedado sus almas en lamento...\n
			¿Pero podrás alcanzar la victoria nuevamente \n en tu valiente pŕoximo intento?\n\nFIN DEL JUEGO.\n");
		return 1;
	}
	
	bool estoy_al_final_sin_llave = ((ciudad_t*)lista_iter_ver_actual(it_ciudades) == ultima_ciudad) && (tengo_llave(jugador));

	if (ver_vida(jugador) == 0 || estoy_al_final_sin_llave) {
		printf("Perdiste lol");
		return 1:
	}

	return 0;
}
*/
/* CORRECTO: 0, INCORRECTO: -1, BASURA: 1 */
int usuario_menu(char entrada, jugador_t* jugador, pila_t* victimas, iterador_t* it_ciudades, iterador_t* it_mapa, iterador_t* it_rostros, ciudad_t* ultima_ciudad) {
	if (entrada == 'I' || entrada == 'i') return dar_datos_persona(jugador, it_ciudades, ultima_ciudad);
	
	if (entrada == 'M' || entrada == 'm') {
		mostrar_mapa(it_mapa);
		return 0;
	}
	
	if (entrada == 'A' || entrada == 'a') return avanzar_mapa(it_ciudades);
	else if (entrada == 'P' || entrada == 'p') return mostrar_proxima_victima(victimas);
	else if (entrada == 'C' || entrada == 'c') return mostrar_rostros_recolectados(it_rostros);
	return 1;
}

/* SEGUIR: 0, GANAR/PERDER = 1, ERROR: -1 */
int jugar(jugador_t* jugador, pila_t* victimas, lista_t* ciudades, iterador_t* it_ciudades, iterador_t* it_mapa, lista_t* rostros, iterador_t* it_rostros, ciudad_t* ultima_ciudad) {
	char opcion[TAM_ENTRADA_USUARIO];
	printf("[Menu]\n");
	fgets(opcion, TAM_ENTRADA_USUARIO, stdin);
	int elegido = usuario_menu(opcion[0], jugador, victimas, it_ciudades, it_mapa, it_rostros, ultima_ciudad);

	while (elegido > 0) {
		printf("[Menu]\n");
		fgets(opcion, TAM_ENTRADA_USUARIO, stdin);
		elegido = usuario_menu(opcion[0], jugador, victimas, it_ciudades, it_mapa, it_rostros, ultima_ciudad);
		printf("Escribí bien\n");
		modificar_vida(jugador, DESCUENTO_VIDA);
	}

	if (elegido < 0) return -1;

	if (!it_rostros) {
		it_rostros = lista_iter_crear(rostros);
		if (!it_rostros) return -1;
	} else if (!it_mapa) {
		it_mapa = lista_iter_crear(ciudades);
		if (!it_mapa) return -1;
	}

	//A partir de ahora es cuando elegido == 0

	//Sacar barritas de comentario a lo siguiente:
	//if (chequear_fin_juego(victimas, jugador, it_ciudades, ultima_ciudad) == 0) return 0;
	return 1;
}

//DESTRUIR jugador_t* jugador, pila_t* victimas, lista_t* rostros, iterador_t* it_rostros, lista_t* ciudades, iterador_t* it_ciudades, iterador_t* it_mapa
//JUGAR jugador_t* jugador, pila_t* victimas, iterador_t* it_ciudades, iterador_t* it_rostros, iterador_t* it_mapa, ciudad_t* ultima_ciudad
int main(int argc, char** argv) {
	printf("Intro\n"); 
	jugador_t* jugador = nuevo_jugador();
	if (!jugador) return -1;
	pila_t* victimas = dar_pila_victimas(jugador);
	lista_t* rostros = dar_lista_rostros(jugador);
	iterador_t* it_rostros = lista_iter_crear(rostros);

	if (!it_rostros) {
		destruir_estructuras(jugador, victimas, rostros, NULL, NULL, NULL, NULL);
		return -1;
	}

	lista_t* ciudades = lista_crear();

	if (!ciudades) {
		destruir_estructuras(jugador, victimas, rostros, it_rostros, NULL, NULL, NULL);
		return -1;
	}

	iterador_t* it_ciudades = NULL;
	iterador_t* it_mapa = NULL;

	if (cargar_ciudades("ciudades.dat", ciudades) != 0 || !(it_ciudades = lista_iter_crear(ciudades)) || !(it_mapa = lista_iter_crear(ciudades))) {
		destruir_estructuras(jugador, victimas, rostros, it_rostros, ciudades, it_ciudades, it_mapa);
		return -1;
	}

	ciudad_t* ultima_ciudad = lista_ver_ultimo(ciudades);
	printf("Estamos en ciudad X etc...\n");
	int sigo_jugando = 0;

	while (sigo_jugando == 0) {
		sigo_jugando = jugar(jugador, victimas, ciudades, it_ciudades, it_mapa, rostros, it_rostros, ultima_ciudad);
	}

	destruir_estructuras(jugador, victimas, rostros, it_rostros, ciudades, it_ciudades, it_mapa);
	if (sigo_jugando == -1) return -1;
	return 0;
}