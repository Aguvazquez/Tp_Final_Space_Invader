/*******************************************************************************
 *                              Allegro setup                                  *
 *                                                                             *
 * Desarrolladores: Mendizabal Francisco(61454), Vazquez Agustin(61420)        *
 * Todos los derechos reservados.                                              *
 *******************************************************************************/

#ifndef ALLEGRO_SETUP_H
#define ALLEGRO_SETUP_H

/*
 * @Brief verifica la inicialización de los archivos necesarios para jugar
 * @Return EXIT_SUCCESS si no hay errores
 *         EXIT_FAILURE si hay errores 
 */

uint8_t files_setup (void);

#ifndef RASPBERRY

/*
 * @Brief  Funcion que se encarga del setup de las funciones de allegro.
 * @Return EXIT_FAILURE si hubo un error
 *         EXIT_SUCCESS si no hubo errores  
 */

uint8_t allegro_ini(void);

/*
 * @Brief  Funcion que desinstala todas las funciones instaladas en allegro_ini. 
 *         Siempre llamar antes de finalizar el programa.
 */

void allegro_shutdown(void);
#else
/*
 * @Brief  Funcion que se encarga del setup de las funciones de raspberry.
 * @Return EXIT_FAILURE si hubo un error
 *         EXIT_SUCCESS si no hubo errores  
 */
int8_t rpi_ini(void);
#endif

#endif