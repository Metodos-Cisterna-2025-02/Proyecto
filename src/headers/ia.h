#ifndef IA_H
#define IA_H

#include "definiciones.h"

int **mapa;


//funciones

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


//revisamos dispos
int ia_tiene_dispositivos(jugador *ia) {
    for (int i = 0; i < 3; i++) {
        if (ia->inventario[i].disponible == 1) {
            return 1;
        }
    }
    return 0;
}



void turnoIA(jugador *ia, jugador *jugador, int metaX, int metaY, int turno) {
	if (!mapa) mapa = getMap();

	int nx, ny;
	int exito = 0;
	int ia_dist = ia_distancia_objetivo(ia->x, ia->y, metaX, metaY);
	int pj_dist = ia_distancia_objetivo(jugador->x, jugador->y, metaX, metaY);
	int tiene_items = ia_tiene_dispositivos(ia);

	printf("\n--- TURNO IA ---\n");
	
	if (ia->bloqueado == 1) {
        	printf(" [ESTADO] IA bloqueada por Hydrus.\n");
        	ia->bloqueado = 0; // Se libera el bloqueo para el proximo turno
        
		if (tiene_items) {
			printf(" [IA] Decide usar un dispositivo aunque no puede moverse...\n");
			activar_dispositivo(turno, 2, ia, jugador);
		
			// PAUSA PARA VISIBILIDAD
			printf("\n (PAUSA)Presiona Enter para continuar...");
			while (getchar() != '\n');
			getchar(); 
		}
		return; // FIN DEL TURNO POSIBLE 1
	}

	// 2. Existe camino valido? (BFS)
	if (ia_dist == -1) {
		printf(" [IA] No hay camino a la meta. ME RINDO PIPIPIPIPI.\n");      	
		ia->rendido = 1;
        	
		registerSimpleAction(turno, 2, SURRENDER, ia->x, ia->y, 1);
		
		return;
    	}

    	// Jugador esta mas cerca o igual??                 DISPP
	if (pj_dist <= ia_dist && tiene_items) {
		printf(" [IA] El jugador esta mas cerca o igual. Atacando...\n");
		activar_dispositivo(turno, 2, ia, jugador);
        
        	// PAUSA PARA VISIBILIDAD
        	printf("\n(PAUSA) Presiona Enter para continuar...");
        	while (getchar() != '\n');
        	getchar();
	}

	//Mover hacia objetivo 
	int puede_moverse = ia_siguiente_paso(ia->x, ia->y, metaX, metaY, &nx, &ny);
	if (puede_moverse) {
		int xOld = ia->x;
		int yOld = ia->y;
		int hOld = mapa[ia->y][ia->x];
		ActionType mov;
		if (ia->x - nx == -1) 
			mov = MOVE_RIGHT;
		else if (ia->x - nx == 1) 
			mov = MOVE_LEFT;
		else if (ia->y - ny == -1)
			mov = MOVE_DOWN;
		else if (ia->y - ny == 1)
			mov = MOVE_UP;
		
		// Actualizar interfaz antes de modificar la estructura
		updateInterfaceAI(xOld, yOld, hOld, nx, ny);
		ia->x = nx;
		ia->y = ny;
		ia->h = mapa[ny][nx];
		
		exito = 1;

		registerSimpleAction(turno, 2, mov, nx, ny, exito);
	} else {
		// TODO: IA no encontró camino, SE RINDEE
		ia->rendido = 1;

		printf("IA no encontro un camino valido hacia la meta.\n");
		registerSimpleAction(turno, 2, SURRENDER, ia->x, ia->y, 1);
		registerSimpleAction(turno, 1, VICTORY, 0, 0, 1);
	}
}



void seleccionardispositivosIA(dispositivousuario inventarioIA[], dispositivousuario inventarioJugador[], int sorteo) {
	int eleccion1, eleccion2, eleccion3;
//logica eleccion IA
	char *nombredispositivos[] = {"Gaius","Quadratus","Hydrus","Phalanx","Argus"};
	if (sorteo == 0) {
		eleccion1=(rand()%5)+1;
		eleccion2=(rand()%5)+1;
		
		while(eleccion1==eleccion2)
			eleccion2=(rand()%5)+1;
		
		eleccion3=(rand()%5)+1;
		
		while(eleccion3==eleccion2||eleccion3==eleccion1)
			eleccion3=(rand()%5)+1;
	}
	else {
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
