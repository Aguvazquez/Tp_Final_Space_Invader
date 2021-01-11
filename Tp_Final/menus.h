#ifndef MAIN_MENU_H
#define MAIN_MENU_H

/*
 *@brief Funcion que se encarga de crear el menù principal.
 *@param param1: puntero a puntero display.
 *@param param2: puntero a arreglo de canciones/sonidos.
 *@param param3: puntero a puntero a cola de eventos
 *@param param4: puntero a arreglo de fuentes
 *@param param5: puntero a arreglo de backgrounds.
 *@param param6: puntero a puntero a timer 
 *@return Devuelve un -1 si hubo error. 
 * 
 */

int main_menu (void);

/*
 *@brief Funcion que se encarga de crear el menu de pausa.
 *@param param1: puntero a puntero display.
 *@param param2: puntero a puntero a cola de eventos.
 *@param param3: puntero a arreglo de fuentes.
 *@param param6: puntero a arreglo de backgrounds.
 *@return Devuelve un -1 si hubo error. 
 * 
 */
#ifndef RASPBERRY
int pause_menu();

/*
 *@brief Funcion que se encarga de la animacion al pasar al siguiente nivel
 *@param param1: puntero a arreglo de fuentes
 *@param param2: nivel que se quiere mostrar. 
 * 
 */
void next_level_animation( uint8_t level);


void lose_animation( uint32_t score);
#endif

char read_difficulty(void);

char* number_to_str(uint32_t score);
#endif