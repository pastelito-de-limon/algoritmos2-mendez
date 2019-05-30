#include "venganza_arya.h"
#include "lista.h"
#include "pila.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#define CANT_OPCIONES 5

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

void destruir_pila_y_elementos(pila_t* pila) {
	while (!pila_esta_vacia(pila)) {
		free(pila_desapilar(pila));
	}
	
	pila_destruir(victimas);
}

void destruir_estructuras(jugador_t* jugador, pila_t* victimas, iterador_t* it_rostros, lista_t* ciudades, iterador_t* it_ciudades) {
	if (it_rostros) {
		lista_destruir(rostros, (void)free(void*));
		lista_iter_destruir(it_rostros);
	}

	if (ciudades) lista_destruir(ciudades, (void)free(void*));
	if (it_ciudades) lista_iter_destruir(it_ciudades);
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

/* CORRECTO: 0, INCORRECTO: -1, BASURA: 1 */
int usuario_menú(char entrada, jugador_t* jugador, pila_t* victimas, iterador_t* it_ciudades, iterador_t* it_rostros) {
	if (entrada == 'I' || entrada == 'i') return dar_datos_persona(jugador, it_ciudades);
	if (entrada == 'M' || entrada == 'm') return mostrar_mapa(iterador_t* it_ciudades);
	if (entrada == 'A' || entrada == 'a') return avanzar_mapa(iterador_t* it_ciudades);
	else if (entrada == 'P' || entrada == 'p') return mostrar_proxima_victima(victimas);
	else if (entrada == 'C' || entrada == 'c') return mostrar_rostros_recolectados(iterador_t* it_rostros);
	return 1;
}

/* SEGUIR: 0, GANAR/PERDER = 1, ERROR: -1 */
int jugar(jugador_t* jugador, pila_t* victimas, iterador_t* it_ciudades, iterador_t* it_rostros, ciudad_t* ultima_ciudad) {
	char opcion[2];

	while (true) { //FALTAN CONDICIONES DE CORTE
		printf("[Menu]\n");
		fgets(opcion, 2, stdin);
		int elegido = usuario_menu(opcion[0], jugador, victimas, it_ciudades, it_rostros);
		
		if (elegido > 0) {
			printf("Escribí bien\n");
			modificar_vida(jugador, DECREMENTO_VIDA);
		} else if (elegido < 0) {
			perror("Mensaje de error");
			return -1;
		}

		//A partir de ahora es cuando elegido == 0
		//Condiciones de corte de ganar/perder y seguir
		if (chequear_fin_juego(victimas, jugador, it_ciudades, ultima_ciudad) == 0) return 0;
		return 1;
	}
}

int main(char** argv, int argc) {
	printf("Intro"); 
	jugador_t* jugador = nuevo_jugador();
	
	if (!jugador) {
		perror("Mensaje de error");
		return -1;
	}

	pila_t* victimas = dar_pila_victimas(jugador);
	iterador_t* it_rostros = lista_iter_crear(dar_lista_rostros(jugador));

	if (!it_rostros) {
		destruir_estructuras(jugador, victimas, it_rostros, NULL, NULL);
		perror("Mensaje de error");
		return -1;
	}

	lista_t* ciudades = lista_crear();

	if (!ciudades) {
		destruir_estructuras(jugador, victimas, it_rostros, ciudades, NULL);
		perror("Mensaje de error");
		return -1;
	}

	iterador_t* it_ciudades = NULL;

	if (cargar_ciudades("ciudades.dat", ciudades) != 0 || !(it_ciudades = lista_iter_crear(ciudades))) {
		destruir_estructuras(jugador, victimas, it_rostros, ciudades, it_ciudades);
		perror("Mensaje de error");
		return -1;
	}

	ciudad_t* ultima_ciudad = lista_ver_ultimo(ciudades);

	printf("Estamos en ciudad X etc...\n")
	int sigo_jugando = 0;

	while (sigo_jugando == 0) {
		sigo_jugando = jugar(jugador, victimas, it_rostros, it_ciudades, ultima_ciudad);
	}

	destruir_estructuras(jugador, victimas, it_rostros, ciudades, it_ciudades);
	if (sigo_jugando == -1) return -1;
	return 0;
}