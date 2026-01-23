#ifndef DISPOSITIVOS_H
#define DISPOSITIVOS_H

#include "definiciones.h"

/* Prototipos de funciones estáticas */
static const char* obtenerNombreDispositivo(int id);
static void usar_Gaius(jugador *usuario, int turno);
static void usar_Quadratus(jugador *rival, int turno);
static void usar_Hydrus(jugador *rival, int turno);
static void usar_Phalanx(jugador *usuario, int turno);
static void usar_Argus(jugador *usuario, jugador *rival, int turno);
static int usar_Argus_ia(jugador *ia, jugador *humano, int turno);
static int usar_Quadratus_ia(jugador *rival, int turno);
static int usar_Phalanx_ia(jugador *ia, int Dest_x, int Dest_y, int turno);
static int usar_Hydrus_ia(jugador *rival, int turno);
static int usar_Gaius_ia(int x, int y, int opcion, int turno);


//Funcion "obtenerNombreDispositivo" UNICAMENTE PARA ARGUS. 
//Convierte IDs a strings para hacer comparacion ya que el struct maneja los dispositivos en strings y para recuperar
//los dispositivos no seleccionados debemos hacer la conversion.
static const char* obtenerNombreDispositivo(int id) {
	switch(id) {
		case GAIUS: return "Gaius";
		case QUADRATUS: return "Quadratus";
		case HYDRUS: return "Hydrus";
		case PHALANX: return "Phalanx";
		case ARGUS: return "Argus";
		default: return "Desconocido";
	}
}

/* Este archivo esta reservado para funciones de dispositivos */

void aplicar_efecto_dispositivo(jugador *sujeto, jugador *rival, dispositivousuario *disp, int turno, int id_jugador) {
	char* nombre = disp->nombre;
	if (strcmp(nombre, "Gaius") == 0) {
		usar_Gaius(sujeto, turno);
	} else if (strcmp(nombre, "Quadratus") == 0) {
		usar_Quadratus(rival, turno);
	} else if (strcmp(nombre, "Hydrus") == 0) {
		usar_Hydrus(rival, turno);
	} else if (strcmp(nombre, "Phalanx") == 0) {
		usar_Phalanx(sujeto, turno);
	} else if (strcmp(nombre, "Argus") == 0) {
		usar_Argus(sujeto, rival, turno);
	} else {
		printf("Dispositivo desconocido. No se aplica ningun efecto.\n");
	}

	return;
}
	

static void usar_Gaius(jugador *usuario, int turno) {
	
	int **mapa = getMap();
	printf("\n--- GAIUS ACTIVADO: Modificador de Terreno ---\n");
	
	for (int i = 0; i < 3; i++) {
		int x;
		int y;
		int opcion;
		printf("\n[Modificacion %d/3] Ingresa coordenada Y (o -1 para terminar): ", i + 1);
		if (scanf("%d", &y) != 1) {
			break; 
		}
		
		printf("\n[Modificacion %d/3] Ingresa coordenada X (o -1 para terminar): ", i + 1);
		if (scanf("%d", &x) != 1) {
			break;
		}
		
		// Salida anticipada si el usuario no quiere usar los 3
		if (x == -1) {
			printf("Terminando uso de Gaius.\n");
			break;
		}

		// Validacion rangos 
		if (x < 0 || x >= MAP_TEST_SIZE || y < 0 || y >= MAP_TEST_SIZE) {
			printf("Error: Coordenadas fuera del mapa.\n");
			i--; // No gastamos el intento si se equivocó de tecla
			continue; 
		}

       
		int alturaActual = mapa[y][x]; 
		printf("Altura en (%d, %d) es: %d. \n  1: Subir (+1)\n  2: Bajar (-1)\n  Elige: ", x, y, alturaActual);
		scanf("%d", &opcion);

		int nuevaAltura = alturaActual;
		
		// Calcular nueva altura
		if (opcion == 1) {
			nuevaAltura++;
		} else if (opcion == 2) {
			nuevaAltura--;
		} else {
			printf("Opcion invalida.\n");
			i--;
			continue;
		}

		// Escribir en el mapa
		writeMap(x, y, nuevaAltura);
		
		// Si writeMap detecta un error la altura no cambia.
		if (mapa[y][x] != alturaActual) {
			printf("Exito!!! Nueva altura: %d\n", mapa[y][x]);
			
			// Registrar en log.
			registerCompleteAction(turno, 1, USE_DEVICE, GAIUS, x, y, 
				alturaActual, mapa[y][x], 0, 
				NONE, NONE, NONE, 1);
		} else {
			printf("Aviso: No se pudo modificar (limites de altura alcanzados).\n");
		}
	}
}


