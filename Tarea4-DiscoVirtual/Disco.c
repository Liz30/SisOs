#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "Disco.h"

#define cant_bytes 1024

FILE *f;
Disco nDisco;
extern NameSize;

unsigned long discbytes;
int blockbytes, fatSize_bytes, reserved_bytes, reserved_blocks_int;
float references;


void printMsg(char* e){
    printf("%s   ", e);
    //system("pause");
    getch();
}

void getInfo(){
  if (strlen(nDisco.header.Name) == 0)
    printMsg("Disco no encontrado");
  else{
    printf("\n\n Nombre del Disco: %s\n", nDisco.header.Name);
    printf(" Tamaño de Disco: %d MB\n", nDisco.header.DiscSize);
    printf(" Tamaño de Bloque: %d KB\n", nDisco.header.BlockSize);
    printf(" Flag Mount: %c\n", nDisco.header.Flag);
    printf(" Formato: " );
    if(nDisco.header.MagicNumber == 0)
        printf("No\n");
    else
        printf("Si\n");
  //  printf(" Primer Bloque Libre: %d\n", nDisco.header.FirstFree);

    printf("\n\n  Disco      Bloques       FAT        Header     Bloques\n");
    printf("                                                Reservados");
    printf("\n %d      %d          %d        %d        %d\n", discbytes, nDisco.header.ftable.nBlocks, fatSize_bytes, reserved_bytes, reserved_blocks_int);

    getch();
  }
}

int DeleteDisc(char * path){
  int r = remove(path);
  if (r == 0)
    return 0;
    // Inicializar variables
  return -1;
}

int CreateDisc(char* name, unsigned long dsize, int bsize){
    f = fopen(name, "r");
    if (f == NULL){  // No existe
      f = fopen(name, "a+");
      fclose(f);
      strcpy(nDisco.header.Name, name);
      nDisco.header.DiscSize = dsize;
      nDisco.header.BlockSize = bsize;
      nDisco.header.Flag = 'U';
      nDisco.header.MagicNumber = 0;
      nDisco.header.FirstFree = -1;
      if ( FormatDisc(name) != 0 )
          return -1;
      return 0;
    }
    fclose(f);
    return -1;
}

void MountDisc(){
  // Leer el arhivo
  // si Flag == M
      // Disco ya esta montado.
  // sino
      // Leer el header
  nDisco.header.Flag = 'M';
    /*   fp=fopen("archivo.txt","r");
       if(fp==NULL)
       {
            printf("Error al abrir el archivo para leer");
            exit(1);
       }
       fclose(fp);*/
}

int FormatDisc(char *path){
  f = fopen(path, "r+");
  if (f == NULL){ // Validar que existe.
      printMsg(strcat(path, " no existe. "));
      fclose(f);
      return -1;
  }
  fclose(f);

  if (nDisco.header.Flag == 'M'){ // Validar que esta desmontado.
      printMsg("Asegurese que el disco este desmontado");
      return -1;
  }

  strcpy(nDisco.header.Name, path);
  blockbytes = (nDisco.header.BlockSize * cant_bytes);
  discbytes = nDisco.header.DiscSize * (unsigned long) cant_bytes * (unsigned long) cant_bytes;
  nDisco.header.ftable.nBlocks = discbytes / blockbytes;

  nDisco.header.ftable.Table = (int *) malloc (sizeof(int)*nDisco.header.ftable.nBlocks);

  references =(float) (nDisco.header.ftable.nBlocks / (float) (blockbytes / 4));
  fatSize_bytes = references * blockbytes;
  CreateFat(fatSize_bytes);
  getInfo();
  return 0;
}

void CreateFat(int size){
  int i = 0;
  reserved_bytes = sizeof(nDisco.header.Name) +
                       sizeof(nDisco.header.Flag) +
                       sizeof(nDisco.header.MagicNumber) +
                       sizeof(nDisco.header.DiscSize) +
                       sizeof(nDisco.header.BlockSize) +
                       sizeof(nDisco.header.FirstFree) +
                       size;

  double reserved_blocks_double = (double) reserved_bytes / (double) (nDisco.header.BlockSize * cant_bytes);
  double intpart;
  double fractpart = modf(reserved_blocks_double, &intpart);

  if (fractpart * 10 > 0)
      reserved_blocks_int = reserved_blocks_double + 1;

  for (i = 0; i < reserved_blocks_int; i=i+1)
      WriteBlock(i, -1);

  for (i = reserved_blocks_int; i < nDisco.header.ftable.nBlocks-1; i=i+1 )
      WriteBlock(i, i+1);

  WriteBlock(i, 0); // Indicar el ultimo
  nDisco.header.MagicNumber = 1; // Indicar que ya esta formateado.
  nDisco.header.FirstFree = reserved_blocks_int; // Set el primer bloque libre.
}

void WriteBlock(int pos, int value){
    nDisco.header.ftable.Table[pos] = value;
}

int ReadBlock(int pos){ // Tiene que devolver lo leido del archivo
    if (pos < nDisco.header.ftable.nBlocks) // Existe
        return nDisco.header.ftable.Table[pos];
    else
        return -1;
}

void getTable(){
    // validar que haya disco montado
    int i;
    for (i = 0; i < nDisco.header.ftable.nBlocks; i = i + 1){
        printf("Tabla[%d]: %d\n", i, nDisco.header.ftable.Table[i] );
    }
}

int getNextFree(){
  int i = 0;
  while( (nDisco.header.ftable.Table[i] < 0) && // -1 sistema; -2 Alocado;
         (i < nDisco.header.ftable.nBlocks) ) {
      i = i + 1;
  }

  if (i == nDisco.header.ftable.nBlocks)
      return -1;
  return i;
}

void AllocateBlock(){
  int p = getNextFree();
  if (p != -1) { // Si no es del sistema.
    WriteBlock(p, -2);
    printf("Bloque [%d] se aloco.\n", p );
  }
  else
    printMsg("Disco Lleno");
}

void AllocateBlocks(int n){
  // Validar que haya disco montado
  int i=0;
  for (i; i<n; i+=1)
    AllocateBlock();
  getch();
}

void FreeBlock(int n){
  // validar que haya disco montado
  if (n >= nDisco.header.ftable.nBlocks)
    printMsg("Bloque No Existe");
  else
    if (nDisco.header.ftable.Table[n] == -1)
      printMsg("Bloque reservado a Header");
    else
      if (nDisco.header.ftable.Table[n] == -2){
            WriteBlock(n, n);
            printMsg("Bloque Liberado");
      }
      else
        printMsg("Bloque No Alocado Actualmente");
}
