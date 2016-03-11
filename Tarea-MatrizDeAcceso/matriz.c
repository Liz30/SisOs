#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "matriz.h"

#define cant_bytes 1024

FILE *f;
MatrizAcceso nMatriz;


void printMsg(char* e){
    printf("%s   ", e);
    //system("pause");
    getch();
}
