#ifndef INTERFAZ_H
#define INTERFAZ_H



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

/* Estructura Dispositivos */
typedef struct {
	char nombre[16];
	int disponible; /* disponible = 1 esta disponible, disponible = 0 no esta disponible */
	int nivel;
} dispositivousuario;

/* Prototipos de Funciones */

char pedirmovimiento(int turno);
int mostrarmenuturno(int turno, int yausodispositivo);
int realizarsorteocarasello();
void seleccionardispositivos(dispositivousuario misdispositivos[]);
void mostrardispositivos(dispositivousuario misdispositivos[], int cantidad);
void usardispositivo(int turno, dispositivousuario *disp);
void turnodeia(int turno);

/* Definiciones de Funciones */

/* Pide una opcion entre W, A, S, D al usuario */
char pedirmovimiento(int turno) {
	char tecla;
	int esvalida = 0; 
	int leidos;       

	while (!esvalida) {
		printf("Moverse (W: arriba, A: izquierda, S: abajo, D: derecha): ");
		
		leidos = scanf(" %c", &tecla);
		
		if (leidos != 1) {
			printf("Error de lectura.\n");
			while (getchar() != '\n')
				continue;
		}

		if (tecla == 'w' || tecla == 'W') {
			esvalida = 1;
			registerSimpleAction(turno, 1, MOVE_UP, 0, 0, esvalida);
		}
		else if (tecla == 'a' || tecla == 'A') {
			esvalida = 1;
			registerSimpleAction(turno, 1, MOVE_LEFT, 0, 0, esvalida);
		}
		else if (tecla == 's' || tecla == 'S') {
			esvalida = 1;
			registerSimpleAction(turno, 1, MOVE_DOWN, 0, 0, esvalida);
		}
		else if (tecla == 'd' || tecla == 'D') {
			esvalida = 1;
			registerSimpleAction(turno, 1, MOVE_RIGHT, 0, 0, esvalida);
		}
		else {
			printf("Tecla invalida. Usa W, A, S o D.\n");
			
		}

		if (!esvalida || getchar() != '\n') 
			while (getchar() != '\n');
	}

	return tecla;
}

int mostrarmenuturno(int turno, int yausodispositivo) {
	int opcion;
	int leidos;

	printf("\n   MENU DE TURNO    \n");

	printf("1. Moverse\n");
	printf(yausodispositivo == 0 ? "2. Usar Dispositivo\n" : "2. [Dispositivo usado]\n");
	printf("3. Rendirse\n");	
	printf("Elige una opcion: ");
	
	leidos = scanf("%d", &opcion);

	if (leidos != 1)
		while (getchar() != '\n'); // Limpiar buffer
			return -1;

	return opcion;
}

int realizarsorteocarasello() {
	int eleccion, resultado, escanear;

	printf("\n SORTEO CARA Y SELLO  \n 1-Cara\n 2-Sello\n");
	while (1) {
		printf("Selecciona una opcion: ");
		escanear = scanf("%d", &eleccion);
		// Si (leyó bien Y es 1) O (leyó bien Y es 2)
		if ((escanear == 1 && eleccion == 1) || (escanear == 1 && eleccion == 2))
			break;
		printf("Invalido. Use 1 o 2.\n");
	}

	resultado = (rand() % 2) + 1;// resultado aleatorio
	if (resultado == 1)
		printf("Resultado: CARA\n");
	else
		printf("Resultado: SELLO\n");

	registerCoinFlip(1, eleccion, resultado);

	if (eleccion == resultado)
		return 1; //retorna 1 si el usuario gano
	
	return 0; //retorna 0 si el usuario perdio
}

void seleccionardispositivos(dispositivousuario misdispositivos[]) {
	char catalogo[5][16] = {"Gaius", "Quadratus", "Hydrus", "Phalanx", "Argus"};
	int usados[5] = {0, 0, 0, 0, 0};
	int eleccion, contador = 0;
	int leidos; 

	printf("\n    ELIGE  3 DISPOSITIVOS   \n");

	while (contador < 3) {
		
		for (int i = 0; i < 5; i++)
			printf("%d. %s\n", i + 1, catalogo[i]);

		printf("Dispositivo %d: ", contador + 1);
		
		leidos = scanf("%d", &eleccion);

		if (leidos != 1) {
			printf("Error: Debes ingresar un numero.\n");
			while (getchar() != '\n'); 
			continue;
		}
		
		if (eleccion >= 1 && eleccion <= 5 && usados[eleccion - 1] == 0) {
			
			strcpy(misdispositivos[contador].nombre, catalogo[eleccion - 1]);
			misdispositivos[contador].disponible = 1;
			misdispositivos[contador].nivel = contador + 1;
			
			usados[eleccion - 1] = 1;

			registerDeviceSelection(1, (DeviceType)(eleccion), contador + 1);
			
			contador++;
		} else
			printf("Error: Numero invalido o dispositivo ya elegido.\n");
		
		while (getchar() != '\n');
	}
}

void mostrardispositivos(dispositivousuario misdispositivos[], int cantidad) {
	
	printf("  DISPOSITIVOS SELECCIONADOS  \n");
	
	for (int i = 0; i < cantidad; i++){
		printf("%d) %s | Disponible: ", i + 1, misdispositivos[i].nombre);

		if (misdispositivos[i].disponible == 1)
			printf("SI\n");
		else
			printf("NO\n");
	}
}

void usardispositivo(int turno, dispositivousuario *disp) {
	DeviceType dispositivo_tipo;
	
	/* Convertir nombre de dispositivo a DeviceType para registro */
	char* dispositivo_nombre = disp->nombre;
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

	if (disp->disponible == 0) {
		printf("El dispositivo ya fue usado.\n");
		registerCompleteAction(turno, 1, USE_DEVICE, dispositivo_tipo, 0, 0, 0, 0, 0,
								NONE, NONE, NONE, 0);
		return;
	}

	// Marcar como usado
	disp->disponible = 0;

	printf("Dispositivo %s marcado como usado.\n", disp->nombre);

	// Aquí se registraría la acción específica del dispositivo y se llamaría a su función
	// Por ahora solo registramos el uso genérico
	registerCompleteAction(turno, 1, USE_DEVICE, dispositivo_tipo, 0, 0, 0, 0, 0,
							NONE, NONE, NONE, 1);
}

void turnodeia(int turno) {
	printf("\n--- TURNO DE LA IA ---\n");

	DeviceType dispositivo_tipo = NONE; // Dispositivo genérico usado por la IA

	// Aquí se llamaría las funciones para la lógica de la IA
	// Por mientras, hacemos un registro de turno genérico
	registerCompleteAction(turno, 2, USE_DEVICE, dispositivo_tipo, 0, 0, 0, 0, 0,
		NONE, NONE, NONE, 1);
	
	registerSimpleAction(turno, 2, MOVE_UP, 0, 0, 1);
}

#endif