/*******************************************************************************
 *                                  Play                                       *
 *                                                                             *
 * Desarrolladores: Mendizabal Francisco(61454), Vazquez Agustin(61420)        *
 * Todos los derechos reservados.                                              *
 *******************************************************************************/

#ifndef PLAY_H
#define PLAY_H

/*
 * @Brief ejecuta el juego, administrando parámetros como el puntaje, las vidas y la dificultad. 
 * @Param1: modo de juego (allegro=0, raspberry=1).
 * @Return  CLOSE_DISPLAY cierra el juego.
 *          RESET_GAME reinicia el juego.
 *          EXIT_SUCCESS cuando termina la partida.
 *          FATAL_ERROR si hay un error.
 */

int8_t play(uint8_t mode);

#endif /* PLAY_H */