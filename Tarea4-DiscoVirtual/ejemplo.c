#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BlockSize 4096
#define DiscSize 1048576
#define size_ptr sizeof(int)

FILE *f;
int* fat;

struct SuperBlock{
    int FatInit;
    int FatSize;
    int FatHead;
};

struct SuperBlock sb;

void WriteBlock(int blocknumber, char* buffer);
void ReadBlock(int blocknumber, char* buffer);
void CrearDisco();
void Format();
void ShowBlock(int n);
void ShowFat();
void printMsg(char* e);


int main()
{
    CrearDisco();
    Format();
    // TODO: Imprimir bloques de disco.
    ShowFat();
    return 0;
}

void printMsg(char* e){
    printf("\n %s   ", e);
    getch();
}

void Format(){
  sb.FatInit = 2;
  //sb.FatSize = (DiscSize / BlockSize) / (BlockSize / 4);
  int nBlocks=(DiscSize / BlockSize);
  float references = ((float)BlockSize / (float) size_ptr);
  double m = nBlocks / references;
  double intpart;
  double decpart = modf(m, &intpart);
  if (decpart * 10 > 0)
      sb.FatSize = (int) intpart + 1;
  else
      sb.FatSize = (int) intpart;
  printf("nblock: %d\nreferences: %f\n", nBlocks, references );
  printf("m: %f\nintpart: %f\ndoublepart: %f\nsize: %d\n", m, intpart, decpart, sb.FatSize );
  getch();

  sb.FatHead = sb.FatInit + sb.FatSize;

  char* buffer = (char*) malloc (BlockSize);
  memcpy(buffer, &sb, sizeof(sb));
  printMsg("Antes de Write...");
  WriteBlock(1, buffer);  // BLoque de Metadata
  printMsg("Despues de Write...");
  fat = (int*) malloc(sb.FatSize * BlockSize);
  int i = 0;
  for (i; i < sb.FatHead; i = i+1)
      fat[i] = -1;
  printMsg("For 1....");
  for (i; i < (sb.FatSize * BlockSize)-1; i=i+1)
      fat[i] = i+1;
  printMsg("For 2....");

  fat[i]=0;

  i = 0;
  for (i; i<sb.FatSize; i=i+1)
      WriteBlock(sb.FatInit + i, (char*)(fat+(i*1024)));
  printMsg("For 3...");

  free(buffer);
  //free(fat);
}

void CrearDisco(){
    f = fopen("disco", "wb");
    fclose(f);
}

void WriteBlock(int blocknumber, char* buffer){
    printMsg("Write Inicio...");
        f = fopen("disco", "r+b"); // Sustituir el contenido
        if (f == NULL)
            printf("Error al escribir\n" );
        else{
            printMsg("Write Else...");
            fseek(f, blocknumber * BlockSize, SEEK_SET); // Byte donde inicia la FAT
            fwrite(buffer, 1, BlockSize, f); // opcion 2: buffer, 1, sizeof(char) * nDisco.header.BlockSize, f
        }
        fclose(f);
    printMsg("Write Fin...");
}

void ReadBlock(int blocknumber, char* buffer){
      f = fopen("disco", "rb");
      if (f == NULL)
          printf("Disco no encontrado\n");
      else{
          fseek(f, blocknumber * BlockSize, SEEK_SET); // Byte donde inicia la FAT
          fread(&buffer, 1, BlockSize, f);
      }
      fclose(f);
}

void ShowBlock(int n){
      printMsg("Show Block...");
      char* buff = (char*) malloc(BlockSize);
      ReadBlock(n, buff);
      printf("Bloque %d: %s\n", n, buff);
      //free(buff);
      getch();
}

void ShowFat(){
    printMsg("Show Fat Init...");
    int i = sb.FatInit;
    printf("Init: %d\n Size: %d\n Head: %d\n", sb.FatInit, sb.FatSize, sb.FatHead );
    getch();
    for (i; i<sb.FatInit+sb.FatSize;i=i+1)
      ShowBlock(i);
}
