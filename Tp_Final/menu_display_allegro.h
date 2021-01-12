#ifndef MENU_DISPLAY_ALLEGRO_H
#define MENU_DISPLAY_ALLEGRO_H

/*
 * @Brief Crea un menú con 3 botones. 
 * @Param1: oración del primer botón.
 * @Param2: oración del segundo botón.
 * @Param3: oración del tercer botón.
 * @Param4: ...?
 * @param5: indica si es menú de pausa o no.
 * @Return  CLOSE_DISPLAY cierra el juego.
 *          1 si se oprime el primer botón.
 *          2 si se oprime el segundo botón.
 *          3 si se oprime el tercer botón. 
 *          FATAL_ERROR si hay un error.
 * @Comment esta función no detiene la música.
 */

int8_t menu_display(char *str0, char *str1, char *str2, char flag, uint8_t pause);

/*
 * @Brief Permite cambiar la dificultad del juego. 
 * @Param1: oración del primer botón.
 * @Param2: oración del segundo botón.
 * @Param3: oración del tercer botón.
 * @Return  CLOSE_DISPLAY cierra el juego.
 *          1 = fácil.
 *          2 = medio.
 *          3 = difícil. 
 *          FATAL_ERROR si hay un error.
 */ 

int8_t Difficulty(char *str0, char *str1, char *str2);

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

#endif /* MENU_DISPLAY_ALLEGRO_H */
