#ifndef INTERFAZ_H
#define INTERFAZ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// estructura dispositivos
typedef struct {
    char nombre[50];
    int disponible;
    int nivel;
} dispositivousuario;

void registraraccion(FILE *log, char *actor, char *accion);
char pedirmovimiento(FILE *log);
int mostrarmenuturno(FILE *log, int yausodispositivo);
int realizarsorteocarasello(FILE *log);
void seleccionardispositivos(dispositivousuario misdispositivos[], FILE *log);


#endif