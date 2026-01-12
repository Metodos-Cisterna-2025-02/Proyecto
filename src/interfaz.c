#include <stdio.h>
#include <string.h> // 
#include <stdlib.h> // 
#include <time.h>   // 
#include <ctype.h>
#include "interfaz.h"


int realizarsorteocarasello() {
    int eleccionusuario;
    int resultadomoneda;
    int resultadoscan; 

   
    srand(time(NULL)); 

    printf("\n----SORTEO CARA O SELLO----\n");
    printf("Elige una opcion:\n");
    printf("1. Cara\n");
    printf("2. Sello\n");

    
    while (1) {
        printf("Opcion: ");
        resultadoscan = scanf("%d", &eleccionusuario);

        if (resultadoscan == 0) {
            
            printf("Error: numero no valido.\n");
            
            
            while (getchar() != '\n'); 
        } 
        else if (eleccionusuario < 1 || eleccionusuario > 2) {
            
            printf("Error:  Elige 1 o 2.\n");
        } 
        else {
            
            break; 
        }
    }
    
    resultadomoneda = (rand() % 2) + 1;

    
    if (resultadomoneda == 1) {
        printf("Resultado Sorteo: CARA\n");
    } else {
        printf("Resultado Sorteo: SELLO\n");
    }

    if (eleccionusuario == resultadomoneda) {
        printf("Ganaste el sorteo\n");
        return 1; 
    } else {
        printf("Perdiste el sorteo.\n");
        return 0; 
    }
}
// devuelve 0 si el usuario perdio
// devuelve 1 si el ususraiogano

void menuseleccioninicialdispositivos(char dispositivoselegidos[][50]) {
    int seleccion;
    int contador = 0;


    char nombresdispositivos[5][50] = {
        "1. Gaius ",
        "2. Quadratus ",
        "3. Hydrus ",
        "4. Phalanx ",
        "5. Argus )"
    };

    printf("\n----SELECCION DE DISPOSITIVOS----\n");
    printf("Debes elegir 3 dispositivos \n");

    while (contador < 3) {
        printf("\nDisponibles:\n");
        
        for (int i = 0; i < 5; i++) {
            printf("%s\n", nombresdispositivos[i]);
        }

        printf("Selecciona el dispositivo #%d: ", contador + 1);
        int leido = scanf("%d", &seleccion);

     
        if (leido == 0) {
            printf("Error: Ingresa el numero de la opcion.\n");
            while (getchar() != '\n'); 
            continue;
        }

       
        if (seleccion < 1 || seleccion > 5) {
            printf("Opcion fuera de rango (1-5).\n");
        } else {
            
            int indice = seleccion - 1;
            int duplicado = 0;

            
            for (int k = 0; k < contador; k++) {
                if (strcmp(dispositivoselegidos[k], nombresdispositivos[indice]) == 0) {
                    duplicado = 1;
                    break;
                }
            }

            if (duplicado==1) {
                printf("Dispositivo ya elegido\n");
            } else {
                
                strcpy(dispositivoselegidos[contador], nombresdispositivos[indice]);
                contador++;
                printf("Dispositivo agregado\n");
            }
        }
    }
}
// se eligen 3 dispositivo