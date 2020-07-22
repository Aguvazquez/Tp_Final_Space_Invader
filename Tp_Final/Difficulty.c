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
#include "main_menu.h"
#include "Top_Score.h"

/*
 * @Brief create the buttons unpressed
 * @Param param1 : Type font.
 *
 */
static void create_button_unpressed(ALLEGRO_FONT*font);

/*
 * @Brief create the buttons pressed.
 * @Param param1 : Type font.
 * @Param param2: Button be market as pressed
 *
 */

static void create_button_pressed(ALLEGRO_FONT*font,uint8_t button);

uint16_t Difficulty(ALLEGRO_DISPLAY**display ,ALLEGRO_SAMPLE *sample[],ALLEGRO_EVENT_QUEUE ** event_queue,ALLEGRO_FONT *font[],ALLEGRO_BITMAP*display_background[]){
uint8_t do_exit=false, check=false,redraw=false ;
FILE* fp;
uint8_t aux=1;

     
    /***********Mouse axes**********/
    float mouse_x = 0 ; 
    float mouse_y =0 ;
    if(!(fp=fopen(".Difficulty.txt","w"))){
        return 0;
    }
    create_button_unpressed(font[0]);
    while(!do_exit){ 
        ALLEGRO_EVENT ev;
        if (al_get_next_event(*event_queue, &ev)) //Toma un evento de la cola, VER RETURN EN DOCUMENT.
        {
             if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE ){
                do_exit = true;
                aux=2;
             }

             else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
                mouse_x = ev.mouse.x;
                mouse_y = ev.mouse.y;
            }
            else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                check=true;
            }
        }
        if(mouse_x>=SCREEN_W / 4.0 && mouse_x<= 3*SCREEN_W / 4.0){
            if(mouse_y>=7*SCREEN_H/16 && mouse_y<= 9*SCREEN_H/16){
                if(check){
                    do_exit=true;
                    fputc('1',fp);
                }
                else{
                    create_button_pressed(font[0],1);
                }
            }
            else if(mouse_y>=5*SCREEN_H/8 && mouse_y<= 3*SCREEN_H/4){
                if(check){
                    do_exit=true;
                    fputc('2',fp);
                }
                else{
                    create_button_pressed(font[0],2);
                }
            }
            else if (mouse_y>=13*SCREEN_H/16 && mouse_y<= 15*SCREEN_H/16){
                if(check){
                    do_exit=true;
                    fputc('3',fp);
                }
                else{
                    create_button_pressed(font[0],3);
                }
            }
            else{
                redraw=true;
            }
        }else{
            redraw=true;
        }
        if(redraw){
            create_button_unpressed(font[0]);
            redraw =false;
        }
        check= false;
    }
    if(aux==2){
        fp=fopen(".Difficulty.txt","w"); //En caso de querer cerrar el programa , para evitar problemas dejar predefinido la dificultdad en normal.
        fputc('2',fp);
    }
    return aux;
}
void create_button_unpressed(ALLEGRO_FONT*font){
    
    al_draw_filled_rectangle(SCREEN_W/4, 7*SCREEN_H/16, 3*SCREEN_W/4, 9*SCREEN_H/16, al_color_name("black"));
    al_draw_rectangle(SCREEN_W/4, 7*SCREEN_H/16, 3*SCREEN_W/4, 9*SCREEN_H/16, al_color_name("white"),2.0);
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, (12*SCREEN_H /25), ALLEGRO_ALIGN_CENTER, "EASY");
    al_draw_filled_rectangle(SCREEN_W/4, 5*SCREEN_H/8, 3*SCREEN_W/4, 3*SCREEN_H/4, al_color_name("black"));
    al_draw_rectangle(SCREEN_W/4, 5*SCREEN_H/8, 3*SCREEN_W/4, 3*SCREEN_H/4, al_color_name("white"),2.0);
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, (267*SCREEN_H / 400), ALLEGRO_ALIGN_CENTER, "NORMAL");
    al_draw_filled_rectangle(SCREEN_W/4, 13*SCREEN_H/16, 3*SCREEN_W/4, 15*SCREEN_H/16, al_color_name("black"));
    al_draw_rectangle(SCREEN_W/4, 13*SCREEN_H/16, 3*SCREEN_W/4, 15*SCREEN_H/16, al_color_name("white"),2.0);
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, (171*SCREEN_H / 200), ALLEGRO_ALIGN_CENTER, "HARD");
    al_flip_display();
    
}


void create_button_pressed(ALLEGRO_FONT*font,uint8_t button){
    switch(button){
        case 1:{
                al_draw_filled_rectangle(SCREEN_W/4, 7*SCREEN_H/16, 3*SCREEN_W/4, 9*SCREEN_H/16, al_color_name("grey"));
                al_draw_text(font, al_map_rgb(0,0,0), SCREEN_W / 2, (12*SCREEN_H /25), ALLEGRO_ALIGN_CENTER, "EASY");
        }
        break;
        case 2:{
                al_draw_filled_rectangle(SCREEN_W/4, 5*SCREEN_H/8, 3*SCREEN_W/4, 3*SCREEN_H/4, al_color_name("grey"));
                al_draw_text(font, al_map_rgb(0,0,0), SCREEN_W / 2, (267*SCREEN_H / 400), ALLEGRO_ALIGN_CENTER, "NORMAL");
        }
        break;
        case 3:{
                al_draw_filled_rectangle(SCREEN_W/4, 13*SCREEN_H/16, 3*SCREEN_W/4, 15*SCREEN_H/16, al_color_name("grey"));
                al_draw_text(font, al_map_rgb(0,0,0), SCREEN_W / 2, (171*SCREEN_H / 200), ALLEGRO_ALIGN_CENTER, "HARD");
        }
        break;
    }
    al_flip_display();
}