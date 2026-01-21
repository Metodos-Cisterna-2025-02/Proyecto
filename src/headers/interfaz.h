#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

/* estructura dispositivos */
typedef struct {
	char nombre[16];
	int disponible; /* disponible = 1 esta disponible, disponible = 0 no esta disponible */
	int nivel;
} dispositivousuario;

/* prototipos de funciones */
char pedirmovimiento(int turno);
int mostrarmenuturno(int turno, int yausodispositivo);
int realizarsorteocarasello();
void seleccionardispositivos(dispositivousuario misdispositivos[]);
void mostrardispositivos(dispositivousuario misdispositivos[], int cantidad);
void usardispositivo(dispositivousuario *disp);
void turnodeia(int turno);

/* definiciones de funciones */

/* pide una opcion entre wasd al usuario */
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
	int esvalida = 0;
	int leidos;

	printf("\n   MENU DE TURNO    \n");
	printf("1. Moverse\n");
	if (yausodispositivo == 0) {
		printf("2. Usar Dispositivo\n");
	} else {
		printf("2. Ya usaste un dispositivo durante este turno\n");
	}
	printf("3. Rendirse\n");

	while (!esvalida) {
		printf("Elige una opcion: ");
		leidos = scanf("%d", &opcion);

		if (leidos != 1) {
			printf("Entrada invalida. Debe ser un numero.\n");
			while (getchar() != '\n')
				continue;
		}

		if (opcion == 1) {
			esvalida = 1;
			pedirmovimiento(turno);
		}
		else if (opcion == 2 && yausodispositivo == 0) {
			esvalida = 1;
			// Aquí se llamaría a la función para usar un dispositivo
			// Por ahora solo registramos la acción de usar dispositivo genérico
			registerCompleteAction(turno, 1, USE_DEVICE, GAIUS, 0, 0, 0, 0, 0,
								NONE, NONE, NONE, esvalida);
		}
		else if (opcion == 3) {
			esvalida = 1;
			registerSimpleAction(turno, 1, SURRENDER, 0, 0, esvalida);
		}
		else
			printf("Opcion no permitida.\n");
	}

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

void usardispositivo(dispositivousuario *disp) {

	if (disp->disponible == 0) {
		printf("El dispositivo ya fue usado.\n");
		return;
	}

	// Marcar como usado
	disp->disponible = 0;

	printf("Dispositivo %s marcado como usado.\n", disp->nombre);

	// Aquí se registraría la acción específica del dispositivo y se llamaría a su función
	// Por ahora solo registramos el uso genérico
	registerCompleteAction(0, 1, USE_DEVICE, GAIUS, 0, 0, 0, 0, 0,
							NONE, NONE, NONE, 1);
}

void turnodeia(int turno) {
	printf("\n--- TURNO DE LA IA ---\n");
	// Aquí se llamaría las funciones para la lógica de la IA
	// Por mientras, hacemos un registro de turno genérico
	registerCompleteAction(turno, 2, USE_DEVICE, GAIUS, 0, 0, 0, 0, 0,
								NONE, NONE, NONE, 1);
	registerSimpleAction(turno, 2, MOVE_UP, 0, 0, 1);
}