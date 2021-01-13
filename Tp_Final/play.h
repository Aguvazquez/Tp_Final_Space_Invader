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
 *          EXIT_MENU vuelve al menú principal.
 *          RESET_GAME reinicia el juego.
 *          EXIT_SUCCESS cuando termina la partida.
 *          FATAL_ERROR si hay un error.
 */

int8_t play(uint8_t mode);

/*
 * @Brief convierte un string en un entero. HAY QUE REVISAR POR QUE ES SIGNADO, SI NO HAY MOTIVO ES INEFICIENTE. 
 * @Param1: string a convertir.
 * @Return  el valor correspondiente del string recibido.
 */

int32_t string_to_number(char str[6]);  //no se si podria ser static

/*
 * @Brief lee la dificultad, guardada en el archivo oculto. 
 * @Return  el valor leido.
 *          FATAL_ERROR si hay un error.
 */

char read_difficulty(void);

/*
 * @Brief crea una animación que indica el nivel que está por empezar. 
 * @Param1: valor del nivel siguiente.
 */

static void next_level_animation(uint8_t level);

/*
 * @Brief indica el final de la partida, y el puntaje total obtenido. 
 * @Param1: puntaje obtenido.
 */

static void lose_animation(uint32_t score);

#endif /* PLAY_H */
