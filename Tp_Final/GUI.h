/*******************************************************************************
 *                                  Menus                                      *
 *                                                                             *
 * Desarrolladores: Mendizabal Francisco(61454), Vazquez Agustin(61420)        *
 * Todos los derechos reservados.                                              *
 *******************************************************************************/

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#ifndef RASPBERRY


/*
 * @Brief Crea un menú con 3 botones. 
 * @Param1: oración del primer botón.
 * @Param2: oración del segundo botón.
 * @Param3: oración del tercer botón.
 * @Param4: uso interno que determina la continuación de la música.
 * @Param5: indica si es menú de pausa o no.
 * @Return  CLOSE_DISPLAY cierra el juego.
 *          el número del botón oprimido.
 *          FATAL_ERROR si hubo un error.
 * @Comment esta función no detiene la música.
 */

int8_t menu_display(char *str0, char *str1, char *str2, char flag, uint8_t pause);
/*
 * @Brief muestra el top score en pantalla.
 * @Return CLOSE_DISPLAY si se cierra la pantalla.
 *         0 si se sale del top score.
 */

int8_t Top_Score(void);
//Funcion que dibuja el mundo en la pantalla.
void draw_world(uint8_t level,uint8_t lives,uint8_t alien_change,
                  elements_t nave_x,elements_t* bloques_x,uint8_t* vida_bloques ,
                  elements_t* alien_x,elements_t* alien_y,elements_t* alien_bullets_x,
                  elements_t* alien_bullets_y, elements_t bullet_x,elements_t bullet_y,elements_t mystery_ship_x,elements_t explosion_x,elements_t explosion_y,uint8_t* explosion_time);
 
#else

//Muestra en el terminal la cantidad de vidas y el puntaje de la partida.
void show_on_terminal(uint8_t lives, uint32_t score);


int pause_menu_terminal(void);

void draw_world_rpi(elements_t nave_x, elements_t* bloques_x, uint8_t* vida_bloques,
        elements_t* alien_x, elements_t* alien_y, elements_t* alien_bullets_x,
        elements_t* alien_bullets_y, elements_t bullet_x, elements_t bullet_y, elements_t mystery_ship_x);
#endif

//Imprime los nombres y puntajes del top score.

void print_top_score(void);
/*
 * @Brief Permite cambiar la dificultad del juego en el archivo. 
 * @Param1: opción elegida (1=facil, 2=medio, 3=dificil)
 * @Return  EXIT_SUCCESS si no hubo errores.
 *          FATAL_ERROR si hubo un error.
 */ 

int8_t switch_difficulty(uint8_t option);

#endif