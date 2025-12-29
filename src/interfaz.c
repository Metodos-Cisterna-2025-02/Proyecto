#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct{
 char nombre[50];
 char raza[50];
 int edad;
 char sexo[50];
 int vacunado;
 }perritos;
 
 int main(){
  perritos perro[100];
  int n,i,opcion;
  char textobuscar[50];
  printf("Ingrse el numero de perritos :  \n");
  scanf("%d",&n);
  for(i=0,i<n,i++){
   printf("Perrito %d ": i+1);
   printf("Nombre: ");
   scanf("%s\n",perro[i].nombre);
   printf("Raza : ");
   scanf("%s\n", perro[i].raza);
   printf(" Edad : ");
   scanf("%d\n",&perro[i].edad);
   printf("Raza :);
   scanf("%s\n",perro[i].sexo);
   printf("Vacunado (1 si , 0 no): ");
   scanf("%d\n",perro[i].&vacunado);
  }
 
  do {
   printf("MENU\n");
   printf(" Presiona 1 para buscar por nombre \n");
   printf(" Presiona 2 para mostrar por sexo \n");
   printf(" Presiona 1 para mostar vacunados\n");
   switch(opcion){
    case 1:
printf("Ingrese nombre a buscar \n");
scanf("%s",textobuscar);
for(i=0;i<n;i++){
 if(strcmp(perro[i].nombre),textobuscar)==0){
  printf("Encontrado: %s, Raza: %s, Edad: %d , Sexo: %s , Vacunado: %s --\n", perros[i].nombre, perros[i].raza, perros[i].edad);
 }
}
break;
case 2:
printf("Ingrese sexo  : \n");
scanf("%s",textobuscar);
for(i=0;i<n;i++){
 if(strcmp(perro[i].nombre),textobuscar)==0){
  printf("%s",perro[i].nombre);
 }
}
case 3:
      printf("Perritos vacunados:\n");
      for(i = 0; i < n; i++) {
       if(perros[i].vacunado == 1) {
        printf("- %s\n", perros[i].nombre);
       }
     
 }
 break;

      case 4:
       printf("Adios!\n");
       break;

      default:
       printf("Opcion incorrecta.\n");
     }
    } while(opcion != 4);

    return 0;
}