static void usar_Phalanx(jugador *usuario, int turno) {
	
	int **mapa = getMap();
	printf("\n--- PHALANX ACTIVADO: Equipo de Escalada ---\n");

	int Dest_x; 
	int Dest_y;
	
	printf("Ingresa coordenada de destino Y: ");
	scanf("%d", &Dest_y);

	printf("Ingresa coordenada de destino X: ");
	scanf("%d", &Dest_x);

	// Validacion limites del mapa
	if (Dest_x < 0 || Dest_x >= MAP_TEST_SIZE || Dest_y < 0 || Dest_y >= MAP_TEST_SIZE) {
		printf("Error: Coordenada fuera del mapa.\n");
		// Registrar fallo en log
		registerCompleteAction(turno, 1, USE_DEVICE, PHALANX, Dest_x, Dest_y, 0, 0, 0, NONE, NONE, NONE, 0);
		return;
	}

	// Validacion distancia de modulos, no valido para diagonales (Distancia "Manhattan" debe ser 1)
	if (abs(usuario->x - Dest_x) + abs(usuario->y - Dest_y) != 1) {
		printf("Error: Phalanx solo permite moverse a casillas adyacentes (no diagonales). QUE PENAAAA!.\n");
		registerCompleteAction(turno, 1, USE_DEVICE, PHALANX, Dest_x, Dest_y, 0, 0, 0, NONE, NONE, NONE, 0);
		return;
	}

	// Obtener alturas
	int h_Origen = mapa[usuario->y][usuario->x];
	int h_Destino = mapa[Dest_y][Dest_x];

	// Verificacion de subida. El destino debe ser mas alto
	if (h_Destino <= h_Origen) {
		printf("Error: Phalanx solo sirve para SUBIR alturas.\n");
		registerCompleteAction(turno, 1, USE_DEVICE, PHALANX, Dest_x, Dest_y, 0, 0, 0, NONE, NONE, NONE, 0);
		return;
	}
	
	// Verificacion de altura, no mas de 3 metros de dif.
	if (h_Destino - h_Origen > 3) {
		printf("Error: El modulo es demasiado alto (> 3 metros).\n");
		registerCompleteAction(turno, 1, USE_DEVICE, PHALANX, Dest_x, Dest_y, 0, 0, 0, NONE, NONE, NONE, 0);
		return;
	}

	// EJECUTAR MOV
	usuario->x = Dest_x;
	usuario->y = Dest_y;
	printf("¡Subida exitosa! Te has movido a (%d, %d) con altura %d. WIIIIIIII!\n", Dest_x, Dest_y, h_Destino);

	// Registrar en log
	registerCompleteAction(turno, 1, USE_DEVICE, PHALANX, Dest_x, Dest_y, 
		h_Origen, h_Destino, 0, NONE, NONE, NONE, 1);
}


static void usar_Hydrus(jugador *rival, int turno) {
	
	printf("\n--- HYDRUS ACTIVADO: Congelacion ---\n");
	
	// Aplicar efecto
	rival->bloqueado = 1; 
	
	printf("El oponente ha sido marcado con hielo. NO podra moverse en su siguiente turno.\n WARNING!!! Todavia puede usar sus dispositivos. \n");

	// Registrar en Log
	// "frozenTurns = 1"
	registerCompleteAction(turno, 1, USE_DEVICE, HYDRUS, rival->x, rival->y, 
		0, 0, 1, NONE, NONE, NONE, 1);
}



