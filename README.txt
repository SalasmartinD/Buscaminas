REQUISITOS

La única librería adicional que utilizamos es SDL2_ttf, versión 2.24.0.

Link para descargar SDL2_ttf:
https://github.com/libsdl-org/SDL_ttf/releases

El archivo correcto es:
SDL2_ttf-devel-2.24.0-mingw.zip


INSTRUCCIONES DE INSTALACIÓN

Descomprimir el archivo descargado.

Copiar la carpeta resultante (SDL2_ttf-2.24.0) a la carpeta del proyecto, donde están los archivos .c y .h.

Copiar el archivo SDL2_ttf.dll de la capeta "lib" a la carpeta del ejecutable:
en la carpeta del proyecto bin/Debug


CONFIGURACIÓN EN CODE::BLOCKS

- Ir a Project → Build options → Linker settings
  En "Link libraries", agregar:

    SDL2main

    SDL2_ttf

- Ir a "Search directories":

  En la pestaña Compiler, agregar:
    SDL2_ttf-2.24.0\x86_64-w64-mingw32\include

  En la pestaña Linker, agregar:
    SDL2_ttf-2.24.0\x86_64-w64-mingw32\lib


MODIFICACIONES EN EL ARCHIVO SDL_ttf.h

Es necesario modificar algunas líneas en el archivo SDL_ttf.h para evitar errores de inclusión:

Línea 39:
Reemplazar
#include "SDL.h"
por
#include <SDL2/SDL.h>

Línea 40:
Reemplazar
#include "begin_code.h"
por
#include <SDL2/begin_code.h>

Línea 2339:
Reemplazar
#include "close_code.h"
por
#include <SDL2/close_code.h>