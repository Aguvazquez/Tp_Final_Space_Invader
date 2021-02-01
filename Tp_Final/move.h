/*******************************************************************************
 *                                   Move                                      *
 *                                                                             *
 * Desarrolladores: Mendizabal Francisco(61454), Vazquez Agustin(61420)        *
 * Todos los derechos reservados.                                              *
 *******************************************************************************/

#ifndef MOVE_H
#define MOVE_H

//defino un tipo de variable que varía según la plataforma, para los elementos del display
#ifdef RASPBERRY
	typedef int elements_t;
#else
        typedef float elements_t;
#endif
        
/*
 * @Brief se encarga del movimiento de los distintos elementos del juego
 * @Param1: dificultad del nivel que está por jugarse
 * @Param2: la cantidad de vidas del jugador al iniciar el nivel
 * @Param3: el número de nivel que se va a jugar
 * @Param4: un puntero al puntaje del jugador
 * @Return  la cantidad de aliens que quedan en el nivel (0 en caso de pasar al siguiente nivel)
 *          EXIT_MENU en caso de volver al menú principal
 *          RESET_GAME en caso de resetear el juego
 *          FATAL_ERROR si ocurre algún error
 */

int8_t move(uint8_t difficulty, uint8_t* lifes, uint8_t level, uint32_t* score, uint8_t multiplier);

#endif