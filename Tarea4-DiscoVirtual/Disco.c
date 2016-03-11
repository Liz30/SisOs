#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <limits.h>
#include <math.h>
#include "Disco.h"

#define cant_bytes 1024
#define nblocks_reserved 2 // (0) Boot; (1) Metadata
//#define size_block 8192 // Tamaño de bloque establecido para leer archivos.
#define size_ptr sizeof(int) // Tamaño del apuntador

FILE *f;
Disco nDisco;
FatTable ftable;
extern NameSize;

int fatSize_blocks, header_bytes, reserved_blocks_int;
float references;

void InitVariables(){
  fatSize_blocks = 0;
  header_bytes = 0;
  reserved_blocks_int = 0;
  references = 0;
}

void printMsg(char* e){
    printf("\n %s   ", e);
    getch();
}

void getInfo(){
  if (DiscMounted() == 0)
    printMsg("Disco no encontrado");
  else{
    printf("\n\n %s         Flag Mount: %c       Formato: ", nDisco.header.Name,nDisco.header.Flag);
    if(nDisco.header.MagicNumber == 0)
        printf("No\n");
    else
        printf("Si\n");
    printf("\n Disco: %d  MB (%d)\n",nDisco.header.DiscSize / cant_bytes / cant_bytes,  nDisco.header.DiscSize);
    printf(" Bloque: %d   KB (%d)\n", nDisco.header.BlockSize / cant_bytes, nDisco.header.BlockSize);
    printf(" N Bloques en Disco: %d\n", ftable.nBlocks);
    printf(" Bloques Reservados: %d         FAT: %d Bloques\n", reserved_blocks_int, nDisco.header.FatBlock);

    printf("\n\n MANEJO DEL ESPACIO EN DISCO: \n");

    printf("\n\n        Bytes                      Bloques\n");
    printf("  Usados:  %d                  Usados: %d\n  Libres:  %d                  Libres: %d", getUsedSpace(), getUsedSpace() / nDisco.header.BlockSize, getFreeSpace(), getFreeSpace() / nDisco.header.BlockSize);
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
      free(ftable.Table);
      InitVariables();
      return 0;
  }
  return -1;
}

int MountDisc(char* path, int size){ // Size de bloque (K)
  if (DiscMounted() == 1){
    printMsg("Disco actualmente montado.");
    return -1;
  }
  else{
       strcpy(nDisco.header.Name, path);
       f=fopen(nDisco.header.Name,"rb");
       if(f==NULL){
            printf("Error al leer el disco.");
            return -1;
       }

       // Leer bloque 1 (Metadata)
       char *buffer = (char *) malloc (sizeof(char) * (size*cant_bytes));
       int t;
       t = fread(buffer, sizeof(char), (size*cant_bytes),f); // Bloque 0
       t = fread(&nDisco.header, sizeof(struct Header), 1, f); // Metadata

       if (nDisco.header.MagicNumber == 1){ // Disco formateado.
           nDisco.header.Flag = 'M';
           reserved_blocks_int = nDisco.header.FatBlock + nDisco.header.FatInitPos;
           ftable.nBlocks = nDisco.header.DiscSize / nDisco.header.BlockSize;
           ftable.Table = (int *) malloc (size_ptr * ftable.nBlocks);
           references = (float) nDisco.header.BlockSize / (float) size_ptr ;
           //if (references < 0)
            //  references =
           setFAT();

           printMsg("Disco montado.");
           //return 0;
       }
       else{
          printMsg("Disco Sin Formato");
          return -1;
      }
      free(buffer);
      fclose(f);
      return 0;
    }
}

void setFAT(){
  printMsg("Inicio set...");
  int* j = (int*) malloc (sizeof(int)); // Reserva sizeof(int)
  int c = nDisco.header.FatInitPos;
  int y, npos, postable;
  postable = 0;
  for (c; c < reserved_blocks_int; c = c+ 1){ // BLOQUE
      int d = 0;
      for (d; d < references; d = d +1){
          npos = (nDisco.header.FatInitPos * nDisco.header.BlockSize) + (d * size_ptr);
          fseek(f, npos, SEEK_SET); // Byte donde inicia la FAT
          y = fread(&ftable.Table[postable], size_ptr, 1, f);
          //ftable.Table[postable] = j;
          postable = postable + 1;
      }
      getch();
  }
  //printf("Y :%d   \n", npos);
  free(j);
  printMsg("Fin set...");
  getTable();
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
      free(ftable.Table);
      InitVariables();
  }
}

