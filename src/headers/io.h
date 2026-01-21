#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>

#define MAP_TEST_SIZE 8
#define MAX_VERTICAL 8   // Agregado para search.h
#define MAX_HORIZONTAL 8

// Solo carga el mapa una vez
static int** getMap() {
    FILE *file = fopen("maps/map_test.txt", "r");
    if (!file) {
        perror("Error al abrir map_test.txt");
        return NULL;
    }

    int** map = (int**)malloc(MAP_TEST_SIZE * sizeof(int*));
    for(int i = 0; i < MAP_TEST_SIZE; i++) {
        map[i] = (int*)malloc(MAP_TEST_SIZE * sizeof(int));
        for (int j = 0; j < MAP_TEST_SIZE; j++) {
            if (fscanf(file, "%d", &map[i][j]) != 1) {
                // Error de lectura
            }
        }
    }
    fclose(file);
    return map;
}

// Ahora recibe el mapa como parámetro para no crear uno nuevo
static void printMap(int** map) {
    if (!map) return;
    
    printf("\n--- ESTADO DEL TERRENO ---\n");
    for (int i = 0; i < MAP_TEST_SIZE; i++) {
        for (int j = 0; j < MAP_TEST_SIZE; j++) {
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
}

// IMPORTANTE: Función para liberar la memoria al final del juego
static void freeMap(int** map) {
    for(int i = 0; i < MAP_TEST_SIZE; i++) {
        free(map[i]);
    }
    free(map);
}

#endif