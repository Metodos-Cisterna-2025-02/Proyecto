#include <stdio.h>
#include <stdlib.h>
#include "search.h"

/*int altura_valida(){
    return 1;
}
*/

void coordenadas_inicio(int *x, int *y, int x_meta, int y_meta){
    int distancia_valida = 0;
    while(distancia_valida == 0){
        *x = rand()%8;
        if(*x == 0 || *x == 7){
            *y = rand()%8;
        }
        else{
            int a = rand()%2;
            if(a==0){
                *y = 0;
            }
            else{
                *y = 7;
            }
        }
        int distancia = distanceTo(*x, *y, x_meta, y_meta);
        if(distancia >= 4){
            distancia_valida = 1;
        }
    }
}
