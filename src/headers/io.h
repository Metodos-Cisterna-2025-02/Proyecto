#ifndef IO_H
#define IO_H

#include "definiciones.h"


/* Variables relacionadas con el mapa */

static FILE* mapFile;
static int* map[MAX_VERTICAL];
static int mapLoaded = 0;
static char buffer[8];


/* Variables relacionadas con la interfaz */

static char interfaceBuffer[MAX_LINE_LENGTH];
static int currentPlayerX = -1, currentPlayerY = -1;
static int currentAIX = -1, currentAIY = -1;
static int currentGoalX = -1, currentGoalY = -1;


/* Prototipos de funciones estáticas */

static void readMap();
static void updateInterfaceComplete(int x, int y, int h, int xPlayer, int yPlayer, int xAI, int yAI);
static char* generateReplacementLine(int x, char* iconPath, char* lineOld);

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

	// Marcar mapa como cargado para evitar releer en cada acceso
	mapLoaded = 1;
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

/* Liberar memoria al final del juego */ 
void freeMap() {
	if (mapLoaded)
		for (int i = 0; i < MAP_TEST_SIZE; i++)
			free(map[i]);
	mapLoaded = 0;
}


/* Definiciones de Funciones de Interfaz */

/* Actualiza la interfaz en la posicion (X, Y)  */
static void updateInterfaceComplete(int x, int y, int h, int xPlayer, int yPlayer, int xAI, int yAI) {
	// Abrir el archivo de interfaz existente
	FILE *interfaceOld = fopen(INTERFACE_FILE, "r");
	if (!interfaceOld) {
		perror("Error al abrir el archivo de interfaz");
		return;
	}

	// Crear archivo temporal para escribir los cambios
	FILE *interfaceNew = fopen(INTERFACE_TMP_FILE, "w");
	if (!interfaceNew) {
		perror("Error al crear el archivo temporal de interfaz");
		fclose(interfaceOld);
		return;
	}

	// Determinar el nuevo icono basado en la altura
	char *newIcon;
	switch(h) {
		case 0: newIcon = HEIGHT0; break;
		case 1: newIcon = HEIGHT1; break;
		case 2: newIcon = HEIGHT2; break;
		case 3: newIcon = HEIGHT3; break;
		case 4: newIcon = HEIGHT4; break;
		case 5: newIcon = HEIGHT5; break;
		default: newIcon = OUT_OF_BOUNDS; break;
	};

	if (x == xPlayer && y == yPlayer)
		newIcon = PLAYER_ICON;
	else if (x == xAI && y == yAI)
		newIcon = BOT_ICON;

	char *replacement;

	/* Copiar linea por linea, reemplazando la linea correspondiente */
	int line = 1;
	while (fgets(interfaceBuffer, MAX_LINE_LENGTH, interfaceOld)) {
		if (line == y + 3) { // +3 para compensar 2 encabezados (version y dimensiones)
			replacement = generateReplacementLine(x, newIcon, interfaceBuffer);
			fputs(replacement,  interfaceNew);
		} else
			fputs(interfaceBuffer, interfaceNew);
		line++;
	}

	fclose(interfaceOld);
	fclose(interfaceNew);
	
	// Reemplazar el archivo original con el modificado

	if (remove(INTERFACE_FILE) != 0) {
		perror("Error al eliminar el archivo de interfaz");
		return;
	}
	
	if (rename(INTERFACE_TMP_FILE, INTERFACE_FILE) != 0) {
		perror("Error al renombrar el archivo de interfaz");
		return;
	}
}

/* Genera una nueva linea para la interfaz con el icono reemplazado */
static char* generateReplacementLine(int x, char* iconPath, char* lineOld) {
	static char lineNew[MAX_LINE_LENGTH];
	char* tokens[MAP_TEST_SIZE];
	int tokenCount = 0;
	char lineCopy[MAX_LINE_LENGTH];
	
	// Copiar línea para no modificar el original
	strncpy(lineCopy, lineOld, MAX_LINE_LENGTH - 1);
	lineCopy[MAX_LINE_LENGTH - 1] = '\0'; // strncpy no garantiza null-termination
	
	// Quitar newline si existe
	char* newlinePos = strchr(lineCopy, '\n');
	if (newlinePos) *newlinePos = '\0';
	
	// Tokenizar por comas ("h1,h2,h3" -> ["h1","h2","h3"])
	char* token = strtok(lineCopy, ",");
	while (token && tokenCount < MAP_TEST_SIZE) { // no puede haber mas tokens que el tamaño del mapa (horizontal)
		tokens[tokenCount++] = token;
		token = strtok(NULL, ","); // obtener siguiente token (primero h1, luego h2, etc.)
	}
	
	// Reemplazar el token en la columna x
	if (x >= 0 && x < tokenCount) {
		tokens[x] = iconPath; // reemplazar el token en la columna x con el nuevo icono
	}
	
	// Reconstruir la línea a partitr de los nuevos tokens con strcat (concatenación de strings)
	lineNew[0] = '\0';
	for (int i = 0; i < tokenCount; i++) {
		strcat(lineNew, tokens[i]);
		if (i < tokenCount - 1) {
			strcat(lineNew, ","); // strcat pide char* (",") no char (',')
		}
	}
	strcat(lineNew, "\n");
	
	return lineNew;
}

