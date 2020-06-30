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
 * @Brief Create the main menu. 
 * @Param param1: pointer to the display.
 * @Param param2: pointer to samples array.
 * @Param param3: pointer to the event queue.
 * @Param param4: pointer to fonts array.
 * @param param5: pointer to backgrounds array.
 * @Return  0 close the game.
 *          1 first button was pressed.
 *          2 second button was pressed.
 *          3 third button was pressed. 
 * @Coment This fuctions doesn't stop the music.
 */
uint16_t menu_display(ALLEGRO_DISPLAY**display ,ALLEGRO_SAMPLE *sample[],ALLEGRO_EVENT_QUEUE ** event_queue,ALLEGRO_FONT *font[],ALLEGRO_BITMAP* display_background[]);
int main_menu (ALLEGRO_DISPLAY**display ,ALLEGRO_SAMPLE *sample[],ALLEGRO_EVENT_QUEUE ** event_queue,ALLEGRO_FONT *font[],ALLEGRO_BITMAP*display_background[]);
#endif





