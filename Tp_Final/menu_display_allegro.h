/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   menu_display_allegro.h
 * Author: aguvazquez
 *
 * Created on 29 de julio de 2020, 10:52
 */

#ifndef MENU_DISPLAY_ALLEGRO_H
#define MENU_DISPLAY_ALLEGRO_H
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
 *          -1 if something  gone wrong.
 * @Coment This fuctions doesn't stop the music.
 */

int menu_display(ALLEGRO_DISPLAY**display ,ALLEGRO_SAMPLE *sample[],
                 ALLEGRO_EVENT_QUEUE ** event_queue,ALLEGRO_FONT *font[],
                 ALLEGRO_BITMAP*display_background[], char *str0, char*str1,
                                                           char*str2,char flag);
int16_t Difficulty(ALLEGRO_DISPLAY**display ,ALLEGRO_SAMPLE *sample[],ALLEGRO_EVENT_QUEUE ** event_queue,ALLEGRO_FONT *font[],ALLEGRO_BITMAP*display_background[],char *str0,char *str1,char *str2);
#endif /* MENU_DISPLAY_ALLEGRO_H */

