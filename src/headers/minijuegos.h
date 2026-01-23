#ifndef MINIJUEGOS_H
#define MINIJUEGOS_H

#include "definiciones.h"

// Para asegurar la portabilidad y compatibilidad de WINDOWS y UNIX
#ifdef _WIN32  // Si es Windows
	#include <windows.h>
	#define sleep(x) Sleep(1000 * (x)) //pasa de Milisegundos a Segundos
#else
	#include <unistd.h>
#endif

/* Prototipos de funciones estáticas */
static int minijuego1(int jugador);
static int minijuego2(int jugador); 
static int minijuego3(int jugador); 

// MINIJUEGO 1 - Numero al azar del 1 al 3
//Variable "jugador" se refiere a 0 = Usuario y 1 = IA

static int minijuego1(int jugador) {
	int numeroSecreto = rand() % 3 + 1;  //funcion random para dejar un numero del 1 al 3 
	int intento;



	// Para hacerlo justo la IA tambien puede perder, quedara en ambitos probabilisticos

	if (jugador) {    
		intento = rand() % 3 + 1;
		return intento == numeroSecreto;
	}

	printf("\nMinijuego 1 - Adivina el numero (1 a 3): ");
	scanf("%d", &intento);

	if (intento == numeroSecreto) {
		printf("Correcto!\n");
		return 1;
	} else {
		printf("Incorrecto. Era %d.\n", numeroSecreto);
		return 0;
	}
}




// MINIJUEGO 2 - memorizar y repetir secuencia con limite de tiempo

static int minijuego2(int jugador) {
	int secuencia[6];
	int respuesta[6];
	int i;


	for (i = 0; i < 6; i++) {
		secuencia[i] = rand() % 9;
	}

	if (jugador) {
		return 1; // IA siempre acertara el minijuego
	}

	printf("\nMemoriza la siguiente secuencia:\n\n");
	for (i = 0; i < 6; i++)
		printf("%d ", secuencia[i]);

	printf("\n\n(Tienes 4 segundos para recordar!!!)\n");
	sleep(4);

	// Ocultar con strings
	for (i = 0; i < 40; i++)
		printf("\n");	

	printf("Ingresa la secuencia de numeros 1 por 1 (separado por digito)\n");
	for (i = 0; i < 6; i++)
		scanf("%d", &respuesta[i]);

	for (i = 0; i < 6; i++) {
		if (respuesta[i] != secuencia[i])
			return 0;
	}

	return 1;
}

// MINIJUEGO 3 - Adivinar el numero con Mayor y Menor

static int minijuego3(int jugador) {


	int inicio = 1;
	int fin = 100;
	int numeroSecreto;
	int intentos = 6;
	int propuesta;

    
	numeroSecreto = rand() % 100 + 1;  // Generar numero secreto

	// Comportamiento IA 
	if (jugador)
		return 1; // la IA resuelve correctamente

	printf("\nMinijuego 3 - Division y Conquista\n");
	printf("Debes encontrar el numero super secreto.\n");
	printf("Rango inicial: [1 - 100]\n");
	printf("Intentos maximos: %d\n", intentos);

	while (inicio <= fin && intentos > 0) {
		printf("\nRango actual: [%d - %d]\n", inicio, fin);
		printf("Ingresa tu propuesta: ");
		scanf("%d", &propuesta);

		// Validar rango
		if (propuesta < inicio || propuesta > fin) {
			printf("Numero fuera de rango. Intenta nuevamente.\n");
			continue; // termina ESTA vuelta del ciclo y pasa a la siguiente
		}

		// Comparar
		if (propuesta == numeroSecreto) {
			printf("Correcto! Has encontrado el numero.\n");
			return 1; // gana
		}

		if (propuesta < numeroSecreto) {
			printf("El numero secreto es MAYOR.\n");
			inicio = propuesta + 1;
		} else {
			printf("El numero secreto es MENOR.\n");
			fin = propuesta - 1;
		}

		intentos--;
		printf("Intentos restantes: %d\n", intentos);
	}

	printf("\nTe quedaste sin intentos. El numero era %d.\n", numeroSecreto);
	
	return 0; // pierde
}

// DESPACHADOR 
int ejecutarMinijuego(int numeroUso, int jugador) {
	// numeroUso = Ejecuta el minijuego según el orden (1, 2 o 3)
	
	// jugador = 0 -> usuario
	// jugador = 1 -> inteligencia artificial
	
	// Retorna 1 si gana, 0 si pierde
	
	if (numeroUso == 1) return minijuego1(jugador);

	if (numeroUso == 2) return minijuego2(jugador);

	if (numeroUso == 3) return minijuego3(jugador);

	return 0;
}





#endif 