/*
Apellido(s), nombre(s): Salas, Martin Daniel
DNI: 45205702
Entrega: Sí

Apellido(s), nombre(s): Ferreyra, Gianluca
DNI: 46026937
Entrega: Sí

Apellido(s), nombre(s): Gomez, Franco Luciano
DNI: 41138313
Entrega: Sí
*/

#include "celdas.h"
#include "menu.h"
#include "estadisticas.h"

int main(int argc, char* argv[])
{
    char nombre_usuario[MAX_NOMBRE];
    int continuar;

    while (1)
    {
        continuar = mostrarMenuInicio(nombre_usuario, MAX_NOMBRE);

        if (continuar == -1)
        {
            // El usuario cerró la ventana del menú
            break;
        }

        jugarPartida(nombre_usuario, continuar);
    }

    printf("\nSaliendo del juego. \nHasta luego\n");
    return 0;
}
