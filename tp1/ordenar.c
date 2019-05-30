#include "ordenar.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void swap(void** vector, size_t i, size_t j) {
	void* aux = vector[i];
	vector[i] = vector[j];
	vector[j] = aux;
}

size_t particion(void** vector, size_t inicio, size_t fin, comparar_t comparar) {
	size_t pivote = inicio;
	size_t pared = inicio;

	for (size_t n = inicio + 1; n <= fin; n++) {
		if (comparar(vector[n], vector[pivote]) < 0) {
			pared++;
			if (n != pared) swap(vector, n, pared);
		}
	}

	if (inicio != pared) swap(vector, pivote, pared);
	return pared;
}

size_t particion_aleatoria(void** vector, size_t inicio, size_t fin, comparar_t comparar) {
	size_t pivote = rand() % (fin - inicio) + inicio;
	swap(vector, pivote, inicio);
	return particion(vector, inicio, fin, comparar);
}

void quicksort(void** vector, size_t inicio, size_t fin, comparar_t comparar) {
	if (inicio < fin) {
		size_t particion = particion_aleatoria(vector, inicio, fin, comparar);
		quicksort(vector, inicio, particion, comparar);
		quicksort(vector, particion + 1, fin, comparar);
	}
}

void ordenar(void** vector, size_t n, comparar_t comparar) {
	srand ((unsigned int)time(NULL));
	quicksort(vector, 0, n - 1, comparar);
}