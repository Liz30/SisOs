#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "Disco.h"

#define cant_bytes 1024
#define nblocks_reserved 2 // (0) Boot; (1) Metadata

FILE *f;
Disco nDisco;
extern NameSize;

int fatSize_blocks, header_bytes, reserved_blocks_int;
float references;

void printMsg(char* e){
    printf("%s   ", e);
    //system("pause");
    getch();
}

void getInfo(){
  if (DiscMounted() == 0)
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

    printf("\n\n  Bloques       FAT        Bloques      Usado     Libre\n");
        printf("                         Reservados    (Bytes)   (Bytes)");
    printf("\n   %d           %d           %d            %d        %d\n", nDisco.header.ftable.nBlocks, fatSize_blocks, reserved_blocks_int,
            getUsedSpace(), getFreeSpace());

    getch();
  }
}

int DiscMounted(){
  if (nDisco.header.Flag == 'M')
    return 1;
  return 0;
}

int DeleteDisc(char * path){
  if (DiscMounted() == 1){
    printMsg("Asegurese que el disco no este montado.");
    return -1;
  }
  int r = remove(path);
  if (r == 0){
      strcpy(nDisco.header.Name, "");
      nDisco.header.DiscSize = 0;
      nDisco.header.BlockSize = 0;
      nDisco.header.Flag = 'N';
      nDisco.header.MagicNumber = 0;
      nDisco.header.FirstFree = -1;
      return 0;
  }
  return -1;
}

int CreateDisc(char* name, unsigned long dsize, int bsize){
    f = fopen(name, "r");
    if (f == NULL){  // No existe
      f = fopen(name, "a+b");
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
  if (DiscMounted() == 1)
    printMsg("Disco actualmente montado.");
  else{
    if (strlen(nDisco.header.Name) == 0)
        printMsg("No hay disco");
    else{
    // Leer el arhivo
       f=fopen(nDisco.header.Name,"rb");
       if(f==NULL){
            printf("Error al leer el disco.");
            exit(1);
       }

       // Leer bloque 1 (Metadata)
       char *buffer;
       fread(buffer, )

       nDisco.header.Flag = 'M';
       fclose(fp);
       printMsg("Disco montado.");
    }
  }
}

void UmountDisc(){
  if (DiscMounted() == 0)
      printMsg("No se encontro Disco.");
  else{
      // escribir en el archivo
      printMsg("Disco desmontado.");
      nDisco.header.Flag = 'U';
  }
}

int FormatDisc(char *path){
  f = fopen(path, "r+");
  if (f == NULL){ // Validar que existe.
      printMsg(strcat(path, " no existe. "));
      fclose(f);
      return -1;
  }
  fclose(f);

  if (DiscMounted() == 1){ // Validar que esta desmontado.
      printMsg("Asegurese que el disco este desmontado");
      return -1;
  }

  strcpy(nDisco.header.Name, path);
  nDisco.header.ftable.nBlocks = nDisco.header.DiscSize / nDisco.header.BlockSize;
  nDisco.header.ftable.Table = (int *) malloc (sizeof(int)*nDisco.header.ftable.nBlocks);

  references =(float) (nDisco.header.ftable.nBlocks) / (float)(sizeof(nDisco.header.ftable.Table));
  fatSize_blocks = nDisco.header.ftable.nBlocks / references;

  CreateFat(fatSize_blocks * references);
  return 0;
}

void CreateFat(int size){
  int i = 0;
  header_bytes = sizeof(nDisco.header.Name) +
                       sizeof(nDisco.header.Flag) +
                       sizeof(nDisco.header.MagicNumber) +
                       sizeof(nDisco.header.DiscSize) +
                       sizeof(nDisco.header.BlockSize) +
                       sizeof(nDisco.header.FirstFree) +
                       size;

  reserved_blocks_int = nblocks_reserved + fatSize_blocks;

  for (i = 0; i < reserved_blocks_int; i=i+1)
      nDisco.header.ftable.Table[i] = -1;

  for (i = reserved_blocks_int; i < nDisco.header.ftable.nBlocks-1; i=i+1 )
      nDisco.header.ftable.Table[i] = i + 1;

  nDisco.header.ftable.Table[i] = 0; // Indicar el ultimo
  nDisco.header.MagicNumber = 1; // Indicar que ya esta formateado.
  nDisco.header.FirstFree = reserved_blocks_int; // Set el primer bloque libre.
}

int getFreeSpace(){
    if (DiscMounted() == 0){
        printMsg("Disco no encontrado.");
        return -1;
    }
    int i = reserved_blocks_int-1;
    int cont = 0;
    for (i; i < nDisco.header.ftable.nBlocks; i=i+1 )
        if (nDisco.header.ftable.Table[i] >= 0)
            cont = cont + 1;
    return (cont * nDisco.header.BlockSize);
}

int getUsedSpace(){
  if (DiscMounted() == 0){
      printMsg("Disco no encontrado.");
      return -1;
  }
  return (nDisco.header.DiscSize - getFreeSpace());
}

void WriteBlock(int pos, int value){
    //if (DiscMounted() == 0)
      // Escribirlo en el archivo
    //else
      printMsg("Write en construccion....");//Disco montado aun.");
}

int ReadBlock(int pos){ // Tiene que devolver lo leido del archivo
    if (pos < nDisco.header.ftable.nBlocks) // Existe
        return nDisco.header.ftable.Table[pos];
    else
        return -1;
}

void getTable(){
    if (DiscMounted() == 1){
        int i;
        for (i = 0; i < nDisco.header.ftable.nBlocks; i = i + 1){
            printf("Tabla[%d]: %d\n", i, nDisco.header.ftable.Table[i] );
        }
        getch();
    }
    else
        printMsg("Disco no montado.");
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
    nDisco.header.ftable.Table[p] = -2;
    printf("Bloque [%d] se aloco.\n", p );
  }
  else
    printMsg("Disco Lleno");
}

void AllocateBlocks(int n){
  if (DiscMounted() == 0)  // No hay disco montado
      printMsg("Disco no encontrado");
  else{
      int i=0;
      for (i; i<n; i+=1)
        AllocateBlock();
      getch();
  }
}

void FreeBlock(int n){
  if (DiscMounted() == 0) // No hay disco montado
      printMsg("Disco no encontrado");
  else{
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
}
