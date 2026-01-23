#ifndef DISPOSITIVOS_H
#define DISPOSITIVOS_H

#include "definiciones.h"


//UNICAMENTE PARA ARGUS. Convierte IDs a strings para hacer comparacion ya que el struct maneja los dispositivos en strings y para recuperar
//los dispositivos no seleccionados debemos hacer la conversion.

// TRADUCCION:
// En definiciones.h, la estructura 'dispositivousuario' guarda el nombre
// del dispositivo como TEXTO (char nombre[16]), no guarda el ID numérico.
// la lógica usa NUMEROS (enum DeviceType: GAIUS=1, etc.) para ser más segura y evitar errores de tecleo.
// Esta función recibe el número (ID 1) y devuelve el texto 
// correspondiente ("Gaius") para poder compararlo con lo que hay guardado 
// en el inventario del jugador usando strcasecmp().


static const char* obtenerNombreDispositivo(int id) {
    switch(id) {
        case GAIUS:
			return "Gaius";
        case QUADRATUS:
			return "Quadratus";
        case HYDRUS:
			return "Hydrus";
        case PHALANX: 
			return "Phalanx";
        case ARGUS: 
			return "Argus";
        default: 
			return "Desconocido";
    }
}

/* Este archivo está reservado para funciones de dispositivos */

	
	

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

        // Validar rangos (Usando define de definiciones.h)
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
        
        // Si writeMap detectó error (ej: subir de 5 a 6), la altura no cambia.
        // Verificamos si cambió para informar al usuario
        if (mapa[y][x] != alturaActual) {
            printf("¡Exito! Nueva altura: %d\n", mapa[y][x]);
            
            // 5. REGISTRO EN LOG
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

    // Validar límites del mapa
    if (Dest_x < 0 || Dest_x >= MAP_TEST_SIZE || Dest_y < 0 || Dest_y >= MAP_TEST_SIZE) {
        printf("Error: Coordenada fuera del mapa.\n");
        // Registramos fallo en log
        registerCompleteAction(turno, 1, USE_DEVICE, PHALANX, Dest_x, Dest_y, 0, 0, 0, NONE, NONE, NONE, 0);
        return;
    }

    // Validar Adyacencia (Distancia "Manhattan"(?) debe ser 1)
    // abs() requiere <stdlib.h> o <math.h>, incluidos en definiciones.h
    if (abs(usuario->x - Dest_x) + abs(usuario->y - Dest_y) != 1) {
        printf("Error: Phalanx solo permite moverse a casillas adyacentes (no diagonales).\n");
        registerCompleteAction(turno, 1, USE_DEVICE, PHALANX, Dest_x, Dest_y, 0, 0, 0, NONE, NONE, NONE, 0);
        return;
    }

    // Obtener alturas
    int h_Origen = mapa[usuario->y][usuario->x];
    int h_Destino = mapa[Dest_y][Dest_x];

    // Reglas especiales Phalanx 
    // Regla A: El destino debe ser mas alto (solo subida)
    if (h_Destino <= h_Origen) {
        printf("Error: Phalanx solo sirve para SUBIR alturas.\n");
        registerCompleteAction(turno, 1, USE_DEVICE, PHALANX, Dest_x, Dest_y, 0, 0, 0, NONE, NONE, NONE, 0);
        return;
    }
    
    // Regla B: Diferencia maxima de 3 metros
    if (h_Destino - h_Origen > 3) {
        printf("Error: La pared es demasiado alta (> 3 metros).\n");
        registerCompleteAction(turno, 1, USE_DEVICE, PHALANX, Dest_x, Dest_y, 0, 0, 0, NONE, NONE, NONE, 0);
        return;
    }

    // EJECUTAR MOVIMIENTO
    usuario->x = Dest_x;
    usuario->y = Dest_y;
    printf("¡Subida exitosa! Te has movido a (%d, %d) con altura %d.\n", Dest_x, Dest_y, h_Destino);

    // Registro exitoso
    registerCompleteAction(turno, 1, USE_DEVICE, PHALANX, Dest_x, Dest_y, 
                           h_Origen, h_Destino, 0, NONE, NONE, NONE, 1);
}


