#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "interfaz.h"

int main() {
    dispositivousuario misdispositivos[3];
    dispositivousuario dispositivosIA[3]; 
    
    FILE *milog = fopen("juego.txt", "w");

    if (milog == NULL) {
        printf("Error al abrir el archivo de log.\n");
        return 1;
    }

    srand(time(NULL));

    int turnousuario;
    int opcion;
    int yausodispositivo;
    int juegoactivo = 1;

    printf("\n   SORTEO INICIAL  \n");
    turnousuario = realizarsorteocarasello(milog);

    if (turnousuario) {
        printf("\nGANASTE EL SORTEO: Eliges primero.\n");
        
        printf("\nTURNO DE ELECCION DE DISPOSITIVOS: USUARIO\n");
        seleccionardispositivos(misdispositivos, milog);
        
        printf("\nTURNO DE ELECCION DE DISPOSITIVOS: IA\n");
        printf("La IA esta eligiendo sus dispositivos \n");
        
    } else {
        printf("\n>> PERDISTE EL SORTEO: La IA elige primero.\n");
        
        printf("\nTURNO DE ELECCION DE DISPOSITIVOS: IA\n");
        printf("La IA esta eligiendo sus dispositivos\n");
        
        printf("\nTURNO DE ELECCION DE DISPOSITIVOS: USUARIO\n");
        seleccionardispositivos(misdispositivos, milog);
    }

    mostrardispositivos(misdispositivos, 3);

    while (juegoactivo) {

        if (turnousuario) {
            
            printf("    TURNO DEL USUARIO      \n");
            
            
            yausodispositivo = 0;
            int finturno = 0;

            while (!finturno) {

                opcion = mostrarmenuturno(milog, yausodispositivo);

                if (opcion == 1) {
                    char movimiento = pedirmovimiento(milog);
                    printf("Seleccionaste : %c\n", movimiento);
                    
                    finturno = 1;
                }

                else if (opcion == 2 && yausodispositivo == 0) {
                    int idx;
                    
                    mostrardispositivos(misdispositivos, 3);
                    printf("Elige el numero del dispositivo a activar (1-3): ");
                    
                    if (scanf("%d", &idx) != 1) {
                        while (getchar() != '\n'); 
                        idx = 0; 
                    } else {
                        while (getchar() != '\n'); 
                    }

                    idx--; 

                    if (idx >= 0 && idx < 3) {
                        if (misdispositivos[idx].disponible == 1) {
                            
                            usardispositivo(&misdispositivos[idx], milog);
                            
                            yausodispositivo = 1; 
                            
                            

                        } else {
                            printf("ERROR: Ese dispositivo ya fue usado antes.\n");
                        }
                    } else {
                        printf("ERROR: Opcion invalida.\n");
                    }
                }

                else if (opcion == 3) {
                    printf(" El usuario ha decidido rendirse.\n");
                    registraraccion(milog, "Usuario", "Se rinde");
                    juegoactivo = 0; 
                    break;           
                }
            }

            if (juegoactivo) {
                turnousuario = 0; 
            }
        }

        else {
            if (juegoactivo) { 
                turnodeia(milog);
                turnousuario = 1; 
            }
        }
    }

    printf("\nFIN DEL JUEGO\n");
    fclose(milog);
    return 0;

}