static void usar_Quadratus(jugador *rival, int turno) {
	
	printf("\n--- QUADRATUS ACTIVADO: Bloqueo de Sistema ---\n");
	
	int idObjetivo;
	
	printf("¿Que dispositivo quieres bloquear al rival?\n");
	printf("1. Gaius\n2. Quadratus\n3. Hydrus\n4. Phalanx\n5. Argus\n");
	printf("Selecciona el ID (1-5): ");
	
	if (scanf("%d", &idObjetivo) != 1){
		return;
	}
	// Convertimos ID a string de nombre para buscar en el inventario opuesto
	char nombreBuscado[16];
	
	switch (idObjetivo) {
		case GAIUS:
			strcpy(nombreBuscado, "Gaius"); 
			break;
			
		case QUADRATUS: 
			strcpy(nombreBuscado, "Quadratus");
			break;
			
		case HYDRUS:
			strcpy(nombreBuscado, "Hydrus"); 
			break;
			
		case PHALANX:
			strcpy(nombreBuscado, "Phalanx");
			break;
			
		case ARGUS:
			strcpy(nombreBuscado, "Argus");
			break;
			
		default:
			printf("ID Invalido. Pierdes el turno del dispositivo. BOOOOO\n");
			// Registramos fallo
			registerCompleteAction(turno, 1, USE_DEVICE, QUADRATUS, 0, 0, 0, 0, 0, NONE, NONE, NONE, 0);
			return;
	}

	// Busqueda de nombre en el inventario del rival
	int encontrado = 0;
	for (int i = 0; i < 3; i++) {
		// Comparacion de nombre que convertimos con lo que tiene el rival
		if (strcasecmp(rival->inventario[i].nombre, nombreBuscado) == 0) {
			
			// Bloqueamos si es que ya estaba disponible
			if (rival->inventario[i].disponible == 1) {
				rival->inventario[i].disponible = 0; 
				printf("Exito!!! El dispositivo %s del rival ha sido inutilizado. SIQUESIIII\n", nombreBuscado);
				encontrado = 1;
				
				// Registrar en log.
				registerCompleteAction(turno, 1, USE_DEVICE, QUADRATUS, 0, 0, 0, 0, 0, 
					(DeviceType)idObjetivo, NONE, NONE, 1);
				break;
			}
		}
	}

	if (!encontrado) {
		printf("Fallo: El rival no tiene un %s disponible.\n", nombreBuscado);
		registerCompleteAction(turno, 1, USE_DEVICE, QUADRATUS, 0, 0, 0, 0, 0, (DeviceType)idObjetivo, NONE, NONE, 0);
	}
}


static void usar_Argus(jugador *yo, jugador *rival, int turno) {
	printf("\n--- ARGUS ACTIVADO: Recuperacion de descartes ---\n");

	// DEBEMOS DETECTAR DESCARTES
	int rivalTiene[6] = {0, 0, 0, 0, 0, 0}; 

	// Recorremos el inventario del rival para ver que tiene
	for (int i = 0; i < 3; i++) {
		for (int id = 1; id <= 5; id++) {
			if (strcasecmp(rival->inventario[i].nombre, obtenerNombreDispositivo(id)) == 0) {
				rivalTiene[id] = 1; 
			}
		}
	}

	// MOSTRAR SOLO DESCARTES
	printf("\nDispositivos en el 'Vacio' (Descartes):\n");
	int existenDescartes = 0;
	
	// Si es 0, es un descarte
	for (int id = 1; id <= 5; id++) {
		if (rivalTiene[id] == 0) { 
			printf("%d. %s\n", id, obtenerNombreDispositivo(id));
			existenDescartes++;
		}
	}

	// SELECCIONAR QUE RECUPERAR
	int idRecuperar;
	printf("Elige el ID del dispositivo a recuperar: ");
	if (scanf("%d", &idRecuperar) != 1) return;

	// Validamos
	if (idRecuperar < 1 || idRecuperar > 5 || rivalTiene[idRecuperar] == 1) {
		printf("Error: Ese dispositivo no esta en el vacio (o no existe).\n");
		registerCompleteAction(turno, 1, USE_DEVICE, ARGUS, 0, 0, 0, 0, 0, NONE, NONE, NONE, 0);
		return;
	}

	// SELECCIONAR QUE SE ROBA AL RIVAL
	printf("\nInventario del Rival:\n");
	for (int i = 0; i < 3; i++) {
		if (rival->inventario[i].disponible)
			printf("%d. %s\n", i + 1, rival->inventario[i].nombre);
		else
			printf("%d. [NO DISPONIBLE]\n", i + 1);
	}

	int slotRival;
	printf("Elige el slot del rival a reemplazar (1-3): ");
	scanf("%d", &slotRival);
	int idxRival = slotRival - 1;

	if (idxRival < 0 || idxRival > 2 || !rival->inventario[idxRival].disponible) {
		printf("Error: Slot invalido o agotado.\n");
		registerCompleteAction(turno, 1, USE_DEVICE, ARGUS, 0, 0, 0, 0, 0, NONE, NONE, NONE, 0);
		return;
	}

	// Ejecutar cambio
	printf("Intercambio!!! !!El rival pierde %s y recibe %s. WOOOOOW\n", 
		rival->inventario[idxRival].nombre, obtenerNombreDispositivo(idRecuperar));

	// Sobreescribimos el nombre en el inventario del rival
	strcpy(rival->inventario[idxRival].nombre, obtenerNombreDispositivo(idRecuperar));
	
	// Le asignamos el nivel por defecto correspondiente a la ID
	rival->inventario[idxRival].nivel = idRecuperar; 
	
	// Marcar dispositivo nuevo como disponible
	rival->inventario[idxRival].disponible = 1;

	// Registrar en log.
	registerCompleteAction(turno, 1, USE_DEVICE, ARGUS, 0, 0, 0, 0, 0, NONE, NONE, NONE, 1);
}


