#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Disco.h"

#define NameSize 10

void CreateDiscMenu();
void MountDiscMenu();
void FormatDiscMenu();
void DeleteDiscMenu();
void AllocateBlockMenu();
void FreeBlockMenu();
void GetTableMenu();

int main(){

  int op = 0;

  while (op!=8){
    system("cls");
    printf("\n  1. Crear Disco \n  2. Montar Disco \n  3. Formatear Disco");
    printf("\n  4. Eliminar Disco\n  5. Alocar Bloque\n  6. Liberar Bloque");
    printf("\n  7. Ver Tabla\n  8. Salir     > ");
    scanf("%d", &op);
    switch (op) {
        case 1: CreateDiscMenu(); op = 0; break ;
        case 2: MountDiscMenu(); op = 0 ; break;
        case 3: FormatDiscMenu(); op = 0 ; break;
        case 4: DeleteDiscMenu(); op =0 ; break;
        case 5: AllocateBlockMenu(); op = 0; break;
        case 6: FreeBlockMenu(); op = 0; break;
        case 7: GetTableMenu(); op = 0; break;
        case 8: ;
    }
  }
  return 0;
}

void CreateDiscMenu(){
    char fname[NameSize];
    strcpy(fname, "disc1");
    int dsize, bsize;
    //printf("\n   Nombre del Disco (Max. 10 caracteres) > " );
    //scanf("%10s", &fname );
    printf("\n   Tamaño del Disco (MB) > " );
    scanf("%d", &dsize );
    printf("   Tamaño del Bloque (KB) > " );
    scanf("%d", &bsize );

    int ferror = CreateDisc(fname, dsize, bsize);
    if (ferror != 0)
        printMsg(" El disco ya existe o no se completo su formato.");
    else
        printMsg(" El Disco fue creado.");
}

void MountDiscMenu(){
  MountDisc();
}

void FormatDiscMenu(){
  char fname[NameSize];
  printf("\n FORMAT - Nombre del Disco (Max. 10 caracteres) > " );
  scanf("%10s", &fname );
  int form = FormatDisc(fname);
  if (form != 0 ) //Error
    printMsg("Formato Incompleto...");
}

void DeleteDiscMenu(){
  char fname[NameSize];
  printf("\n ELIMINAR -  Nombre del Disco (Max. 10 caracteres) > " );
  scanf("%10s", &fname );
  int form = DeleteDisc(fname);
  if (form != 0 ) //Error
    printMsg("Disco NO fue eliminado...");
  else
    printMsg("Disco fue eliminado exitosamente...");
}

void AllocateBlockMenu(){
  int n;
  printf("\n   Cantidad de Bloques > " );
  scanf("%d", &n );
  AllocateBlocks(n);
}

void FreeBlockMenu(){
  int n;
  printf("\n   Posicion de Bloque > " );
  scanf("%d", &n );
  FreeBlock(n);
}

void GetTableMenu(){
  getTable();
  getch();
}
