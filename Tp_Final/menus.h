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
 * @Brief Funcion que se encarga de crear el menú principal.
 * @Return FATAL_ERROR si hubo error.  
 */
void main_menu(void);

/*
 *@Brief Funcion que se encarga de crear el menú de pausa.
 *@Return FATAL_ERROR si hubo error.
 */
int8_t pause_menu(void);

#endif //MENUS_H
#endif //RASPBERRY