/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   allegro_setup.h
 * Author: aguvazquez
 *
 * Created on 25 de junio de 2020, 17:51
 */

#ifndef ALLEGRO_SETUP_H
#define ALLEGRO_SETUP_H
/*
 *@brief Funcion que se encarga del setup de las funciones de allegro.
 *@param param1: puntero a puntero display
 *@param param2: puntero a puntero a cola de eventos
 *@param param3: puntero a arreglo de fuentes
 *@param param4: puntero a arreglo de canciones/sonidos.
 *@return Devuelve un -1 si no se pudo iniciar algo , devuelve un 1 si sale todo bien 
 * 
 */
uint16_t allegro_ini(ALLEGRO_DISPLAY** display,ALLEGRO_EVENT_QUEUE**  event_queue
                        ,ALLEGRO_FONT*font[],ALLEGRO_SAMPLE *sample[],ALLEGRO_TIMER **timer);
uint16_t allegro_shudown(ALLEGRO_EVENT_QUEUE** event_queue,ALLEGRO_DISPLAY **display);
    
#endif



