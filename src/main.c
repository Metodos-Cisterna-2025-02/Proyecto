#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "headers/definiciones.h"
#include "headers/io.h"
#include "headers/interfaz.h"
#include "headers/jugador.h"
#include "headers/search.h"
#include "headers/ia.h"
#include "headers/log.h"
#include "headers/dispositivos.h"

int** mapa;

void printMapWithPlayers(jugador *j, jugador *ia, int metaX, int metaY) {
	// Limpia la consola cada vez que un jugador se mueve
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif

	if (!mapa)
		mapa = getMap();

	printf("\n========================================\n");
	printf("            ESTADO DEL JUEGO            \n");
	printf("========================================\n");
	printf(" Jugador (J) en: (%d, %d, %d) | IA (I) en: (%d, %d, %d)\n", j->x, j->y, j->h, ia->x, ia->y, ia->h);
	printf(" Meta (M) en: (%d, %d)\n", metaX, metaY);
	printf("----------------------------------------\n\n");

	for (int y = 0; y < MAP_TEST_SIZE; y++) {
		for (int x = 0; x < MAP_TEST_SIZE; x++) {
			/* Prioridad visual: Jugador > IA > Meta > Altura */
			if (j->x == x && j->y == y)
				printf("J  ");
			else if (ia->x == x && ia->y == y)
				printf("I  ");
			else if (x == metaX && y == metaY)
				printf("M  ");
			else
				printf("%d  ", mapa[y][x]);
		}
		printf("\n");
	}
	printf("\n========================================\n");
}

int main(int argc, char *argv[]) {
	srand(time(NULL));

	if (!mapa)
		mapa = getMap();

	if (mapa == NULL) {
		printf("Error: No se pudo cargar el mapa.\n");
		return 1;
	}

	int metaX = 7;
	int metaY = 7;

	jugador player;
	player.x = 0;
	player.y = 0;
	player.h = 1;
	player.bloqueado = 0;
	player.rendido = 0;
	player.puedeSubir3 = 0;

	
	

	jugador ia;
	ia.x = 0;
	ia.y = 7;
	ia.h = 5;
	ia.bloqueado = 0;
	ia.rendido = 0;
	ia.puedeSubir3 = 0;

	printf("--- BIENVENIDO AL PROYECTO DE PROGRAMACION ---\n");
    int sorteo = realizarsorteocarasello();

    if (sorteo==1) {
        printf("\nGanaste el sorteo - Eliges tus dispositivos primero.\n");
        
        seleccionardispositivos(player.inventario);
        
        seleccionardispositivosIA(ia.inventario, player.inventario, 1);
    } else {
        printf("\nPerdiste - La IA elige sus dispositivos primero.\n");
        
        seleccionardispositivosIA(ia.inventario, player.inventario, 0);
        
        seleccionardispositivos(player.inventario);
    }

	int turno = 1;
	int juegoTerminado = 0;

	while (!juegoTerminado) {
		// Mostrar el mapa
		printMapWithPlayers(&player, &ia, metaX, metaY);

		//turno jugador
		printf("\n>> ES TU TURNO\n");
		turno_jugador(&player, &ia, turno);


		//cambio de estado rendido y termina el juego
		if (player.rendido) {
			printf("\nTe has rendido. Gana la IA.\n");
			juegoTerminado = 1;
			continue;
		}

		printMapWithPlayers(&player, &ia, metaX, metaY);

		if (player.x == metaX && player.y == metaY) {
			printf("\n HAS GANADO!!!! Llegaste a la meta.\n");
			juegoTerminado = 1;
			continue;   //No sera mejor un break?
		}

		// Pausa necesaria para que el usuario vea su posición antes del turno de la IA
		printf("\nPresiona Enter para que la IA mueva...");
		while (getchar() != '\n'); 
		getchar(); 

		//turno ia
		printf("\n>> TURNO DE LA IA\n");
		turnoIA(&ia, &player, metaX, metaY, turno);

		//perdistes
		if (ia.x == metaX && ia.y == metaY) {
			printMapWithPlayers(&player, &ia, metaX, metaY);
			printf("\nLa IA ha llegado a la meta. Gana la máquina.\n");
			juegoTerminado = 1;
		}

		turno++;
	}

	freeMap(mapa);
	printf("\nJuego terminado.\n");

	return 0;
}
