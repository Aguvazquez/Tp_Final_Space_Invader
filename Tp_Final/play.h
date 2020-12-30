/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   play.h
 * Author: frmendizabal
 *
 * Created on 30 de junio de 2020, 17:12
 */


#ifndef PLAY_H
#define PLAY_H


int play(ALLEGRO_SAMPLE* sample[], ALLEGRO_DISPLAY**display,ALLEGRO_FONT *font[],ALLEGRO_EVENT_QUEUE **event_queue,ALLEGRO_TIMER **timer,ALLEGRO_BITMAP *display_background[],uint8_t mode);
int32_t string_to_number(char str[6]);
uint8_t get_top_score(uint32_t score);



#endif /* PLAY_H */

