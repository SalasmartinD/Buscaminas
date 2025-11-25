#ifndef CELDAS_H_INCLUDED
#define CELDAS_H_INCLUDED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define PIXEL_SIZE 1
#define PIXEL_POR_LADO 16
#define MIN_TAM 8
#define MAX_TAM 32
#define TAM_LINEA 64
#define CODIGO_HACK "SOHCAHTOA"
#define MAX_CODIGO_LEN 10

#define N 0 // NEGRO
#define R 1 // ROJO
#define B 2 // AZUL
#define G 3  // GRIS OSCURO
#define C 4  // GRIS CLARO
#define W 5 // BLANCO
#define T 6// T Azul Oscuro
#define V 7// V Verde
#define M 8// M Bordo
#define J 9// J Celeste
#define H 10// H Violeta
#define O 11// O GrisOscuro

typedef struct
{
    bool tiene_mina;
    bool abierta;
    bool bandera;
    bool pregunta;
    int minas_alrededor;

}tCelda;

typedef struct {
    int filas;
    int columnas;
    int cantidadMinas;
    Uint32 tiempo_transcurrido;
    int resultado;
} tEstadoJuego;

typedef struct {
    tCelda** tablero;
    int filas;
    int columnas;
} tEstadoTablero;

int contarBanderas(tCelda** tablero, int filas, int columnas); //Cuenta las minas totales en el tablero
void** matrizCrear(size_t filas, size_t columnas, size_t tamElem); //Reserva la memoria para el tablero
void dibujarTablero(SDL_Renderer *renderer, tCelda** tablero, int filas, int columnas, int pixel_size, int altura_encabezado); //Renderiza el ultimo estado del tablero en la pantalla
void inicializarTablero(tCelda** tablero, int filas, int columnas); // Inicializa el tablero con todas las celdas cerradas y sin minas
void destruirMatriz(void** m, size_t filas); // Libera la memoria reservada para el tablero cuando termina la partida, ya sea por victoria, derrota o no haya llegado a un resultado
void colocarMinas(tCelda** tablero, int filas, int columnas, int cantidad, int fila_segura, int col_segura); //Coloca las minas aleatoriamente en el tablero excepto en la celda de primer clic
void contarMinasAlrededor(tCelda** tablero,int filas,int columnas);//Cuenta las minas alrededor de cada celda y le asigna ese numero a la celda para
void revelarCelda(int fila, int columna, tCelda** tablero, int* corriendo, int filas, int columnas, bool* exploto, int* primerClic, int cantidadMinas);//Abre la celda seleccionada
void marcarCelda(int x, int y, tCelda** tablero, int minasRestantes);// Marca la celda ya sea con bandera o signo de pregunta
void manejarEventos(SDL_Event evento, tCelda** tablero, int* corriendo, int filas, int columnas, int cantMinas
    ,int minasRestantes, bool* exploto, int altura_encabezado, int pixel_size,int* primerClic);//Discrimina los tipos de evento en el juego, ya sea clic izquierdo o derecho
void expandirCeldas(int x, int y, tCelda** tablero, int filas, int columnas);// Usando recursividad revela las celdas aledañas de la celda que fue abierta si es que no tiene bombas alrededor
void dibujarEncabezado(SDL_Renderer* renderer, int minas_restantes, int segundos, int filas, int columnas, int pixel_size);// Renderiza el encabezado con la cantidad de bombas restantes y temporizador
void mostrarBombas(tCelda** tablero, int filas, int columnas);// Cuando perdes por pisar una bomba, revela el resto para ver donde estaban
void revelarCeldasVecinas(int fila, int columna, tCelda** tablero, int filas, int columnas, int* corriendo, bool* exploto, int* primerClic, int cantidadMinas);// Revela las celdas que estan alrededor de una celda ya abierta si es que no tienen bandera
int contarMinasEncontradas(tCelda** tablero, int filas, int columnas);// Cuenta el total de minas que encontro el jugador
int contarBanderasCelda(tCelda** tablero, int y, int x, int filas, int columnas);// cuenta las banderas alrededor de una celda
void escribirLog(const char* evento, int fila, int columna);// Guarda un registro de todas las acciones durante el juego
tCelda** cargarPartida(const char* archivo, int* filas, int* columnas, int* cantidadMinas, Uint32* tiempo_inicio);// Carga la partida anterior
void guardarPartidaAlSalir(const char* archivo, tCelda** tablero, int filas, int columnas, int cantidadMinas, Uint32 tiempo_inicio, int resultado);//Registra las ultimas 10 partidas jugadas
tCelda** crearPartidaNueva(int* filas, int* columnas, int* cantidadMinas, Uint32* tiempo_inicio);// inicia una partida totalmente nueva
int contarCeldasCerradas(tCelda** tablero, int filas, int columnas);// Cuenta las Celdas cerradas en tablero
int resultadoPartidaAnterior(const char* archivo);// Evalua al resultado de la partida anterior
bool obtenerConfiguracion(int* filas, int* columnas, int* cantidadMinas);// Lee la configuracion del archivo .conf
int jugarPartida(const char* nombre_usuario, int continuar);// Funcion principal que maneja el juego
void marcarMinaAleatoria(tCelda** tablero, int filas, int columnas);// Hack usando el codigo "sohcahtoa"
int calcularPixelSize(int filas, int columnas);// Calcula el tamaño del pixel para que se adapte bien a la pantalla
int contarCeldasAbiertas(tCelda** tablero, int filas, int columnas);// cuenta las celdas abiertas en tablero
void dibujarFooter(SDL_Renderer *renderer,int filas,int columnas,int pixel_size);
void rehacer(tCelda** tablero_actual, int filas, int columnas, tEstadoTablero* historial, int cantidad_estados, int* indice_actual);
tCelda** copiar_tablero(tCelda** original, int filas, int columnas);
void deshacer(tCelda** tablero_actual, int filas, int columnas, tEstadoTablero* historial, int* indice_actual);
void guardarEstado(tCelda** tablero_actual, int filas, int columnas, tEstadoTablero** historial, int* cantidad_estados, int* indice_actual);

#endif
