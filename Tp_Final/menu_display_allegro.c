#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "config.h"
#include "Top_Score.h"
#include "menu_display_allegro.h"
#include "allegro_setup.h"

#ifndef RASPBERRY

#include <allegro5/allegro.h>  
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_ttf.h> 
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h> 

extern  ALLEGRO_DISPLAY * display;
extern  ALLEGRO_EVENT_QUEUE * event_queue;
extern  ALLEGRO_TIMER * timer;
extern  ALLEGRO_FONT *font[FONTS] ; //Para incluir mas de un tipo de letra , es decir mayusculas y bla bla bla
extern  ALLEGRO_SAMPLE * samples[SAMPLES]; //arreglo de canciones , para saber cuantas hay que iniciar.
extern  ALLEGRO_BITMAP* display_background[BACKGROUNDS]; // arreglo para incluir fondos.

/*
 * @Brief create the buttons unpressed
 * @Param param1 : Type font.
 *
 */

static void create_button_unpressed(char *str0,char *str1,char *str2);

/*
 * @Brief create the buttons pressed.
 * @Param param1 : Type font.
 * @Param param2: Button be market as pressed
 *
 */

static void create_button_pressed(uint8_t button, char *str0, char *str1, char *str2);

int8_t menu_display(char *str0, char *str1, char *str2, char flag, uint8_t pause){
    
    bool do_exit=false, check=false, redraw=false ;
    int8_t aux=CLOSE_DISPLAY;
    
    /***********Mouse axes**********/
    float mouse_x=0, mouse_y=0 ;
    
    if(Create_Top_Score()){ 
        return FATAL_ERROR;
    }
    
    /**********Title and bakground menu.*********************/
    al_draw_scaled_bitmap(display_background[0], 0, 0, al_get_bitmap_width(display_background[0]), al_get_bitmap_height(display_background[0]), 
        0, 0, al_get_display_width(display), al_get_display_height(display), 0);
    if(pause){
        al_draw_text(font[1], al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 5, ALLEGRO_ALIGN_CENTER, "PAUSE MENU");
    }
    else{
        al_draw_scaled_bitmap(display_background[1], 0, 0, al_get_bitmap_width(display_background[1]), al_get_bitmap_height(display_background[1]), 
            0, 0, al_get_display_width(display), al_get_display_height(display) / 3, 0);
    }
    /********************************************************/
    
    if(!flag){
        al_play_sample(samples[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL); 
    }
    create_button_unpressed(str0, str1, str2);
    al_flip_display();
    
    while(!do_exit){ 
        ALLEGRO_EVENT ev;
        if (al_get_next_event(event_queue, &ev)){ //Toma un evento de la cola, VER RETURN EN DOCUMENT.
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                do_exit=true;
            }
            else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES){
                mouse_x = ev.mouse.x;
                mouse_y = ev.mouse.y;
            }
            else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                check=true;
            }
        }
        if(mouse_x >= SCREEN_W/4 && mouse_x <= 3*SCREEN_W/4){
            if(mouse_y >= 7*SCREEN_H/16 && mouse_y <= 9*SCREEN_H/16){
                if(check){
                    do_exit=true;
                    aux=1;  //primer botón
                }
                else{
                    create_button_pressed(1, str0, str1, str2);
                }
            }
            else if(mouse_y >= 5*SCREEN_H/8 && mouse_y <= 3*SCREEN_H/4){
                if(check){
                    do_exit=true;
                    aux=2;  //segundo botón
                }
                else{
                    create_button_pressed(2, str0, str1, str2);
                }
            }
            else if (mouse_y >= 13*SCREEN_H/16 && mouse_y <= 15*SCREEN_H/16){
                if(check){
                    do_exit=true;
                    aux=3;  //tercer botón
                }
                else{
                    create_button_pressed(3, str0, str1, str2);
                }
            }
            else{
                redraw=true;
            }
        }
        else{
            redraw=true;
        }
        if(redraw){
            create_button_unpressed(str0, str1, str2);
            redraw=false;
        }
        check=false;
    }
    return aux;
}