static void usar_Hydrus(jugador *rival, int turno) {
	
    printf("\n--- HYDRUS ACTIVADO: Congelacion ---\n");
    
    // Aplicar efecto
    // La variable 'bloqueado' será leída en el turno del rival para impedirle moverse
    rival->bloqueado = 1; 
    
    printf("El oponente ha sido marcado con hielo. NO podra moverse en su siguiente turno.\n !CUIDADO¡ Todavia puede usar sus dispositivos. \n");

    // Registro en Log
    // frozenTurns = 1
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
	// TRADUCTOR: Convertimos ID -> Nombre para buscar en el inventario del rival
	char nombreBuscado[16];
    
	switch (idObjetivo) {
	case GAIUS: strcpy(nombreBuscado, "Gaius"); break;
	case QUADRATUS: strcpy(nombreBuscado, "Quadratus"); break;
        case HYDRUS: strcpy(nombreBuscado, "Hydrus"); break;
        case PHALANX: strcpy(nombreBuscado, "Phalanx"); break;
        case ARGUS: strcpy(nombreBuscado, "Argus"); break;
			
        default:
            printf("ID Invalido. Pierdes el turno del dispositivo.\n");
            // Registramos fallo
            registerCompleteAction(turno, 1, USE_DEVICE, QUADRATUS, 0, 0, 0, 0, 0, NONE, NONE, NONE, 0);

            return;
    	}

	// BUSQUEDA: Ahora buscamos ese nombre en la mochila del rival
	int encontrado = 0;
	for (int i = 0; i < 3; i++) {
		// Comparamos el nombre que tradujimos con lo que tiene el rival
		if (strcasecmp(rival->inventario[i].nombre, nombreBuscado) == 0) {
			// Verificamos si ya estaba gastado
			if (rival->inventario[i].disponible == 1) {
				rival->inventario[i].disponible = 0; // ¡BLOQUEADO!
				printf("¡Exito! El dispositivo %s del rival ha sido inutilizado.\n", nombreBuscado);
				encontrado = 1;
                
				// Registramos éxito
				registerCompleteAction(turno, 1, USE_DEVICE, QUADRATUS, 0, 0, 0, 0, 0, 
						(DeviceType)idObjetivo, NONE, NONE, 1);
				break; // Ya lo encontramos, salimos
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

	// DETECTAR DESCARTES
	// Usamos un array simple para marcar que items tiene el rival
	// Indices 1 a 5 (GAIUS a ARGUS). Iniciamos todo en 0 (No lo tiene)
	int rivalTiene[6] = {0, 0, 0, 0, 0, 0}; 

	// Recorremos el inventario del rival para ver qué tiene
	for (int i = 0; i < 3; i++) {
		// Buscamos cual es el ID de su dispositivo comparando nombres
      		// (Un poco rudimentario, pero necesario porque el struct guarda strings)
        	for (int id = 1; id <= 5; id++) {
        		if (strcasecmp(rival->inventario[i].nombre, obtenerNombreDispositivo(id)) == 0) {
                		rivalTiene[id] = 1; // Marcamos: "El rival tiene este ID"
			}
		}
	}

	// MOSTRAR SOLO DESCARTES (Los que quedaron en 0)
	printf("\nDispositivos en el 'Vacio' (Descartes):\n");
	int existenDescartes = 0;
    
	for (int id = 1; id <= 5; id++) {
		if (rivalTiene[id] == 0) { // Si es 0, es un descarte
        		printf("%d. %s\n", id, obtenerNombreDispositivo(id));
        		existenDescartes++;
		}
	}

	// SELECCIONAR QUE RECUPERAR
	int idRecuperar;
	printf("Elige el ID del dispositivo a recuperar: ");
	if (scanf("%d", &idRecuperar) != 1) return;

		// Validamos: Debe ser un ID valido (1-5) Y el rival NO debe tenerlo
		if (idRecuperar < 1 || idRecuperar > 5 || rivalTiene[idRecuperar] == 1) {
			printf("Error: Ese dispositivo no esta en el vacio (o no existe).\n");
			registerCompleteAction(turno, 1, USE_DEVICE, ARGUS, 0, 0, 0, 0, 0, NONE, NONE, NONE, 0);
			return;
		}

	// SELECCIONAR QUE ROBO AL RIVAL
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
	printf("¡Intercambio! El rival pierde %s y recibe %s. WOOOOOW\n", 
        	rival->inventario[idxRival].nombre, obtenerNombreDispositivo(idRecuperar));

	// Sobreescribimos el nombre en el inventario del rival
	strcpy(rival->inventario[idxRival].nombre, obtenerNombreDispositivo(idRecuperar));
    
	// Le asignamos el nivel por defecto correspondiente al ID (ej: Gaius = nivel 1)
	// Esto es un buen detalle para mantener la coherencia
	rival->inventario[idxRival].nivel = idRecuperar; 
    
	// El dispositivo llega "nuevo", asi que esta disponible
	rival->inventario[idxRival].disponible = 1;

	// Registrar Éxito
	registerCompleteAction(turno, 1, USE_DEVICE, ARGUS, 0, 0, 0, 0, 0, NONE, NONE, NONE, 1);
}


//FUNCIONES DE DISPOSITIVOS EXCLUSIVAS PARA LA IA
//(Sin scanf, sin preguntas, ejecutan directo)


// IA GAIUS: Recibe coordenadas y operación (1: subir, 2: bajar)
static int usar_Gaius_ia(int x, int y, int opcion, int turno) {
	
	int **mapa = getMap();

	// Validar limites
	if (x < 0 || x >= MAP_TEST_SIZE || y < 0 || y >= MAP_TEST_SIZE) {
		return 0;
	}

	// Aplicar lógica
	int alturaActual = mapa[y][x];
	int nuevaAltura = alturaActual;

	switch (opcion) {
		case 1: nuevaAltura++; break;
		case 2: nuevaAltura--; break;
		default: return 0; // Opcion invalida
	}
	
	// Escribir mapa
	writeMap(x, y, nuevaAltura);

	// Registrar si hubo cambio
	if (mapa[y][x] != alturaActual) {
		printf("IA Usa GAIUS en (%d,%d). Altura %d -> %d\n", x, y, alturaActual, mapa[y][x]);
		registerCompleteAction(turno, 2, USE_DEVICE, GAIUS, x, y, 
                               alturaActual, mapa[y][x], 0, NONE, NONE, NONE, 1);
		return 1; // Éxito
	}

	return 0; // Fallo
}

// IA HYDRUS: Solo necesita saber a quién congelar
static int usar_Hydrus_ia(jugador *rival, int turno) {
	
	rival->bloqueado = 1;
	
	printf("[IA] Usa HYDRUS. ¡Te han congelao!\n");
	
	registerCompleteAction(turno, 2, USE_DEVICE, HYDRUS, rival->x, rival->y, 
                           0, 0, 1, NONE, NONE, NONE, 1);
	return 1;
}

static int usar_Phalanx_ia(jugador *ia, int Dest_x, int Dest_y, int turno) {
	
	int **mapa = getMap();

	// Validaciones básicas
	if (Dest_x< 0 || Dest_x >= MAP_TEST_SIZE || Dest_y < 0 || Dest_y >= MAP_TEST_SIZE) {
		return 0;
	}
	
	// Distancia Manhattan == 1
	if (abs(ia->x - Dest_x) + abs(ia->y - Dest_y) != 1) {
		return 0;
	}
	
	int h_Origen = mapa[ia->y][ia->x];
	int h_Destino = mapa[Dest_y][Dest_x];

	// Reglas Phalanx (Solo subir, max 3)
	if (h_Destino <= h_Origen || h_Destino - h_Origen > 3) {
		return 0;
	}
	
	// Mover IA
	ia->x = Dest_x;
	ia->y = Dest_y;
	ia->h = h_Destino;
    
	printf("IA Usa PHALANX para escalar a (%d,%d)\n", Dest_x, Dest_y);
	
	registerCompleteAction(turno, 2, USE_DEVICE, PHALANX, Dest_x, Dest_y, 
                           h_Origen, h_Destino, 0, NONE, NONE, NONE, 1);
	
			   return 1;
}

static int usar_Quadratus_ia(jugador *rival, int turno) {
	
    // Busca el primer dispositivo disponible del humano y lo rompe
	for(int i=0; i<3; i++) {
		if(rival->inventario[i].disponible == 1) {
        		rival->inventario[i].disponible = 0;
		
			printf("IA Usa QUADRATUS. Ha bloqueado tu %s DAAAAAMN\n", rival->inventario[i].nombre);
 		
			// Convertimos el nombre a ID para el log (un poco truco, pero funcional)
			// Asumimos que registerCompleteAction aceptará NONE si no queremos complicarnos con el ID exacto ahora
			registerCompleteAction(turno, 2, USE_DEVICE, QUADRATUS, 0, 0, 0, 0, 0, QUADRATUS, NONE, NONE, 1);

			return 1;
		}
	}

	return 0; // No encontró nada que romper
}

static int ia_usar_Argus(jugador *ia, jugador *humano, int turno) {
	
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

	// CREAR LISTA DE CANDIDATOS
	int listaDescartes[5];
	int cantidadDescartes = 0;
    
	for (int id = 1; id <= 5; id++) {
		if (humanoTiene[id] == 0) { // Si el humano no lo tiene, es un descarte
        		listaDescartes[cantidadDescartes] = id;
        		cantidadDescartes++;
		}
	}

	// IA ELIGE UN DISPOSITIVO DEL VACIO (AL AZAR)
	// rand() % N te da un numero entre 0 y N-1
	int indiceAleatorio = rand() % cantidadDescartes;
	int idA_Recuperar = listaDescartes[indiceAleatorio];

	// IA ELIGE QUE ESPACIO DEL HUMANO ATACAR
	// Buscamos espacios que el humano tenga disponibles para hacer mas daño
	int slotsDisponibles[3];
	int cantidadSlots = 0;

	for (int i = 0; i < 3; i++) {
		if (humano->inventario[i].disponible == 1) {
			slotsDisponibles[cantidadSlots] = i;
			cantidadSlots++;
		}
	}

	// Si el humano no tiene nada disponible, atacamos cualquier slot (0, 1 o 2)
	int slotObjetivo;
    	if (cantidadSlots > 0) {
		slotObjetivo = slotsDisponibles[rand() % cantidadSlots];
	} else {
		slotObjetivo = rand() % 3;
	}

	// EJECUTAR CAMBIO
	const char* nombreNuevo = obtenerNombreDispositivo(idA_Recuperar);
	const char* nombreViejo = humano->inventario[slotObjetivo].nombre;

	printf("[IA] ARGUS: ¡La IA ha reemplazado tu %s por un %s! WHAAAAT?\n", nombreViejo, nombreNuevo);

	// Sobreescribir inventario del humano
	strcpy(humano->inventario[slotObjetivo].nombre, nombreNuevo);
	humano->inventario[slotObjetivo].nivel = idA_Recuperar; // Asignamos nivel por defecto según ID
	humano->inventario[slotObjetivo].disponible = 1; // El nuevo item llega listo para usar

	// Registro
	registerCompleteAction(turno, 2, USE_DEVICE, ARGUS, 0, 0, 0, 0, 0, NONE, NONE, NONE, 1);
    
	return 1;
}


void aplicar_efecto_dispositivo(jugador *sujeto, jugador *rival, dispositivousuario *disp, int turno, int id_jugador) {
	char *nombre = disp->nombre;

	if (id_jugador == 1) { // Usuario
		if (strcasecmp(nombre, "Gaius") == 0) {
			usar_Gaius(sujeto, turno);
		}
		else if (strcasecmp(nombre, "Quadratus") == 0) {
			usar_Quadratus(rival, turno);
		}
		else if (strcasecmp(nombre, "Hydrus") == 0) {
			usar_Hydrus(rival, turno);
		}
		else if (strcasecmp(nombre, "Phalanx") == 0) {
			usar_Phalanx(sujeto, turno);
		}
		else if (strcasecmp(nombre, "Argus") == 0) {
			usar_Argus(sujeto, rival, turno);
		}
	}
	else { // IA (id_jugador == 2)
		if (strcasecmp(nombre, "Gaius") == 0) {
        		usar_Gaius_ia(rival->x, rival->y, 1, turno);
		}
        	else if (strcasecmp(nombre, "Quadratus") == 0) {
        		usar_Quadratus_ia(rival, turno);
		}
		else if (strcasecmp(nombre, "Hydrus") == 0) {
        		usar_Hydrus_ia(rival, turno);
		}
		else if (strcasecmp(nombre, "Phalanx") == 0) {
			// Intenta escalar adyacente
        		if (usar_Phalanx_ia(sujeto, sujeto->x + 1, sujeto->y, turno) == 0) {
        			usar_Phalanx_ia(sujeto, sujeto->x, sujeto->y - 1, turno);
        		}
        	}
		else if (strcasecmp(nombre, "Argus") == 0) {
        		ia_usar_Argus(sujeto, rival, turno);
		}
	}
}

/* La función usardispositivojugador está implementada en jugador.h */

#endif