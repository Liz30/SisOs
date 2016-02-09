#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Disco.h"

#define NameSize 10

void CreateDiscMenu();
void MountDiscMenu();
void FormatDiscMenu();
void DeleteDiscMenu();
void UmountDiscMenu();
void AllocateBlockMenu();
void FreeBlockMenu();
void GetTableMenu();

int main(){

  int op = 1;

  while (op != 0){
    system("cls");
    printf("\n  1. Crear Disco \n  2. Montar Disco \n  3. Formatear Disco");
    printf("\n  4. Eliminar Disco\n  5. Desmontar Disco\n  6. Propiedades del Disco\n  7. Alocar Bloque\n  8. Liberar Bloque");
    printf("\n  9. Ver Tabla\n  0. Salir               > ");
    scanf("%d", &op);
    switch (op) {
        case 1: CreateDiscMenu(); break ;
        case 2: MountDiscMenu(); break;
        case 3: FormatDiscMenu(); break;
        case 4: DeleteDiscMenu(); break;
        case 5: UmountDiscMenu(); break;
        case 6: getInfo(); break;
        case 7: AllocateBlockMenu(); break;
        case 8: FreeBlockMenu(); break;
        case 9: GetTableMenu(); break;
        case 0: UmountDiscMenu(); system("cls"); break;
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
    printf("\n   Tamaño del Disco (Bytes) > " );
    scanf("%d", &dsize );
    printf("   Tamaño del Bloque (Bytes) > " );
    scanf("%d", &bsize );

    int ferror = CreateDisc(fname, dsize, bsize);
    if (ferror == -1)
        printMsg(" El disco ya existe.");
    if (ferror == -2)
        printMsg(" No se completo su formato.");
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

void UmountDiscMenu(){
  UmountDisc();
}

void FreeBlockMenu(){
  int n;
  printf("\n   Posicion de Bloque > " );
  scanf("%d", &n );
  FreeBlock(n);
}

void GetTableMenu(){
  getTable();
}
