#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "matriz.h"

//#define nDominios 5
//#define nObjetos 10000

/*MatrizAcceso nMatriz;
extern int nDominios; // Cantidad de dominios
extern int nObjetos; // Cantidad de objetos
Dominio* _primerDominio; // Arreglo para dominios
Dominio* _ultimoDominio;
Objeto* _objetos; // Arreglo para objetos
Privilegio* _privilegios;

void printMsg(char* e){
    printf("\n%s   ", e);
    //system("pause");
    getch();
}

void LlenarPrivilegios(){
    _privilegios = (struct Privilegio*) malloc ((sizeof(struct Privilegio)) * _nPrivilegios);
    _privilegios[0]->n = 0;
    memcpy(_privilegios[0]->nombre, "Read", sizeof("Read")+1);
    _privilegios[0]->id='R';
}

void CrearMatriz(){
    printf("Creando matriz de acceso...\n");
    //int h = 0;
    _primerDominio = NULL;
    _objetos = NULL;
    LlenarDominios(nDominios);
  //  ListarDominios();
}

void LlenarDominios(int n){
    Dominio* nuevo;
    char ddesc[10]="D";
    char ci[10];

    int i;
    printMsg("Antes del for...");
    printf("Dominios %d\n", nDominios);
    nuevo = (struct Dominio*) malloc(sizeof(struct Dominio));

    for (i=0; i<nDominios; i++){
        // puede ser getDominio(); devuelva dominios al azar
        nuevo->id = i;
        sprintf(ci, "%d", i);
        memcpy(&ddesc, "D", 2);
        strcat(ddesc, ci);
        memcpy(nuevo->nombre, ddesc, strlen(ddesc)+1);
        nuevo->tipo='u';

        nuevo->objs = NULL;
        nuevo->sig = NULL;
        // if (!es admin)
        nuevo->privilegios[0] = 1;
        nuevo->privilegios[1] = 1;
        nuevo->privilegios[2] = 0;
        nuevo->privilegios[3] = 1;
        nuevo->privilegios[4] = 0;

        if (_primerDominio==NULL){
          _primerDominio = nuevo;
          _ultimoDominio = nuevo;
        }
        else{
          _ultimoDominio->sig = nuevo;
          _ultimoDominio = nuevo;
        }
        printMsg(ddesc);
    }
}

void ListarDominios(){
    /*Dominio* temp;
    int i;

    i = 0;
    /*temp = _dominios->primero;
    while(temp!=NULL){
        printMsg(strcat(temp->desc,"\n"));
        temp = temp->sig;
        i++;
    }

    if (i == 0)
        printMsg("Lista de Dominios vacia");
}*/
