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

  while (op!=9){
    system("cls");
    printf("\n  1. Crear Disco \n  2. Montar Disco \n  3. Formatear Disco");
    printf("\n  4. Eliminar Disco\n  5. Propiedades del Disco\n  6. Alocar Bloque\n  7. Liberar Bloque");
    printf("\n  8. Ver Tabla\n  9. Salir     > ");
    scanf("%d", &op);
    switch (op) {
        case 1: CreateDiscMenu(); op = 0; break ;
        case 2: MountDiscMenu(); op = 0 ; break;
        case 3: FormatDiscMenu(); op = 0 ; break;
        case 4: DeleteDiscMenu(); op =0 ; break;
        case 5: getInfo(); op = 0; break;
        case 6: AllocateBlockMenu(); op = 0; break;
        case 7: FreeBlockMenu(); op = 0; break;
        case 8: GetTableMenu(); op = 0; break;
        case 9: ;
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
