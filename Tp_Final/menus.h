/*******************************************************************************
 *                                  Menus                                      *
 *                                                                             *
 * Desarrolladores: Mendizabal Francisco(61454), Vazquez Agustin(61420)        *
 * Todos los derechos reservados.                                              *
 *******************************************************************************/

#ifndef RASPBERRY //EN caso de compilar en la Rpi, este header queda vacio.

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

/*
 *@Brief Funcion que se encarga de crear el menú principal.
 *@Return FATAL_ERROR si hubo error.  
 */

int8_t main_menu(void);

/*
 * @Brief Crea un menú con 3 botones. 
 * @Param1: oración del primer botón.
 * @Param2: oración del segundo botón.
 * @Param3: oración del tercer botón.
 * @Param4: ...?
 * @Param5: indica si es menú de pausa o no.
 * @Return  CLOSE_DISPLAY cierra el juego.
 *          1 si se oprime el primer botón.
 *          2 si se oprime el segundo botón.
 *          3 si se oprime el tercer botón.
 * @Comment esta función no detiene la música ni puede devolver error.
 */

int8_t menu_display(char *str0, char *str1, char *str2, char flag, uint8_t pause);

/*
 *@Brief Funcion que se encarga de crear el menú de pausa.
 *@Return FATAL_ERROR si hubo error.
 */

int8_t pause_menu(void);

/*
 * @Brief Permite cambiar la dificultad del juego. 
 * @Param1: oración del primer botón.
 * @Param2: oración del segundo botón.
 * @Param3: oración del tercer botón.
 * @Return  CLOSE_DISPLAY cierra el juego.
 *          1 = fácil.
 *          2 = medio.
 *          3 = difícil.
 */ 

static int8_t Difficulty(char *str0, char *str1, char *str2);

/*
 * @Brief crea los botones sin presionar.
 * @Param1: oración del primer botón.
 * @Param2: oración del segundo botón.
 * @Param3: oración del tercer botón.
 */

static void create_button_unpressed(char *str0, char *str1, char *str2);

/*
 * @Brief crea un botón presionado.
 * @Param1: botón presionado.
 * @Param2: oración del primer botón.
 * @Param3: oración del segundo botón.
 * @Param4: oración del tercer botón.
 */

static void create_button_pressed(uint8_t button, char *str0, char *str1, char *str2);

/*
 * @Brief muestra el top score en pantalla.
 * @Return CLOSE_DISPLAY si se cierra la pantalla.
 *         0 si se sale del top score.
 */

static int8_t Top_Score(void);

/*
 * @Brief crea el botón de salida de top score sin presionar.
 */

static void create_button_unpressed_top_score(void);

/*
 * @Brief crea el botón de salida de top score presionado.
 */

static void create_button_pressed_top_score(void);

/*
 * @Brief crea la tabla del top score.
 */

static void create_table_top_score(void);

/*
 * @Brief imprime los nombres y puntajes del top score.
 */

static void print_top_score(void);

#endif //MENUS_H
#endif //RASPBERRY