#ifndef JUGADOR_H
#define JUGADOR_H



#include <stdio.h>
#include "interfaz.h"
#include "dispositivos.h"


typedef struct dispositivousuario dispositivousuario; //avisamos que existe el struck de joaco-pancho

typedef struct { // con item
    int x;
    int y;

    int bloqueado;
    int rendido;
    int puedeSubir3;

    dispositivousuario inventario[3];  
} jugador;



void turno_jugador(jugador *j, jugador *oponente, int mapa[8][8], FILE *log);  
static int mover_jugador(jugador *j, char direccion, int mapa[8][8]);
static int es_movimiento_valido(jugador *j, int nx, int ny, int mapa[8][8]);
static void usardispositivojugador(jugador *j, jugador *oponente, FILE *log);





void turno_jugador(jugador *j, jugador *oponente, int mapa[8][8], FILE *log) {   // oponente es IA

    int turnoTerminado = 0;
    int yaUsoDispositivo = 0;
    int opcion;
    char direccion;

    while (!turnoTerminado && !j->rendido) {

        opcion = mostrarmenuturno(log, yaUsoDispositivo);

        if (opcion == 1) { // MOVERSE

            if (j->bloqueado) {
                printf("Estas bloqueado, no puedes moverte\n");
            } else {
                direccion = pedirmovimiento(log);

                if (mover_jugador(j, direccion, mapa)) {
                    turnoTerminado = 1;
                }
            }

        } else if (opcion == 2) { // USAR DISPOSITIVO

            usardispositivojugador(j, log);                     // FALTAAAAAAAAAAAAAAA?
            yaUsoDispositivo = 1;

        } else if (opcion == 3) { // RENDIRSE

            j->rendido = 1;
            turnoTerminado = 1;
        }
    }

    if (j->bloqueado) {    // para que cuando se aplique el bloqueo de Hydrux se resetee
        j->bloqueado = 0;
    }
}


static int mover_jugador(jugador *j, char direccion, int mapa[8][8]) {

    int nx = j->x;
    int ny = j->y;

    if (direccion == 'w') ny--;
    else if (direccion == 's') ny++;
    else if (direccion == 'a') nx--;
    else if (direccion == 'd') nx++;

    if (es_movimiento_valido(j, nx, ny, mapa)) {
        j->x = nx;
        j->y = ny;
        return 1;
    }

    return 0;

}


static int es_movimiento_valido(jugador *j, int nx, int ny, int mapa[8][8]) {
    
    int x = j->x;
    int y = j->y;

    // 1. Verificar límites del mapa
    if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) {
        return 0;
    }

    // Verificamos que sea movimiento horizontal o vertical
    if (!((nx == x && ny == y + 1) ||

          (nx == x && ny == y - 1) ||

          (nx == x + 1 && ny == y) ||


          (nx == x - 1 && ny == y))) {
        return 0;
    }

    //  Verificar diferencia de altura
    int alturaActual = mapa[y][x];
    int alturaDestino = mapa[ny][nx];

    int diferencia = alturaDestino - alturaActual;


// CASO SUBIR
if (diferencia > 0) {

    if (diferencia <= 2) {
        return 1; // subida normal bien
    }

    // Phalanx activo permite subir hasta 3
    if (diferencia == 3 && j->puedeSubir3) {
        j->puedeSubir3 = 0; // efecto se consume
        printf("Efecto Phalanx se realizo");
        return 1;
    }

    return 0;
}

// CASO BAJAR
if (diferencia < 0) {

    if (-diferencia <= 2) {  //valor abs para ver bajada 
        return 1;
    }

    return 0;
}


return 1;
}




static void usardispositivojugador(jugador *j, jugador *oponente, FILE *log) {

    int opcion;
    dispositivousuario *disp;

    mostrardispositivos(j->inventario, 3);  // muestra

    printf("Seleccione dispositivo a usar (1-3): ");
    scanf("%d", &opcion);
    while (getchar() != '\n');

    if (opcion < 1 || opcion > 3) {
        printf("Opcion invalida\n");
        return;
    }

    disp = &j->inventario[opcion - 1];

    if (disp->disponible) {
        usardispositivo(disp, log);          // marca usado + log
        llamardispositivo()
             
    } else {
        printf("Ese dispositivo ya fue usado o no esta disponible\n");
    }
}








#endif
