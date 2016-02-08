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

void printMsg(char* e){
    printf("%s   ", e);
    system("pause");
}

void getInfo(){
  printf("\n\nNombre del Disco: %s\n", nDisco.Name);
  printf("Tamaño de Disco: %d MB\n", nDisco.header.DiscSize);
  printf("Tamaño de Bloque: %d KB\n", nDisco.header.BlockSize);
  printf("Flag: %c\n", nDisco.header.Flag);
  printf("MagicNumber: %d\n", nDisco.header.MagicNumber);
  system("pause");
}

int CreateDisc(char* name, unsigned long dsize, int bsize){
    nDisco.Name = name;
    nDisco.header.DiscSize = dsize;
    nDisco.header.BlockSize = bsize;
    nDisco.header.Flag = 'U';
    nDisco.header.MagicNumber = 0;
    f = fopen(name, "r");
    if (f == NULL){  // No existe
      f = fopen(name, "a+");
      fclose(f);
      if ( FormatDisc(name) != 0 )
          return -1;
      return 0;
  }
  fclose(f);
  return -1;
}

int DeleteDisc(char * path){
  int r = remove(path);
  if (r == 0)
    return 0;
  return -1;
}

int FormatDisc(char *path){
  f = fopen(path, "r+");
  if (f == NULL){
      printMsg(strcat(path, " no existe. "));
      fclose(f);
      return -1;
  }
  fclose(f);

  // Calculos
  unsigned long discbytes;
  int blockbytes, fatSize_bytes;
  float references;
  blockbytes = (nDisco.header.BlockSize * cant_bytes);
  discbytes = nDisco.header.DiscSize * (unsigned long) cant_bytes * (unsigned long) cant_bytes;
  nDisco.header.ftable.nBlocks = discbytes / blockbytes;
  references =(float) (nDisco.header.ftable.nBlocks / (float) (blockbytes / 4));
  fatSize_bytes = references * blockbytes;
  getInfo();
  printf("\n\nblockbytes: %d\ndiscbytes: %d\nnBlocks: %d\nreferences: %f\nfatSize_bytes: %d", blockbytes, discbytes,nDisco.header.ftable.nBlocks, references, fatSize_bytes);

  //CreateFat
  CreateFat(fatSize_bytes);

  return 0;
}

void CreateFat(int size){
  int i = 0;
  int t[nDisco.header.ftable.nBlocks];
  int reserved_bytes = sizeof(nDisco.header.Flag) +
                          sizeof(nDisco.header.MagicNumber) +
                          sizeof(nDisco.header.DiscSize) +
                          sizeof(nDisco.header.BlockSize) +
                          size;

  int reserved_blocks_int;
  double reserved_blocks_double = (double) reserved_bytes / (double) (nDisco.header.BlockSize * cant_bytes);
  double intpart;
  double fractpart = modf(reserved_blocks_double, &intpart);

  if (fractpart * 10 > 0)
      reserved_blocks_int = reserved_blocks_double + 1;

  for (i = 0; i < reserved_blocks_int; i=i+1)
      WriteBlock(i, -1);

  for (i; i < nDisco.header.ftable.nBlocks-1; i=i+1 )
      WriteBlock(i, i+1);
  WriteBlock(i, 0); // Indicar el ultimo

  getTable();

  printf("\n\nreserved_bytes: %d\n reserved_blocks: %d\n", reserved_bytes, reserved_blocks_int);
  system ("pause");
}

void WriteBlock(int pos, int value){
    nDisco.header.ftable.Table[pos] = value;
}

void getTable(){
    int i;
    for (i = 0; i < nDisco.header.ftable.nBlocks; i = i + 1){
        printf("Tabla[%d]: %d\n", i, nDisco.header.ftable.Table[i] );
    }
}
