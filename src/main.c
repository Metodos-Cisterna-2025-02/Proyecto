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
#include "headers/escenario.h"

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
	int escenario[8][8];
	do{
		generar_escenario(escenario);
	}while(altura_valida(escenario) == 0);

	mapa = (int**) malloc(sizeof(int*)* 8); 
    for (int i = 0; i < 8; i++) {
        mapa[i] = (int*) malloc(sizeof(int)* 8);
        for (int j = 0; j < 8; j++) {
            mapa[i][j] = escenario[i][j];
        }
    }
	mapLoaded = 1;
	searchMap = mapa;
	int metaX = rand()%8;
	int metaY = rand()%8;

	jugador player;
	coordenadas_inicio(&player.x, &player.y, metaX, metaY);
	player.h = mapa[player.y][player.x];
	player.bloqueado = 0;
	player.rendido = 0;
	player.puedeSubir3 = 0;
	

	printf("--- BIENVENIDO AL PROYECTO DE PROGRAMACION ---\n");
	seleccionardispositivos(player.inventario);

	jugador ia;
	do{
		coordenadas_inicio(&ia.x, &ia.y, metaX, metaY);
	}while(ia.x == player.x && ia.y == player.y);
	ia.h = mapa[ia.y][ia.x];
	ia.bloqueado = 0;
	ia.rendido = 0;
	ia.puedeSubir3 = 0;
	

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

	//freeMap(mapa);
	for (int i = 0; i < 8; i++) {
        free(mapa[i]); 
    }
    // Liberar el puntero de punteros
    free(mapa);
	printf("\nJuego terminado.\n");
	
	return 0;
}
