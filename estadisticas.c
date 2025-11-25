#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "estadisticas.h"

void guardarEstadistica(Estadistica nueva)
{
    Estadistica estadisticas[MAX_ESTADISTICAS];
    int count = 0;

    // Leer estadísticas previas si existen
    FILE* file = fopen(ARCHIVO_ESTADISTICAS, "rb");
    if (file)
    {
        count = fread(estadisticas, sizeof(Estadistica), MAX_ESTADISTICAS, file);
        fclose(file);
    }

    // Si hay 10 registros, desplazamos para borrar el más antiguo
    if (count == MAX_ESTADISTICAS)
    {
        for (int i = 1; i < MAX_ESTADISTICAS; i++)
        {
            estadisticas[i - 1] = estadisticas[i];
        }
        estadisticas[MAX_ESTADISTICAS - 1] = nueva;
    }
    else
    {
        estadisticas[count] = nueva;
        count++;
    }

    // Guardar estadísticas actualizadas
    file = fopen(ARCHIVO_ESTADISTICAS, "wb");
    if (!file)
    {
        printf("Error al abrir archivo para guardar estadísticas\n");
        return;
    }
    fwrite(estadisticas, sizeof(Estadistica), count, file);
    fclose(file);
}

void mostrarEstadisticasSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Error SDL_Init: %s\n", SDL_GetError());
        return;
    }
    if (TTF_Init() == -1)
    {
        printf("Error TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return;
    }

    SDL_Window* ventana = SDL_CreateWindow("Estadísticas - Últimas 10 partidas",
                                           SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           820, 500, 0);
    if (!ventana)
    {
        printf("Error creando ventana: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("Error creando renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(ventana);
        TTF_Quit();
        SDL_Quit();
        return;
    }

    TTF_Font* fuente = TTF_OpenFont("fnt\\arial.ttf", 20);
    if (!fuente)
    {
        printf("Error abriendo fuente: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ventana);
        TTF_Quit();
        SDL_Quit();
        return;
    }

    SDL_Surface *icon = SDL_LoadBMP("img/logo.bmp");  // Nota que debe ser .bmp
    if (icon == NULL) {
    SDL_Log("No se pudo cargar el ícono: %s", SDL_GetError());
    } else {
    SDL_SetWindowIcon(ventana, icon);
    SDL_FreeSurface(icon);  // Liberar después de setear
    }

    Estadistica estadisticas[MAX_ESTADISTICAS];
    int count = 0;
    FILE* file = fopen(ARCHIVO_ESTADISTICAS, "rb");
    if (file)
    {
        count = fread(estadisticas, sizeof(Estadistica), MAX_ESTADISTICAS, file);
        fclose(file);
    }

    SDL_Event event;
    int running = 1;

    SDL_Rect botonVolver = {820/2-140/2, 440, 140, 40};

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE))
            {
                running = 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                int x = event.button.x;
                int y = event.button.y;

                if (x >= botonVolver.x && x <= botonVolver.x + botonVolver.w && y >= botonVolver.y && y <= botonVolver.y + botonVolver.h)
                {
                    running = 0;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        SDL_Color colorTexto = {255, 255, 255, 255};
        int y_offset = 20;
        char resultado[20];

        for (int i = 0; i < count && y_offset < 420; i++)
        {
            if (estadisticas[i].resultado == 0)
            {
                strcpy(resultado, "Perdio");
                estadisticas[i].puntaje = 0;
            }
            else if (estadisticas[i].resultado == 1)
                strcpy(resultado, "Gano");
            else
                strcpy(resultado, "Sin Terminar");

            char texto[256];
            snprintf(texto, sizeof(texto),
                     "Partida %d: %s - %dx%d, Minas: %d, Tiempo: %ds, Resultado: %s, Puntaje: %.2f",
                     i + 1,
                     estadisticas[i].nombre,
                     estadisticas[i].filas,
                     estadisticas[i].columnas,
                     estadisticas[i].minas,
                     estadisticas[i].tiempo,
                     resultado,
                    estadisticas[i].puntaje);

            SDL_Surface* surfaceTexto = TTF_RenderText_Blended(fuente, texto, colorTexto);
            SDL_Texture* textureTexto = SDL_CreateTextureFromSurface(renderer, surfaceTexto);
            SDL_FreeSurface(surfaceTexto);

            SDL_Rect dstrect = {20, y_offset, 0, 0};
            SDL_QueryTexture(textureTexto, NULL, NULL, &dstrect.w, &dstrect.h);
            SDL_RenderCopy(renderer, textureTexto, NULL, &dstrect);
            SDL_DestroyTexture(textureTexto);

            y_offset += dstrect.h + 5;
        }

        // Dibujar botón "Volver"
        SDL_SetRenderDrawColor(renderer, 70, 130, 180, 255);
        SDL_RenderFillRect(renderer, &botonVolver);

        SDL_Surface* textoVolver = TTF_RenderText_Blended(fuente, "Volver", (SDL_Color){255, 255, 255, 255});
        SDL_Texture* texturaVolver = SDL_CreateTextureFromSurface(renderer, textoVolver);
        SDL_FreeSurface(textoVolver);

        SDL_Rect dstVolver =
        {
            botonVolver.x + (botonVolver.w - 70) / 2,
            botonVolver.y + 10,
            70, 20
        };
        SDL_RenderCopy(renderer, texturaVolver, NULL, &dstVolver);
        SDL_DestroyTexture(texturaVolver);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    TTF_CloseFont(fuente);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
}
