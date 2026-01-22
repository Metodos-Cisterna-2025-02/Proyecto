#ifndef IA_H
#define IA_H

#include <stdio.h>
#include "search.h"
#include "jugador.h"
#include "log.h"

// Ahora reciben el mapa para pasárselo a search()
int ia_distancia_objetivo(int xStart, int yStart, int xEnd, int yEnd, int **mapa) {
    search(xStart, yStart, xEnd, yEnd, mapa);
    return distanceTo(xStart, yStart, xEnd, yEnd);
}

int ia_siguiente_paso(int xStart, int yStart, int xEnd, int yEnd, int **mapa, int *nx, int *ny) {
    search(xStart, yStart, xEnd, yEnd, mapa);

    struct pos* next = getNext(xStart, yStart, xEnd, yEnd);
    if (next == NULL)
        return 0;

    *nx = posGetX(*next);
    *ny = posGetY(*next);
    return 1;
}

void turnoIA(jugador *ia, jugador *jugador, int **mapa, int metaX, int metaY) {
    printf("\n--- TURNO IA ---\n");

    if (ia->bloqueado == 1) {
        printf("IA esta bloqueada (Hydrus activo)\n");
        ia->bloqueado = 0; 
        return;
    }

    int nx, ny;
    // IMPORTANTE: Pasamos el mapa y actualizamos las coordenadas reales
    if (ia_siguiente_paso(ia->x, ia->y, metaX, metaY, mapa, &nx, &ny) == 1) {
        printf("IA se mueve de (%d, %d) a (%d, %d)\n", ia->x, ia->y, nx, ny);
        
        ActionType mov;
        if (ia->x - nx == -1) // x - (x + 1) = -1
            mov = MOVE_RIGHT;
        else if (ia->x - nx == 1) // x - (x - 1) = 1
            mov / MOVE_LEFT;
        else if (ia->y - ny == -1)
            mov = MOVE_DOWN;
        else if (ia->y - ny == 1)
            mov = MOVE_UP;
        
            
        
        ia->x = nx;
        ia->y = ny;

            registerSimpleAction(-1, 2, mov, nx, ny, 1);
    } else {
        printf("IA no encontro un camino valido hacia la meta.\n");
    }
}

#endif
