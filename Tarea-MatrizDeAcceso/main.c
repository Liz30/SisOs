#include <stdio.h>
#include <stdlib.h>
#include "matriz.h"

#define NameSize 10

void CrearMatriz();


int main(){

  int op = 0;
  while (op!=8){
    system("cls");
    printf("\n  1. Crear Matriz \n  2. Montar Disco \n  3. Formatear Disco");
    printf("\n  4. Eliminar Disco\n  5. Alocar Bloque\n  6. Liberar Bloque");
    printf("\n  7. Ver Tabla\n  8. Salir     > ");
    scanf("%d", &op);
    switch (op) {
        case 1: CrearMatriz(); op = 0; break ;
        case 8: ;
    }
  }
  return 0;
}

void CrearMatriz(){
    char fname[NameSize] = "disc1";
    int dcant, ocant;
    //printf("\n   Nombre del Disco (Max. 10 caracteres) > " );
    //scanf("%10s", &fname );
    printf("\n   Cantidad de Dominios > " );
    scanf("%d", &dcant );
    printf("   Cantidad de Objetos > " );
    scanf("%d", &ocant );
}
