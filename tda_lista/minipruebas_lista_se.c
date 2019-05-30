#include "lista_se.h"
#include <stdio.h>

int main(){
	lista_se_t* lista = crear_lista();

	char elemento_1 = 'A';
	char elemento_2 = 'l';
	char elemento_3 = 'g';
	char elemento_4 = 'o';
	char elemento_5 = '2';

	for (int i = 0; i < 3; i++) {
		insertar(lista, &elemento_1);
		insertar(lista, &elemento_2);
		insertar(lista, &elemento_3);
		insertar(lista, &elemento_4);
		insertar(lista, &elemento_5);
	}

	insertar_en_posicion(lista, &elemento_1, 0);
	insertar_en_posicion(lista, &elemento_2, 2);
	insertar_en_posicion(lista, &elemento_3, 4);
	insertar_en_posicion(lista, &elemento_4, 6);
	insertar_en_posicion(lista, &elemento_5, 8);

	for (int i = 0; i < 10; i+=2) {
		printf("%c\n", *(char*)elemento_en_posicion(lista, i));
	}

	for (int i = 0; i < 10; i+=2) {
		borrar_de_posicion(lista, i);
	}

	destruir_lista(lista);

	printf("------INCIO DE PRUEBAS DE MICAELA------\n");
	pruebas_micaela();	
	return 0;
}