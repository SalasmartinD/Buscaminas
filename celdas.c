#include "celdas.h"
#include "estadisticas.h"
#include "spritesJuego.h"
#include "spriteNumeros.h"
#include "spriteNumerosJuego.h"
#include "menu.h"

SDL_Color colores[] = {
    {  0,   0,   0, 255 }, // N Negro
    {255,   0,   0, 255 }, // R Rojo
    {  0,   0, 255, 255 }, // B Azul
    { 96,  96,  96, 255 }, // G Gris
    {192, 192, 192, 255 }, // C Gris Claro
    {255, 255, 255, 255 }, // W Blanco
    {  0,   0, 133, 255 }, // T Azul Oscuro
    {10 , 188,  26, 255 }, // V Verde
    {162,   0,   0, 255 }, // M Bordo
    {  0, 211, 208, 255 }, // J Celeste
    {186,   0, 208, 255 }, // H Violeta
    {180, 180, 180, 255 }  // O GrisOscuro
};

const int (*numeroCont[NUM_SPRITES_CONT])[PIXEL_POR_LADO] = {numero0, numero1, numero2, numero3, numero4, numero5, numero6, numero7, numero8, numero9};
const int (*numeroJuego[NUM_SPRITES_JUEGO])[PIXEL_POR_LADO] = {numeroJ1, numeroJ2, numeroJ3, numeroJ4, numeroJ5, numeroJ6, numeroJ7, numeroJ8};

void** matrizCrear(size_t filas, size_t columnas, size_t tamElem)
{
    void** m = (void**)malloc(sizeof(void*)*filas);
    if(!m)
        return NULL;

    void** ult = m + filas - 1;

    for(void** i=m; i<=ult ; i++)
    {
        *i = (void*)malloc(tamElem*columnas);
        if(!*i)
        {
            destruirMatriz(m, i-m);
            return NULL;
        }
    }

    return m;
}

void dibujarTablero(SDL_Renderer *renderer, tCelda** tablero, int filas, int columnas, int pixel_size, int altura_encabezado)
{
    for (int y = 0; y < filas; y++)
    {
        for (int x = 0; x < columnas; x++)
        {
            const int (*sprite)[PIXEL_POR_LADO];

            if (tablero[y][x].abierta)
            {
                if (tablero[y][x].tiene_mina)
                    sprite = mina_explotada;
                else if (tablero[y][x].minas_alrededor == 0)
                    sprite = celda_abierta;
                else
                    sprite = numeroJuego[(tablero[y][x].minas_alrededor > 8 ? 8 : tablero[y][x].minas_alrededor)-1];
            }
            else if (tablero[y][x].bandera)
            {
                sprite = bandera;
            }
            else if (tablero[y][x].pregunta)
            {
                sprite = signo_pregunta;
            }
            else
            {
                sprite = celda_cerrada;
            }

            for (int py = 0; py < PIXEL_POR_LADO; py++)
            {
                for (int px = 0; px < PIXEL_POR_LADO; px++)
                {
                    SDL_Color color = colores[sprite[py][px]];
                    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                    SDL_Rect rect = {
                        x * PIXEL_POR_LADO * pixel_size + px * pixel_size,
                        y * PIXEL_POR_LADO * pixel_size + py * pixel_size + altura_encabezado,
                        pixel_size,
                        pixel_size
                    };
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }
    }
}

void destruirMatriz(void** m, size_t filas)
{
    void** ult = m + filas - 1;

    for(void** i = m; i<ult; i++)
    {
        free(*i);
    }
    free(m);
}

void inicializarTablero(tCelda** tablero, int filas, int columnas)
{
    for(size_t i = 0; i < filas; i++)
    {
        for(size_t j = 0; j < columnas; j++)
        {
            tablero[i][j].abierta = false;
            tablero[i][j].bandera = false;
            tablero[i][j].pregunta = false;
            tablero[i][j].tiene_mina = false;
            tablero[i][j].minas_alrededor = 0;
        }
    }
}

void colocarMinas(tCelda** tablero, int filas, int columnas, int cantidad, int fila_segura, int col_segura)
{
    int colocadas = 0;
    while (colocadas < cantidad)
    {
        int f = rand() % filas;
        int c = rand() % columnas;

        // Evitar colocar en la celda segura
        if ((f == fila_segura && c == col_segura) || tablero[f][c].tiene_mina)
            continue;

        tablero[f][c].tiene_mina = true;
        colocadas++;
    }
}

void contarMinasAlrededor(tCelda** tablero,int filas,int columnas)
{
    for (int y = 0; y < filas; y++)
    {
        for (int x = 0; x < columnas; x++)
        {
            if (tablero[y][x].tiene_mina)
                continue;

            int contador = 0;
            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    int ny = y + dy;
                    int nx = x + dx;
                    if (nx >= 0 && nx < columnas && ny >= 0 && ny < filas)
                    {
                        if (tablero[ny][nx].tiene_mina)
                            contador++;
                    }
                }
            }
            tablero[y][x].minas_alrededor = contador;
        }
    }
}

