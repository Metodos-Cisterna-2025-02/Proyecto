#ifndef INTERFAZ_H
#define INTERFAZ_H

#include "definiciones.h"


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
		}
		else if (tecla == 'a' || tecla == 'A') {
			esvalida = 1;
		}
		else if (tecla == 's' || tecla == 'S') {
			esvalida = 1;
		}
		else if (tecla == 'd' || tecla == 'D') {
			esvalida = 1;
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

	if (leidos != 1) {
		while (getchar() != '\n'); // Limpiar buffer

		return -1;
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

//funcion fuerte de dispositivos
void activar_dispositivo(int turno, int id_jugador, jugador *sujeto, jugador *rival) {
	int seleccion = -1;
	dispositivousuario *disp = NULL;  //Nos tiro bug asi q definimos para que no apunte a nada inicialmente

	// SELECCION DE DISPOSITIVOSS

	if (id_jugador == 1) { 
		// Logica para el Usuario 
		mostrardispositivos(sujeto->inventario, 3);
		printf("Seleccione dispositivo (1-3) o 0 para cancelar: ");
        
        
  		scanf("%d", &seleccion);
        
       
		if (seleccion < 1 || seleccion > 3) {
			printf("Accion cancelada o opcion invalida.\n");
		
			return; 
        	}
    	}
	else {                           //logica IA
        	for (int i = 0; i < 3; i++) {
        		if (sujeto->inventario[i].disponible) {
                		seleccion = i + 1;
                		break;
        		}
		}

		if (seleccion == -1) return; // No tiene nada dispo
	}

	disp = &sujeto->inventario[seleccion - 1]; //DISPOSITIVOOOO
	if (!disp->disponible) return;
	
	int id_para_minijuego;

	// RETO DEL MINIJUEGO
	// Convertimos el id_jugador para q se adapte a minijuegos.h
	if (id_jugador == 1) {
		id_para_minijuego = 0; 
	} else {
		id_para_minijuego = 1; 
	}

	printf("\n Intentando activar %s (Nivel %d)...\n", disp->nombre, disp->nivel);

	if (ejecutarMinijuego(disp->nivel, id_para_minijuego)) {
		printf("LOGRADO!!!! Dispositivo %s activado.\n", disp->nombre);
       
		//X-X_X-X-X-X-X-XX-X_X-X-X-X-X-X SEGUNDA PARTE IMPORTANTE X-X_X-X-X-X-X-X X-X_X-X-X-X-X-X
		// Marcamos como usado y llamamos al efecto planteadoop
		disp->disponible = 0; 
		aplicar_efecto_dispositivo(sujeto, rival, disp, turno, id_jugador);// FUNCION PANCHOOO ACAAAA

		// Registro exitoso en el log
		//registerCompleteAction(turno, id_jugador, USE_DEVICE, NONE, 0, 0, 0, 0, 0, NONE, NONE, NONE, 1); pq ta mute
	} 
	else {
		printf("FALLOOO CUEEEk, El dispositivo %s se ha desperdiciado.\n", disp->nombre);
		disp->disponible = 0; 
		registerCompleteAction(turno, id_jugador, USE_DEVICE, NONE, 0, 0, 0, 0, 0, NONE, NONE, NONE, 0);
	}
}


#endif





