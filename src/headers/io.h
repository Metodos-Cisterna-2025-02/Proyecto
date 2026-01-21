#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* Logs related declarations */

/* Map related declarations */
#define MAX_VERTICAL 32
#define MAX_HORIZONTAL 32
#define MAX_HEIGHT 5
#define MIN_HEIGHT 0
#define MAP_TEST "maps/map_test.txt"
#define MAP_TEST_SIZE 8

/* Map related variables */
static FILE* mapFile;
static int* map[MAX_VERTICAL];
static int mapLoaded = 0;
static char buffer[8];

/* Map related function prototypes */
static void readMap();
int** writeMap(int x, int y, int value);
int** getMap();
void printMap();


/* Maps related functions */

/* Retorna el mapa como una matriz de NxN enteros */
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
        map[i] = (int*) calloc(MAX_HORIZONTAL, __SIZEOF_INT__);
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
			current = snprintf(buffer, round(MAX_HEIGHT / 10) + 2, "%d", map[i][j]);
			if (current < MIN_HEIGHT || current > MAX_HEIGHT) { 
				map[i][j] = 0; /* valor por defecto en caso de no cumplir condicion de altura */
			}
		}
	}

	fclose(mapFile);
}
