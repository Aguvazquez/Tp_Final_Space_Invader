/*******************************************************************************
 *                                Back End                                     *
 *                                                                             *
 * Desarrolladores: Mendizabal Francisco(61454), Vazquez Agustin(61420)        *
 * Todos los derechos reservados.                                              *
 *******************************************************************************/

#ifndef BACK_END_H
#define BACK_END_H

/*
 * @Brief crea el top score con el formato apropiado, en caso que no se encuentre el archivo. 
 * @Return  EXIT_FAILURE si no encontró ni pudo crear el archivo.
 *          EXIT_SUCCESS en caso contrario.
 */

uint8_t create_Top_Score(void);

/*
 * @Brief revisa si el puntaje obtenido entra o no en el top score. 
 * @Param1: puntaje.
 * @Return  devuelve la posición que obtiene el jugador.
 *          EXIT_SUCCESS si no entra en el top.
 *          FATAL_ERROR si ocurre un error.
 */

int8_t get_top_score(uint32_t score);

/*
 * @Brief coloca al jugador en su lugar correspondiente del top score. 
 * @Param1: puntaje.
 * @Param2: nombre del jugador.
 */

void put_on_top_score(uint32_t score, char *str);

/*
 * @Brief crea el archivo de dificultad en dificultad "normal", en caso que no se encuentre el archivo. 
 * @Return  EXIT_FAILURE si no encontró ni pudo crear el archivo.
 *          EXIT_SUCCESS en caso contrario.
 */

uint8_t create_difficulty(void);

/*
 * @Brief lee la dificultad, guardada en el archivo oculto. 
 * @Return  el valor leido.
 *          FATAL_ERROR si hay un error.
 */

int8_t read_difficulty(void);

/*
 * @Brief Permite cambiar la dificultad del juego en el archivo. 
 * @Param1: opción elegida (1=facil, 2=medio, 3=dificil)
 * @Return  EXIT_SUCCESS si no hubo errores.
 *          FATAL_ERROR si hubo un error.
 */ 

int8_t switch_difficulty(uint8_t option);

#endif