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

#define N 16
#ifndef PLAY_H
#define PLAY_H

uint8_t play(ALLEGRO_DISPLAY **display);
void set_level(uint8_t game[N][N], uint8_t level);
void print_game(uint8_t game[N][N], ALLEGRO_DISPLAY **display);

#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif /* PLAY_H */

