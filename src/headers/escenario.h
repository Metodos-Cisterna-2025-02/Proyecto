#ifndef ESCENARIO_H
#define ESCENARIO_H

#include <stdio.h>
#include <stdlib.h>
#include "search.h"
//falta busqueda de caminos validos para ambos jugadores

void generar_escenario(int escenario[8][8]){
    int i, j, referencia;
    for(i=0; i<8; i++){
        for(j=0; j<8; j++){
            if(i == 0 && j == 0){
                escenario[0][0] = rand()%6;
            }
            else{
                if(j>0){
                    referencia = escenario[i][j-1];
                }
                else{
                    referencia = escenario[i-1][j];
                }
                if(referencia == 0){
                    escenario[i][j] = 1;
                }
                else if(referencia == 5){
                    escenario[i][j] = 4;
                }
                else{
                    if(rand()%2 == 0){
                        escenario[i][j] = referencia + 1;
                    }
                    else{
                        escenario[i][j] = referencia - 1;
                    }
                }
            }
        }
    }
}

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

#endif // ESCENARIO_H
