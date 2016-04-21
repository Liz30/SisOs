#ifndef _MATRIZ_H_
#define _MATRIZ_H_

#include <stdio.h>
#include <time.h>

//////// PERMISOS //////
#define _READ r    // 0
#define _WRITE w   // 1
#define _EXECUTE x // 2
#define _SWITCH s  // 3
#define _FULL f   // 4
#define _nPrivilegios 5
////////////////////////

////////// TIPOS DE DOMINIOS /////////
#define _USUARIO u
#define _ADMIN a

///////// TIPOS DE OBJETOS ///////////


typedef struct MatrizAcceso MatrizAcceso;
typedef struct Dominio Dominio; // Dominios existentes
typedef struct Objeto Objeto; // Objetos Existentes
typedef struct Privilegio* Privilegio;

//////////////////////////////// PRIVILEGIO ///////////////
struct Privilegio{
    int n;
    char* nombre; // Read, Write, Execute...
    char id; // R, W, X, S, F
};
/////////////////////////////// OBJETO //////////////////
struct Objeto{
    int id;
    char* nombre;
    char tipo;
    time_t fcreacion; // printf(ctime(&fcreacion));
    time_t fmodif;
    time_t ultimoacceso;
    long size; // Bytes
    Dominio* dominios; // Dominios que pueden accesar a el
    Dominio* owner;
    Objeto* sig;
};
/////////////////////////////// DOMINIO //////////////////
struct Dominio{
    int id;
    char* nombre;
    char tipo; // Usuario, administrador
    int privilegios[_nPrivilegios]; // Por default. Ej. Admin los tiene todos
    Objeto* objs; // Objetos de los que es owner
    Dominio* sig;
};

///////////////////////////// MATRIZ DE ACCESO ////////////
struct MatrizAcceso{
    Dominio* _dominios;
    Objeto* _objetos;
    int _privilegios[_nPrivilegios];
};

void CrearMatriz();
void ListarPrivilegios(Dominio d); // Buscar ese dominio en la matriz
int Acceso(Dominio d, Objeto o); // 0 No tiene acceso; 1 Tiene acceso

////////////////////////// OTROS ///////////////////////////
void printMsg(char* e);
void LlenarDominios(int n);
void LlenarObjetos(int n);
void LlenarPrivilegios();
void ListarDominios();
void ListarObjetos();


#endif
