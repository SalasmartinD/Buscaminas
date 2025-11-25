#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H

#include <time.h>

#define MAX_ESTADISTICAS 10
#define ARCHIVO_ESTADISTICAS "estadisticas.dat"
#define MAX_NOMBRE_USUARIO 50

typedef struct
{
    int filas;
    int columnas;
    int minas;
    int tiempo;       // tiempo en segundos
    int resultado;    // 1 = ganó, 0 = perdió
    float puntaje;
    char nombre[MAX_NOMBRE_USUARIO];
} Estadistica;

void guardarEstadistica(Estadistica nueva); //Guarda las estadisticas de las ultimas 10 partidas
void mostrarEstadisticasSDL(); //Genera la ventana para ver las estadisticas de las ultimas 10 partidas

#endif