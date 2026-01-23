#ifndef ACTION_LOG_H
#define ACTION_LOG_H

#include "definiciones.h"

static FILE* logFile;

/* Prototipos de funciones estáticas */
static void createLogFile();
static char* actionToNotation(Action *action);

/* Crea un archivo de log con formato game_YYYYMMDD_HHMMSS.log */
static void createLogFile() {
        if (logFile != NULL) {
            return; // El archivo ya está abierto
        }

	time_t t = time(NULL);
	struct tm *date = localtime(&t);
	char logFilename[48];
	
	/* Formato: game_YYYYMMDD_HHMMSS.log */
	snprintf(logFilename, sizeof(logFilename), "logs/game_%04d%02d%02d_%02d%02d%02d.log",
			 date->tm_year + 1900, date->tm_mon + 1, date->tm_mday,
			 date->tm_hour, date->tm_min, date->tm_sec);
	
	logFile = fopen(logFilename, "w");
	
	if (!logFile) {
		perror("Error al crear archivo de log");
		return;
	}
}


// Conversión a notación compacta
// Ejemplos:
// "0.J1:COIN[C]->C+" = Turno 0, Jugador 1 eligió Cara, resultado Cara, ganó
// "0.J2:COIN[S]->C-" = Turno 0, Jugador 2 eligió Sello, resultado Cara, perdió
// "0.J1:SELECT[G]#1" = Turno 0, Jugador 1 selecciona Gaius como dispositivo #1
// "0.J1:SELECT[Q]#2" = Turno 0, Jugador 1 selecciona Quadratus como dispositivo #2
// "1.J1:N(5,3)+" = Turno 1, Jugador 1, Movimiento arriba desde (5,3), exitoso
// "2.J2:H(4,6)->1t+" = Turno 2, Jugador 2, Hydrus en (4,6), congela 1 turno, exitoso
// "3.J1:G(3,2):0->1+" = Turno 3, Jugador 1, Gaius en (3,2), altura 0->1, exitoso
// "4.J2:Q[P]+" = Turno 4, Jugador 2, Quadratus bloquea Phalanx, exitoso
// "5.J1:A[G<->H]+" = Turno 5, Jugador 1, Argus intercambia Gaius por Hydrus, exitoso
// "6.J2:X" = Turno 6, Jugador 2, Rendición
static char* actionToNotation(Action *action) {
	static char notation[50];
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	char typeChar[] = {'C', 'D', 'N', 'S', 'W', 'E', 'U', 'X'};
	char deviceChar[] = {'G', 'Q', 'H', 'P', 'A'};
	char result = action->successful ? '+' : '-';

	sprintf(notation, "[%02d:%02d:%02d] ", t->tm_hour, t->tm_min, t->tm_sec);
	
	// Verificar si es un uso de dispositivo
	if (action->actionType == USE_DEVICE) {
		if (action->device == HYDRUS) {
			// Congelación: "3.J1:H(4,6)->1t+"
			// (jugador congelado durante 1 turno, exitoso)
			sprintf(notation, "%d.J%d:%c(%d,%d)->%dt%c",
					action->turn, action->player, deviceChar[action->device],
					action->targetX, action->targetY,
					action->frozenTurns, result);
		}
		else if (action->device == GAIUS) {
			// Cambio de altura: "4.J2:G(3,2):0->1+"
			sprintf(notation, "%d.J%d:%c(%d,%d):%d->%d%c",
					action->turn, action->player, deviceChar[action->device],
					action->targetX, action->targetY,
					action->previousValue, action->newValue, result);
		}
		else if (action->device == QUADRATUS) {
			// Bloqueo de dispositivo: "4.J2:Q[P]+"
			// (bloquea dispositivo Phalanx del enemigo)
			sprintf(notation, "%d.J%d:%c[%c]%c",
					action->turn, action->player, deviceChar[action->device],
					deviceChar[action->blockedDevice], result);
		}
		else if (action->device == ARGUS) {
			// Intercambio de dispositivos: "5.J1:A[G<->H]+"
			// (intercambia Gaius por Hydrus)
			sprintf(notation, "%d.J%d:%c[%c<->%c]%c",
					action->turn, action->player, deviceChar[action->device],
					deviceChar[action->swapDevice1], deviceChar[action->swapDevice2],
					result);
		}
		else {
			// Otros dispositivos (Phalanx)
			sprintf(notation, "%d.J%d:%c%c",
					action->turn, action->player, deviceChar[action->device], result);
		}
	}
	else if (action->actionType == SURRENDER) {
		// Rendición: "6.J2:X"
		sprintf(notation, "%d.J%d:%c",
				action->turn, action->player, typeChar[action->actionType]);
	}
	else if (action->actionType == COIN_FLIP) {
		// Sorteo de moneda: "0.J1:COIN[C]->C+"
		// choice: 1=Cara, 2=Sello; result: 1=Cara, 2=Sello
		char choiceChar = (action->coinChoice == 1) ? 'C' : 'S';
		char resultChar = (action->coinResult == 1) ? 'C' : 'S';
		char outcome = action->successful ? '+' : '-';
		sprintf(notation, "%d.J%d:COIN[%c]->%c%c",
				action->turn, action->player, choiceChar, resultChar, outcome);
	}
	else if (action->actionType == SELECT_DEVICE) {
		// Selección de dispositivo: "0.J1:SELECT[G]#1"
		// selectedDevice: GAIUS, QUADRATUS, etc.; order: 1, 2, 3
		char deviceChar[] = {'X', 'G', 'Q', 'H', 'P', 'A'}; // X para NONE
		sprintf(notation, "%d.J%d:SELECT[%c]#%d",
				action->turn, action->player, 
				deviceChar[action->selectedDevice], 
				action->selectionOrder);
	}
	else {
		// Movimiento normal
		sprintf(notation, "%d.J%d:%c(%d,%d)%c",
				action->turn, action->player, typeChar[action->actionType],
				action->targetX, action->targetY, result);
	}
	
	return notation;
}

