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

uint16_t menu_display(ALLEGRO_DISPLAY**display ,ALLEGRO_SAMPLE *sample[],ALLEGRO_EVENT_QUEUE ** event_queue,ALLEGRO_FONT *font[],ALLEGRO_BITMAP** display_background);
void create_button_unpressed(ALLEGRO_FONT*font);
void create_button_pressed(ALLEGRO_FONT*font,uint8_t button);
#endif