int CreateDisc(char* name, int dsize, int bsize){
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

/*void CreateDisc2(char* name, int dsize, int bsize){
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
  printf("Size of Disco: %d\n", sizeof(nDisco) );
  getch();
}*/

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

  if (strlen(nDisco.header.Name) == 0 ){  // Archivo nuevo
    int dsize, bsize;
    printf("\n   Tamaño del Disco (MB) > " );
    scanf("%d", &dsize );
    printf("   Tamaño del Bloque (KB) > " );
    scanf("%d", &bsize );
    nDisco.header.DiscSize = dsize * cant_bytes * cant_bytes;
    nDisco.header.BlockSize = bsize * cant_bytes;
    nDisco.header.FirstFree = 0;
    nDisco.header.LastFree = 0;
    nDisco.header.FatInitPos = -1;
    nDisco.header.FatBlock = -1;
    printMsg("2...");
  }
  printMsg("3...");

  strcpy(nDisco.header.Name, path);
  ftable.nBlocks = nDisco.header.DiscSize / nDisco.header.BlockSize;
  ftable.Table = (int *) malloc (sizeof(int)*ftable.nBlocks);
  references = (float) nDisco.header.BlockSize / (float) size_ptr ;

  double m = ftable.nBlocks / references;
  double intpart;
  double decpart = modf(m, &intpart);
  if (decpart * 10 > 0)
      fatSize_blocks = (int) intpart + 1;
  else
      fatSize_blocks = (int) intpart;

  printf("FAT...: %d\n", fatSize_blocks); getch();

  CreateFat(fatSize_blocks * references);

  char* buffer = (char *) malloc (sizeof(char) * nDisco.header.BlockSize);
  memset(buffer, 0, sizeof(char) * nDisco.header.BlockSize);

  printMsg("Escribiendo bloques reservados en el disco...");
  //fwrite(buffer, sizeof(char), sizeof(char) * nDisco.header.BlockSize, f); // opcion 2: buffer, 1, sizeof(char) * nDisco.header.BlockSize, f
  WriteBlock(0, buffer);
  //fwrite(&nDisco.header, sizeof(struct Header), 1, f);
  WriteBlock(1, (char*)&nDisco.header);

  int bytes_restantes = nDisco.header.BlockSize - sizeof(nDisco.header);
  free(buffer);
  char* buffer1 = (char *) malloc(sizeof(char) * bytes_restantes);
  printf("    %d \n", sizeof(char) * bytes_restantes );
  memset(buffer1, 0, sizeof(char) * bytes_restantes);

  //fwrite(buffer1, sizeof(char), sizeof(char) * bytes_restantes, f);
  //WriteBlock()
  free(buffer);

  //fwrite(ftable.Table, sizeof(ftable.Table), 1, f);
  WriteBlock(nDisco.header.FatInitPos, (char*)&ftable.Table);
  printMsg("Escritura finalizada...");
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
    int i = reserved_blocks_int;
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

void WriteBlock(int blocknumber, char* buffer){
    //if (DiscMounted() == 0)/////
    //    printMsg("Disco no encontrado");///
    //else{//
        f = fopen(nDisco.header.Name, "r+b"); // Sustituir el contenido
        if (f == NULL)
            printMsg("Disco no encontrado");
        else{
            fseek(f, blocknumber * nDisco.header.BlockSize, SEEK_SET); // Byte donde inicia la FAT
            fwrite(buffer, 1, nDisco.header.BlockSize, f); // opcion 2: buffer, 1, sizeof(char) * nDisco.header.BlockSize, f
        }
        fclose(f);
    //}
}

void ReadBlock(int blocknumber, char* buffer){ // Tiene que devolver lo leido del archivo
  /*if (DiscMounted() == 0)
      printMsg("Disco no encontrado");
  else{*/
      f = fopen(nDisco.header.Name, "rb");
      if (f == NULL)
          printMsg("Disco no encontrado");
      else{
          fseek(f, blocknumber * nDisco.header.BlockSize, SEEK_SET); // Byte donde inicia la FAT
          fread(buffer, 1, nDisco.header.BlockSize, f);
      }
      fclose(f);
  //}
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
