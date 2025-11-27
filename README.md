# 🎮 Integración de SDL2_ttf (v2.24.0)

Este documento detalla la configuración necesaria para integrar la librería **SDL2_ttf** en el proyecto, esencial para el manejo de fuentes TrueType.

## ⚠️ Requisitos Previos

Asegúrate de tener una instalación funcional de **SDL2** y el entorno de desarrollo **MinGW-w64**.

| Recurso | Enlace de Descarga | Archivo Correcto |
| :--- | :--- | :--- |
| **SDL2_ttf** | https://github.com/libsdl-org/SDL_ttf/releases | `SDL2_ttf-devel-2.24.0-mingw.zip` |

---

## ⚙️ Instrucciones de Instalación Local

Estos pasos preparan tu entorno para que el IDE (Code::Blocks) y el programa puedan encontrar los archivos de la librería.

### 1. Colocación de Archivos

1.  **Descarga y Descomprime** el archivo `SDL2_ttf-devel-2.24.0-mingw.zip`.
2.  **Mover Carpeta:** Copia la carpeta resultante (ej., `SDL2_ttf-2.24.0`) a la **carpeta raíz de tu proyecto** (donde se encuentran tus archivos `.c` y `.h`).
3.  **Copiar la DLL de Ejecución:** Copia el archivo **`SDL2_ttf.dll`** de la subcarpeta `x86_64-w64-mingw32/bin` a la carpeta de tu ejecutable: `[Carpeta del Proyecto]/bin/Debug`.

### 2. Configuración en Code::Blocks

Ve a **Project → Build options...** en Code::Blocks.

#### A. Pestaña "Linker settings" (Bibliotecas de Enlace)

En el cuadro **"Link libraries"**, agrega las siguientes librerías **en este orden**:

1.  `SDL2main`
2.  `SDL2_ttf`

#### B. Pestaña "Search directories"

| Pestaña | Ruta a Agregar | Propósito |
| :--- | :--- | :--- |
| **Compiler** | `SDL2_ttf-2.24.0\x86_64-w64-mingw32\include` | Archivos de cabecera (`.h`) |
| **Linker** | `SDL2_ttf-2.24.0\x86_64-w64-mingw32\lib` | Archivos binarios (`.a`) |

---

## ⚠️ Modificaciones Críticas en el Código Fuente

Es **obligatorio** modificar el archivo de cabecera de la librería para evitar errores de inclusión (`file not found`).

Abre el archivo **`SDL2_ttf-2.24.0\x86_64-w64-mingw32\include\SDL2\SDL_ttf.h`** y realiza los siguientes reemplazos:

| Línea | Reemplazar | Por |
| :--- | :--- | :--- |
| **39** | `#include "SDL.h"` | `#include <SDL2/SDL.h>` |
| **40** | `#include "begin_code.h"` | `#include <SDL2/begin_code.h>` |
| **2339** | `#include "close_code.h"` | `#include <SDL2/close_code.h>` |