/* Inicializar archivo de interfaz */
void initializeInterface(int xPlayer, int yPlayer, int xAI, int yAI, int xGoal, int yGoal) {
	// Guardar posiciones iniciales
	currentPlayerX = xPlayer;
	currentPlayerY = yPlayer;
	currentAIX = xAI;
	currentAIY = yAI;
	currentGoalX = xGoal;
	currentGoalY = yGoal;

	FILE *interfaceFile = fopen(INTERFACE_FILE, "w");
	if (!interfaceFile) {
		perror("Error al crear el archivo de interfaz");
		return;
	}

	char goal_icon[MAX_LINE_LENGTH];
	
	// Escribir los encabezados
	fputs("1\n", interfaceFile); // Encabezado "version"
	fputs("8,8\n", interfaceFile); // Encabezado "dimensiones"

	for (int i = 0; i < MAP_TEST_SIZE; i++) {
		for (int j = 0; j < MAP_TEST_SIZE; j++) {
			// Icono para la altura
			char *iconPath;
			switch(map[i][j]) {
				case 0: iconPath = HEIGHT0; break;
				case 1: iconPath = HEIGHT1; break;
				case 2: iconPath = HEIGHT2; break;
				case 3: iconPath = HEIGHT3; break;
				case 4: iconPath = HEIGHT4; break;
				case 5: iconPath = HEIGHT5; break;
				default: iconPath = OUT_OF_BOUNDS; break;
			}

			// Icono para la meta (usa icono especial según altura)
			if (map[i][j] >= 0 && map[i][j] <= 5) {
				snprintf(goal_icon, sizeof(goal_icon), "media/goal_height_%d.png", map[i][j]);
			} else {
				strncpy(goal_icon, OUT_OF_BOUNDS, sizeof(goal_icon));
				goal_icon[sizeof(goal_icon) - 1] = '\0';
			}

			if (j == xPlayer && i == yPlayer) // Si es la posicion del jugador
				fprintf(interfaceFile, "%s", PLAYER_ICON);
			else if (j == xAI && i == yAI) // Si es la posicion de la IA
				fprintf(interfaceFile, "%s", BOT_ICON);
			else if (j == xGoal && i == yGoal) // Si es la posicion de la meta
				fprintf(interfaceFile, "%s", goal_icon);
			else
				fprintf(interfaceFile, "%s", iconPath);

			// Agregar coma de separacion si no es el ultimo elemento
			if (j < MAP_TEST_SIZE - 1)
				fprintf(interfaceFile, ",");
		}
		fprintf(interfaceFile, "\n");
	}

	fclose(interfaceFile);
}

/* Actualizar el icono de la altura en la posición (x, y) en la interfaz */
void updateInterfaceHeight(int x, int y, int h) {
	updateInterfaceComplete(x, y, h, currentPlayerX, currentPlayerY, currentAIX, currentAIY);
}

/* Regenerar interfaz completa con nuevas posiciones */
void regenerateInterface(int xPlayer, int yPlayer, int xAI, int yAI) {
	FILE *interfaceFile = fopen(INTERFACE_FILE, "w");
	if (!interfaceFile) {
		perror("Error al crear el archivo de interfaz");
		return;
	}

	// Escribir los encabezados
	fputs("1\n", interfaceFile); // Encabezado "version"
	fputs("8,8\n", interfaceFile); // Encabezado "dimensiones"

	for (int i = 0; i < MAP_TEST_SIZE; i++) {
		for (int j = 0; j < MAP_TEST_SIZE; j++) {
			char *iconPath;
			switch(map[i][j]) {
				case 0: iconPath = HEIGHT0; break;
				case 1: iconPath = HEIGHT1; break;
				case 2: iconPath = HEIGHT2; break;
				case 3: iconPath = HEIGHT3; break;
				case 4: iconPath = HEIGHT4; break;
				case 5: iconPath = HEIGHT5; break;
				default: iconPath = OUT_OF_BOUNDS; break;
			}

			// Icono para la meta (usa icono especial según altura)
			char goal_icon[MAX_LINE_LENGTH];
			if (map[i][j] >= 0 && map[i][j] <= 5) {
				snprintf(goal_icon, sizeof(goal_icon), "media/goal_height_%d.png", map[i][j]);
			} else {
				strncpy(goal_icon, OUT_OF_BOUNDS, sizeof(goal_icon));
				goal_icon[sizeof(goal_icon) - 1] = '\0';
			}

			if (j == xPlayer && i == yPlayer) // si se movió el jugador, actualizar su icono
				fprintf(interfaceFile, "%s", PLAYER_ICON);
			else if (j == xAI && i == yAI) // si se movió la IA, actualizar su icono
				fprintf(interfaceFile, "%s", BOT_ICON);
			else if (j == currentGoalX && i == currentGoalY) // mantener icono de la meta
				fprintf(interfaceFile, "%s", goal_icon);
			else
				fprintf(interfaceFile, "%s", iconPath);

			if (j < MAP_TEST_SIZE - 1) // agregar coma de separacion si no es el ultimo elemento
				fprintf(interfaceFile, ",");
		}
		fprintf(interfaceFile, "\n"); // nueva linea al final de cada fila
	}

	fclose(interfaceFile);
}

/* Actualizar el icono del jugador en la interfaz */
void updateInterfacePlayer(int xOld, int yOld, int hOld, int xPlayer, int yPlayer) {
	// Actualizar las posiciones actuales
	currentPlayerX = xPlayer;
	currentPlayerY = yPlayer;
	regenerateInterface(currentPlayerX, currentPlayerY, currentAIX, currentAIY);
}

/* Actualizar el icono de la IA en la interfaz */
void updateInterfaceAI(int xOld, int yOld, int hOld, int xAI, int yAI) {
	// Actualizar las posiciones actuales
	currentAIX = xAI;
	currentAIY = yAI;
	regenerateInterface(currentPlayerX, currentPlayerY, currentAIX, currentAIY);
}

#endif