#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "headers/io.h"
#include "headers/interfaz.h"
#include "headers/jugador.h"
#include "headers/search.h"
#include "headers/ia.h"

void printMapWithPlayers(int **mapa, jugador *j, jugador *ia, int metaX, int metaY) {
    // Limpia la consola para que el mapa parezca "animado"
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("\n========================================\n");
    printf("            ESTADO DEL JUEGO            \n");
    printf("========================================\n");
    printf(" Jugador (J) en: (%d, %d, %d) | IA (I) en: (%d, %d, %d)\n", j->x, j->y, ia->x, ia->y);
    printf(" Meta (M) en: (%d, %d)\n", metaX, metaY);
    printf("----------------------------------------\n\n");

    for (int y = 0; y < MAP_TEST_SIZE; y++) {
        for (int x = 0; x < MAP_TEST_SIZE; x++) {
            /* Prioridad visual: Jugador > IA > Meta > Altura */
            if (j->x == x && j->y == y) {
                printf("J  ");
            } else if (ia->x == x && ia->y == y) {
                printf("I  ");
            } else if (x == metaX && y == metaY) {
                printf("M  ");
            } else {
                printf("%d  ", mapa[y][x]);
            }
        }
        printf("\n");
    }
    printf("\n========================================\n");
}

int main() {
    srand(time(NULL));

    /* 1. CARGAR MAPA */
    int **mapa = getMap(); // Asegúrate de que map_test.txt no tenga el ""
    if (mapa == NULL) {
        printf("Error: No se pudo cargar el mapa.\n");
        return 1;
    }

    /* 2. DEFINIR META */
    int metaX = 7;
    int metaY = 7;

    /* 3. INICIALIZAR JUGADOR */
    jugador player;
    player.x = 0;
    player.y = 0;
    player.h = 1;
    player.bloqueado = 0;
    player.rendido = 0;
    player.puedeSubir3 = 0;

    printf("--- BIENVENIDO AL PROYECTO DE PROGRAMACION ---\n");
    seleccionardispositivos(player.inventario);

    /* 4. INICIALIZAR IA */
    jugador ia;
    ia.x = 0;
    ia.y = 7;
    ia.h = 5;
    ia.bloqueado = 0;
    ia.rendido = 0;
    ia.puedeSubir3 = 0;

    /* 5. LOOP PRINCIPAL */
    FILE *log = NULL; 
    int juegoTerminado = 0;

    while (!juegoTerminado) {
        // Mostramos el mapa al inicio de cada ciclo
        printMapWithPlayers(mapa, &player, &ia, metaX, metaY);

        /* TURNO JUGADOR */
        printf("\n>> ES TU TURNO\n");
        turno_jugador(&player, &ia, mapa);

        if (player.rendido) {
            printf("\nTe has rendido. Gana la IA.\n");
            juegoTerminado = 1;
            continue;
        }

        // Mostramos el mapa tras tu movimiento para que veas el resultado
        printMapWithPlayers(mapa, &player, &ia, metaX, metaY);

        if (player.x == metaX && player.y == metaY) {
            printf("\n¡FELICIDADES! Has llegado a la meta.\n");
            juegoTerminado = 1;
            continue;
        }

        // Pausa necesaria para que el usuario vea su posición antes del turno de la IA
        printf("\nPresiona Enter para que la IA mueva...");
        while (getchar() != '\n'); 
        getchar(); 

        /* TURNO IA */
        printf("\n>> TURNO DE LA IA\n");
        turnoIA(&ia, &player, mapa, metaX, metaY);

        if (ia.x == metaX && ia.y == metaY) {
            printMapWithPlayers(mapa, &player, &ia, metaX, metaY);
            printf("\nLa IA ha llegado a la meta. Gana la máquina.\n");
            juegoTerminado = 1;
        }
    }

    /* 6. LIMPIEZA */
    freeMap(mapa); // Liberación de memoria dinámica
    printf("\nJuego terminado.\n");

    return 0;
}