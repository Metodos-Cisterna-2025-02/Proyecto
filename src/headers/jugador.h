#ifndef JUGADOR_H
#define JUGADOR_H

#include <stdio.h>
#include <stdlib.h>
#include "interfaz.h" // Necesario para dispositivousuario
#include "log.h"     // Necesario para el registro de acciones

/* 1. Definimos la estructura PRIMERO */
typedef struct {
	int x;
	int y;
	int h;
	dispositivousuario inventario[3]; // Nombre corregido
	int bloqueado;
	int rendido;
	int puedeSubir3; 
} jugador;

/* 2. AHORA incluimos dispositivos.h (así ya sabrá qué es un jugador) */
#include "dispositivos.h"
/* Prototipos de funciones */
void turno_jugador(jugador *j, jugador *oponente, int **mapa);
static int mover_jugador(jugador *j, char direccion, int **mapa);
static int es_movimiento_valido(jugador *j, int nx, int ny, int **mapa);


static int es_movimiento_valido(jugador *j, int nx, int ny, int **mapa) {
	if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8)
		return 0;

	int h_actual = mapa[j->y][j->x];
	int h_nueva = mapa[ny][nx];
	int diferencia = abs(h_nueva - h_actual);

	if (j->puedeSubir3) {
		if (diferencia > 3)
			return 0;
	} else {
		if (diferencia > 2)
			return 0;
	}

	return 1;
}

static int mover_jugador(jugador *j, char direccion, int **mapa) {
	int nx = j->x;
	int ny = j->y;

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

	if (es_movimiento_valido(j, nx, ny, mapa)) {
		j->x = nx;
		j->y = ny;
		registerSimpleAction(-1, 1, mov, nx, ny, 1);
		return 1;
	}

	return 0;
}

void turno_jugador(jugador *j, jugador *oponente, int **mapa) {
	int turnoTerminado = 0;
	int yaUsoDispositivo = 0;
	int opcion;
	char direccion;
	int turno = 1; 

	while (!turnoTerminado && !j->rendido) {
		opcion = mostrarmenuturno(turno, yaUsoDispositivo);

		if (opcion == 1) { 
			if (j->bloqueado) {
				turnoTerminado = 1;
			} else {
				direccion = pedirmovimiento(turno);
				if (mover_jugador(j, direccion, mapa)) {
					turnoTerminado = 1;
				}
			}
		} else if (opcion == 2) { 
			usardispositivojugador(j, oponente); // Ahora sí la reconocerá
			yaUsoDispositivo = 1;
		} else if (opcion == 3) { 
			registerSimpleAction(turno, 1, SURRENDER, 0, 0, 1);
			j->rendido = 1;
			turnoTerminado = 1;
		}
	}
	if (j->bloqueado) j->bloqueado = 0;
}

#endif