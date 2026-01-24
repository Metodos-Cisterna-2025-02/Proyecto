#ifndef ESCENARIO_H
#define ESCENARIO_H

#include "definiciones.h"

/* Definiciones de funciones */
int altura_valida(int** escenario){
	int i, j, diferencia_altura = 0, es_valido;
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
        		es_valido = 0;
            		if(i>0){
                		diferencia_altura = abs(escenario[i][j] -escenario[i-1][j]);
                		if(diferencia_altura == 1)
                    			es_valido = 1;
            		}
            		if(i<7){
                		diferencia_altura = abs(escenario[i][j] -escenario[i+1][j]);
                		if(diferencia_altura == 1)
                    			es_valido = 1;
            		}
            		if(j>0){
                		diferencia_altura = abs(escenario[i][j] -escenario[i][j-1]);
                		if(diferencia_altura == 1)
                    			es_valido = 1;
            		}
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

void generar_escenario(int** escenario){
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
        	if(*x == 0 || *x == 7)
           		*y = rand()%8;
        	else{
            		int a = rand()%2;
            		if(a==0){
                		*y = 0;
            		}
            		else{
                		*y = 7;
            		}
        	}
        int distancia = abs(*x - x_meta) + abs(*y - y_meta);
        if(distancia >= 4)
        	distancia_valida = 1;
    	}
}

#endif // ESCENARIO_H
