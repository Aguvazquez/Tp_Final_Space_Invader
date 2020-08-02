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

#define STR_LONG 6

static void get_top_score(ALLEGRO_DISPLAY * display , ALLEGRO_FONT *font);
static void create_button_unpressed_top_score(ALLEGRO_FONT * font);
static void create_button_pressed_top_score(ALLEGRO_FONT * font);
static void create_table_top_score();
uint16_t Create_Top_Score(){
    if(!fopen(".Top_Score.txt","r+")){ // para la primera vez que se ejecute el programa crea el archivo donde ubicaremos los top scores
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
    create_table_top_score(font[0]);
    get_top_score(*display,font[0]);
    al_flip_display();
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
        if(mouse_x>=13*SCREEN_W/16.0 && mouse_x<= 15*SCREEN_W/16.0){
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
        check= false;
        
    }
    
    return aux;
}
static void create_button_unpressed_top_score(ALLEGRO_FONT * font){
    al_draw_filled_rectangle(13*SCREEN_W/16, 13*SCREEN_H/16, 15*SCREEN_W/16, 15*SCREEN_H/16, al_color_name("black"));
    al_draw_rectangle(13*SCREEN_W/16, 13*SCREEN_H/16, 15*SCREEN_W/16, 15*SCREEN_H/16, al_color_name("white"),2.0);
    al_draw_text(font, al_map_rgb(255, 255, 255), 7*SCREEN_W / 8, (171*SCREEN_H / 200), ALLEGRO_ALIGN_CENTER, "EXIT");
    al_flip_display();
}
static void create_button_pressed_top_score(ALLEGRO_FONT * font){
    al_draw_filled_rectangle(13*SCREEN_W/16, 13*SCREEN_H/16, 15*SCREEN_W/16, 15*SCREEN_H/16, al_color_name("grey"));
    al_draw_text(font, al_map_rgb(255, 255, 255), 7*SCREEN_W / 8, (171*SCREEN_H / 200), ALLEGRO_ALIGN_CENTER, "EXIT");
    al_flip_display();
}
static void create_table_top_score(ALLEGRO_FONT*font){
    //RECTANGULO BASE
    al_draw_filled_rectangle(SCREEN_W/4, SCREEN_H/3, 3*SCREEN_W/4, 5*SCREEN_H/6, al_color_name("black"));
    al_draw_rectangle(SCREEN_W/4, SCREEN_H/3, 3*SCREEN_W/4, 5*SCREEN_H/6, al_color_name("white"),2.0);
    //LINEAS VERTICALES 
    al_draw_line(13*SCREEN_W/24, SCREEN_H/3,13*SCREEN_W/24, 5*SCREEN_H/6, al_color_name("white"),2.0);
    al_draw_line(SCREEN_W/3, SCREEN_H/3,SCREEN_W/3, 5*SCREEN_H/6, al_color_name("white"),2.0);//vertical
    //SEPARADORES
    al_draw_rectangle(SCREEN_W/4, 5*SCREEN_H/12,3*SCREEN_W/4, SCREEN_H/2, al_color_name("white"),2.0);
    al_draw_rectangle(SCREEN_W/4, 7*SCREEN_H/12, 3*SCREEN_W/4, 2*SCREEN_H/3, al_color_name("white"),2.0);
    al_draw_rectangle(SCREEN_W/4, 3*SCREEN_H/4, 3*SCREEN_W/4, 5*SCREEN_H/6, al_color_name("white"),2.0);
    //TEXTO
    al_draw_text(font, al_map_rgb(255,255,255), 29*SCREEN_W / 96, (17*SCREEN_H /48), ALLEGRO_ALIGN_CENTER, "N°");
    al_draw_text(font, al_map_rgb(255,255,255), 29*SCREEN_W / 96, (7*SCREEN_H /16), ALLEGRO_ALIGN_CENTER, "1");
    al_draw_text(font, al_map_rgb(255,255,255), 29*SCREEN_W / 96, (25*SCREEN_H /48), ALLEGRO_ALIGN_CENTER, "2");
    al_draw_text(font, al_map_rgb(255,255,255), 29*SCREEN_W / 96, (29*SCREEN_H /48), ALLEGRO_ALIGN_CENTER, "3");
    al_draw_text(font, al_map_rgb(255,255,255), 29*SCREEN_W / 96, (11*SCREEN_H /16), ALLEGRO_ALIGN_CENTER, "4");
    al_draw_text(font, al_map_rgb(255,255,255), 29*SCREEN_W / 96, (37*SCREEN_H /48), ALLEGRO_ALIGN_CENTER, "5");
    
    al_draw_text(font, al_map_rgb(255,255,255), 7*SCREEN_W / 16, (17*SCREEN_H /48), ALLEGRO_ALIGN_CENTER, "SCORE");
    
    al_draw_text(font, al_map_rgb(255,255,255), 31*SCREEN_W / 48, (17*SCREEN_H /48), ALLEGRO_ALIGN_CENTER, "NAME");
    
    al_flip_display();
}
/*Cuando guardemos algo en el archivo de top score hay que guardarlo de la siguiente manera
 *                      sssss 
 *                      xxxxx
 *                      sssss
 *                      xxxxx
 *                      sssss
 *                      xxxxx
 *         (s de score ,x de nombre) Si el nombre que elige la persona 
 * es menor a 5 letras , rellenar con espacios , es mas facil la programacion asi 
 */
static void get_top_score(ALLEGRO_DISPLAY * display , ALLEGRO_FONT *font){
    uint8_t i;
    static FILE*  fp;
    char str[STR_LONG];
    fp = fopen(".Top_Score.txt","r"); // Con el punto , se crea el archivo de forma oculta.
    
    for(i=0; i<5; i++){
        //SCORE
        fgets(str,STR_LONG,fp);
        al_draw_text(font, al_map_rgb(255,255,255), 7*SCREEN_W/16, (21+4*i)*SCREEN_H/48, ALLEGRO_ALIGN_CENTER, str);
        fgetc(fp);  // "aumento" el fp a la siguiente linea 
        //NAME
        fgets(str,STR_LONG,fp);
        al_draw_text(font, al_map_rgb(255,255,255), 31*SCREEN_W/48, (21+4*i)*SCREEN_H/48, ALLEGRO_ALIGN_CENTER, str);
        fgetc(fp);
    }
}