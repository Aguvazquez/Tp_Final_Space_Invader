/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <allegro5/allegro.h>  
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_ttf.h> 
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h> 
#include "config.h"
#include "Top_Score.h"

#define STR_LONG 20

static uint8_t put_top_score(ALLEGRO_DISPLAY * display , ALLEGRO_FONT *font);

uint16_t Create_Top_Score(){
    if(!fopen(".Top_Score.txt","w")){ // para la primera vez que se ejecute el programa crea el archivo donde ubicaremos los top scores
    return 0;
    }
    return 1;
}
uint16_t Top_Score(ALLEGRO_DISPLAY**display ,ALLEGRO_SAMPLE *sample[],ALLEGRO_EVENT_QUEUE ** event_queue,ALLEGRO_FONT *font[],ALLEGRO_BITMAP*display_background[]){
    uint8_t do_exit=false, check=false,redraw=false ;
    uint8_t aux=0;
    float mouse_x = 0 ;
    float mouse_y =0 ;
    al_draw_scaled_bitmap(display_background[0],0, 0, al_get_bitmap_width(display_background[0]), al_get_bitmap_height(display_background[0]), 
            0, 0, al_get_display_width(*display), al_get_display_height(*display), 
            0);
    al_draw_scaled_bitmap(display_background[1],0, 0, al_get_bitmap_width(display_background[1]), al_get_bitmap_height(display_background[1]),
            0, 0, al_get_display_width(*display), al_get_display_height(*display)/3,
            0);
    create_button_unpressed_top_score(font[0]);
    al_flip_display();
    while(!do_exit){ 
        ALLEGRO_EVENT ev;
        if (al_get_next_event(*event_queue, &ev)) //Toma un evento de la cola, VER RETURN EN DOCUMENT.
        {
             if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE )
                do_exit = true;

             else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
                mouse_x = ev.mouse.x;
                mouse_y = ev.mouse.y;
            }
            else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                check=true;
            }
        }
        if(mouse_x>=3*SCREEN_W/4.0 && mouse_x<= 7*SCREEN_W/8.0){
            if(mouse_y>=13*SCREEN_H/16 && mouse_y<= 15*SCREEN_H/16){
                if(check){
                    do_exit=true;
                    aux=1;
                }
                else{
                    create_button_pressed_top_score(font[0]);
                }
         
            }
            else{
                redraw=true;
            }
        }else{
            redraw=true;
        }
        if(redraw){
            create_button_unpressed_top_score(font[0]);
            redraw =false;
        }
        
    }
    
    return aux;
}
void create_button_unpressed_top_score(ALLEGRO_FONT * font){
    al_draw_filled_rectangle(3*SCREEN_W/4, 13*SCREEN_H/16, 7*SCREEN_W/8, 15*SCREEN_H/16, al_color_name("black"));
    al_draw_rectangle(3*SCREEN_W/4, 13*SCREEN_H/16, 7*SCREEN_W/8, 15*SCREEN_H/16, al_color_name("white"),2.0);
    al_draw_text(font, al_map_rgb(255, 255, 255), 13*SCREEN_W / 16, (171*SCREEN_H / 200), ALLEGRO_ALIGN_CENTER, "EXIT");
    al_flip_display();
}
void create_button_pressed_top_score(ALLEGRO_FONT * font){
    al_draw_filled_rectangle(3*SCREEN_W/4, 13*SCREEN_H/16, 7*SCREEN_W/8, 15*SCREEN_H/16, al_color_name("grey"));
    al_draw_text(font, al_map_rgb(255, 255, 255), 13*SCREEN_W / 16, (171*SCREEN_H / 200), ALLEGRO_ALIGN_CENTER, "EXIT");
    al_flip_display();
}
static uint8_t put_top_score(ALLEGRO_DISPLAY * display , ALLEGRO_FONT *font){
    static FILE*  fp;
    char str[STR_LONG];
    fp = fopen(".Top_Score.txt","r+"); // Con el punto , se crea el archivo de forma oculta.
    
    
}