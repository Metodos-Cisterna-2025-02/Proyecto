#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generar_escenario(int escenario[8][8]){
    int i, j, referencia;
    for(i=0; i<8; i++){
        for(j=0; j<8; j++){
            if(i == 0 && j == 0){
                escenario[0][0] = rand()%6;    //primera casilla del escenario con altura aleatoria
            }
            else{
                if(j>0){
                    referencia = escenario[i][j-1];    //si no es la primera columna, toma de referencia la casilla izquierda
                }
                else{
                    referencia = escenario[i-1][j];    //si es la primera columna, toma de referencia la casilla de arriba
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
    //mientras no haya una distancia valida, se generan nuevas coordenadas repetidamente
    while(distancia_valida == 0){   
        *x = rand()%8;    //genera coordenada x aleatoria para el jugador o IA
        if(*x == 0 || *x == 7){
            *y = rand()%8;   //si x es borde superior o inferior, genera coordenada y aleatoria
        }
        else{
            int a = rand()%2;  //si x no es borde superior o inferior, genera coordenada y en uno de los bordes laterales
            if(a==0){     
                *y = 0;
            }
            else{
                *y = 7;
            }
        }
        int distancia_x = abs(*x - x_meta);
        int distancia_y = abs(*y - y_meta);
        if((distancia_x + distancia_y) >= 4){    //verifica que la distancia directa a la meta sea como minimo 4
            distancia_valida = 1;
        }//si cumple modifica las coordenadas declaradas en main
    }
}


int main(){
    srand(time(NULL));
    int escenario[8][8];
    int i,j;
    int altura;
    generar_escenario(escenario);
    //Define las variables para las coordenadas
    int x_jugador, y_jugador, x_ia, y_ia, x_meta, y_meta;
    //Se genera las coordenadas de la meta
    x_meta = rand()%8;
    y_meta = rand()%8;
    //Genera las coordenadas de inicio del jugador
    coordenadas_inicio(&x_jugador, &y_jugador, x_meta, y_meta);
    //Genera las coordenadas de inicio de la IA
    coordenadas_inicio(&x_ia, &y_ia, x_meta, y_meta);
    //Si las coordenadas de ambos son iguales, cambia la posicion de la IA
    if(x_ia == x_jugador && y_ia == y_jugador){
        while(x_ia == x_jugador && y_ia == y_jugador){
            coordenadas_inicio(&x_ia, &y_ia, x_meta, y_meta);
        }
    }
    //Muestra escenario
    printf("\nESCENARIO\n\n");
    for(i=0; i<8; i++){
        for(j=0; j<8; j++){
            printf("%d ", escenario[i][j]);
        }
        printf("\n");
    }
    //Muestra las posiciones de inicio y la meta
    printf("\nPosicion inicio jugador: %d , %d\n", x_jugador, y_jugador);
    printf("Posicion inicio IA: %d , %d\n", x_ia, y_ia);
    printf("Posicion meta: %d , %d", x_meta, y_meta);
    return 0;
}

