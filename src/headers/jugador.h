#ifndef JUGADOR_H
#define JUGADOR_H

#include "definiciones.h"

int** mapa;

/* Prototipos de funciones estáticas */
static int mover_jugador(jugador *j, char direccion, int turno);
static int es_movimiento_valido(jugador *j, int nx, int ny);
static void usardispositivojugador(int turno, jugador *j, jugador *oponente);

/* Definiciones de funciones */

static int es_movimiento_valido(jugador *j, int nx, int ny) {
	if (!mapa)
		mapa = getMap();

	if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8)
		return 0;

	int h_actual = mapa[j->y][j->x];
	int h_nueva = mapa[ny][nx];
	int diferencia = abs(h_nueva - h_actual);

	if (j->puedeSubir3)
		if (h_nueva - h_actual <= 3)
			return 1;

	if (diferencia > 2)
		return 0;

	return 1;
}

static int mover_jugador(jugador *j, char direccion, int turno) {
	if (!mapa)
		mapa = getMap();

	int nx = j->x;
	int ny = j->y;
	int nh = mapa[j->y][j->x];

	ActionType mov;

	if (direccion == 'W' || direccion == 'w') {
		ny--;
		mov = MOVE_UP;
	}
	else if (direccion == 'S' || direccion == 's') {
		ny++;
		mov = MOVE_DOWN;
	}
	else if (direccion == 'A' || direccion == 'a') {
		nx--;
		mov = MOVE_LEFT;
	}
	else if (direccion == 'D' || direccion == 'd') {
		nx++;
		mov = MOVE_RIGHT;
	}
	else
		return 0;

	if (es_movimiento_valido(j, nx, ny)) {
		updateInterfacePlayer(j->x, j->y, nh, nx, ny);
		j->x = nx;
		j->y = ny;
		j->h = mapa[ny][nx];
		registerSimpleAction(turno, 1, mov, nx, ny, 1);
		return 1;
	}

	return 0;
}

void turno_jugador(jugador *j, jugador *oponente, int turno) {

	//Paso mapa funcionamiento.
	if (!mapa)
		mapa = getMap();

	int turnoTerminado = 0;
	int yaUsoDispositivo = 0;
	int opcion;
	char direccion;

	while (!turnoTerminado && !j->rendido) {
		opcion = mostrarmenuturno(turno, yaUsoDispositivo);

		//En caso de estar bloquedo x Hydrux
		if (opcion == 1) { 
			if (j->bloqueado)
				turnoTerminado = 1;
			else {
				direccion = pedirmovimiento(turno);
				if (mover_jugador(j, direccion, turno))
					turnoTerminado = 1;
			}
		} //Seleccion 2. disp.
		else if (opcion == 2) {
			if (yaUsoDispositivo) {
				printf("Ya has usado un dispositivo este turno.\n");
			} else {
				activar_dispositivo(turno, 1, j, oponente);
				yaUsoDispositivo = 1;
			}
		}
		else if (opcion == 3) {
			registerSimpleAction(turno, 1, SURRENDER, 0, 0, 1);
			registerSimpleAction(turno, 2, VICTORY, 0, 0, 1);
			j->rendido = 1;
			turnoTerminado = 1;
		}
	}
	if (j->bloqueado) j->bloqueado = 0; //Reseteo estado Bloqueo (Hydruss)
}
 


#endif
