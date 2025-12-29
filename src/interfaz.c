#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//struct dispositvos


int main(){
    // inciar generador de numeros aleatorios
    srand(time(NULL));
    //opcion del menu inicial
    int opcion=0;
    // menu inicial
    while (opcion!=3){
        printf("--------Menu-Inicial-------\n");
        printf("1-Iniciar nueva partida \n");
        printf("2-Ver reglas \n");
        printf("3-Salir \n");
        printf("Seleccione una opcion: ");
        
        if (scanf("%d",&opcion)!=1){
            //scanf devuelve cuantos valores se leyeron correctamente
         while (getchar()!='\n');
         printf("opcion no valida  \n ");
         continue;
        }
        if(opcion==2){
         printf("Reglas\n");

        }
        if(opcion==3){
         printf("Saliendo del juego...\n");
         break;
        }else if (opcion==1){
            int monedajugador,resultadomoneda;
            printf("SORTEO\n");
            do {
            printf("Elige 1-SELLO , 2-CARA  : ");
             if (scanf("%d", &monedajugador) != 1){
             printf("Entrada invalida. Debe ser numero.\n");
             while (getchar() != '\n');
             continue;
             }
             if (monedajugador != 1 && monedajugador != 2){
              printf("Opcion invalida. Solo 1 o 2.\n");
             }
             resultadomoneda=(rand()%2)+1;
            } while (monedajugador != 1 && monedajugador != 2);
        }
    return 0;

    }
}
