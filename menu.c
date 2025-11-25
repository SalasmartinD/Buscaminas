#include "menu.h"
#include "estadisticas.h"

int estaEnRect(SDL_Point p, SDL_Rect r)
{
    return (p.x >= r.x && p.x <= r.x + r.w && p.y >= r.y && p.y <= r.y + r.h);
}

int mostrarMenuInicio(char* nombre_usuario, size_t max_largo)
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* ventana = SDL_CreateWindow("Inicio Buscaminas", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ANCHO, ALTO, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* fuente = TTF_OpenFont("fnt\\arial.ttf", 24);
    if (!fuente || !ventana || !renderer)
    {
        printf("Error inicializando SDL o TTF: %s\n", TTF_GetError());
        return -1;
    }


    SDL_Surface *icon = SDL_LoadBMP("img/logo.bmp");  // Nota que debe ser .bmp
    if (icon == NULL) {
    SDL_Log("No se pudo cargar el �cono: %s", SDL_GetError());
    } else {
    SDL_SetWindowIcon(ventana, icon);
    SDL_FreeSurface(icon);  // Liberar despu�s de setear
    }

    const int ANCHO_BOTON = 240;
    const int ALTO_BOTON = 50;
    const int X_CENTRO = (ANCHO - ANCHO_BOTON) / 2;

    SDL_Rect botonContinuar = { X_CENTRO, 180, ANCHO_BOTON, ALTO_BOTON };
    SDL_Rect botonNueva = { X_CENTRO, 240, ANCHO_BOTON, ALTO_BOTON };
    SDL_Rect botonEstadisticas = { X_CENTRO, 300, ANCHO_BOTON, ALTO_BOTON };
    SDL_Rect botonConfiguracion = { X_CENTRO, 360, ANCHO_BOTON, ALTO_BOTON };
    SDL_Rect botonNuevo = { X_CENTRO, 420, ANCHO_BOTON, ALTO_BOTON };

    SDL_StartTextInput();
    int continuar = -1;
    int corriendo = 1;
    SDL_Event evento;
    strcpy(nombre_usuario, "");

    Uint32 tiempoAnterior = SDL_GetTicks();
    int mostrarCursor = 1;

    while (corriendo)
    {
        while (SDL_PollEvent(&evento))
        {
            if (evento.type == SDL_QUIT)
            {
                corriendo = 0;
            }
            else if (evento.type == SDL_TEXTINPUT)
            {
                if (strlen(nombre_usuario) < max_largo - 1)
                {
                    strcat(nombre_usuario, evento.text.text);
                }
            }
            else if (evento.type == SDL_KEYDOWN && evento.key.keysym.sym == SDLK_BACKSPACE)
            {
                if (strlen(nombre_usuario) > 0)
                {
                    nombre_usuario[strlen(nombre_usuario) - 1] = '\0';
                }
            }
            else if (evento.type == SDL_MOUSEBUTTONDOWN && evento.button.button == SDL_BUTTON_LEFT)
            {
                SDL_Point punto = { evento.button.x, evento.button.y };
                if (estaEnRect(punto, botonContinuar))
                {
                    continuar = 1;
                    corriendo = 0;
                }
                else if (estaEnRect(punto, botonNueva))
                {
                    continuar = 0;
                    corriendo = 0;
                }
                else if (estaEnRect(punto, botonEstadisticas))
                {
                    mostrarEstadisticasSDL();
                }
                else if (estaEnRect(punto, botonConfiguracion))
                {
                    mostrarVentanaConfiguracion();
                    SDL_StartTextInput();
                }
            }
        }

        if (SDL_GetTicks() - tiempoAnterior > 500)
        {
            mostrarCursor = !mostrarCursor;
            tiempoAnterior = SDL_GetTicks();
        }

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
        SDL_RenderFillRect(renderer, &botonContinuar);
        SDL_RenderFillRect(renderer, &botonNueva);
        SDL_RenderFillRect(renderer, &botonEstadisticas);
        SDL_RenderFillRect(renderer, &botonConfiguracion);
        SDL_RenderFillRect(renderer, &botonNuevo);

        SDL_Color negro = { 0, 0, 0, 255 };

        SDL_Surface* textoContinuar = TTF_RenderText_Solid(fuente, "Continuar", negro);
        SDL_Texture* texContinuar = SDL_CreateTextureFromSurface(renderer, textoContinuar);
        SDL_Rect rectTexto1 = {
            botonContinuar.x + (botonContinuar.w - textoContinuar->w) / 2,
            botonContinuar.y + (botonContinuar.h - textoContinuar->h) / 2,
            textoContinuar->w,
            textoContinuar->h
        };
        SDL_RenderCopy(renderer, texContinuar, NULL, &rectTexto1);

        SDL_Surface* textoNueva = TTF_RenderText_Solid(fuente, "Nueva", negro);
        SDL_Texture* texNueva = SDL_CreateTextureFromSurface(renderer, textoNueva);
        SDL_Rect rectTexto2 = {
            botonNueva.x + (botonNueva.w - textoNueva->w) / 2,
            botonNueva.y + (botonNueva.h - textoNueva->h) / 2,
            textoNueva->w,
            textoNueva->h
        };
        SDL_RenderCopy(renderer, texNueva, NULL, &rectTexto2);

        SDL_Surface* textoEstadisticas = TTF_RenderText_Solid(fuente, "Estadisticas", negro);
        SDL_Texture* texEstadisticas = SDL_CreateTextureFromSurface(renderer, textoEstadisticas);
        SDL_Rect rectTexto3 = {
            botonEstadisticas.x + (botonEstadisticas.w - textoEstadisticas->w) / 2,
            botonEstadisticas.y + (botonEstadisticas.h - textoEstadisticas->h) / 2,
            textoEstadisticas->w,
            textoEstadisticas->h
        };
        SDL_RenderCopy(renderer, texEstadisticas, NULL, &rectTexto3);

        SDL_Surface* textoConfiguracion = TTF_RenderText_Solid(fuente, "Configuracion", negro);
        SDL_Texture* texConfiguracion = SDL_CreateTextureFromSurface(renderer, textoConfiguracion);
        SDL_Rect rectTexto4 = {
            botonConfiguracion.x + (botonConfiguracion.w - textoConfiguracion->w) / 2,
            botonConfiguracion.y + (botonConfiguracion.h - textoConfiguracion->h) / 2,
            textoConfiguracion->w,
            textoConfiguracion->h
        };
        SDL_RenderCopy(renderer, texConfiguracion, NULL, &rectTexto4);

        SDL_Surface* textoNuevo = TTF_RenderText_Solid(fuente, "Dou", negro);
        SDL_Texture* texNuevo = SDL_CreateTextureFromSurface(renderer, textoNuevo);
        SDL_Rect rectTexto5 = {
            botonNuevo.x + (botonNuevo.w - textoNuevo->w) / 2,
            botonNuevo.y + (botonNuevo.h - textoNuevo->h) / 2,
            textoNuevo->w,
            textoNuevo->h
        };
        SDL_RenderCopy(renderer, texNuevo, NULL, &rectTexto5);

        // Campo de entrada resaltado
        SDL_Rect fondoInput = { 45, 95, 330, 40 };
        SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
        SDL_RenderFillRect(renderer, &fondoInput);

        char nombreConCursor[64];
        snprintf(nombreConCursor, sizeof(nombreConCursor), "%s%s", nombre_usuario, mostrarCursor ? "|" : "");

        char entradaTexto[64];
        snprintf(entradaTexto, sizeof(entradaTexto), "Jugador: %s", nombreConCursor);
        SDL_Surface* textoNombre = TTF_RenderText_Solid(fuente, entradaTexto, negro);
        SDL_Texture* texNombre = SDL_CreateTextureFromSurface(renderer, textoNombre);
        SDL_Rect rectNombre = { 50, 100, textoNombre->w, textoNombre->h };
        SDL_RenderCopy(renderer, texNombre, NULL, &rectNombre);

        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(texContinuar);
        SDL_DestroyTexture(texNueva);
        SDL_DestroyTexture(texEstadisticas);
        SDL_DestroyTexture(texConfiguracion);
        SDL_DestroyTexture(texNuevo);
        SDL_DestroyTexture(texNombre);
        SDL_FreeSurface(textoContinuar);
        SDL_FreeSurface(textoNueva);
        SDL_FreeSurface(textoEstadisticas);
        SDL_FreeSurface(textoConfiguracion);
        SDL_FreeSurface(textoNuevo);
        SDL_FreeSurface(textoNombre);
    }

    SDL_StopTextInput();
    TTF_CloseFont(fuente);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    TTF_Quit();
    SDL_Quit();
    return continuar;
}