int8_t Difficulty(char *str0, char *str1, char *str2){
    
    FILE* fp=fopen(".Difficulty.txt","w");  //Creo el archivo difficulty en donde guardaremos el nivel de dificultad.
    
    if(!fp){
        return FATAL_ERROR;    
    }

    int8_t aux = menu_display(str0, str1, str2, 1, 0);
    switch(aux){
        case CLOSE_DISPLAY:{
            break;
        }
        case 1:{
            fputs(EASY_CODE, fp);
            break;
        }
        case 2:{
            fputs(NORMAL_CODE, fp);
            break;
        }
        case 3:{
            fputs(HARD_CODE, fp);
            break;
        }
        default:{
            aux=FATAL_ERROR;
            break;
        }
    }
    fclose(fp);
    return aux;    
}

static void create_button_unpressed(char *str0,char *str1,char *str2){
    
        al_draw_filled_rectangle(SCREEN_W/4, 7*SCREEN_H/16, 3*SCREEN_W/4, 9*SCREEN_H/16, al_color_name("black"));
        al_draw_rectangle(SCREEN_W/4, 7*SCREEN_H/16, 3*SCREEN_W/4, 9*SCREEN_H/16, al_color_name("white"),2.0);
        al_draw_text(font[0], al_map_rgb(255, 255, 255), SCREEN_W / 2, (12*SCREEN_H /25), ALLEGRO_ALIGN_CENTER, str0);
        al_draw_filled_rectangle(SCREEN_W/4, 5*SCREEN_H/8, 3*SCREEN_W/4, 3*SCREEN_H/4, al_color_name("black"));
        al_draw_rectangle(SCREEN_W/4, 5*SCREEN_H/8, 3*SCREEN_W/4, 3*SCREEN_H/4, al_color_name("white"),2.0);
        al_draw_text(font[0], al_map_rgb(255, 255, 255), SCREEN_W / 2, (267*SCREEN_H / 400), ALLEGRO_ALIGN_CENTER, str1);
        al_draw_filled_rectangle(SCREEN_W/4, 13*SCREEN_H/16, 3*SCREEN_W/4, 15*SCREEN_H/16, al_color_name("black"));
        al_draw_rectangle(SCREEN_W/4, 13*SCREEN_H/16, 3*SCREEN_W/4, 15*SCREEN_H/16, al_color_name("white"),2.0);
        al_draw_text(font[0], al_map_rgb(255, 255, 255), SCREEN_W / 2, (171*SCREEN_H / 200), ALLEGRO_ALIGN_CENTER, str2);
        al_flip_display();
    
}

static void create_button_pressed(uint8_t button,char *str0,char *str1,char *str2){
    switch(button){
        case 1:{
                al_draw_filled_rectangle(SCREEN_W/4, 7*SCREEN_H/16, 3*SCREEN_W/4, 9*SCREEN_H/16, al_color_name("grey"));
                al_draw_text(font[0], al_map_rgb(0,0,0), SCREEN_W / 2, (12*SCREEN_H /25), ALLEGRO_ALIGN_CENTER, str0);
        break;
        }
        case 2:{
                al_draw_filled_rectangle(SCREEN_W/4, 5*SCREEN_H/8, 3*SCREEN_W/4, 3*SCREEN_H/4, al_color_name("grey"));
                al_draw_text(font[0], al_map_rgb(0,0,0), SCREEN_W / 2, (267*SCREEN_H / 400), ALLEGRO_ALIGN_CENTER, str1);
        break;
        }
        case 3:{
                al_draw_filled_rectangle(SCREEN_W/4, 13*SCREEN_H/16, 3*SCREEN_W/4, 15*SCREEN_H/16, al_color_name("grey"));
                al_draw_text(font[0], al_map_rgb(0,0,0), SCREEN_W / 2, (171*SCREEN_H / 200), ALLEGRO_ALIGN_CENTER, str2);
        break;
        }
    }
    al_flip_display();
}

#endif