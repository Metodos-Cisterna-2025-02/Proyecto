#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int altura_valida(int escenario[8][8]){
    int i, j, diferencia_altura = 0, es_valido;
    for(i=0; i<8; i++){
        for(j=0; j<8; j++){
            es_valido = 0;
            //Mira la diferencia de altura con la casilla de arriba
            if(i>0){
                diferencia_altura = abs(escenario[i][j] -escenario[i-1][j]);
                if(diferencia_altura == 1)
                    es_valido = 1;
            }
            //Mira la diferencia de altura con la casilla de abajo
            if(i<7){
                diferencia_altura = abs(escenario[i][j] -escenario[i+1][j]);
                if(diferencia_altura == 1)
                    es_valido = 1;
            }
            //Mira la diferencia de altura con la casilla izquierda
            if(j>0){
                diferencia_altura = abs(escenario[i][j] -escenario[i][j-1]);
                if(diferencia_altura == 1)
                    es_valido = 1;
            }
            //Mira la diferencia de altura con la casilla derecha
            if(j<7){
                diferencia_altura = abs(escenario[i][j] -escenario[i][j+1]);
                if(diferencia_altura == 1)
                    es_valido = 1;
            }
            if(es_valido == 0)
                return 0;
        }
    }
    return 1;
}
void generar_escenario(int escenario[8][8]){
    int i, j, referencia;
    for(i=0; i<8; i++){
        for(j=0; j<8; j++){
            if((i<4 && j<4) || (i>=4 && j>=4)){
                escenario[i][j] = rand()%6;
            }
            else{
                if((i==0 && j==4) || (i==4 && j==0)){
                    escenario[i][j] = rand()%6;
                }
                else{
                    if(j>0 && j!=4){
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
        int distancia_x = abs(*x - x_meta);
        int distancia_y = abs(*y - y_meta);
        if((distancia_x + distancia_y) >= 4){
            distancia_valida = 1;
        }
    }
}

int main(){
    srand(time(NULL));
    int escenario[8][8];
    int i, j;
    do{
        generar_escenario(escenario);
    }
    while(altura_valida(escenario) == 0);
    int x_jugador, y_jugador, x_ia, y_ia, x_meta, y_meta;
    //Se genera las coordenadas de la meta
    x_meta = rand()%8;
    y_meta = rand()%8;
    //Genera las coordenadas de inicio del jugador
    coordenadas_inicio(&x_jugador, &y_jugador, x_meta, y_meta);
    //Genera las coordenadas de inicio de la IA
    //Si las coordenadas de ambos son iguales, cambia la posicion de la IA
    do{
    coordenadas_inicio(&x_ia, &y_ia, x_meta, y_meta);
    }
    while(x_ia == x_jugador && y_ia == y_jugador);
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