// **FUNCIONES DE DISPOSITIVOS EXCLUSIVAS PARA LA IA**

static int usar_Gaius_ia(int x, int y, int opcion, int turno) {
	
	int **mapa = getMap();

	// Validar limites
	if (x < 0 || x >= MAP_TEST_SIZE || y < 0 || y >= MAP_TEST_SIZE) {
		return 0;
	}

	// Aplicar logica
	int alturaActual = mapa[y][x];
	int nuevaAltura = alturaActual;

	if (opcion == 1) {
		nuevaAltura++;
	}
	
	else if (opcion == 2) { 
		nuevaAltura--;
	}
	
	else {
		return 0;
	}

	// Escribir mapa
	writeMap(x, y, nuevaAltura);

	// Registrar en log si hubo cambio
	if (mapa[y][x] != alturaActual) {
		printf("IA Usa GAIUS en (%d,%d). Altura %d -> %d\n", x, y, alturaActual, mapa[y][x]);
		registerCompleteAction(turno, 2, USE_DEVICE, GAIUS, x, y, 
			alturaActual, mapa[y][x], 0, NONE, NONE, NONE, 1);
		return 1;
	}
	return 0;
}

static int usar_Hydrus_ia(jugador *rival, int turno) {
	
	rival->bloqueado = 1;
	
	printf("[IA] Usa HYDRUS. Te han congelao!!!\n");
	
	registerCompleteAction(turno, 2, USE_DEVICE, HYDRUS, rival->x, rival->y, 
		0, 0, 1, NONE, NONE, NONE, 1);
	return 1;
}

static int usar_Phalanx_ia(jugador *ia, int Dest_x, int Dest_y, int turno) {
	
	int **mapa = getMap();

	// Validacion de limites
	if (Dest_x< 0 || Dest_x >= MAP_TEST_SIZE || Dest_y < 0 || Dest_y >= MAP_TEST_SIZE) {
		return 0;
	}
	
	// No diagonales. Distancia Manhattan == 1
	if (abs(ia->x - Dest_x) + abs(ia->y - Dest_y) != 1) {
		return 0;
	}
	
	int h_Origen = mapa[ia->y][ia->x];
	int h_Destino = mapa[Dest_y][Dest_x];

	// Solo subida
	if (h_Destino <= h_Origen) {
		return 0;
	}
	
	// Diferencia de alturas
	if (h_Destino - h_Origen > 3) {
		return 0;
	}
	
	// Mover
	ia->x = Dest_x;
	ia->y = Dest_y;
	ia->h = h_Destino;
	
	printf("IA Usa PHALANX para escalar a (%d,%d)\n", Dest_x, Dest_y);
	
	registerCompleteAction(turno, 2, USE_DEVICE, PHALANX, Dest_x, Dest_y, 
		h_Origen, h_Destino, 0, NONE, NONE, NONE, 1);
	return 1;
}

