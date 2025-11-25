#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>

#define ANCHO 420
#define ALTO 500
#define MAX_NOMBRE 32

int estaEnRect(SDL_Point p, SDL_Rect r);
int mostrarMenuInicio(char* nombre_usuario, size_t max_largo);
void mostrarVentanaConfiguracion();
void guardarConfiguracion(const char* tam, const char* bombas);

#endif