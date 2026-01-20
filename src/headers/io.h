#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_VERTICAL 32
#define MAX_HORIZONTAL 32
#define MAX_HEIGHT 5
#define MIN_HEIGHT 0
#define MAP_TEST "maps/map_test.txt"
#define MAP_TEST_SIZE 8

static FILE* mapFile;
static int* map[MAX_VERTICAL];
static int mapLoaded = 0;
char buffer[8];

static void readMap();
int** getMap();
void printMap();


int** getMap() {
    if (!mapLoaded)
        readMap();
    
    return map;
}

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
