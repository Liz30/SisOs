#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "Disco.h"

#define cant_bytes 1024
#define nblocks_reserved 2 // (0) Boot; (1) Metadata
#define size_block 4096 // Tamaño de bloque establecido para leer archivos.

FILE *f;
Disco nDisco;
FatTable ftable;
extern NameSize;

int fatSize_blocks, header_bytes, reserved_blocks_int;
float references;

void printMsg(char* e){
    printf("\n %s   ", e);
    getch();
}

void getInfo(){
  if (DiscMounted() == 0)
    printMsg("Disco no encontrado");
  else{
    printf("\n\n Nombre del Disco: %s\n", nDisco.header.Name);
    printf(" Tamaño de Disco: %d Bytes\n", nDisco.header.DiscSize);
    printf(" Tamaño de Bloque: %d Bytes\n", nDisco.header.BlockSize);
    printf(" Flag Mount: %c\n", nDisco.header.Flag);
    printf(" Formato: " );
    if(nDisco.header.MagicNumber == 0)
        printf("No\n");
    else
        printf("Si\n");

    printf("\n\n  Bloques       FAT        Bloques      Usado     Libre\n");
        printf("                         Reservados    (Bytes)   (Bytes)");
    printf("\n   %d           %d           %d            %d        %d\n", ftable.nBlocks, fatSize_blocks, reserved_blocks_int,
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
      nDisco.header.Flag = 'U';
      nDisco.header.MagicNumber = 0;
      nDisco.header.FirstFree = -1;
      nDisco.header.LastFree = -1;
      nDisco.header.FatInitPos = -1;
      nDisco.header.FatBlock = -1;
      return 0;
  }
  return -1;
}

void MountDisc(){
  if (DiscMounted() == 1)
    printMsg("Disco actualmente montado.");
  else{
    //if (strlen(nDisco.header.Name) == 0)
    //    printMsg("No hay disco");
    //else{
    // Leer el arhivo
      strcpy(nDisco.header.Name, "disc1");
       f=fopen(nDisco.header.Name,"rb");
       if(f==NULL){
            printf("Error al leer el disco.");
            exit(1);
       }

       // Leer bloque 1 (Metadata)
       char *buffer = (char *) malloc (sizeof(char) * size_block);
       int t;
       t = fread(buffer, sizeof(char), size_block,f); // Bloque 0
       t = fread(&nDisco.header, sizeof(struct Header), 1, f); // Metadata
       if (nDisco.header.MagicNumber == 1){
           nDisco.header.Flag = 'M';
           printMsg("Disco montado.");
       }
       else
          printMsg("Disco Sin Formato");
      free(buffer);
      fclose(f);
    //}
  }
}

void UmountDisc(){
  if (DiscMounted() == 0)
      printMsg("No se encontro Disco.");
  else{
      // escribir en el archivo
      // modo wb
      printMsg("Disco desmontado.");
      nDisco.header.Flag = 'U';
      strcpy(nDisco.header.Name, "");
      nDisco.header.DiscSize = 0;
      nDisco.header.BlockSize = 0;
      nDisco.header.MagicNumber = 0;
      nDisco.header.FirstFree = -1;
      nDisco.header.LastFree = -1;
      nDisco.header.FatInitPos = -1;
      nDisco.header.FatBlock = -1;
  }
}

int CreateDisc(char* name, unsigned long dsize, int bsize){
    f = fopen(name, "rb");
    if (f == NULL){  // No existe
      f = fopen(name, "wb"); // Crearlo vacio.
      fclose(f);
      strcpy(nDisco.header.Name, name);
      nDisco.header.DiscSize = dsize * cant_bytes * cant_bytes;
      nDisco.header.BlockSize = bsize * cant_bytes;
      nDisco.header.Flag = 'U';
      nDisco.header.MagicNumber = 0;
      nDisco.header.FirstFree = 0;
      nDisco.header.LastFree = 0;
      nDisco.header.FatInitPos = -1;
      nDisco.header.FatBlock = -1;
      if ( FormatDisc(name) == -1 ) // Produjo un error.
          return -2;
      return 0;
    }
    fclose(f);
    return -1;  // El disco ya existe
}

int FormatDisc(char *path){

  if (DiscMounted() == 1){ // Validar que esta desmontado.
      printMsg("Asegurese que el disco este desmontado");
      return -1;
  }

  f = fopen(path, "r+b");
  if (f == NULL){ // Validar que existe.
      printMsg(strcat(path, " no existe. "));
      return -1;
  }

  strcpy(nDisco.header.Name, path);
  ftable.nBlocks = nDisco.header.DiscSize / nDisco.header.BlockSize;
  ftable.Table = (int *) malloc (sizeof(int)*ftable.nBlocks);

  references =(float) (ftable.nBlocks) / (float)(sizeof(ftable.Table));
  fatSize_blocks = ftable.nBlocks / references;
  CreateFat(fatSize_blocks * references);

  char* buffer = (char *) malloc (sizeof(char) * nDisco.header.BlockSize);
  memset(buffer, 0, sizeof(char) * nDisco.header.BlockSize);

  // Escribir en el archivo.
  // Bloque 0, Boot (Null)
  printMsg("Escribiendo bloques reservados en el disco...");
  fwrite(buffer, sizeof(char), sizeof(char) * nDisco.header.BlockSize, f); // opcion 2: buffer, 1, sizeof(char) * nDisco.header.BlockSize, f
  fwrite(&nDisco.header, sizeof(struct Header), 1, f);

  int bytes_restantes = nDisco.header.BlockSize - sizeof(nDisco.header);
  free(buffer);
  char* buffer1 = (char *) malloc(sizeof(char) * bytes_restantes);
  printf("    %d \n", sizeof(char) * bytes_restantes );
  memset(buffer1, 0, sizeof(char) * bytes_restantes);

  fwrite(buffer1, sizeof(char), sizeof(char) * bytes_restantes, f);
  free(buffer);

  fwrite(ftable.Table, sizeof(ftable.Table), 1, f);
  printMsg("Escritura finalizada...");
  // Escribir la Tabla
  /*int i = nDisco.header.FatInitPos;
  for (i; i < nDisco.header.FatBlock; i=i+1){ // Bloques de la Fat
      int j = 0;
      for (j; j < references; j = j+1)

  }*/
  fclose(f);
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
      ftable.Table[i] = -1;

  for (i = reserved_blocks_int; i < ftable.nBlocks-1; i=i+1 )
      ftable.Table[i] = i + 1;

  ftable.Table[i] = 0; // Indicar el ultimo
  nDisco.header.MagicNumber = 1; // Indicar que ya esta formateado.
  nDisco.header.FirstFree = reserved_blocks_int; // Set el primer bloque libre.
  nDisco.header.FatInitPos = nblocks_reserved;
  nDisco.header.FatBlock = fatSize_blocks;
  nDisco.header.LastFree = i;
}

int getFreeSpace(){
    if (DiscMounted() == 0){
        printMsg("Disco no encontrado.");
        return -1;
    }
    int i = reserved_blocks_int-1;
    int cont = 0;
    for (i; i < ftable.nBlocks; i=i+1 )
        if (ftable.Table[i] >= 0)
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
    if (DiscMounted() == 0)
        printMsg("Disco no encontrado");
    else{
        f = fopen(nDisco.header.Name, "r+b"); // Sustituir el contenido
        if (f == NULL)
            printMsg("Disco no encontrado");
        else{
            printMsg("Iniciando a Escribir");
            fwrite(&nDisco.header.Name, sizeof(nDisco.header.Name), strlen(nDisco.header.Name)+1, f);
            fseek(f, sizeof(nDisco.header.Name)+1, SEEK_SET);
            fwrite(&nDisco.header.DiscSize, sizeof(nDisco.header.DiscSize), 1, f);
            printMsg("Termino Escritura...");
        }
        fclose(f);
    }
}

int ReadBlock(int pos){ // Tiene que devolver lo leido del archivo
  if (DiscMounted() == 0)
      printMsg("Disco no encontrado");
  else{
      f = fopen(nDisco.header.Name, "rb"); //
      if (f == NULL)
          printMsg("Disco no encontrado");
      else{
          char *buff1 = (char*) malloc(sizeof(nDisco.header.Name));
          char *buff2 = (char*) malloc(sizeof(nDisco.header.DiscSize));
          printMsg("Iniciando Lectura...");
          fread(buff1, sizeof(nDisco.header.Name), 1, f);
          fread(buff2, sizeof(nDisco.header.DiscSize), 1, f);
          printMsg("Finalizando lectura...");
          printf("Buffer1: %s\n", buff1 );
          printf("Buffer2: %d\n", buff2 );
          free(buff1);
          free(buff2);
          getch();
      }
      fclose(f);
  }
}

void getTable(){
    if (DiscMounted() == 1){
        printf("\nFirstFree: %d     LastFree: %d\n\n", nDisco.header.FirstFree, nDisco.header.LastFree);
        int i;
        for (i = 0; i < ftable.nBlocks; i = i + 1){
            printf("Tabla[%d]: %d\n", i, ftable.Table[i] );
        }
        getch();
    }
    else
        printMsg("Disco no montado.");
}

int getNextFree(){
  /*int i = 0;
  while( (ftable.Table[i] < 0) && // -1 sistema; -2 Alocado;
         (i < ftable.nBlocks) ) {
      i = i + 1;
  }

  if (i == ftable.nBlocks)
      return -1;
  return i;
  // Segunda Forma
  int i = nDisco.header.FirstFree;
  while ( i < ftable.nBlocks ){
      if ( ftable.Tabla[i] >= 0 ) // Esta libre
          return i;
      i = i + 1;
  }
  return -1; // Llego al final de la tabla.*/
  return nDisco.header.FirstFree;
}

void AllocateBlock(){
  int p = getNextFree();
  if (ftable.Table[p] != -1) { // Si no es del sistema.
    nDisco.header.FirstFree = ftable.Table[p];
    ftable.Table[p] = -2;

    if (nDisco.header.FirstFree == 0)
        nDisco.header.LastFree = 0;

    printf("Bloque [%d] se aloco.\n", p );
  }
  else
    printMsg("Disco Lleno");
}

void AllocateBlocks(int n){
  if (DiscMounted() == 0)  // No hay disco montado
      printMsg("Disco no encontrado");
  else{
      if (ftable.Table[nDisco.header.FirstFree] < 0) // Disco lleno
          printMsg("Disco Lleno");
      else{
          int i=0;
          for (i; i<n; i=i+1)
            AllocateBlock();
      }
      getch();
  }
}

void FreeBlock(int n){
  if (DiscMounted() == 0) // No hay disco montado
      printMsg("Disco no encontrado");
  else{
      if (n >= ftable.nBlocks)
        printMsg("Bloque No Existe");
      else
        if (ftable.Table[n] == -1)
          printMsg("Bloque reservado por el Sistema de Archivos.");
        else
          if (ftable.Table[n] == -2){
                ftable.Table [n] = 0;
                if ( nDisco.header.FirstFree == 0 )
                        nDisco.header.FirstFree = n;
                else
                    ftable.Table[nDisco.header.LastFree] = n;
                nDisco.header.LastFree = n;
                printMsg("Bloque Liberado");
          }
          else
            printMsg("Bloque No Alocado Actualmente");
    }
}