// Función para registrar acciones completas
// Ejemplos de uso:
// Jugador 1 usa Hydrus en (4,6), congela 1 turnos, exitoso:
// registerCompleteAction(3, 1, USE_DEVICE, HYDRUS, 4, 6, 0, 0, 1, GAIUS, GAIUS, GAIUS, 1);
// -> Escribe: "3.J1:H(4,6)->1t+"
//
// Jugador 2 usa Gaius en (3,2), cambia altura de 0 a 1, exitoso:
// registerCompleteAction(4, 2, USE_DEVICE, GAIUS, 3, 2, 0, 1, 0, GAIUS, GAIUS, GAIUS, 1);
// -> Escribe: "4.J2:G(3,2):0->1+"
//
// Jugador 1 usa Quadratus para bloquear Phalanx del enemigo:
// registerCompleteAction(5, 1, USE_DEVICE, QUADRATUS, 0, 0, 0, 0, 0, PHALANX, GAIUS, GAIUS, 1);
// -> Escribe: "5.J1:Q[P]+"
//
// Jugador 2 usa Argus para intercambiar Gaius por Hydrus:
// registerCompleteAction(6, 2, USE_DEVICE, ARGUS, 0, 0, 0, 0, 0, GAIUS, GAIUS, HYDRUS, 1);
// -> Escribe: "6.J2:A[G<->H]+"
void registerCompleteAction(int turn, int player, ActionType actionType,
							DeviceType device, int x, int y,
							int prevValue, int newVal, int turns,
							DeviceType blocked, DeviceType swap1, DeviceType swap2,
							int successful) {
	if (logFile == NULL) {
		createLogFile();
	}
	
	Action action;
	action.turn = turn;
	action.player = player;
	action.actionType = actionType;
	action.device = device;
	action.targetX = x;
	action.targetY = y;
	action.previousValue = prevValue;
	action.newValue = newVal;
	action.frozenTurns = turns;
	action.blockedDevice = blocked;
	action.swapDevice1 = swap1;
	action.swapDevice2 = swap2;
	action.selectedDevice = NONE;
	action.selectionOrder = 0;
	action.successful = successful;
	action.timestamp = time(NULL);
	
	fprintf(logFile, "%s\n", actionToNotation(&action));
	fflush(logFile);
}

