#ifndef DISPOSITIVOS_H
#define DISPOSITIVOS_H

#include <stdio.h>
#include <stdlib.h>
#include "log.h"

typedef struct {
	int x;
	int y;
	int h;
	dispositivousuario inventario[3];
	int bloqueado;
	int rendido;
	int puedeSubir3; 
} jugador;

/* Prototipos de Funciones */

void usardispositivojugador(int turno, jugador *j, jugador *oponente);


/* Definiciones de Funciones */

void usardispositivojugador(int turno, jugador *j, jugador *oponente) {
	printf("\n[SISTEMA] Abriendo interfaz de dispositivos...\n");

	/* Hacer conversion de nombre de dispositivo a DeviceType para registro */
	char* dispositivo_nombre = j->inventario[0].nombre;
	DeviceType dispositivo_tipo;
	if (strcmp(dispositivo_nombre, "Gaius") == 0)
		dispositivo_tipo = GAIUS;
	else if (strcmp(dispositivo_nombre, "Quadratus") == 0)
		dispositivo_tipo = QUADRATUS;
	else if (strcmp(dispositivo_nombre, "Hydrus") == 0)
		dispositivo_tipo = HYDRUS;
	else if (strcmp(dispositivo_nombre, "Phalanx") == 0)
		dispositivo_tipo = PHALANX;
	else if (strcmp(dispositivo_nombre, "Argus") == 0)
		dispositivo_tipo = ARGUS;
	else
		dispositivo_tipo = NONE;


	// Lógica de dispositivos aquí
	// Hacer registro de uso genérico de dispositivo
	registerCompleteAction(turno, 1, USE_DEVICE, dispositivo_tipo, j->x, j->y, 0, 0, 0, 0, NONE, NONE, 1);
}

#endif