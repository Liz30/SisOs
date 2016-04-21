#include <stdio.h>
#include <stdlib.h>
#include "matriz.h"

typedef struct Usuario Usuario;
typedef struct Recurso Recurso;

//////////////////// USUARIO //////////////////
struct Usuario{
  char user[10];
  int grupo;
};

struct Recurso{
  char nombre[50];
  Usuario owner;
};

/////////////////// GLOBALES //////////////////
Usuario CurrentUser;
Usuario usuario;
Usuario usuarios[10];
int nusuarios;

Recurso recurso;
Recurso recursos[100];
int nrecursos;

void CrearUsuario();
void ListarUsuarios();
void CrearRecurso();
void ListarRecursos();
void AsignarPermisos();

int Login();
Usuario getUser(char* u);
Recurso getRecurso(char *nombre);


int main(){

  int op = 0;
  int opc = 0;
  nusuarios = 0;
  nrecursos = 0;
  while (op!=3){
    system("cls");
    printf("\n  1. Crear Usuario");
    printf("\n  2. Login");
    printf("\n  3. Salir     > ");
    scanf("%d", &op);
    switch (op) {
        case 1: CrearUsuario(); ListarUsuarios(); op = 0; break ;
        case 2: if (Login() == 0){ // No existe
                    printf("Usuario no existe...\n");
                    getch();
                }
                else {
                    while(opc!=4){
                        printf("\n       1. Crear Recurso");
                        printf("\n       2. Asignar Permisos");
                        printf("\n       3. Verificar Permisos");
                        printf("\n       4. Logout                 > ");
                        scanf("%d", &opc);
                        switch(opc){
                            case 1: CrearRecurso(); op = 0; break;
                            case 4: break;
                        }
                  }
                }
        case 3: break;
    }
  }
  return 0;
}

void AsignarPermisos(){
    printf(" Asignar Permisos...\n");
    char n[50];
    Usuario un;
    printf("\n   Nombre del Recurso: ");
    scanf("%s", &n);
    recurso = (Usuario) getRecurso(n);
    un = (struct Usuario) recurso.owner;
    if (strcmp(&un.user, CurrentUser.user) == 0){
        
    }
    else{
        printf("Acceso denegado...\n", );
        getch();
    }
}

void CrearRecurso(){
    printf("Crear Recurso...\n");
    char n[50];
    printf("\n   Nombre del Recurso: ");
    scanf("%s", &n);
    strcpy(recurso.nombre, n);
    recurso.owner = (Usuario) CurrentUser;
    recursos[nrecursos] = recurso;
    nrecursos++;
    printf("Recurso Creado... \n");
    getch();
    ListarRecursos();
}

void ListarRecursos(){
    printf("\n Listar recursos" ); getch();
    int z = 0;
    Usuario un;
    for (z; z < nusuarios; z++){
         un = (struct Usuario)recursos[z].owner;
        printf("\n  Recurso: %s     Owner: %s", recursos[z].nombre, un.user);
    }
    getch();
}

Recurso getRecurso(char *nombre){
  int z = 0;
  for (z; z < nrecursos; z++){
      if (strcmp(&recursos[z].nombre, nombre) == 0)
        return recursos[z];
  }
}

void CrearUsuario(){
    char u[10];
    printf("\n   User [Max 10 caracteres]: ");
    scanf("%s", &u);
    printf("   Grupo [1-10]: ");
    scanf("%d", &usuario.grupo);
    strcpy(usuario.user, u);
    usuarios[nusuarios] = usuario;
    nusuarios++;
    printf("\n Usuario Creado");
    getch();
}

void ListarUsuarios(){
    printf("\n Listar usuarios" ); getch();
    int z = 0;
    //printf("nusuarios: %d\n", nusuarios);
    printf(" grupo: %d\n", usuarios[0].grupo);
    for (z; z < nusuarios; z++){
        printf("\n  Usuario: %s     Grupo: %d", usuarios[z].user, usuarios[z].grupo);
    }
    getch();
}

int Login(){
    char u[10];
    printf("\n   User: ");
    scanf("%s", &u);
    if (existe(u)==1){
      CurrentUser = (Usuario) getUser(u);
      return 1;
    }
    return 0;
}

int existe(char* u){
  int z = 0;
  for (z; z < nusuarios; z++){
      //printf("cmp: %d\n", strcmp(&usuarios[z].user, u));
      if (strcmp(&usuarios[z].user, u) == 0)
        return 1;
  }
  return 0;
}

Usuario getUser(char* u){
  int z = 0;
  for (z; z < nusuarios; z++){
      //printf("cmp: %d\n", strcmp(&usuarios[z].user, u));
      if (strcmp(&usuarios[z].user, u) == 0)
        return usuarios[z];
  }
}
