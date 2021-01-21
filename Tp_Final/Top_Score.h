#ifndef TOP_SCORE_H
#define TOP_SCORE_H

#ifndef RASPBERRY

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
 * @Brief crea el top score con el formato apropiado, en caso que no se encuentre el archivo. 
 * @Return devuelve EXIT_FAILURE si no encontró ni pudo crear el archivo.
 *                  EXIT_SUCCESS en caso contrario.
 */

uint8_t create_Top_Score(void);

#endif

#endif