#ifndef TOP_SCORE_H
#define TOP_SCORE_H

/*
 * @Brief revisa si el puntaje obtenido entra o no en el top score. 
 * @Param1: puntaje.
 * @Return  devuelve la posición que obtiene el jugador, o 0 si no entra.
 */

int8_t get_top_score(uint32_t score);  //no se si podria ser static

void put_on_top_score(uint32_t score, char *str);

int8_t create_Top_Score();

#endif