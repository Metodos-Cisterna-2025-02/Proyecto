#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 


typedef struct {
    char nombre[20];
    int elegido;   // 1 = el jugador lo eligio al inicio
    int usado;     // 1 = ya fue usado en la partida
} dispositivo;




void reiniciardispositivos(dispositivo d[]) {
    strcpy(d[0].nombre, "Gaius");    
				d[0].elegido = 0; 
				d[0].usado = 0;
    strcpy(d[1].nombre, "Quadratus"); 
				d[1].elegido = 0; 
				d[1].usado = 0;
    strcpy(d[2].nombre, "Hydrus");   
				d[2].elegido = 0; 
				d[2].usado = 0;
    strcpy(d[3].nombre, "Phalanx");  
				d[3].elegido = 0; 
				d[3].usado = 0;
    strcpy(d[4].nombre, "Argus");   
				d[4].elegido = 0; 
				d[4].usado = 0;
}

void mostrardispositivos(dispositivo d[], int n) {
    printf("\n----Dispositivos----\n");
    for (int i = 0; i < n; i++) {
        printf("%d - %s [", i + 1, d[i].nombre);
        if (d[i].elegido)
            printf("ELEGIDO");
        else
            printf("NO ELEGIDO");
        printf("]\n");
    }
}

void elegirdispositivos(dispositivo d[], int n) {
    int elegidos = 0;
    int opcion;

    while (elegidos < 3) {
        mostrardispositivos(d, n);
        printf("Elige dispositivo %d: ", elegidos + 1);

        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (opcion < 1 || opcion > n || d[opcion - 1].elegido==1) {
            printf("Opcion invalida.\n");
            continue;
        }

        d[opcion - 1].elegido = 1;
        printf("Seleccionaste %s.\n", d[opcion - 1].nombre);
        elegidos++;
    }
}

void moverjugador() {
    char dir;
    printf("Mover (W A S D): ");
    scanf(" %c", &dir);

    
    if (dir == 'w' || dir == 'W') {
        printf("Te mueves ARRIBA.\n");
    }
    
    else if (dir == 's' || dir == 'S') {
        printf("Te mueves ABAJO.\n");
    }
    
    else if (dir == 'a' || dir == 'A') {
        printf("Te mueves IZQUIERDA.\n");
    }
    
    else if (dir == 'd' || dir == 'D') {
        printf("Te mueves DERECHA.\n");
    }
    
    else {
        printf("Direccion invalida.\n");
    }
}

void usardispositivo(dispositivo d[], int n) {
    int opcion;
    int haydisponibles = 0;

    printf("\n----Dispositivos disponibles----\n");
    for (int i = 0; i < n; i++) {
        if (d[i].elegido && !d[i].usado) {
            printf("%d - %s\n", i + 1, d[i].nombre);
            haydisponibles = 1;
        }
    }

    if (haydisponibles==0) {
        printf("No te quedan dispositivos disponibles.\n");
        return;
    }

    printf("Elige un dispositivo: ");
    if (scanf("%d", &opcion) != 1) {
        while (getchar() != '\n');
        return;
    }

    if (opcion < 1 || opcion > n || !d[opcion - 1].elegido || d[opcion - 1].usado==1) {
        printf("No puedes usar ese dispositivo.\n");
        return;
    }

    d[opcion - 1].usado = 1;
    printf("Usaste el dispositivo %s.\n", d[opcion - 1].nombre);
}


int main() {
    srand(time(NULL));

   
    dispositivo dispositivos[5];
    reiniciardispositivos(dispositivos); 
    
    int turno = 0;            
    int partidasuspendida = 0; 
    int opcion = 0;
    int entraraljuego = 0;     

    while (opcion != 3) {
        entraraljuego = 0; 

        printf("\n-------- MENU INICIAL --------\n");
        printf("1 - Iniciar nueva partida\n");
        printf("2 - Ver reglas\n");
        printf("3 - Salir\n");
        if (partidasuspendida==1) {
            printf("4 - Reanudar\n");
        }
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (opcion == 2) {
            printf("\nREGLAS:\n");
            printf("1 - \n");
            printf("2 - \n");
            continue;
        }

        if (opcion == 3) {
            break; 
        }

        
        if (opcion == 1) {
            if (partidasuspendida) {
                printf("Partida anterior borrada.\n");
            }
            
            reiniciardispositivos(dispositivos); 

            int monedajugador, resultadomoneda;
            printf("\n----SORTEO----\n");
            do {
                printf("Elige 1-SELLO | 2-CARA: ");
                if (scanf("%d", &monedajugador) != 1) while (getchar() != '\n');
            } while (monedajugador != 1 && monedajugador != 2);

            resultadomoneda = (rand() % 2) + 1;
            turno = (monedajugador == resultadomoneda);
            printf(turno ? "Partes tu.\n" : "Parte la IA.\n");

            elegirdispositivos(dispositivos, 5);
            
            entraraljuego = 1;
            partidasuspendida = 0;
        }
        
        
        else if (opcion == 4 && partidasuspendida==1) {
            printf("Reanudando partida...\n");
            entraraljuego = 1;
        }

       
        if (entraraljuego==1) {
            int juegoactivo = 1;

            while (juegoactivo==1) {

                if (turno == 1) { 
                    
                    int yasemovio = 0;
                    int usodispositivo = 0;
                    int opcionturno;

                    printf("\n----TU TURNO----\n");

                    while (yasemovio==0) {
                        printf("1 - Moverse\n");
                        printf("2 - Usar dispositivo\n");
                        printf("3 - Rendirse\n");
                        printf("4 - Pausar juego\n");
                        printf("Opcion: ");

                        if (scanf("%d", &opcionturno) != 1) {
                            while (getchar() != '\n');
                            continue;
                        }

                        switch (opcionturno) {
                            case 1:
                                moverjugador();
                                yasemovio = 1;
                                break;
                            case 2:
                                if (usodispositivo==0) {
                                    usardispositivo(dispositivos, 5);
                                    usodispositivo = 1;
                                } else {
                                    printf("Ya usaste un dispositivo este turno.\n");
                                }
                                break;
                            case 3:
                                printf("Te rendiste. Gana la IA.\n");
                                juegoactivo = 0;       
                                partidasuspendida = 0; 
                                yasemovio = 1;
                                break;
                            case 4:
                                printf("\n----PAUSA----\n");
                                partidasuspendida = 1; 
                                juegoactivo = 0;       
                                yasemovio = 1;         
                                break;
                            default:
                                printf("Opcion invalida.\n");
                        }
                    }

                } else {
                    
                    printf("\n----TURNO DE LA IA----\n");
                    printf("La IA realiza su movimiento.\n");
                    
                }

               
                if (juegoactivo==1) {
                    turno = !turno; 
                }
            }
        }
    }
    printf("\nSaliendo del juego...\n");
    return 0;
}