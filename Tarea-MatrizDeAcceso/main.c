#include <stdio.h>
#include <stdlib.h>
#include "matriz.h"

#define NameSize 10

int nDominios; // Cantidad de dominios
int nObjetos; // Cantidad de objetos

void mCrearMatriz();


int main(){

  int op = 0;
  while (op!=8){
    system("cls");
    printf("\n  1. Crear Matriz \n  8. Salir     > ");
    scanf("%d", &op);
    switch (op) {
        case 1: mCrearMatriz(); op = 0; break ;
        case 8: ;
    }
  }
  return 0;
}

void mCrearMatriz(){
    //char fname[NameSize] = "disc1";
    //printf("\n   Nombre del Disco (Max. 10 caracteres) > " );
    //scanf("%10s", &fname );
    printf("\n   Cantidad de Dominios > " );
    scanf("%d", &nDominios );
    printf("   Cantidad de Objetos > " );
    scanf("%d", &nObjetos );
    CrearMatriz();
}
