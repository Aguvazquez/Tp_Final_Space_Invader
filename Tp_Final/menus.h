/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main_menu.h
 * Author: aguvazquez
 *
 * Created on 26 de junio de 2020, 16:44
 */

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

int main_menu (ALLEGRO_DISPLAY**display ,ALLEGRO_SAMPLE *sample[],ALLEGRO_EVENT_QUEUE ** event_queue,ALLEGRO_FONT *font[],ALLEGRO_BITMAP*display_background[],ALLEGRO_TIMER**timer);

/*
 *@brief Funcion que se encarga de crear el menu de pausa.
 *@param param1: puntero a puntero display.
 *@param param2: puntero a puntero a cola de eventos.
 *@param param3: puntero a arreglo de fuentes.
 *@param param6: puntero a arreglo de backgrounds.
 *@return Devuelve un -1 si hubo error. 
 * 
 */

int pause_menu(ALLEGRO_DISPLAY**display ,ALLEGRO_EVENT_QUEUE ** event_queue,ALLEGRO_FONT *font[],ALLEGRO_BITMAP*display_background[]);

/*
 *@brief Funcion que se encarga de la animacion al pasar al siguiente nivel
 *@param param1: puntero a arreglo de fuentes
 *@param param2: nivel que se quiere mostrar. 
 * 
 */
void next_level_animation(ALLEGRO_FONT *font[], uint8_t level);

char read_difficulty(void);
void lose_animation(ALLEGRO_FONT *font[], uint32_t score);

#endif





