#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/* Map related declarations */

#define MAX_VERTICAL 32
#define MAX_HORIZONTAL 32
#define MAX_HEIGHT 5
#define MIN_HEIGHT 0
#define MAP_TEST "maps/map_test.txt"
#define MAP_TEST_SIZE 8


/* Interface related declarations */

#define INTERFACE_FILE "interface/interface.txt"
#define HEIGHT0 "/media/height0.png"
#define HEIGHT1 "/media/height1.png"
#define HEIGHT2 "/media/height2.png"
#define HEIGHT3 "/media/height3.png"
#define HEIGHT4 "/media/height4.png"
#define HEIGHT5 "/media/height5.png"
#define OUT_OF_BOUNDS "/media/out_of_bounds.png"
#define PLAYER_ICON "/media/protoman.png"
#define BOT_ICON "/media/rockman.png"
#define MAX_LINE_LENGTH 256


/* Map related variables */

static FILE* mapFile;
static int* map[MAX_VERTICAL];
static int mapLoaded = 0;
static char buffer[8];


/* Interface related variables */

static FILE* interfaceFile;
static int interfaceLoaded = 0;
static char buffer[MAX_LINE_LENGTH];


/* Map related function prototypes */

static void readMap();
int** writeMap(int x, int y, int value);
int** getMap();
void freeMap();
void printMap();


/* Interface related function prototypes */

static void readInterface();
static void updateInterface(int x, int y, int h);


/* Definiciones de Funciones de Mapa */

/* Lee el archivo del mapa y lo guarda como una matriz de enteros */
static void readMap() {
	int i;
	int j;

	mapFile = fopen(MAP_TEST, "r");

	if (!mapFile) {
		perror("Error al abrir el archivo");
		return;
	}

	for (i = 0; i < MAP_TEST_SIZE; i++) {
	map[i] = (int*) calloc(MAX_HORIZONTAL, sizeof(int));
		if (!map[i]) {
			perror("Error al alocar memoria para fila");
			fclose(mapFile);
			return;
		}
	}

	for (i = 0; i < MAP_TEST_SIZE; i++) {
		for (j = 0; j < MAP_TEST_SIZE; j++) {
			fscanf(mapFile, "%d", &map[i][j]); /* valor (i,j) a map[i][j] */
			int current;
			current = snprintf(buffer, floor(MAX_HEIGHT / 10) + 2, "%d", map[i][j]);
			if (current < MIN_HEIGHT || current > MAX_HEIGHT) { 
				map[i][j] = 0; /* valor por defecto en caso de no cumplir condicion de altura */
			}
		}
	}

	fclose(mapFile);
}

/* Retorna el mapa como una matriz de NxM enteros */
int** getMap() {
	if (!mapLoaded)
		readMap();

	return map;
}

/* Escribe un valor de altura en la posicion (X, Y) en el mapa */
int** writeMap(int x, int y, int value) {
	if (!mapLoaded)
		readMap();
	
	if (x < 0 || x >= MAP_TEST_SIZE || y < 0 || y >= MAP_TEST_SIZE) {
		printf("Error: Coordenadas fuera de rango.\n");
		return map;
	}
	
	if (value < MIN_HEIGHT || value > MAX_HEIGHT) {
		printf("Error: Altura fuera de rango.\n");
		return map;
	}
	
	map[y][x] = value;
	return map;
}

/* Imprime el mapa a la consola */
void printMap() {
	if (!mapLoaded)
		readMap();
	
	int i;
	int j;
	for (i = 0; i < MAP_TEST_SIZE; i++) {
		for (j = 0; j < MAP_TEST_SIZE; j++)
			printf("%d ", map[i][j]);
		printf("\n");
	}
}

/* liberar memoria al final del juego */ 
void freeMap() {
	free(map);
}

/* Definiciones de Funciones de Interfaz */

/* Lee el archivo de la interfaz */
static void readInterface() {
	interfaceFile = fopen(INTERFACE_FILE, "r");

	if (!interfaceFile) {
		perror("Error al abrir el archivo de la interfaz");
		return;
	}

	interfaceLoaded = 1;
}

/* Actualiza la interfaz en la posicion (X, Y, H) */
void updateInterface(int x, int y, int h) {
	if (!interfaceLoaded)
		readInterface();

	

}

#endif