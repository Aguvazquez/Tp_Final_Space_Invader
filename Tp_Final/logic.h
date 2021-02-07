/*******************************************************************************
 *                                   Move                                      *
 *                                                                             *
 * Desarrolladores: Mendizabal Francisco(61454), Vazquez Agustin(61420)        *
 * Todos los derechos reservados.                                              *
 *******************************************************************************/

#ifndef LOGIC_H
#define LOGIC_H

//defino un tipo de variable que varía según la plataforma, para los elementos del display
 

/*
 * @Brief ejecuta el juego, administrando parámetros como el puntaje, las vidas y la dificultad. 
 * @Return  CLOSE_DISPLAY cierra el juego.
 *          RESET_GAME reinicia el juego.
 *          EXIT_SUCCESS cuando termina la partida.
 *          FATAL_ERROR si hay un error.
 */

int8_t play(void);
#endif