void revelarCelda(int fila, int columna, tCelda** tablero, int* corriendo, int filas, int columnas, bool* exploto, int* primerClic, int cantidadMinas)
{
    if (tablero[fila][columna].abierta)
        return;

    if (*primerClic)
    {
        *primerClic = 0;
        // Asegurar que la primera celda no tenga mina
        colocarMinas(tablero, filas, columnas, cantidadMinas, fila, columna);
        contarMinasAlrededor(tablero, filas, columnas);
    }

    if (tablero[fila][columna].tiene_mina)
    {
        tablero[fila][columna].abierta = true;
        printf("Tocaste una mina en (%d, %d)\nTermino el juego\n", fila, columna);
        mostrarBombas(tablero, filas, columnas);
        *corriendo = 0;
        *exploto = true;
        return;
    }
    if (tablero[fila][columna].minas_alrededor == 0)
    {
        expandirCeldas(fila, columna, tablero, filas, columnas);
    }
    else
    {
        tablero[fila][columna].abierta = true;
        printf("Celda (%d, %d) revelada, minas alrededor: %d\n", fila, columna, tablero[fila][columna].minas_alrededor);
    }
}

void marcarCelda(int fila, int columna, tCelda** tablero, int minasRestantes)
{
    if (!tablero[fila][columna].abierta)
    {
        if(minasRestantes > 0)
        {
            if (tablero[fila][columna].pregunta)
            {
                tablero[fila][columna].pregunta = false;
                tablero[fila][columna].bandera = false;
            }
            else if (tablero[fila][columna].bandera)
            {
                tablero[fila][columna].bandera = false;
                tablero[fila][columna].pregunta = true;
            }
            else
            {
                tablero[fila][columna].bandera = true;
            }
        }
        else if(tablero[fila][columna].bandera)
        {
            tablero[fila][columna].bandera = false;
            tablero[fila][columna].pregunta = true;
        }
    }
}

void manejarEventos(SDL_Event evento, tCelda** tablero, int* corriendo, int filas, int columnas, int cantMinas, int minasRestantes, bool* exploto, int altura_encabezado, int pixel_size, int* primerClic)
{
    if (evento.button.y < altura_encabezado)
        return;

    if (evento.type == SDL_MOUSEBUTTONDOWN)
    {
        int x = evento.button.x / (PIXEL_POR_LADO * pixel_size);
        int y = (evento.button.y - altura_encabezado) / (PIXEL_POR_LADO * pixel_size);

        if (x < 0 || x >= columnas || y < 0 || y >= filas)
            return;

        if (evento.button.button == SDL_BUTTON_LEFT)
        {
            printf("Hiciste clic izquierdo en la celda (%d, %d)\n", y, x);
            escribirLog("Clic izquierdo", y, x);

            if (!(*exploto) && (tablero[y][x].abierta && (tablero[y][x].minas_alrededor == contarBanderasCelda(tablero, y, x, filas, columnas))))
            {
                // Si ya está abierta, revelamos sus celdas vecinas
                revelarCeldasVecinas(y, x, tablero, filas, columnas, corriendo, exploto, primerClic, cantMinas);
            } else {
                // Si no, revelamos la celda como normal
                revelarCelda(y, x, tablero, corriendo, filas, columnas, exploto, primerClic, cantMinas);
            }
            if(contarMinasEncontradas(tablero, filas, columnas) == cantMinas || contarCeldasCerradas(tablero, filas, columnas) == cantMinas)
            {
                printf("\nEncontraste todas las minas!\nGanaste!\n");
                *corriendo = 0;
            }
        }
        else if (evento.button.button == SDL_BUTTON_RIGHT)
        {
            printf("Hiciste clic derecho en la celda (%d, %d)\n", y, x); // Marcamos y/o desmarcamos la celda con Bandera o Signo de Pregunta
            escribirLog("Clic derecho", y, x);
            marcarCelda(y, x, tablero, minasRestantes);
            if(contarMinasEncontradas(tablero, filas, columnas) == cantMinas || contarCeldasCerradas(tablero, filas, columnas) == cantMinas)
            {
                printf("\nEncontraste todas las minas!\nGanaste!\n");
                *corriendo = 0;
            }
        }
    }
}

