#ifndef DEFINICIONES_H
#define DEFINICIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>



/* ==================== DEFINES DE IO.H ==================== */

/* Definiciones de mapa */

#define MAX_VERTICAL 32
#define MAX_HORIZONTAL 32
#define MAX_HEIGHT 5
#define MIN_HEIGHT 0
#define MAP_TEST "maps/map_test.txt"
#define MAP_TEST_SIZE 8

/* Definiciones de interfaz */

#define INTERFACE_FILE "interfaz.txt"
#define INTERFACE_TMP_FILE "interfaz_tmp.txt"
#define HEIGHT0 "media/height_0.png"
#define HEIGHT1 "media/height_1.png"
#define HEIGHT2 "media/height_2.png"
#define HEIGHT3 "media/height_3.png"
#define HEIGHT4 "media/height_4.png"
#define HEIGHT5 "media/height_5.png"
#define OUT_OF_BOUNDS "media/out_of_bounds.png"
#define PLAYER_ICON "media/protoman.png"
#define BOT_ICON "media/rockman.png"
#define MAX_LINE_LENGTH 256

/* ==================== DEFINES DE SEARCH.H ==================== */
#define MAX_DIFF 2
#define MAX_DEPTH 1024

/* ==================== TIPOS ENUMERADOS DE LOG.H ==================== */
typedef enum {
	NONE = 0,
	GAIUS = 1,
	QUADRATUS = 2,
	HYDRUS = 3,
	PHALANX = 4,
	ARGUS = 5
} DeviceType;

typedef enum {
	COIN_FLIP = 0,
	SELECT_DEVICE = 1,
	MOVE_UP = 2,
	MOVE_DOWN = 3,
	MOVE_LEFT = 4,
	MOVE_RIGHT = 5,
	USE_DEVICE = 6,
	SURRENDER = 7
} ActionType;

/* ==================== ESTRUCTURAS ==================== */
typedef struct {
	char nombre[16];
	int disponible;
	int nivel;
} dispositivousuario;

typedef struct {
	int x;
	int y;
	int h;
	dispositivousuario inventario[3];
	int bloqueado;
	int rendido;
	int puedeSubir3;
} jugador;

typedef struct {
	int turn;
	int player;
	ActionType actionType;
	DeviceType device;
	int targetX;
	int targetY;
	int previousValue;
	int newValue;
	int frozenTurns;
	DeviceType blockedDevice;
	DeviceType swapDevice1;
	DeviceType swapDevice2;
	int coinChoice;
	int coinResult;
	DeviceType selectedDevice;
	int selectionOrder;
	int successful;
	time_t timestamp;
} Action;

struct pos {
	int x;
	int y;
	int h;
};

/* ==================== PROTOTIPOS DE ESCENARIO.H ==================== */
void generar_escenario(int escenario[8][8]);
void coordenadas_inicio(int *x, int *y, int x_meta, int y_meta);

/* ==================== PROTOTIPOS DE SEARCH.H ==================== */
int distanceTo(int xStart, int yStart, int xEnd, int yEnd);
struct pos* getNext(int xStart, int yStart, int xEnd, int yEnd);
int posGetX(struct pos target);
int posGetY(struct pos target);
int posGetH(struct pos target);

/* ==================== PROTOTIPOS DE IO.H ==================== */
int** writeMap(int x, int y, int value);
int** getMap();
void freeMap();
void printMap();
void initializeInterface(int xPlayer, int yPlayer, int xAI, int yAI, int xGoal, int yGoal);
void updateInterfaceHeight(int x, int y, int h);
void updateInterfacePlayer(int xOld, int yOld, int hOld, int xPlayer, int yPlayer);
void updateInterfaceAI(int xOld, int yOld, int hOld, int xAI, int yAI);

/* ==================== PROTOTIPOS DE INTERFAZ.H ==================== */
char pedirmovimiento(int turno);
int mostrarmenuturno(int turno, int yausodispositivo);
int realizarsorteocarasello();
void seleccionardispositivos(dispositivousuario misdispositivos[]);
void mostrardispositivos(dispositivousuario misdispositivos[], int cantidad);
void usardispositivo(int turno, dispositivousuario *disp);
void turnodeia(int turno);

/* ==================== PROTOTIPOS DE JUGADOR.H ==================== */
void turno_jugador(jugador *j, jugador *oponente, int turno);


/* ==================== PROTOTIPOS DE DISPOSITIVOS.H ==================== */

/* ==================== PROTOTIPOS DE IA.H ==================== */
int ia_distancia_objetivo(int xStart, int yStart, int xEnd, int yEnd);
int ia_siguiente_paso(int xStart, int yStart, int xEnd, int yEnd, int *nx, int *ny);
void turnoIA(jugador *ia, jugador *jugador, int metaX, int metaY, int turno);

/* ==================== PROTOTIPOS DE LOG.H ==================== */
void registerCompleteAction(int turn, int player, ActionType actionType,
							DeviceType device, int x, int y,
							int prevValue, int newVal, int turns,
							DeviceType blocked, DeviceType swap1, DeviceType swap2,
							int successful);
void registerSimpleAction(int turn, int player, ActionType actionType,
						 int x, int y, int successful);
void registerCoinFlip(int player, int choice, int result);
void registerDeviceSelection(int player, DeviceType device, int order);

/* ==================== PROTOTIPOS DE MINIJUEGOS.H ==================== */
int ejecutarMinijuego(int numeroUso, int jugador);
void activar_dispositivo(int turno, int id_jugador, jugador *sujeto, jugador *rival);
void seleccionardispositivosIA(dispositivousuario inventarioIA[], dispositivousuario inventarioJugador[], int sorteo);
void aplicar_efecto_dispositivo(jugador *sujeto, jugador *rival, dispositivousuario *disp, int turno, int id_jugador);


#endif