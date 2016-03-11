#ifndef _DISCO_H_
#define _DISCO_H_


extern NameSize;
typedef struct Disco Disco;
//typedef struct Bloque Bloque;
typedef struct FatTable FatTable;

/*struct Bloque{
    // apuntadores
    Bloque* Next; // Siguiente Bloque de data.
    char type; // Archivo, Directorio
};*/

struct FatTable{
    int nBlocks;  // Cantidad de Bloques.
    int* Table;   // Tabla Fat.
};

struct Header{
    char Name[10];
    int DiscSize;   // tamaño del disco (bytes).
    int BlockSize;  // Tamaño del bloque (bytes).
    char Flag;      // Indicador si esta montado (M) o no (U).
    int MagicNumber; // Indicador si esta formateado (1) o no (0).
    int FirstFree; // Primer bloque libre.
    short FatInitPos; // Numero de bloque donde inicia la FAT
    int FatBlock; // Numero de Bloque que alocan la FAT.
    int LastFree; // Ultimo Bloque Libre.
};

struct Disco {
    struct Header header;
    //FatTable ftable; // Fat Table.
    //Bloque DataSpace[];
};

void CreateFat(int blocknumber); // Done
void ReadBlock(int blocknumber, char* buffer);
void WriteBlock(int blocknumber, char* buffer);
void getTable(); // Carga a Memoria la tabla. Done
int getNextFree(); // Devuelve el primer bloque libre. Done.
void setFAT(); // Llenar la tabla desde el archivo.

int CreateDisc(char* name, int dsize, int bsize);  // Done
//void CreateDisc2(char* name, int dsize, int bsize);  // Done
int FormatDisc(char* path); // Done
int DeleteDisc(char* path); // Done
int MountDisc(char* path, int size); // size de bloque
void UmountDisc(); //
void AllocateBlock(); // Done
void AllocateBlocks( int n); // Done
void FreeBlock(int n); // Done
void getInfo(); // Done
int getFreeSpace();
int getUsedSpace();
int DiscMounted();  // 1 true; 0 false; Done
//void InitVariables();

void printMsg(char* e); // Done


#endif
