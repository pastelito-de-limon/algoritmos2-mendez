#include "pila_vd.h"
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#define CANT_ELEMENTOS 5
#define VOLUMEN 1000

void imprimir_prueba(char* cadena, bool estado) {
	char* cadena_estado = "SI";
	if (!estado) cadena_estado = "NO";
	printf("%s: %s\n", cadena, cadena_estado);
}

bool aux_apilar(pila_t* pila, int argc, void** argv) {
	bool no_hay_errores = true;

	for (int n = 0; n < argc; n++) {
		no_hay_errores &= ((apilar(pila, argv[n]) == 0) & (!vacia(pila)) & (tope(pila) == argv[n]));
	}

	return no_hay_errores;
}

bool aux_desapilar(pila_t* pila, int argc, void** argv) {
	bool no_hay_errores = true;

	for (int n = argc - 1; n >= 0; n--) {
		no_hay_errores &= ((!vacia(pila)) & (tope(pila) == argv[n]) & (desapilar(pila) == 0));
	}

	return no_hay_errores;
}

void pruebas_pila_vacia() {

	printf("---INCIO DE PRUEBAS PILA VACIA---\n");

	pila_t* pila = crear_pila();

	imprimir_prueba("Crear pila vacia", pila != NULL);
	imprimir_prueba("Esta vacia es true", vacia(pila));
	imprimir_prueba("Desapilar es false", desapilar(pila) == 1);
	imprimir_prueba("Ver tope es null", tope(pila) == NULL);
	imprimir_prueba("Destruir pila", destruir_pila(pila) == 0);
}

void pruebas_pila_con_algunos_elementos() {

	printf("---INCIO DE PRUEBAS PILA CON ALGUNOS ELEMENTOS---\n");

	pila_t* pila = crear_pila();

	int a = 1;
	float b = 2;
	double c = 3;
	int d[] = {4};
	void* e = NULL;

	void* elementos[CANT_ELEMENTOS] = {&a, &b, &c, d, e};

	imprimir_prueba("Apilar, ver tope y esta vacia andan bien", aux_apilar(pila, CANT_ELEMENTOS, elementos));
	imprimir_prueba("Desapilar, ver tope y esta vacia andan bien", aux_desapilar(pila, CANT_ELEMENTOS, elementos));

	imprimir_prueba("Pila esta vacia es true", vacia(pila));
	imprimir_prueba("Desapilar es false", desapilar(pila) == 1);
	imprimir_prueba("Ver tope es null", tope(pila) == NULL);
	imprimir_prueba("Destruir pila", destruir_pila(pila) == 0);
}

void pruebas_pila_con_muchos_elementos() {
	
	printf("---INCIO DE PRUEBAS PILA CON MUCHOS ELEMENTOS---\n");

	pila_t* pila = crear_pila();
	void* elementos[VOLUMEN];

		for (int n = 0; n < VOLUMEN; n += 5) {
			int a = 1;
			float b = 2;
			double c = 3;
			int d[] = {4};
			void* e = NULL;

			elementos[n] = &a;
			elementos[n + 1] = &b;
			elementos[n + 2] = &c;
			elementos[n + 3] = d;
			elementos[n + 4] = e;
		}

	bool no_hay_errores = true;
	no_hay_errores &= aux_apilar(pila, VOLUMEN, elementos);
	imprimir_prueba("Apilar, ver tope y esta vacia andan bien", no_hay_errores);
	no_hay_errores = true;
	no_hay_errores &= aux_desapilar(pila, VOLUMEN, elementos);
	imprimir_prueba("Desapilar, ver tope y esta vacia andan bien", no_hay_errores);
	imprimir_prueba("Destruir pila", destruir_pila(pila) == 0);
}

int main(){
	pila_t* pila = crear_pila();
	
	char elemento_1 = '2';
	char elemento_2 = 'o';
	char elemento_3 = 'g';
	char elemento_4 = 'l';
	char elemento_5 = 'A';
	
	for (int i = 0; i < 3; i++) {
		apilar(pila, &elemento_1);
		apilar(pila, &elemento_2);
		apilar(pila, &elemento_3);
		apilar(pila, &elemento_4);
		apilar(pila, &elemento_5);
		printf("Tamanio pila: %i\n", pila->tamanio);
	}
	
	for (int i = 0; i < 5; i++) {
		printf("%c\n", *(char*)tope(pila));
		desapilar(pila);
	}
	printf("Tamanio pila: %i\n", pila->tamanio);
	
	for (int i = 0; i < 5; i++) {
		desapilar(pila);
	}
	printf("Tamanio pila: %i\n", pila->tamanio);
	
	destruir_pila(pila);

	printf("------PRUEBAS DE MICAELA------\n");
	pruebas_pila_vacia();
	pruebas_pila_con_algunos_elementos();
	pruebas_pila_con_muchos_elementos();
	return 0;
}