void expandirCeldas(int fila, int columna, tCelda** tablero, int filas, int columnas)
{
    if (fila < 0 || fila >= filas || columna < 0 || columna >= columnas)
        return;

    tCelda* celda = &tablero[fila][columna];

    if (celda->abierta || celda->tiene_mina || celda->bandera)
        return;

    celda->abierta = true;

    if (celda->minas_alrededor > 0)
        return;

    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            if (dy != 0 || dx != 0)
                expandirCeldas(fila + dy, columna + dx, tablero, filas, columnas);
        }
    }
}

int contarBanderas(tCelda** tablero, int filas, int columnas)
{
    int total = 0;
    for (int y = 0; y < filas; y++)
    {
        for (int x = 0; x < columnas; x++)
        {
            if (tablero[y][x].bandera)
                total++;
        }
    }
    return total;
}

void dibujarEncabezado(SDL_Renderer* renderer, int minas_restantes, int segundos, int filas, int columnas, int pixel_size)
{

    if (minas_restantes < 0 || minas_restantes > 999)
        minas_restantes = 0;

    if (segundos < 0 || segundos > 999)
        segundos = 0;

    int digitos_minas[3] = {
        (minas_restantes / 100) % 10,
        (minas_restantes / 10) % 10,
        minas_restantes % 10
    };

    int digitos_tiempo[3] = {
        (segundos / 100) % 10,
        (segundos / 10) % 10,
        segundos % 10
    };

    for (int i = 0; i < 3; i++) {
        const int (*sprite)[PIXEL_POR_LADO] = numeroCont[digitos_minas[i]];
        for (int py = 0; py < PIXEL_POR_LADO; py++)
        {
            for (int px = 0; px < PIXEL_POR_LADO; px++)
            {
                SDL_Color color = colores[sprite[py][px]];
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_Rect rect = {
                    i * PIXEL_POR_LADO * pixel_size + px * pixel_size,
                    py * pixel_size,
                    pixel_size, pixel_size
                };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    int offset_x = columnas * PIXEL_POR_LADO * pixel_size - 3 * PIXEL_POR_LADO * pixel_size;
    for (int i = 0; i < 3; i++) {
        const int (*sprite)[PIXEL_POR_LADO] = numeroCont[digitos_tiempo[i]];
        for (int py = 0; py < PIXEL_POR_LADO; py++) {
            for (int px = 0; px < PIXEL_POR_LADO; px++) {
                SDL_Color color = colores[sprite[py][px]];
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_Rect rect = {
                    offset_x + i * PIXEL_POR_LADO * pixel_size + px * pixel_size,
                    py * pixel_size,
                    pixel_size, pixel_size
                };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void mostrarBombas(tCelda** tablero, int filas, int columnas)
{
    for(int i = 0; i < filas ; i++)
    {
        for(int j = 0; j < columnas; j++)
        {
            if(tablero[i][j].tiene_mina)
                tablero[i][j].abierta = true;
        }
    }
}

void revelarCeldasVecinas(int fila, int columna, tCelda** tablero, int filas, int columnas, int* corriendo, bool* exploto, int* primerClic, int cantidadMinas)
{
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            int nx = columna + dx;
            int ny = fila + dy;

            if (nx >= 0 && ny >= 0 && nx < columnas && ny < filas)
            {
                if (!tablero[ny][nx].abierta && !tablero[ny][nx].bandera)
                {
                    revelarCelda(ny, nx, tablero, corriendo, filas, columnas, exploto, primerClic, cantidadMinas);
                }
            }
        }
    }
}

int contarMinasEncontradas(tCelda** tablero, int filas, int columnas)
{
    int cont = 0;
    for(int i = 0; i < filas; i++)
    {
        for(int j = 0; j < columnas; j++)
        {
            if(tablero[i][j].bandera && tablero[i][j].tiene_mina)
                cont++;
        }
    }

    return cont;
}

int contarBanderasCelda(tCelda** tablero, int y, int x, int filas, int columnas)
{
    int cont = 0;
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            int nx = x + dx;
            int ny = y + dy;

            if (dx == 0 && dy == 0)
                continue;

            if (nx >= 0 && ny >= 0 && nx < columnas && ny < filas)
            {
                if (tablero[ny][nx].bandera)
                    cont++;
            }
        }
    }

    return cont;
}

void escribirLog(const char* evento, int fila, int columna)
{
    FILE* log = fopen("buscaminas.log", "a");
    if (!log)
    {
        fprintf(stderr, "No se pudo abrir el archivo de log.\n");
        return;
    }

    time_t ahora = time(NULL);
    struct tm* t = localtime(&ahora);

    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

    if (fila >= 0 && columna >= 0)
    {
        fprintf(log, "[%s] %s en celda (%d, %d)\n", timestamp, evento, fila, columna);
    }
    else
    {
        fprintf(log, "[%s] %s\n", timestamp, evento);
    }

    fclose(log);
}

tCelda** cargarPartida(const char* archivo, int* filas, int* columnas, int* cantidadMinas, Uint32* tiempo_transcurrido)
{
    FILE* f = fopen(archivo, "rb");
    if (!f)
    {
        perror("Error al abrir archivo para cargar");
        return NULL;
    }

    tEstadoJuego estado;
    fread(&estado, sizeof(tEstadoJuego), 1, f);
    *filas = estado.filas;
    *columnas = estado.columnas;
    *cantidadMinas = estado.cantidadMinas;
    *tiempo_transcurrido = estado.tiempo_transcurrido;

    // Reservar memoria para el tablero
    tCelda** tablero = malloc(estado.filas * sizeof(tCelda*));

    for (int i = 0; i < estado.filas; i++)
    {
        tablero[i] = malloc(estado.columnas * sizeof(tCelda));
        fread(tablero[i], sizeof(tCelda), estado.columnas, f);
    }

    fclose(f);
    printf("Partida cargada con exito.\n");
    return tablero;
}

void guardarPartidaAlSalir(const char* archivo, tCelda** tablero, int filas, int columnas, int cantidadMinas, Uint32 tiempo_transcurrido, int resultado)
{
    FILE* f = fopen(archivo, "wb");
    if (!f)
    {
        perror("Error al guardar partida");
        return;
    }

    tEstadoJuego estado = {filas, columnas, cantidadMinas, tiempo_transcurrido, resultado};
    fwrite(&estado, sizeof(tEstadoJuego), 1, f);

    for (int i = 0; i < filas; i++)
    {
        fwrite(tablero[i], sizeof(tCelda), columnas, f);
    }

    fclose(f);
    printf("Partida guardada automaticamente.\n");
}

tCelda** crearPartidaNueva(int* filas, int* columnas, int* cantidadMinas, Uint32* tiempo_transcurrido)
{
    bool config;

    config = obtenerConfiguracion(filas,columnas,cantidadMinas);

    if(!config)
    {//config por defecto si falla leer el archivo o la config es erronea
        printf("\nAlgo fallo, usando configuracion por defecto\n");
        *filas = 16;
        *columnas = 16;
        *cantidadMinas = 10;
        *tiempo_transcurrido = SDL_GetTicks();
    }

    tCelda** tablero = (tCelda**)matrizCrear(*filas, *columnas, sizeof(tCelda));
    inicializarTablero(tablero, *filas, *columnas);
    return tablero;
}

int contarCeldasCerradas(tCelda** tablero, int filas, int columnas)
{
    int cont = 0;
    for(int i = 0; i < filas; i++)
    {
        for(int j = 0; j < columnas; j++)
        {
            if(!tablero[i][j].abierta)
                cont++;
        }
    }

    return cont;
}

int resultadoPartidaAnterior(const char* archivo)
{
    int resultado;
    FILE* f = fopen(archivo, "rb");
    if (!f)
    {
        printf("Error al abrir archivo para cargar");
    }

    tEstadoJuego estado;
    fread(&estado, sizeof(tEstadoJuego), 1, f);
    resultado = estado.resultado;

    fclose(f);
    return resultado;
}

bool obtenerConfiguracion(int* filas, int* columnas, int* cantidadMinas)
{
    printf("\nObteniendo configuracion\n");
    FILE* archivo = fopen("buscaminas.conf", "r");

    if (!archivo)
    {
        printf("\nArchivo de configuracion no encontrado. Creando archivo con configuracion por defecto.\n");

        archivo = fopen("buscaminas.conf", "w");
        if (!archivo)
        {
            printf("Error al crear el archivo de configuracion.\n");
            return false;
        }

        // Valores por defecto
        int tamDefecto = 10;
        int minasDefecto = 15;

        fprintf(archivo, "Tamanio=%d\n", tamDefecto);
        fprintf(archivo, "Cantidad_minas=%d\n", minasDefecto);
        fclose(archivo);

        // Reabrimos para lectura
        archivo = fopen("buscaminas.conf", "r");
        if (!archivo)
        {
            printf("Error al reabrir el archivo de configuracion.\n");
            return false;
        }
    }

    char linea[TAM_LINEA];
    int tamanio = 0;
    int minas = 0;
    bool esPorcentaje = false;
    bool leyoTamanio = false;
    bool leyoMinas = false;

    while (fgets(linea, sizeof(linea), archivo))
    {
        char* salto = strchr(linea, '\n');
        if (salto)
            *salto = '\0';

        if (strncmp(linea, "Tamanio=", 8) == 0)
        {
            tamanio = atoi(linea + 8);
            if (tamanio < MIN_TAM || tamanio > MAX_TAM)
            {
                printf("\nTamanio fuera de los limites (%d a %d), usando configuracion por defecto\n", MIN_TAM, MAX_TAM);
                fclose(archivo);
                return false;
            }
            leyoTamanio = true;
        }
        else if (strncmp(linea, "Cantidad_minas=", 15) == 0)
        {
            char* valor = linea + 15;
            int len = strlen(valor);
            if (len > 0 && valor[len - 1] == '%')
            {
                valor[len - 1] = '\0';
                minas = atoi(valor);
                esPorcentaje = true;
                if (minas < 0 || minas > 100)
                {
                    fclose(archivo);
                    return false;
                }
            }
            else
            {
                minas = atoi(valor);
                if (minas < 1)
                {
                    printf("\nCantidad de minas incorrecta (menor a 1), usando configuracion por defecto\n");
                    fclose(archivo);
                    return false;
                }
                esPorcentaje = false;
            }
            leyoMinas = true;
        }
    }

    fclose(archivo);

    if (!leyoTamanio || !leyoMinas)
        return false;

    int totalCeldas = tamanio * tamanio;
    int minasFinal;

    if (esPorcentaje)
    {
        minasFinal = (minas * totalCeldas) / 100;
        if (minasFinal >= totalCeldas)
            minasFinal = totalCeldas - 1;
    }
    else
    {
        minasFinal = minas;
        if (minasFinal >= totalCeldas)
            minasFinal = totalCeldas - 1;
    }

    *filas = *columnas = tamanio;
    *cantidadMinas = minasFinal;

    printf("\nArchivo leido correctamente\n");
    return true;
}

int jugarPartida(const char* nombre_usuario, int continuar)
{
    Uint32 tiempo_base = SDL_GetTicks();
    Uint32 tiempo_transcurrido = 0, tiempo_actual;
    tCelda** tablero = NULL;
    int cantidadMinas, segundos, minas_restantes, filas, columnas, celdasSinMina, celdasAbiertas;
    int resultado, contadorClicks,bufferLen = 0;
    int primerClic = 1;
    float puntaje;
    bool exploto = false;
    char bufferCodigo[MAX_CODIGO_LEN] = {0};
    tEstadoTablero* historial = NULL;
    int cantidad_estados = 0;
    int indice_actual = -1;

    if (continuar == 0)
    {
        tablero = crearPartidaNueva(&filas, &columnas, &cantidadMinas, &tiempo_base);
    }
    else if (resultadoPartidaAnterior("partida_guardada.bin") == 2)
    {
        tablero = cargarPartida("partida_guardada.bin", &filas, &columnas, &cantidadMinas, &tiempo_transcurrido);
        if (!tablero) tablero = crearPartidaNueva(&filas, &columnas, &cantidadMinas, &tiempo_base);
    }
    else
    {
        printf("\nNo se puede continuar partida anterior (ganada/perdida). Generando nueva partida.\n");
        tablero = crearPartidaNueva(&filas, &columnas, &cantidadMinas, &tiempo_base);
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Error al inicializar SDL: %s\n", SDL_GetError());
        return -1;
    }

    int pixel_size = calcularPixelSize(filas, columnas);

    int screen_width = columnas * PIXEL_POR_LADO * pixel_size;
    int screen_height = (filas + 2) * PIXEL_POR_LADO * pixel_size;
    int altura_encabezado = PIXEL_POR_LADO * pixel_size;

    SDL_Window* ventana = SDL_CreateWindow("Buscaminas",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        screen_width,
                                        screen_height,
                                        SDL_WINDOW_SHOWN);
    if (!ventana)
    {
        fprintf(stderr, "Error al crear ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        fprintf(stderr, "Error al crear renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(ventana);
        SDL_Quit();
        return -1;
    }

    SDL_Surface *icon = SDL_LoadBMP("img/logo.bmp");  // Nombre correcto
    if (icon == NULL) {
    SDL_Log("Error al cargar el ícono: %s", SDL_GetError());
    } else {
    SDL_SetWindowIcon(ventana, icon);
    SDL_FreeSurface(icon);
    }
    int corriendo = 1;
    int juegoTerminado = 0;
    SDL_Event evento;

    escribirLog("Inicio del juego", -1, -1);

    while (corriendo)
    {
        minas_restantes = cantidadMinas - contarBanderas(tablero, filas, columnas);

        while (SDL_PollEvent(&evento))
        {
            if (evento.type == SDL_QUIT)
            {
                corriendo = 0;
            }
            if (evento.type == SDL_TEXTINPUT)
            {
                // Añadimos la letra al buffer (solo mayúsculas para simplificar)
                if (bufferLen < MAX_CODIGO_LEN - 1)
                {
                    char c = evento.text.text[0];
                    if (c >= 'a' && c <= 'z')
                        c -= 32;  // convertir a mayúscula

                    bufferCodigo[bufferLen++] = c;
                    bufferCodigo[bufferLen] = '\0';

                    // Comprobar si coincide con el código hack
                    if (strcmp(bufferCodigo, CODIGO_HACK) == 0)
                    {
                        marcarMinaAleatoria(tablero, filas, columnas);
                        bufferLen = 0; // resetear buffer para que se pueda ingresar de nuevo
                    }
                    else
                    {
                        // Si el buffer no es prefijo válido, reseteamos para evitar "atascos"
                        if (strncmp(CODIGO_HACK, bufferCodigo, bufferLen) != 0)
                        {
                            bufferLen = 0;
                            bufferCodigo[0] = '\0';
                        }
                    }
                }
            }

            contadorClicks++;
            manejarEventos(evento, tablero, &corriendo, filas, columnas, cantidadMinas, minas_restantes, &exploto, altura_encabezado, pixel_size, &primerClic);

            if (evento.type == SDL_MOUSEBUTTONDOWN && corriendo && !juegoTerminado)
            {
                int mouse_x = evento.button.x;
                int mouse_y = evento.button.y;

                SDL_Rect botonAtras =
                {
                    0,
                    (filas + 1) * pixel_size * PIXEL_POR_LADO,
                    PIXEL_POR_LADO * pixel_size,
                    pixel_size * PIXEL_POR_LADO
                };

                SDL_Rect botonAdelante =
                {
                    (columnas - 1) * pixel_size * PIXEL_POR_LADO - 30,
                    (filas + 1) * pixel_size * PIXEL_POR_LADO,
                    PIXEL_POR_LADO * pixel_size + 30,
                    pixel_size * PIXEL_POR_LADO
                };

                if (SDL_PointInRect(&(SDL_Point){mouse_x, mouse_y}, &botonAtras))
                {
                    deshacer(tablero, filas, columnas, historial, &indice_actual);
                }
                else if (SDL_PointInRect(&(SDL_Point){mouse_x, mouse_y}, &botonAdelante))
                {
                    rehacer(tablero, filas, columnas, historial, cantidad_estados, &indice_actual);
                }
                else
                {
                    guardarEstado(tablero, filas, columnas, &historial, &cantidad_estados, &indice_actual);
                }
            }
        }

        SDL_RenderClear(renderer);
        tiempo_actual = SDL_GetTicks();
        segundos = (tiempo_actual - tiempo_base + tiempo_transcurrido) / 1000;

        dibujarFooter(renderer, filas, columnas, pixel_size);
        dibujarEncabezado(renderer, minas_restantes, segundos, filas, columnas, pixel_size);
        dibujarTablero(renderer, tablero, filas, columnas, pixel_size, altura_encabezado);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
        if (!corriendo && !juegoTerminado)
        {
            SDL_Delay(3000);
            juegoTerminado = 1;
        }
    }

    tiempo_transcurrido = tiempo_actual - tiempo_base + tiempo_transcurrido;
    escribirLog("Fin del juego", -1, -1);

    celdasSinMina = filas * columnas - cantidadMinas;
    celdasAbiertas = contarCeldasAbiertas(tablero, filas, columnas);

    if (exploto)
        resultado = 0;  // Perdió
    else if (celdasAbiertas == celdasSinMina || contarBanderas(tablero, filas, columnas) == cantidadMinas)
        resultado = 1;  // Ganó
    else
        resultado = 2;  // Juego sin terminar

    guardarPartidaAlSalir("partida_guardada.bin", tablero, filas, columnas, cantidadMinas, tiempo_transcurrido, resultado);

    puntaje = (float)(filas * columnas * cantidadMinas) / (float)(tiempo_transcurrido/1000);
    Estadistica est = {filas, columnas, cantidadMinas, segundos, resultado, puntaje};
    strncpy(est.nombre, nombre_usuario, MAX_NOMBRE_USUARIO - 1);
    est.nombre[MAX_NOMBRE_USUARIO - 1] = '\0';

    guardarEstadistica(est);

    for (int i = 0; i < cantidad_estados; i++)
    {
        for (int f = 0; f < historial[i].filas; f++)
            free(historial[i].tablero[f]);
        free(historial[i].tablero);
    }
    free(historial);

    destruirMatriz((void**)tablero, filas);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();

    return resultado;
}

void marcarMinaAleatoria(tCelda** tablero, int filas, int columnas)
{
    srand((unsigned int)time(NULL));
    int intentos = 0;
    while (intentos < 1000)
    {  // Para evitar loops infinitos
        int y = rand() % filas;
        int x = rand() % columnas;
        if (tablero[y][x].tiene_mina && !tablero[y][x].bandera)
        {
            tablero[y][x].bandera = true;
            tablero[y][x].pregunta = false;
            printf("Hack activado: mina marcada en (%d, %d)\n", y, x);
            escribirLog("Hack: mina marcada", y, x);
            return;
        }
        intentos++;
    }
    printf("Hack: no se encontro mina sin marcar para poner bandera.\n");
}

int calcularPixelSize(int filas, int columnas)
{
    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(0, &dm) != 0)
    {
        // Error al obtener resolución, valor fijo razonable
        return 16;
    }

    int max_tam = dm.h * 90 / 100; // 90% altura pantalla para tablero + encabezado

    // Calculamos pixel_size máximo para que quepa verticalmente (filas + 1 fila para encabezado)
    int pixel_size = max_tam / ((filas + 1) * PIXEL_POR_LADO);

    return pixel_size;
}

int contarCeldasAbiertas(tCelda** tablero, int filas, int columnas)
{
    int abiertas = 0;
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            if (tablero[i][j].abierta)
                abiertas++;
        }
    }
    return abiertas;
}

void dibujarFooter(SDL_Renderer *renderer,int filas,int columnas,int pixel_size)
{
    TTF_Init();
    TTF_Font* fuente = TTF_OpenFont("fnt\\arial.ttf", 24);

    if (!fuente)
    {
        printf("Error inicializando TTF: %s\n", TTF_GetError());
        return;
    }

    SDL_Rect botonAtras = {0,(filas+1)*pixel_size*PIXEL_POR_LADO ,
        PIXEL_POR_LADO*pixel_size ,pixel_size*PIXEL_POR_LADO};
        
    SDL_Rect botonAdelante = {(columnas-1)*pixel_size*PIXEL_POR_LADO-30,(filas+1)*pixel_size*PIXEL_POR_LADO,
        PIXEL_POR_LADO*pixel_size+30 ,pixel_size*PIXEL_POR_LADO};

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderClear(renderer);


    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
    SDL_RenderFillRect(renderer, &botonAtras);
    SDL_RenderFillRect(renderer, &botonAdelante);

    SDL_Color negro = { 0, 0, 0, 255 };

    SDL_Surface* textoAtras = TTF_RenderText_Solid(fuente, "Atras", negro);
        SDL_Texture* texAtras = SDL_CreateTextureFromSurface(renderer, textoAtras);
        SDL_Rect rectTexto1 = {
            botonAtras.x + (botonAtras.w - textoAtras->w) / 2,
            botonAtras.y + (botonAtras.h - textoAtras->h) / 2,
            textoAtras->w,
            textoAtras->h
    };
    SDL_RenderCopy(renderer, texAtras, NULL, &rectTexto1);

    SDL_Surface* textoAdelante = TTF_RenderText_Solid(fuente, "Adelante", negro);
        SDL_Texture* texAdelante = SDL_CreateTextureFromSurface(renderer, textoAdelante);
        SDL_Rect rectTexto2 = {
            botonAdelante.x + (botonAdelante.w - textoAdelante->w) / 2,
            botonAdelante.y + (botonAdelante.h - textoAdelante->h) / 2,
            textoAdelante->w,
            textoAdelante->h
    };
    SDL_RenderCopy(renderer, texAdelante, NULL, &rectTexto2);

}

void guardarEstado(tCelda** tablero_actual, int filas, int columnas, tEstadoTablero** historial, int* cantidad_estados, int* indice_actual)
{
    if (*indice_actual < *cantidad_estados - 1)
    {
        for (int i = *indice_actual + 1; i < *cantidad_estados; i++)
        {
            for (int f = 0; f < (*historial)[i].filas; f++)
                free((*historial)[i].tablero[f]);
            free((*historial)[i].tablero);
        }
        *cantidad_estados = *indice_actual + 1;
    }

    *historial = realloc(*historial, (*cantidad_estados + 1) * sizeof(tEstadoTablero));
    (*historial)[*cantidad_estados].filas = filas;
    (*historial)[*cantidad_estados].columnas = columnas;
    (*historial)[*cantidad_estados].tablero = copiar_tablero(tablero_actual, filas, columnas);

    (*cantidad_estados)++;
    (*indice_actual)++;
}

void deshacer(tCelda** tablero_actual, int filas, int columnas, tEstadoTablero* historial, int* indice_actual)
{
    if (*indice_actual <= 0)
        return;

    (*indice_actual)--;
    for (int i = 0; i < filas; i++)
    {
        memcpy(tablero_actual[i], historial[*indice_actual].tablero[i], columnas * sizeof(tCelda));
    }
}

void rehacer(tCelda** tablero_actual, int filas, int columnas, tEstadoTablero* historial, int cantidad_estados, int* indice_actual)
{
    if (*indice_actual + 1 >= cantidad_estados)
        return;

    (*indice_actual)++;
    for (int i = 0; i < filas; i++)
    {
        memcpy(tablero_actual[i], historial[*indice_actual].tablero[i], columnas * sizeof(tCelda));
    }
}

tCelda** copiar_tablero(tCelda** original, int filas, int columnas)
{
    tCelda** copia = malloc(filas * sizeof(tCelda*));
    for (int i = 0; i < filas; i++)
    {
        copia[i] = malloc(columnas * sizeof(tCelda));
        memcpy(copia[i], original[i], columnas * sizeof(tCelda));
    }
    return copia;
}
