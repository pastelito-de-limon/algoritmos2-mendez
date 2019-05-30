#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_BOLD          "\x1b[1m"
#define ANSI_RESET         "\x1b[0m"

typedef struct persona {
  char nombre[50];
  char descripcion[500];
  char msj_muerte[100];
  char msj_perdon[100];
  int en_lista;    // 1 si está, 0 en caso contrario.
  int culpable;    // 1 si lo es, 0 en caso contrario.
  int danio;       // daño proporcionado por una persona inocente.
  int beneficio;   // 1 aumenta vida, 2 elimina víctima , 3 llave del castillo.
} persona_t;

typedef struct ciudad {
  char nombre[50];
  persona_t posible_victima;
}ciudad_t;

void leer_string(const char* prompt, void* variable, int max_size){
  bzero(variable, max_size);
  printf(ANSI_COLOR_MAGENTA ANSI_BOLD "%s" ANSI_RESET, prompt);
  fgets((char*)variable, max_size, stdin);
  if(!isatty(0)) printf("%s",variable);
 
}

void leer_bool(const char* prompt, int &variable){
  printf(ANSI_COLOR_MAGENTA ANSI_BOLD "%s" ANSI_RESET, prompt);
  char buffer[20];
  fgets(buffer, 20, stdin);
  variable = atoi(buffer);
  if(!isatty(0)) printf("%i\n", variable);
}

// Lee una ciudad por entrada estandar
ciudad_t leer_ciudad(){

  ciudad_t ciudad;

  leer_string("Nombre de la ciudad: ", ciudad.nombre, 50);  
    
  leer_string("Nombre de la persona en esta ciudad: ", ciudad.posible_victima.nombre, 50);  

  leer_string("Descripción de la persona: ", ciudad.posible_victima.descripcion, 500);  

  leer_string("Mensaje de muerte de la persona: ", ciudad.posible_victima.msj_muerte, 100);

  leer_string("Mensaje de perdon de la persona: ", ciudad.posible_victima.msj_perdon, 100);

  leer_bool("¿Está en la lista? (1/0): ", ciudad.posible_victima.en_lista);

  leer_bool("¿Es culpable? (1/0): ", ciudad.posible_victima.culpable);

  leer_bool("¿Cuanto daño hace si es inocente? (>0): ", ciudad.posible_victima.danio);

  leer_bool("Beneficio de matarlo si es culpable (1,2,3): ", ciudad.posible_victima.beneficio);

  return ciudad;
}

void cargar_datos (const char* nombre_archivo){
  FILE * archivo;
  archivo = fopen (nombre_archivo , "w+");
  if (archivo == NULL){
    perror("No se pudo crear el archivo de salida");
    return;
  }

  ciudad_t ciudad;
  char c[5];
  
  do{
    ciudad = leer_ciudad();
    fwrite ((&ciudad) , sizeof(ciudad_t) , 1 , archivo);

    printf(ANSI_COLOR_RED ANSI_BOLD "¿Desea cargar otra ciudad? (S/N)" ANSI_RESET);
    if(!isatty(0)) printf("\n");
    printf("\n");
  }while(fgets(c, 5, stdin) && c[0] != 'n' && c[0] != 'N');

  fclose(archivo);
}

int main (){
  cargar_datos("ciudades.dat");
  return 0;
}
