#ifndef IA_H
#define IA_H
#include <time.h>
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



void  seleccionardispositivosIA(dispositivousuario inventarioIA[],dispositivousuario inventarioJugador[],int sorteo) {
 int eleccion1, eleccion2, eleccion3;

 char *nombredispositivos[] = {"Gaius","Quadratus","Hydrus","Phalanx","Argus"};
	if (sorteo == 0) {
  eleccion1=(rand()%5)+1;
		eleccion2=(rand()%5)+1;
		while(eleccion1==eleccion2){
		 eleccion2=(rand()%5)+1;
		}
		eleccion3=(rand()%5)+1;
		while(eleccion3==eleccion2||eleccion3==eleccion1){
		 eleccion3=(rand()%5)+1;
		}
	}
	else{
	 int indices_dispositivo_jugador[3];
		for(int i=0;i<3;i++){
			if (strcmp(inventarioJugador[i].nombre,"Gaius")==0)
				indices_dispositivo_jugador[i]=1;
			else if (strcmp(inventarioJugador[i].nombre,"Quadratus")==0)
				indices_dispositivo_jugador[i]=2;
			else if (strcmp(inventarioJugador[i].nombre,"Hydrus")==0)
				indices_dispositivo_jugador[i]=3;
			else if (strcmp(inventarioJugador[i].nombre,"Phalanx")==0)
				indices_dispositivo_jugador[i]=4;
			else
				indices_dispositivo_jugador[i]=5;
		}
	
			eleccion1=indices_dispositivo_jugador[rand()%3];
			eleccion2=(rand()%5)+1;
			while(eleccion2==indices_dispositivo_jugador[0]||eleccion2==indices_dispositivo_jugador[1]||eleccion2==indices_dispositivo_jugador[2])
			 eleccion2=(rand()%5)+1;
			eleccion3=(rand()%5)+1;
			while(eleccion3==eleccion1||eleccion3==eleccion2)
			 eleccion3=(rand()%5)+1;
	}
				
	strcpy(inventarioIA[0].nombre, nombredispositivos[eleccion1 - 1]);
 inventarioIA[0].disponible = 1;
 inventarioIA[0].nivel = 1;

 strcpy(inventarioIA[1].nombre, nombredispositivos[eleccion2 - 1]);
 inventarioIA[1].disponible = 1;
 inventarioIA[1].nivel = 2;

 strcpy(inventarioIA[2].nombre, nombredispositivos[eleccion3 - 1]);
 inventarioIA[2].disponible = 1;
 inventarioIA[2].nivel = 3;
			
 registerDeviceSelection(2, (DeviceType)eleccion1, 1);
 registerDeviceSelection(2, (DeviceType)eleccion2, 2);
 registerDeviceSelection(2, (DeviceType)eleccion3, 3);
}
#endif
