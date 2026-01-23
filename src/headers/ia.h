#ifndef IA_H
#define IA_H

#include "definiciones.h"


int **mapa;


/* Definiciones de Funciones */

int ia_distancia_objetivo(int xStart, int yStart, int xEnd, int yEnd) {
	return distanceTo(xStart, yStart, xEnd, yEnd);
}

int ia_siguiente_paso(int xStart, int yStart, int xEnd, int yEnd, int *nx, int *ny) {
	struct pos* next = getNext(xStart, yStart, xEnd, yEnd);
	if (next == NULL)
		return 0;

	*nx = posGetX(*next);
	*ny = posGetY(*next);

	return 1;
}

void turnoIA(jugador *ia, jugador *jugador, int metaX, int metaY, int turno) {
	if (!mapa)
		mapa = getMap();

	int exito;

	int nx, ny;
	int puede_moverse = ia_siguiente_paso(ia->x, ia->y, metaX, metaY, &nx, &ny);

	printf("\n--- TURNO IA ---\n");
	
	if (ia->bloqueado == 1) {
		printf("IA esta bloqueada (Hydrus activo)\n");
		ia->bloqueado = 0; 
		exito = 0;

		return;
	}

	if (puede_moverse) {
		printf("IA se mueve de (%d, %d, %d) a (%d, %d, %d)\n", ia->x, ia->y, ia->h, nx, ny, mapa[ny][nx]);
		
		ActionType mov;
		if (ia->x - nx == -1) // x - (x + 1) = -1
			mov = MOVE_RIGHT;
		else if (ia->x - nx == 1) // x - (x - 1) = 1
			mov = MOVE_LEFT;
		else if (ia->y - ny == -1)
			mov = MOVE_DOWN;
		else if (ia->y - ny == 1)
			mov = MOVE_UP;
		
		ia->x = nx;
		ia->y = ny;
		ia->h = mapa[ny][nx];
		
		exito = 1;

		registerSimpleAction(turno, 2, mov, nx, ny, exito);
	} else {
		// TODO: IA no encontró camino, implementar lógica de rendición de IA
		ia->rendido = 1;

		printf("IA no encontro un camino valido hacia la meta.\n");
		registerSimpleAction(turno, 2, SURRENDER, ia->x, ia->y, 1);
	}
}

#endif
