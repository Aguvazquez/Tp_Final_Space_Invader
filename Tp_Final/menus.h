/*******************************************************************************
 *                                  Menus                                      *
 *                                                                             *
 * Desarrolladores: Mendizabal Francisco(), Vazquez Agustin(61420)             *
 * Todos los derechos reservados.                                              *
 *******************************************************************************/
#ifndef RASPBERRY //EN caso de compilar en la Rpi, este header queda vacio.

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

/*
 *@brief Funcion que se encarga de crear el menù principal.
 *@return Devuelve un -1 si hubo error.  
 */

int main_menu (void);

/*
 *@brief Funcion que se encarga de crear el menu de pausa.
 *@return Devuelve un -1 si hubo error.  
 */

int pause_menu(void);

/*
 *@brief Funcion que se encarga de la animacion al pasar al siguiente nivel
 *@param param2: nivel que se quiere mostrar.  
 */

void next_level_animation( uint8_t level);


void lose_animation( uint32_t score);

#endif //MENUS_H
#endif //RASPBERRY

char read_difficulty(void);

char* number_to_str(uint32_t score);