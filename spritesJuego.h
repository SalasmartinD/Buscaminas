#ifndef SPRITESJUEGO_H_INCLUDED
#define SPRITESJUEGO_H_INCLUDED

const int mina_explotada[PIXEL_POR_LADO][PIXEL_POR_LADO] = {
    {C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C},
    {C,C,C,C,C,N,C,N,N,C,N,C,C,C,C,C},
    {C,C,C,C,C,C,N,N,N,N,C,C,C,C,C,C},
    {C,C,C,C,C,N,N,R,R,N,N,C,C,C,C,C},
    {C,C,C,C,N,N,R,R,R,R,N,N,C,C,C,C},
    {C,N,C,N,N,R,R,N,N,R,R,N,N,C,N,C},
    {C,C,N,N,R,R,N,N,N,N,R,R,N,N,C,C},
    {C,N,N,R,R,N,N,N,N,N,N,R,R,N,N,C},
    {C,N,N,R,R,N,N,N,N,N,N,R,R,N,N,C},
    {C,C,N,N,R,R,N,N,N,N,R,R,N,N,C,C},
    {C,N,C,N,N,R,R,N,N,R,R,N,N,C,N,C},
    {C,C,C,C,N,N,R,R,R,R,N,N,C,C,C,C},
    {C,C,C,C,C,N,N,R,R,N,N,C,C,C,C,C},
    {C,C,C,C,C,C,N,N,N,N,C,C,C,C,C,C},
    {C,C,C,C,C,N,C,N,N,C,N,C,C,C,C,C},
    {C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C}
};

const int mina[PIXEL_POR_LADO][PIXEL_POR_LADO] = {
    {C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C},
    {C,C,C,C,C,C,C,N,N,C,C,C,C,C,C,C},
    {C,C,C,C,C,C,N,N,N,N,C,C,C,C,C,C},
    {C,C,C,C,C,N,N,G,G,N,N,C,C,C,C,C},
    {C,C,C,C,N,N,G,G,G,G,N,N,C,C,C,C},
    {C,C,C,N,N,G,G,N,N,G,G,N,N,C,C,C},
    {C,C,N,N,G,G,N,N,N,N,G,G,N,N,C,C},
    {C,N,N,G,G,N,N,N,N,N,N,G,G,N,N,C},
    {C,N,N,G,G,N,N,N,N,N,N,G,G,N,N,C},
    {C,C,N,N,G,G,N,N,N,N,G,G,N,N,C,C},
    {C,C,C,N,N,G,G,N,N,G,G,N,N,C,C,C},
    {C,C,C,C,N,N,G,G,G,G,N,N,C,C,C,C},
    {C,C,C,C,C,N,N,G,G,N,N,C,C,C,C,C},
    {C,C,C,C,C,C,N,N,N,N,C,C,C,C,C,C},
    {C,C,C,C,C,C,C,N,N,C,C,C,C,C,C,C},
    {C,C,C,C,C,C,C,C,C,C,C,C,C,C,C,C}
};

const int celda_cerrada[PIXEL_POR_LADO][PIXEL_POR_LADO] = {
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {G,G,G,G,G,G,G,G,G,G,G,G,G,G,G,G}
};

const int celda_abierta[PIXEL_POR_LADO][PIXEL_POR_LADO] = {
    {G,G,G,G,G,G,G,G,G,G,G,G,G,G,G,G},
    {G,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {G,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {G,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {G,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {G,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {G,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {G,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {G,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {G,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {G,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {G,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {G,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {G,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {G,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {G,G,G,G,G,G,G,G,G,G,G,G,G,G,G,G}
};

const int bandera[PIXEL_POR_LADO][PIXEL_POR_LADO] = {
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,C,C,C,C,C,C,C,C,R,R,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,R,R,R,C,C,C,C,G},
    {W,C,C,C,C,C,C,R,R,R,R,C,C,C,C,G},
    {W,C,C,C,C,C,R,R,R,R,R,C,C,C,C,G},
    {W,C,C,C,C,R,R,R,R,R,R,C,C,C,C,G},
    {W,C,C,C,R,R,R,R,R,R,R,C,C,C,C,G},
    {W,C,C,R,R,R,R,R,R,R,R,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,N,N,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,N,N,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,N,N,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,N,N,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,N,N,C,C,C,C,G},
    {W,C,C,C,N,N,N,N,N,N,N,N,N,C,C,G},
    {W,C,C,N,N,N,N,N,N,N,N,N,N,N,C,G},
    {G,G,G,G,G,G,G,G,G,G,G,G,G,G,G,G}
};

int signo_pregunta[PIXEL_POR_LADO][PIXEL_POR_LADO] = {
    {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
    {W,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,N,N,N,N,N,C,C,C,C,G},
    {W,C,C,C,C,N,N,C,C,C,N,N,C,C,C,G},
    {W,C,C,C,N,N,C,C,C,C,C,N,N,C,C,G},
    {W,C,C,C,N,N,C,C,C,C,C,N,N,C,C,G},
    {W,C,C,C,C,C,C,C,C,C,C,N,N,C,C,G},
    {W,C,C,C,C,C,C,C,C,C,N,N,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,N,N,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,N,N,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,N,N,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,N,N,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,C,C,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,N,N,C,C,C,C,C,C,G},
    {W,C,C,C,C,C,C,N,N,C,C,C,C,C,C,G},
    {G,G,G,G,G,G,G,G,G,G,G,G,G,G,G,G},
};

#endif