void mostrarVentanaConfiguracion()
{
    SDL_Window* ventana = SDL_CreateWindow("Configuracion", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 420, 180, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* fuente = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 20);
    SDL_Surface *icon = SDL_LoadBMP("img/logo.bmp");  // Nombre correcto
    if (icon == NULL) {
    SDL_Log("Error al cargar el �cono: %s", SDL_GetError());
    } else {
    SDL_SetWindowIcon(ventana, icon);
    SDL_FreeSurface(icon);
    }
    char entradaTam[10] = "";
    char entradaBombas[10] = "";

    int activo = 1, campoActivo = 0;
    SDL_StartTextInput();
    SDL_Event evento;

    SDL_Rect rectEntradaTam = { 30, 50, 340, 30 };
    SDL_Rect rectEntradaBombas = { 30, 100, 380, 30 };

    Uint32 tiempoAnterior = SDL_GetTicks();
    int mostrarCursor = 1;

    while (activo)
    {
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT || (evento.type == SDL_WINDOWEVENT && evento.window.event == SDL_WINDOWEVENT_CLOSE))
            {
                if (evento.window.windowID == SDL_GetWindowID(ventana))
                {
                    activo = 0;
                }
            } else if (evento.type == SDL_MOUSEBUTTONDOWN && evento.button.button == SDL_BUTTON_LEFT) {
                SDL_Point punto = { evento.button.x, evento.button.y };
                if (SDL_PointInRect(&punto, &rectEntradaTam)) campoActivo = 0;
                else if (SDL_PointInRect(&punto, &rectEntradaBombas)) campoActivo = 1;
            } else if (evento.type == SDL_KEYDOWN && evento.key.keysym.sym == SDLK_RETURN) {
                guardarConfiguracion(entradaTam, entradaBombas);
                activo = 0;
            }
            else if (evento.type == SDL_KEYDOWN && evento.key.keysym.sym == SDLK_BACKSPACE)
            {
                char* campo = campoActivo == 0 ? entradaTam : entradaBombas;
                int len = strlen(campo);
                if (len > 0) campo[len - 1] = '\0';
            }
            else if (evento.type == SDL_TEXTINPUT)
            {
                char* campo = campoActivo == 0 ? entradaTam : entradaBombas;
                if (strlen(campo) < 9) strcat(campo, evento.text.text);
            }
        }

        if (SDL_GetTicks() - tiempoAnterior > 500)
        {
            mostrarCursor = !mostrarCursor;
            tiempoAnterior = SDL_GetTicks();
        }

        SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
        SDL_RenderClear(renderer);
        SDL_Color negro = { 0, 0, 0 };

        SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
        if (campoActivo == 0) SDL_RenderFillRect(renderer, &rectEntradaTam);
        if (campoActivo == 1) SDL_RenderFillRect(renderer, &rectEntradaBombas);

        char buffer[128];
        snprintf(buffer, sizeof(buffer), "Tam (de 8 a 32): %s%s", entradaTam, (campoActivo == 0 && mostrarCursor) ? "|" : "");
        SDL_Surface* superficieTam = TTF_RenderText_Solid(fuente, buffer, negro);
        SDL_Texture* texTam = SDL_CreateTextureFromSurface(renderer, superficieTam);
        SDL_Rect rectTextoTam = { rectEntradaTam.x + 5, rectEntradaTam.y + 5, superficieTam->w, superficieTam->h };
        SDL_RenderCopy(renderer, texTam, NULL, &rectTextoTam);

        snprintf(buffer, sizeof(buffer), "Cantidad de Bombas (numero o %%): %s%s", entradaBombas, (campoActivo == 1 && mostrarCursor) ? "|" : "");
        SDL_Surface* superficieBombas = TTF_RenderText_Solid(fuente, buffer, negro);
        SDL_Texture* texBombas = SDL_CreateTextureFromSurface(renderer, superficieBombas);
        SDL_Rect rectTextoBombas = { rectEntradaBombas.x + 5, rectEntradaBombas.y + 5, superficieBombas->w, superficieBombas->h };
        SDL_RenderCopy(renderer, texBombas, NULL, &rectTextoBombas);

        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(texTam);
        SDL_DestroyTexture(texBombas);
        SDL_FreeSurface(superficieTam);
        SDL_FreeSurface(superficieBombas);
    }

    SDL_StopTextInput();
    TTF_CloseFont(fuente);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
}


void guardarConfiguracion(const char* tam, const char* bombas)
{
    FILE* f = fopen("buscaminas.conf", "w");
    if (!f) {
        perror("Error al abrir buscaminas.conf");
        return;
    }
    fprintf(f, "Tamanio=%s\n", tam);
    fprintf(f, "Cantidad_minas=%s\n", bombas);
    fclose(f);
}
