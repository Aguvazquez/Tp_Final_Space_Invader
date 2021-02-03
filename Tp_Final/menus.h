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
 * @Brief Funcion que se encarga de crear el menú principal.
 * @Return FATAL_ERROR si hubo error.  
 */

void main_menu(void);

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

#endif

//Muestra en el terminal la cantidad de vidas y el puntaje de la partida.
void show_on_terminal(uint8_t lives, uint32_t score);

//Menu de raspberry.
void main_menu_terminal(void);

#endif