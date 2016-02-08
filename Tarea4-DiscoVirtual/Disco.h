#ifndef _DISCO_H_
#define _DISCO_H_


extern NameSize;
typedef struct Disco Disco;

struct Bloque{
    // apuntadores
    int *ptrs;
};

struct FatTable{
    int nBlocks;  // Cantidad de Bloques.
    int Table[];   // Tabla Fat.
    //int* firstFree; // Apunta al primer bloque libre. IMPLEMENTAR
};

struct Header{
    unsigned long DiscSize;   // tamaño del disco (bytes).
    int BlockSize;  // Tamaño del bloque (bytes).
    char Flag;      // Indicador si esta montado (M) o no (U).
    int MagicNumber; // Indicador si esta formateado (-1) o no (0).
    struct FatTable ftable; // Fat Table.
};

struct Disco {
    struct Header header;
    char* Name;
};

void CreateFat(int size); // Done
int ReadBlock(int pos);
void WriteBlock(int pos, int value); // Done
void getTable(); // Carga a Memoria la tabla. Done
int getNextFree(); // Devuelve el primer bloque libre. Done.

int CreateDisc(char* name, unsigned long dsize, int bsize);  // Done
int FormatDisc(char* path); // Done
int DeleteDisc(char* path); // Done
void MountDisc();
void UmountDisc();
void AllocateBlock(); // Done
void AllocateBlocks( int n); // Done
void FreeBlock(int n);
void getFreeSpace();
void getUsedSpace();
void getInfo(); // Done

void printMsg(char* e); // Done


#endif
