/*******************************************************************************
 *                                   GUI                                       *
 *                                                                             *
 * Desarrolladores: Mendizabal Francisco(61454), Vazquez Agustin(61420)        *
 * Todos los derechos reservados.                                              *
 *******************************************************************************/

#ifndef GUI_H
#define GUI_H

#ifndef RASPBERRY

/*
 * @Brief Crea un menú con 3 botones. 
 * @Param1: oración del primer botón.
 * @Param2: oración del segundo botón.
 * @Param3: oración del tercer botón.
 * @Param4: uso interno que determina la continuación de la música (0=iniciar , 1=no iniciar)
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

/*
 * @Brief crea una animación que indica el nivel que está por empezar. 
 * @Param1: valor del nivel siguiente.
 */

void next_level_animation(uint8_t level);

/*
 * @Brief indica el final de la partida, y el puntaje total obtenido. 
 * @Param1: puntaje obtenido.
 */

void lose_animation(uint32_t score);

/* @Brief funcion que dibuja el mundo en la pantalla.
 * @Params: recibe el nivel, el puntaje y la cantidad de vidas del jugador
 *          recibe una variable que permite la animación de movimiento de los aliens
 *          recibe la vida de los bloques, para variar su imagen a medida que son destruidos
 *          recibe las coordenadas de todos los posibles elementos de juego
 */

void draw_world(uint8_t level, uint32_t score, uint8_t lives, uint8_t alien_change, elements_t nave_x, elements_t* bloques_x, 
                uint8_t* vida_bloques, elements_t* alien_x, elements_t* alien_y, elements_t* alien_bullets_x, 
                elements_t* alien_bullets_y, elements_t bullet_x, elements_t bullet_y, elements_t mystery_ship_x,
                elements_t explosion_x, elements_t explosion_y, uint8_t* explosion_time);

/* @Brief funcion que muestra las instrucciones en la pantalla.
 * @Return: false , en caso de que todo ande normal, 
 *          true, en caso de que cierren display
 */
bool instructions(void);

#else

//Muestra en el terminal la cantidad de vidas y el puntaje de la partida.

void show_on_terminal(uint8_t lives, uint32_t score);

//Menú de pausa en raspberry

int pause_menu_terminal(void);

/* @Brief funcion que dibuja el mundo en la placa.
 * @Params: recibe la vida de los bloques, para ver si deben ser dibujados o no
 *          recibe las coordenadas de todos los posibles elementos de juego
 */

void draw_world_rpi(elements_t nave_x, elements_t* bloques_x, uint8_t* vida_bloques, elements_t* alien_x, 
                    elements_t* alien_y, elements_t* alien_bullets_x, elements_t* alien_bullets_y, 
                    elements_t bullet_x, elements_t bullet_y, elements_t mystery_ship_x);

#endif  //RASPBERRY

/*
 * @Brief recibe el nombre del jugador que acaba de entrar en el top score.
 * @Param1: arreglo donde se almacenará el nombre.
 */

void new_player_in_top(char name[STR_LONG]);

//Imprime los nombres y puntajes del top score.

void print_top_score(void);

#endif