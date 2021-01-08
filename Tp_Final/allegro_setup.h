#ifndef ALLEGRO_SETUP_H
#define ALLEGRO_SETUP_H
/*
 *@brief Funcion que se encarga del setup de las funciones de allegro.
 *@param param1: puntero a puntero display
 *@param param2: puntero a puntero a cola de eventos
 *@param param3: puntero a arreglo de fuentes
 *@param param4: puntero a arreglo de canciones/sonidos.
 *@param param5: puntero a puntero a timer
 *@param param6: puntero a arreglo de backgrounds. 
 *@return Devuelve un 0 si no se pudo iniciar algo , devuelve un 1 si sale todo bien 
 * 
 */
uint16_t allegro_ini(void);

/*
 * @brief Funcion que desinstala todas las funciones instaladas en allegro_ini. Siempre llamar antes de finalizar el programa
 * @param param1: puntero a puntero a cola de eventos
 * @param param2 : puntero a puntero a display 
 * 
 */
void allegro_shutdown(void);
    
#endif



