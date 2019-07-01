#define _GNU_SOURCE
#include "venganza_arya.h"
#include "lista.h"
#include "pila.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#define TOTAL_VIDA 100
#define TAM_ENTRADA_USUARIO 3
#define DESCUENTO_VIDA -20
#define AUMENTO_VIDA 5

jugador_t* nuevo_jugador() {
	jugador_t* jugador = malloc(sizeof(jugador_t));
	if (!jugador) return NULL;
	jugador->vida = TOTAL_VIDA;
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

int cargar_ciudades(char archivo[MAX_NOMBRE], lista_t* ciudades) {
	FILE* archivo_ciudades = fopen(archivo, "r");
	if (!archivo_ciudades) return -1;
	size_t tamanio = sizeof(ciudad_t);

	while(!feof(archivo_ciudades)){
		ciudad_t* ciudad = malloc(tamanio);

		if (!ciudad) {
			fclose(archivo_ciudades);
			return -1;
		}

		fread(ciudad, tamanio, 1, archivo_ciudades);
		
		if ((strcmp(ciudad->nombre, "") == 0) || (strcmp(ciudad->nombre, " ") == 0)) {
			free(ciudad);
			break;
		}

		if (!lista_insertar_ultimo(ciudades, ciudad)) {
			free(ciudad);
			fclose(archivo_ciudades);
			return -1;
		}	
	}

	fclose(archivo_ciudades);
	return 0;
}

int cargar_victimas(char archivo[MAX_NOMBRE], pila_t* victimas) {
	FILE* archivo_victimas = fopen(archivo, "r");
	if (!archivo_victimas) return -1;
	char* linea = NULL;
	ssize_t leido;
	size_t tam;
	char* victima;

	while((leido = getline(&linea, &tam, archivo_victimas)) != -1){

		victima = malloc(sizeof(char) * ((size_t)leido + 1));
		
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

void mostrar_mapa(lista_t* ciudades, iterador_t* it_mapa, int* ciudad_numero) { 
	for (int i = 0; i < (*ciudad_numero); i++) {
		lista_iter_avanzar(it_mapa);
	}

	char* ciudad = (char*)lista_iter_ver_actual(it_mapa);
	printf("Ciudad actual: \n%s\n\n", ciudad);
	lista_iter_avanzar(it_mapa);
	if (lista_iter_al_final(it_mapa)) return;
	printf("Mapa:\n");

	while (!lista_iter_al_final(it_mapa)) {
		ciudad = (char*)lista_iter_ver_actual(it_mapa);
		printf("%s\n", ciudad);
		lista_iter_avanzar(it_mapa);
	}

	lista_iter_destruir(it_mapa);
	it_mapa = lista_iter_crear(ciudades);
	printf("\n");
}

int mostrar_rostros_recolectados(lista_t* rostros, iterador_t* it_rostros) {
	if (lista_iter_al_final(it_rostros)) {
		printf("Aún no tenés rostros.\n\n");
		return 0;
	}

	char* rostro;

	while (!lista_iter_al_final(it_rostros)) {
		rostro =  (char*)lista_iter_ver_actual(it_rostros);
		printf("%s\n", rostro);
		lista_iter_avanzar(it_rostros);
	}

	lista_iter_destruir(it_rostros);
	it_rostros = lista_iter_crear(rostros);
	printf("\n");
	return 0;
}

int mostrar_proxima_victima(pila_t* victimas) {
	if (!pila_ver_tope(victimas)) return -1;
	char* proxima_victima = (char*)pila_ver_tope(victimas);
	printf("La próxima vez que sangre sea derramada,\nTendrá %s una muerte desalmada.\n\n", proxima_victima);
	return 0;
}

int ver_vida(jugador_t* jugador) {
	return jugador->vida;
}

void modificar_vida(jugador_t* jugador, int cantidad) {
	jugador->vida += cantidad;
	if (jugador->vida < 0) jugador->vida = 0;
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

int avanzar_mapa(jugador_t* jugador, iterador_t* it_ciudades, ciudad_t* ultima_ciudad, int* ciudad_numero) {
	ciudad_t* ciudad_previa = lista_iter_ver_actual(it_ciudades);
	
	if (strcmp(ciudad_previa->nombre, ultima_ciudad->nombre) == 0) {
		printf("¡Ya te encuentras al final del camino!\n\n");
		return 0;
	}

	bool victima_esta_viva = (strcmp((char*)pila_ver_tope(jugador->victimas), ciudad_previa->posible_victima.nombre) == 0);
	
	if (victima_esta_viva) {
		modificar_vida(jugador, -TOTAL_VIDA);
		printf("Un corazón muy puro\nTe ha cegado en tu camino\nA un rufián de tu lista\nVivir le has permitido.\n\n");
		return 0;
	}

	lista_iter_avanzar(it_ciudades);
	if (lista_iter_al_final(it_ciudades)) return -1;
	ciudad_t* ciudad_actual = lista_iter_ver_actual(it_ciudades);
	printf("Haz llegado de %s a tu pŕoximo destino.\n¿Habrá de ser %s tu triufar o tu castigo?\n\n", ciudad_previa->nombre, ciudad_actual->nombre);	
	(*ciudad_numero)++;
	return 0;
}

void matar_victima(jugador_t* jugador, iterador_t* it_rostros, char* rostro) {
	free(pila_desapilar(jugador->victimas));
	lista_iter_insertar(it_rostros, rostro);
}

int eliminar_ciudad(jugador_t* jugador, lista_t* ciudades, iterador_t* it_rostros) {
	iterador_t* aux = lista_iter_crear(ciudades);
	if (!aux) return -1;
	
	while (!lista_iter_al_final(aux)) {
		if (strcmp((char*)pila_ver_tope(jugador->victimas), ((ciudad_t*)lista_iter_ver_actual(aux))->posible_victima.nombre) == 0) {
			break;
		}

		lista_iter_avanzar(aux);
	}
	
	persona_t persona = ((ciudad_t*)lista_iter_ver_actual(aux))->posible_victima;
	char* rostro = malloc(sizeof(char) * strlen(persona.nombre));
	if (!rostro) return -1;
	strcpy(rostro, persona.nombre);
	matar_victima(jugador, it_rostros, rostro);
	ciudad_t* borrado = lista_iter_borrar(aux);
	free(aux);
	if (!borrado) return -1;
	return 0;
}

int actualizar_juego(jugador_t* jugador, persona_t persona, lista_t* ciudades, iterador_t* it_rostros) {
	if (persona.en_lista == 1) {
		if (strcmp((char*)pila_ver_tope(jugador->victimas), persona.nombre) != 0) { 
			jugador->vida = 0;
			return -1;
		}
		
		char* rostro = malloc(sizeof(char) * strlen(persona.nombre));
		if (!rostro) return -1;
		strcpy(rostro, persona.nombre);
		matar_victima(jugador, it_rostros, rostro);
		printf("Un nuevo rufián destruído,\nOtra escoria finalmente derrotada.\nY al haber su maldad detenido\nPuedes seguir tu ruta planeada...\n\n");
		return 0;
	}

	if (persona.culpable == 1) {
		printf("Al ir más allá del esfuerzo necesario,\nLibrando al mundo de un mal no sabido,\nLos dioses han de tenerte en su gloria\nDándote un premio bien merecido...\n\n");
		if (persona.beneficio == 1) modificar_vida(jugador, AUMENTO_VIDA);
		else if (persona.beneficio == 2) return eliminar_ciudad(jugador, ciudades, it_rostros);
		else jugador->posee_llave = true;
	} else {
		printf("¡Haz cubierto de sangre a un inocente!\nTendrán los dioses crueldad con tu suerte...\n\n");
		modificar_vida(jugador, -persona.danio);
	}

	return 0;
}

int dar_datos_persona(jugador_t* jugador, lista_t* ciudades, iterador_t* it_ciudades, iterador_t* it_rostros, ciudad_t* ultima_ciudad, int* ciudad_numero) {
	persona_t persona = ((ciudad_t*)lista_iter_ver_actual(it_ciudades))->posible_victima;
	printf("%s\n\n", persona.descripcion);
	char opcion[TAM_ENTRADA_USUARIO];
	printf("¿Haz de tomar en tus manos a esta persona desconocida\nY proclamar frente a los dioses el fin de su vida?\n(S/N):\n\n");
	fgets(opcion, TAM_ENTRADA_USUARIO, stdin);
	printf("\n");
	int aceptado = usuario_acepta(opcion[0]);

	while(aceptado > 0) {
		printf("La prisa transforma al sabio nuevamente\nEn el más bestial analfabeta.\nSigue errando de esta forma\nY verás cómo la soga aprieta.\n\n");
		modificar_vida(jugador, DESCUENTO_VIDA);
		printf("¿Haz de tomar en tus manos a esta persona desconocida\nY proclamar frente a los dioses el fin de su vida?\n(S/N):\n\n");
		fgets(opcion, TAM_ENTRADA_USUARIO, stdin);
		printf("\n");
		aceptado = usuario_acepta(opcion[0]);
	}

	if (aceptado < 0) {
		printf("La misericordia es hoy tu virtud.\nTe saluda %s en gratitud:\n'%s'\n\n", persona.nombre, persona.msj_perdon);
		return 0;
	}

	printf("%s susurra antes de perecer\nPalabras que atormentarán tu ser:\n'%s'\n\n",persona.nombre, persona.msj_muerte);
	
	/*Si la persona es culpable y su beneficio es matar a la próxima víctima, 
	pero la próxima víctima es la última que hay entonces cambio el beneficio*/
	bool caso_borde = (persona.culpable == 1) && (persona.beneficio == 2) && (strcmp(pila_ver_tope(jugador->victimas), ultima_ciudad->posible_victima.nombre) == 0);
	if (caso_borde) persona.beneficio = 1;
	if (actualizar_juego(jugador, persona, ciudades, it_rostros) != 0) return -1;

	if (strcmp(((ciudad_t*)lista_iter_ver_actual(it_ciudades))->nombre, ultima_ciudad->nombre) != 0) {
		avanzar_mapa(jugador, it_ciudades, ultima_ciudad, ciudad_numero);
	}

	return 0;
}