static int usar_Quadratus_ia(jugador *rival, int turno) {
	
	// Buscamos dispositivos disponibles
	int slotsDisponibles[3];
	int cantidad = 0;

	for(int i = 0; i < 3; i++) {
		if(rival->inventario[i].disponible == 1) {
			slotsDisponibles[cantidad] = i; 
			cantidad++;
		}
	}

	// Si no hay disposi. disponibles falla
	if (cantidad == 0) {
		return 0;
	}

	//ELIGE UNO AL AZAR USANDO RAND()
	int indiceAleatorio = rand() % cantidad;
	int slotA_Romper = slotsDisponibles[indiceAleatorio];

	// Bloquea
	rival->inventario[slotA_Romper].disponible = 0;
	
	printf("IA Usa QUADRATUS. Ha bloqueado tu %s. DAAAAAAAMN\n", rival->inventario[slotA_Romper+1].nombre);
	
	// Registrar en log
	registerCompleteAction(turno, 2, USE_DEVICE, QUADRATUS, 0, 0, 0, 0, 0, QUADRATUS, NONE, NONE, 1);
	
	return 1;
}

static int usar_Argus_ia(jugador *ia, jugador *humano, int turno) {
	
	printf("[IA] Activando ARGUS...\n");

	// DETECTAR DESCARTES
	int humanoTiene[6] = {0, 0, 0, 0, 0, 0}; 

	for (int i = 0; i < 3; i++) {
		for (int id = 1; id <= 5; id++) {
			
			if (strcasecmp(humano->inventario[i].nombre, obtenerNombreDispositivo(id)) == 0) {
				humanoTiene[id] = 1; 
			}
		}
	}

	int listaDescartes[5];
	int cantidadDescartes = 0;
	
	for (int id = 1; id <= 5; id++) {
		if (humanoTiene[id] == 0) { // Si el humano no lo tiene, es un descarte
			listaDescartes[cantidadDescartes] = id;
			cantidadDescartes++;
		}
	}

	// IA ELIGE UN DISPOSITIVO DEL VACIO AL AZAR USANDO RAND()
	int indiceAleatorio = rand() % cantidadDescartes;
	int idA_Recuperar = listaDescartes[indiceAleatorio];

	// IA ELIGE QUE ESPACIO DEL HUMANO ATACAR
	int slotsDisponibles[3];
	int cantidadSlots = 0;

	for (int i = 0; i < 3; i++) {
		if (humano->inventario[i].disponible == 1) {
			slotsDisponibles[cantidadSlots] = i;
			cantidadSlots++;
		}
	}

	// Si el humano no tiene nada disponible, usa cualquier espacio
	int slotObjetivo;
	if (cantidadSlots > 0) {
		slotObjetivo = slotsDisponibles[rand() % cantidadSlots];
	} else {
		slotObjetivo = rand() % 3;
	}

	// EJECUTAR CAMBIO
	const char* nombreNuevo = obtenerNombreDispositivo(idA_Recuperar);
	const char* nombreViejo = humano->inventario[slotObjetivo].nombre;

	printf("[IA] ARGUS: La IA ha reemplazado tu %s por un %s! WHAAAAT?\n", nombreViejo, nombreNuevo);

	// Sobreescribir inventario
	strcpy(humano->inventario[slotObjetivo].nombre, nombreNuevo);
	humano->inventario[slotObjetivo].nivel = idA_Recuperar; // Asignamos nivel por defecto según ID
	humano->inventario[slotObjetivo].disponible = 1; // El nuevo item llega listo para usar

	// Registrar en lof.
	registerCompleteAction(turno, 2, USE_DEVICE, ARGUS, 0, 0, 0, 0, 0, NONE, NONE, NONE, 1);
	
	return 1;
}


/* La función usardispositivojugador está implementada en jugador.h */

#endif