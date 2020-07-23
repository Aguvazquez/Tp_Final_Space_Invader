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
#define N 16

uint8_t play(ALLEGRO_DISPLAY**display,ALLEGRO_EVENT_QUEUE **event_queue,ALLEGRO_TIMER **timer);
void set_level(uint8_t game[N][N], uint8_t level);
void print_game(uint8_t game[N][N], ALLEGRO_DISPLAY **display);



#endif /* PLAY_H */

