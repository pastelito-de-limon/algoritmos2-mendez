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
Menú:
	I: Información. 	Datos de persona en ciudad actual.
	M: Mapa. 			Nombra ciudad actual y restantes en orden.
	A: Avanzar. 		Avanza a la próxima ciudad.
	P: Próximo. 		Imprime próxima víctima.
	C: Colección. 		Imprime todos los rostros actuales.
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

/* GANAR/PERDER: 1, SEGUIR: 0 */

int chequear_fin_juego(pila_t* victimas, jugador_t* jugador, iterador_t* it_ciudades, ciudad_t* ultima_ciudad) {
	if (pila_esta_vacia(victimas)) {
		printf("Has aniquilado a tus enemigos finalmente\nY han quedado sus almas en lamento...\n¿Pero podrás alcanzar la victoria nuevamente\nEn tu valiente pŕoximo intento?\n\nFIN DEL JUEGO.\n");
		return 1;
	}
	
	bool estoy_al_final_sin_llave = (strcmp(((ciudad_t*)lista_iter_ver_actual(it_ciudades))->nombre, ultima_ciudad->nombre) == 0) && (!tengo_llave(jugador));

	if (estoy_al_final_sin_llave) printf("Si tan sólo hubieses aniquilado\nA quien tenía las llaves del último paso\nNo estarías acá inmobilizado\nTerminando en el total fracaso.\n\n");

	if (ver_vida(jugador) < 1 || estoy_al_final_sin_llave) {
		printf("La victoria tan cercana parecía\nY el final ha llegado prematuramente\nNo era más que sólo fantasía\nY es ahora cuando enfrentás a tu muerte.\n\nFIN DEL JUEGO.\n");
		return 1;
	}

	return 0;
}

/* CORRECTO: 0, INCORRECTO: -1, BASURA: 1 */
int usuario_menu(char entrada, jugador_t* jugador, pila_t* victimas, lista_t* ciudades, iterador_t* it_ciudades, iterador_t* it_mapa, lista_t* rostros, iterador_t* it_rostros, ciudad_t* ultima_ciudad, int* ciudad_numero) {
	if (entrada == 'I' || entrada == 'i') return dar_datos_persona(jugador, ciudades, it_ciudades, it_rostros, ultima_ciudad, ciudad_numero);
	
	if (entrada == 'M' || entrada == 'm') {
		mostrar_mapa(ciudades, it_mapa, ciudad_numero);
		return 0;
	}

	if (entrada == 'A' || entrada == 'a') return avanzar_mapa(jugador, it_ciudades, ultima_ciudad, ciudad_numero);
	if (entrada == 'P' || entrada == 'p') return mostrar_proxima_victima(victimas);
	if (entrada == 'C' || entrada == 'c') return mostrar_rostros_recolectados(rostros, it_rostros);
	return 1;
}

/* SEGUIR: 0, GANAR/PERDER = 1, ERROR: -1 */
int jugar(jugador_t* jugador, pila_t* victimas, lista_t* ciudades, iterador_t* it_ciudades, iterador_t* it_mapa, lista_t* rostros, iterador_t* it_rostros, ciudad_t* ultima_ciudad, int* ciudad_numero) {
	char opcion[TAM_ENTRADA_USUARIO];
	printf("Ingresá una letra:\nI: Información (Sobre la persona en %s).\nM: Mapa (Ver todas las ciudades restantes).\nA: Avanzar (Ir a la próxima ciudad).\nP: Próximo (Ver próxima víctima en tu lista).\nC: Colección (Ver los rostros recolectados hasta ahora).\n\n", ((ciudad_t*)lista_iter_ver_actual(it_ciudades))->nombre);
	fgets(opcion, TAM_ENTRADA_USUARIO, stdin);
	printf("\n");
	int elegido = usuario_menu(opcion[0], jugador, victimas, ciudades, it_ciudades, it_mapa, rostros, it_rostros, ultima_ciudad, ciudad_numero);

	while (elegido > 0) {
		printf("La prisa transforma al sabio nuevamente\nEn el más bestial analfabeta.\nSigue errando de esta forma\nY verás cómo la soga aprieta.\n\n");
		modificar_vida(jugador, DESCUENTO_VIDA);
		printf("--- Vida: %i, Llave: %s ---\n\n", ver_vida(jugador), tengo_llave(jugador)? "Sí":"No");
		if (chequear_fin_juego(victimas, jugador, it_ciudades, ultima_ciudad) > 0) return 1;
		fgets(opcion, TAM_ENTRADA_USUARIO, stdin);
		printf("\n");
		elegido = usuario_menu(opcion[0], jugador, victimas, ciudades, it_ciudades, it_mapa, rostros, it_rostros, ultima_ciudad, ciudad_numero);		
	}

	if (elegido < 0) return -1;
	printf("--- Vida: %i, Llave: %s ---\n\n", ver_vida(jugador), tengo_llave(jugador)? "Sí":"No");
	if (chequear_fin_juego(victimas, jugador, it_ciudades, ultima_ciudad) == 0) return 0;
	return 1;
}

int main(int argc, char** argv) {
	printf("__     ___     __ __ _____ __  __  ____   ___  __  __ ____   ___\n||    ||=||    \\\\ // ||==  ||\\\\|| (( ___ ||=|| ||\\\\||   //  ||=||\n||__| || ||     \\V/  ||___ || \\||  \\\\_|| || || || \\||  //__ || ||\n"); 
	printf("_____ _____     ___  _____ _  _  ___\n||  ) ||==     ||=|| ||_// \\\\// ||=||\n||_// ||___    || || || \\\\  //  || ||\n\n");
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
	int* ciudad_numero = malloc(sizeof(int));
	
	if (!ciudad_numero) {
		destruir_estructuras(jugador, victimas, rostros, it_rostros, ciudades, it_ciudades, it_mapa);
		return -1;
	}

	(*ciudad_numero) = 0; 
	printf("En %s tu destino comienza finalmente.\nCon cautela haz de atacar, con el uso de tu mente.\n\n", ((ciudad_t*)lista_iter_ver_actual(it_ciudades))->nombre);
	printf("--- Vida: %i, Llave: %s ---\n\n", ver_vida(jugador), tengo_llave(jugador)? "Sí":"No");
	int sigo_jugando = 0;

	while (sigo_jugando == 0) {
		sigo_jugando = jugar(jugador, victimas, ciudades, it_ciudades, it_mapa, rostros, it_rostros, ultima_ciudad, ciudad_numero);
	}

	destruir_estructuras(jugador, victimas, rostros, it_rostros, ciudades, it_ciudades, it_mapa);
	free(ciudad_numero);
	
	if (sigo_jugando == -1) {
		printf("Hubo un error en la ejecución del juego.\n");
		return -1;
	}
	
	return 0;
}
