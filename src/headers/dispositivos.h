#ifndef DISPOSITIVOS_H
#define DISPOSITIVOS_H

#include <stdio.h>
#include <stdlib.h>

static void usardispositivojugador(jugador *j, jugador *oponente) {
    printf("\n[SISTEMA] Abriendo interfaz de dispositivos...\n");
    // Lógica de dispositivos aquí
    // Hacer registro de uso de dispositivo generico
    registerCompleteAction(-1, 1, USE_DEVICE, GAIUS, j->x, j->y, 0, 0, 0, 0, NONE, NONE, 1);
}

#endif