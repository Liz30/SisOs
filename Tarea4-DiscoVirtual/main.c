#include <stdio.h>
#include <stdlib.h>
#include "Disco.h"

#define NameSize 10

void CreateDiscMenu();
void MountDiscMenu();
void FormatDiscMenu();
void EliminarDiscoMenu();

int main(){

  int op = 0;
  while (op!=5){
    system("cls");
    printf("\n  1. Crear Disco \n  2. Montar Disco \n  3. Formatear Disco\n  4. Eliminar Disco\n  5. Salir     > ");
    scanf("%d", &op);
    switch (op) {
        case 1: CreateDiscMenu(); op = 0; break ;
        case 2: MountDiscMenu(); op = 0 ; break;
        case 3: FormatDiscMenu(); op = 0 ; break;
        case 4: EliminarDiscoMenu(); op =0 ; break;
        case 5: ;
    }
  }
  return 0;
}

void CreateDiscMenu(){
    char fname[NameSize] = "disc1";
    int dsize, bsize;
    //printf("\n   Nombre del Disco (Max. 10 caracteres) > " );
    //scanf("%10s", &fname );
    printf("\n   Tamaño del Disco (MB) > " );
    scanf("%d", &dsize );
    printf("   Tamaño del Bloque (KB) > " );
    scanf("%d", &bsize );

    int ferror = CreateDisc(fname, dsize, bsize);
    if (ferror != 0)
        printMsg(" El disco ya existe o no se completo su format.");
    else
        printMsg(" El Disco fue creado. ");
}

void MountDiscMenu(){

  /*   fp=fopen("archivo.txt","r");
     if(fp==NULL)
     {
          printf("Error al abrir el archivo para leer");
          exit(1);
     }
     fclose(fp);*/
}

void FormatDiscMenu(){
  char fname[NameSize];
  printf("\n FORMAT - Nombre del Disco (Max. 10 caracteres) > " );
  scanf("%10s", &fname );
  int form = FormatDisc(fname);
  if (form != 0 ) //Error
    printMsg("Formato Incompleto...");
}

void EliminarDiscoMenu(){
  char fname[NameSize];
  printf("\n ELIMINAR -  Nombre del Disco (Max. 10 caracteres) > " );
  scanf("%10s", &fname );
  int form = DeleteDisc(fname);
  if (form != 0 ) //Error
    printMsg("Disco NO fue eliminado...");
  printMsg("Disco fue eliminado exitosamente...");
}