// Función para registrar acciones simples (movimientos o rendición)
void registerSimpleAction(int turn, int player, ActionType actionType,
						 int x, int y, int successful) {
	registerCompleteAction(turn, player, actionType, NONE, x, y, 0, 0, 0,
						   NONE, NONE, NONE, successful);
}

// Función para registrar sorteo de moneda (cara y sello)
// Ejemplo de uso:
// Jugador 1 elige Cara (1), resultado es Cara (1), gana:
// registerCoinFlip(1, 1, 1);
// -> Escribe: "0.J1:COIN[C]->C+"
//
// Jugador 2 elige Sello (2), resultado es Cara (1), pierde:
// registerCoinFlip(2, 2, 1);
// -> Escribe: "0.J2:COIN[S]->C-"
void registerCoinFlip(int player, int choice, int result) {
	if (logFile == NULL) {
		createLogFile();
	}
	
	Action action;
	action.turn = 0;  // El sorteo siempre es turno 0
	action.player = player;
	action.actionType = COIN_FLIP;
	action.device = NONE;
	action.targetX = 0;
	action.targetY = 0;
	action.previousValue = 0;
	action.newValue = 0;
	action.frozenTurns = 0;
	action.blockedDevice = NONE;
	action.swapDevice1 = NONE;
	action.swapDevice2 = NONE;
	action.coinChoice = choice;  // 1=Cara, 2=Sello
	action.coinResult = result;  // 1=Cara, 2=Sello
	action.selectedDevice = NONE;
	action.selectionOrder = 0;
	action.successful = (choice == result) ? 1 : 0;  // Ganó si coincide
	action.timestamp = time(NULL);
	
	fprintf(logFile, "%s\n", actionToNotation(&action));
	fflush(logFile);
}

// Función para registrar selección de dispositivos (pre-juego)
// Ejemplo de uso:
// Jugador 1 selecciona Gaius como su primer dispositivo:
// registerDeviceSelection(1, GAIUS, 1);
// -> Escribe: "0.J1:SELECT[G]#1"
//
// Jugador 2 selecciona Hydrus como su segundo dispositivo:
// registerDeviceSelection(2, HYDRUS, 2);
// -> Escribe: "0.J2:SELECT[H]#2"
void registerDeviceSelection(int player, DeviceType device, int order) {
	if (logFile == NULL) {
		createLogFile();
	}
	
	Action action;
	action.turn = 0;  // La selección siempre es turno 0 (pre-juego)
	action.player = player;
	action.actionType = SELECT_DEVICE;
	action.device = NONE;
	action.targetX = 0;
	action.targetY = 0;
	action.previousValue = 0;
	action.newValue = 0;
	action.frozenTurns = 0;
	action.blockedDevice = NONE;
	action.swapDevice1 = NONE;
	action.swapDevice2 = NONE;
	action.coinChoice = 0;
	action.coinResult = 0;
	action.selectedDevice = device;  // GAIUS, QUADRATUS, HYDRUS, PHALANX, ARGUS
	action.selectionOrder = order;   // 1, 2 o 3
	action.successful = 1;  // La selección siempre es exitosa
	action.timestamp = time(NULL);
	
	fprintf(logFile, "%s\n", actionToNotation(&action));
	fflush(logFile);
}

#endif // ACTION_LOG_H
