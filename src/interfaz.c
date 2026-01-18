#include "interfaz.h"

void registraraccion(FILE *log, char *actor, char *accion) {
    if (log == NULL) return;
    time_t ahora = time(NULL);
    struct tm *t = localtime(&ahora);
    fprintf(log, "[%02d:%02d] %s: %s\n", t->tm_hour, t->tm_min, actor, accion);
    fflush(log);
}

//pide una opcion entre wasd al usuario
char pedirmovimiento(FILE *log) {
    char tecla;
    int esvalida = 0; 
    int leidos;       

    while (!esvalida) {
        printf("Moverse (W: arriba, A: izquierda, S: abajo, D: derecha): ");

        
        leidos = scanf(" %c", &tecla);

        
        if (leidos != 1) {
            printf("Error de lectura.\n");
            while (getchar() != '\n'); 
            continue;
        }

        
        if (tecla == 'w' || tecla == 'W') {
            registraraccion(log, "Usuario", "Intenta movimiento: arriba");
            esvalida = 1;
        }
        else if (tecla == 'a' || tecla == 'A') {
            registraraccion(log, "Usuario", "Intenta movimiento: izquierda");
            esvalida = 1;
        }
        else if (tecla == 's' || tecla == 'S') {
            registraraccion(log, "Usuario", "Intenta movimiento: abajo");
            esvalida = 1;
        }
        else if (tecla == 'd' || tecla == 'D') {
            registraraccion(log, "Usuario", "Intenta movimiento: derecha");
            esvalida = 1;
        }
        else {
            printf("Tecla invalida. Usa W, A, S o D.\n");
           
        }

        if (!esvalida || getchar() != '\n') { 
             while (getchar() != '\n');
        }
    }

    return tecla;
}



int mostrarmenuturno(FILE *log, int yausodispositivo) {
    int opcion;
    int esvalida = 0;
    int leidos;

    printf("\n   MENU DE TURNO    \n");
    printf("1. Moverse\n");
    if (yausodispositivo == 0) {
        printf("2. Usar Dispositivo\n");
    } else {
        printf("2. Ya usaste un dispositivo durante este turno\n");
    }
    printf("3. Rendirse\n");

    while (!esvalida) {
        printf("Elige una opcion: ");
        leidos = scanf("%d", &opcion);

        if (leidos != 1) {
            printf("Entrada invalida. Debe ser un numero.\n");
            while (getchar() != '\n');
            continue;
        }

        if (opcion == 1) {
            registraraccion(log, "Usuario", "selecciono opcion 1: Moverse");
            esvalida = 1;
        }
        else if (opcion == 2 && yausodispositivo == 0) {
            registraraccion(log, "Usuario", "selecciono opcion 2: Usar dispositivo");
            esvalida = 1;
        }
        else if (opcion == 3) {
            registraraccion(log, "Usuario", "selecciono opcion 3: Rendirse");
            esvalida = 1;
        }
        else {
            printf("Opcion no permitida.\n");
        }

        while (getchar() != '\n');
    }

    return opcion;
}

int realizarsorteocarasello(FILE *log) {
    int eleccion, resultado, escanear;

    printf("\n SORTEO CARA Y SELLO  \n 1-Cara\n 2-Sello\n");
    while (1) {
        printf("Selecciona una opcion: ");
        escanear = scanf("%d", &eleccion);
        // Si (leyó bien Y es 1) O (leyó bien Y es 2)
        if ((escanear == 1 && eleccion == 1) || (escanear == 1 && eleccion == 2)) {
         break;
        }
        printf("Invalido. Use 1 o 2.\n");
        while (getchar() != '\n');
    }

    resultado = (rand() % 2) + 1;// resultado aleatorio
    if (resultado == 1) {
     printf("Resultado: CARA\n");
    } 
				else {
     printf("Resultado: SELLO\n");
    }

    if (eleccion == resultado) {
        
        registraraccion(log, "Sistema", "Usuario gana sorteo");
        return 1; //retorna 1 si el usuario gano
    } else {
        registraraccion(log, "Sistema", "Ia gana sorteo");
        return 0;//retorna 0 si el usuario perdio
    }
}


void seleccionardispositivos(dispositivousuario misdispositivos[], FILE *log) {
    char catalogo[5][50] = {"Gaius", "Quadratus", "Hydrus", "Phalanx", "Argus"};
    int usados[5] = {0};
    int eleccion, contador = 0;
    int leidos; 
    char mensaje_log[100];

    printf("\n    ELIGE  3 DISPOSITIVOS   \n");

    while (contador < 3) {
        
        for (int i = 0; i < 5; i++) {
            printf("%d. %s\n", i + 1, catalogo[i]);
        }

        printf("Dispositivo %d: ", contador + 1);
        
        
        leidos = scanf("%d", &eleccion);

        if (leidos != 1) {
            printf("Error: Debes ingresar un numero.\n");
            while (getchar() != '\n'); 
            continue;
        }

        
        if (eleccion >= 1 && eleccion <= 5 && usados[eleccion - 1] == 0) {
            
            strcpy(misdispositivos[contador].nombre, catalogo[eleccion - 1]);
            misdispositivos[contador].disponible = 1;
            misdispositivos[contador].nivel = contador + 1;
            
           
            usados[eleccion - 1] = 1;

            
            strcpy(mensaje_log, "seleccionó ");             
            strcat(mensaje_log, catalogo[eleccion - 1]);    
            registraraccion(log, "Usuario", mensaje_log);
            
            contador++;
        } else {
            printf("Error: Numero invalido o dispositivo ya elegido.\n");
        }

        
        while (getchar() != '\n');
    }
}



void mostrardispositivos(dispositivousuario misdispositivos[], int cantidad) {
    printf("\n==============================\n");
    printf("  DISPOSITIVOS SELECCIONADOS  \n");
    printf("==============================\n");

    for (int i = 0; i < cantidad; i++) {
        printf("%d) %s | Disponible: ",
               i + 1,
               misdispositivos[i].nombre);

        if (misdispositivos[i].disponible == 1) {
            printf("SI\n");
        } else {
            printf("NO\n");
        }
    }
}

