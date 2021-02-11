/*******************************************************************************
 *                               Space Invaders                                *
 *                           Trabajo Práctico Final                            *
 *                               Programación I                                *
 *                                   2020Q1                                    *
 * Desarrolladores: Mendizabal Francisco(61454), Vazquez Agustin(61420)        *
 * Todos los derechos reservados.                                              *
 *******************************************************************************/


/***************************** Standard libraries ******************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************/

/******************************* Locals headers ********************************/

#include "setup.h"
#include "menus.h"

/*******************************************************************************/

/******************************* Main function *********************************/

int main(void) {

    if(files_setup()) {
        return EXIT_FAILURE;
    }
    
#ifdef RASPBERRY
    if(rpi_init()){
        fprintf(stderr, "Hubo un error de inicialización.\n");
        return EXIT_FAILURE;
    }
    main_menu();
    rpi_shutdown();
#else

    if (allegro_ini()) { //inicialización de allegro
        fprintf(stderr, "Hubo un error de inicialización.\n");
        return EXIT_FAILURE;
    }

    main_menu(); //función que ejecuta el juego, hasta que es cerrado.

    allegro_shutdown(); //finalización de Allegro

#endif

    return EXIT_SUCCESS;
}

/********************************** END FILE ***********************************/