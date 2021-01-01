/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Top_Score.h
 * Author: aguvazquez
 *
 * Created on 27 de junio de 2020, 11:24
 */

#ifndef TOP_SCORE_H
#define TOP_SCORE_H

uint16_t Top_Score();

uint16_t Create_Top_Score();
void put_on_top_score(uint32_t score , char *str);
void reorder_top_score(void);
#endif



