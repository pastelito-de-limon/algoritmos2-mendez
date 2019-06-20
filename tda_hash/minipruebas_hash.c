#include "hash.h"
#include <stdio.h>
#include <string.h>

extern char* strdup(const char*);

void destruir_string(void* elemento){
  if(elemento){
    printf("(Destructor) Libero el vehiculo: %s\n", (char*)elemento);
    free(elemento);
  }
}

void guardar_vehiculo(hash_t* garage, const char* patente, const char* descripcion){
  int retorno = guardar_elemento(garage, patente, strdup(descripcion));
  printf("Guardando vehiculo patente %s (%s): ", patente, descripcion);
  printf("%s\n", retorno==0?"OK":"ERROR");
}

void quitar_vehiculo(hash_t* garage, const char* patente){
  int retorno = quitar_elemento(garage, patente);
  printf("Retirando vehiculo patente %s: ", patente);
  printf("%s\n", retorno==0?"OK":"ERROR");
}

void verificar_vehiculo(hash_t* garage, const char* patente, bool deberia_existir){
  printf("Verifico el vehiculo patente %s: ", patente);
  bool retorno = existe_clave(garage, patente);
  printf("%s\n", (retorno==deberia_existir)?"OK":"ERROR");
}

int main(){
  hash_t* garage = crear_hash(destruir_string);

  printf("Agrego autos al garage\n");
  
  guardar_vehiculo(garage, "AC123BD", "Auto de Mariano");
  guardar_vehiculo(garage, "OPQ976", "Auto de Lucas");
  guardar_vehiculo(garage, "A421ACB", "Moto de Manu");
  guardar_vehiculo(garage, "AA442CD", "Auto de Guido");
  guardar_vehiculo(garage, "AC152AD", "Auto de Agustina");
  guardar_vehiculo(garage, "DZE443", "Auto de Jonathan");
  guardar_vehiculo(garage, "AA436BA", "Auto de Gonzalo");
  guardar_vehiculo(garage, "QDM443", "Auto de Daniela");
  guardar_vehiculo(garage, "BD123AC", "Auto de Pablo");
  guardar_vehiculo(garage, "CD442AA", "Auto de Micaela");
  guardar_vehiculo(garage, "PQO697", "Auto de Juan");
  guardar_vehiculo(garage, "DZE443", "Auto de Jonathan otra vez");
  guardar_vehiculo(garage, "AC152AD", "Auto de Agustina otra vez");

  verificar_vehiculo(garage, "QDM443", true);
  verificar_vehiculo(garage, "PQO697", true);

  quitar_vehiculo(garage, "QDM443");
  quitar_vehiculo(garage, "PQO697");

  verificar_vehiculo(garage, "QDM443", false);
  verificar_vehiculo(garage, "PQO697", false);
  
  hash_iter_t* iter = crear_iterador(garage);
  int listados = 0;
  while(avanzar_iterador(iter)){
    listados++;
    const char* clave = elemento_actual(iter);
    printf("Patente: %s -- Vehiculo: %s\n", clave, (char*)obtener_elemento(garage, clave));
  }

  printf("Cantidad de autos guardados: %zu. Cantidad de autos listados: %i -- %s\n", cantidad(garage), listados, (cantidad(garage)==listados)?"OK":"ERROR");
  
  hash_iterador_destruir(iter);
  destruir_hash(garage);
  
  return 0;
}

