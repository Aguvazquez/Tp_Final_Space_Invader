/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 *
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
#include "menus.h"
#include "Top_Score.h"
#include "play.h"

/**********Header of locals fucntions*********/

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
static int menu_display(ALLEGRO_DISPLAY**display ,ALLEGRO_SAMPLE *sample[],ALLEGRO_EVENT_QUEUE ** event_queue,ALLEGRO_FONT *font[],ALLEGRO_BITMAP* display_background[],char *str0, char*str1, char*str2,char flag);

/*
 * @Brief create the buttons unpressed
 * @Param param1 : Type font.
 *
 */
static void create_button_unpressed(ALLEGRO_FONT*font,char *str0,char *str1,char *str2);

/*
 * @Brief create the buttons pressed.
 * @Param param1 : Type font.
 * @Param param2: Button be market as pressed
 *
 */

static void create_button_pressed(ALLEGRO_FONT*font,uint8_t button,char *str0,char *str1,char *str2);

/*
 * @Brief Create the Difficulty menu. 
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
 * @Coment This fuctions doesn't stop the music
 */ 
static uint16_t Difficulty(ALLEGRO_DISPLAY**display ,ALLEGRO_SAMPLE *sample[],ALLEGRO_EVENT_QUEUE ** event_queue,ALLEGRO_FONT *font[],ALLEGRO_BITMAP*display_background[],char *str0, char*str1, char*str2);

/****************************Global fuctions***********************/
int main_menu (ALLEGRO_DISPLAY**display ,ALLEGRO_SAMPLE *sample[],ALLEGRO_EVENT_QUEUE ** event_queue,ALLEGRO_FONT *font[],ALLEGRO_BITMAP*display_background[],ALLEGRO_TIMER **timer){
    bool do_exit=false;
    int aux=0;
    bool flag = false;
    
    
    while(!do_exit){
        if(!flag){
        aux=menu_display(display,sample,event_queue,font,display_background,"PLAY","DIFFICULTY","TOP SCORE",0);
        flag=true;
        }
        switch(aux){
            case 0 : do_exit=true;//Exit value 
            break;
            case 1 :{
                
                al_stop_samples();
                aux=play(display,font,event_queue,timer,display_background);
                if(aux==CLOSE_DISPLAY){
                    do_exit=true;
                }
                else if(aux==RESET_GAME){
                    aux=1;
                }
                else{
                    flag=false;
                }
                //do_exit=true;
            }
            break;
            case 2 :{ 
               aux =Difficulty(display,sample,event_queue,font,display_background,"EASY","NORMAL","HARD");
               flag =false;
               if(!aux){
                   fprintf(stderr,"Hubo un error tato , volve a descargar el archivo , gracias\n");
                   return -1;

               }
               else if(aux==2) //Si fue un 2 , entonces se apreto la x del display
                   do_exit =true;
                
            
            }
            break;
            case 3 :{
                aux =Top_Score(display,sample,event_queue,font,display_background);
                flag=false;
               if(!aux){
                   fprintf(stderr,"Hubo un error kpo\n");
                   return -1;

               }
               else if(aux==2) //Si fue un 2 , entonces se apreto la x del display
                   do_exit =true;
               

            }
            break;
            default :{ fprintf(stderr,"Hubo un error  , volve a descargar el archivo , gracias\n");
                   return -1;
            }
            break;


        }
    }
    return aux;
}
int pause_menu(ALLEGRO_DISPLAY**display ,ALLEGRO_EVENT_QUEUE ** event_queue,ALLEGRO_FONT *font[],ALLEGRO_BITMAP*display_background[]){
    int output;
    
    if((output=menu_display(display,NULL,event_queue,font,display_background,"RESUME","RESET GAME","EXIT",1))==-1){
        fprintf(stderr,"Something happened , please try it again latter");
        return -1;
    }
    return output;
}
void next_level_animation(ALLEGRO_FONT *font[],int level){
    char str[]={'L','E','V','E','L',' ',' ',' '};
    
    if(level>=10){
        str[6]=(char)((level/10)+ASCII);
    }
    str[7]=(char)((level%10)+ASCII);
    
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font[1], al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 3, ALLEGRO_ALIGN_CENTER, str);
    al_flip_display();
    al_rest(2.0);
}
char read_difficulty(void){
    
    FILE* fp;
    char output=0;
    fp=fopen(".Difficulty.txt","r");
    output=(fgetc(fp)-ASCII)*10; //Convierto en número.
    output+=(fgetc(fp)-ASCII);
    fclose(fp);
    return output;
}
/**********************Local functions***************/

 static int menu_display(ALLEGRO_DISPLAY**display ,ALLEGRO_SAMPLE *sample[],ALLEGRO_EVENT_QUEUE ** event_queue,ALLEGRO_FONT *font[],ALLEGRO_BITMAP*display_background[], char *str0, char*str1, char*str2,char flag){
    
    uint8_t do_exit=false, check=false,redraw=false ;
    uint8_t aux=0;
     
    /***********Mouse axes**********/
    float mouse_x = 0 ; 
    float mouse_y =0 ;
    
    if(!Create_Top_Score()){ 
        return -1;
    }
    
    /**********Title and bakground menu.*********************/
    
    al_draw_scaled_bitmap(display_background[0],0, 0, al_get_bitmap_width(display_background[0]), al_get_bitmap_height(display_background[0]), 
            0, 0, al_get_display_width(*display), al_get_display_height(*display),0);
    al_draw_scaled_bitmap(display_background[1],0, 0, al_get_bitmap_width(display_background[1]), al_get_bitmap_height(display_background[1]), 
            0, 0, al_get_display_width(*display), al_get_display_height(*display)/3,0);
    
    /********************************************************/
    if(!flag){
        al_play_sample(sample[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL); 
    }
    create_button_unpressed(font[0],str0,str1,str2);
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
        if(mouse_x>=SCREEN_W / 4.0 && mouse_x<= 3*SCREEN_W / 4.0){
            if(mouse_y>=7*SCREEN_H/16 && mouse_y<= 9*SCREEN_H/16){
                if(check){
                    do_exit=true;
                    aux=1;
                }
                else{
                    create_button_pressed(font[0],1,str0,str1,str2);
                }
            }
            else if(mouse_y>=5*SCREEN_H/8 && mouse_y<= 3*SCREEN_H/4){
                if(check){
                    do_exit=true;
                    aux=2;
                }
                else{
                    create_button_pressed(font[0],2,str0,str1,str2);
                }
            }
            else if (mouse_y>=13*SCREEN_H/16 && mouse_y<= 15*SCREEN_H/16){
                if(check){
                    do_exit=true;
                    aux=3;
                }
                else{
                    create_button_pressed(font[0],3,str0,str1,str2);
                }
            }
            else{
                redraw=true;
            }
        }else{
            redraw=true;
        }
        if(redraw){
            create_button_unpressed(font[0],str0,str1,str2);
            redraw =false;
        }
        check= false;
    }
    
    return aux;
}
static uint16_t Difficulty(ALLEGRO_DISPLAY**display ,ALLEGRO_SAMPLE *sample[],ALLEGRO_EVENT_QUEUE ** event_queue,ALLEGRO_FONT *font[],ALLEGRO_BITMAP*display_background[],char *str0,char *str1,char *str2){
uint8_t do_exit=false, check=false,redraw=false ;
FILE* fp;
uint8_t aux=1;

     
    /***********Mouse axes**********/
    float mouse_x = 0 ; 
    float mouse_y =0 ;
    if(!(fp=fopen(".Difficulty.txt","w"))){
        return 0;
    }
    create_button_unpressed(font[0],"EASY","NORMAL","HARD");
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
                    fputs(EASY_CODE,fp);
                }
                else{
                    create_button_pressed(font[0],1,"EASY","NORMAL","HARD");
                }
            }
            else if(mouse_y>=5*SCREEN_H/8 && mouse_y<= 3*SCREEN_H/4){
                if(check){
                    do_exit=true;
                    fputs(NORMAL_CODE,fp);
                }
                else{
                    create_button_pressed(font[0],2,"EASY","NORMAL","HARD");
                }
            }
            else if (mouse_y>=13*SCREEN_H/16 && mouse_y<= 15*SCREEN_H/16){
                if(check){
                    do_exit=true;
                    fputs(HARD_CODE,fp);
                }
                else{
                    create_button_pressed(font[0],3,"EASY","NORMAL","HARD");
                }
            }
            else{
                redraw=true;
            }
        }else{
            redraw=true;
        }
        if(redraw){
            create_button_unpressed(font[0],"EASY","NORMAL","HARD");
            redraw =false;
        }
        check= false;
    }
    if(aux==2){
        fp=fopen(".Difficulty.txt","w"); //En caso de querer cerrar el programa , para evitar problemas dejar predefinido la dificultdad en normal.
        fputs(NORMAL_CODE,fp);
    }
    fclose(fp);
    return aux;
}



static void create_button_unpressed(ALLEGRO_FONT*font,char *str0,char *str1,char *str2){
    
        al_draw_filled_rectangle(SCREEN_W/4, 7*SCREEN_H/16, 3*SCREEN_W/4, 9*SCREEN_H/16, al_color_name("black"));
        al_draw_rectangle(SCREEN_W/4, 7*SCREEN_H/16, 3*SCREEN_W/4, 9*SCREEN_H/16, al_color_name("white"),2.0);
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, (12*SCREEN_H /25), ALLEGRO_ALIGN_CENTER, str0);
        al_draw_filled_rectangle(SCREEN_W/4, 5*SCREEN_H/8, 3*SCREEN_W/4, 3*SCREEN_H/4, al_color_name("black"));
        al_draw_rectangle(SCREEN_W/4, 5*SCREEN_H/8, 3*SCREEN_W/4, 3*SCREEN_H/4, al_color_name("white"),2.0);
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, (267*SCREEN_H / 400), ALLEGRO_ALIGN_CENTER, str1);
        al_draw_filled_rectangle(SCREEN_W/4, 13*SCREEN_H/16, 3*SCREEN_W/4, 15*SCREEN_H/16, al_color_name("black"));
        al_draw_rectangle(SCREEN_W/4, 13*SCREEN_H/16, 3*SCREEN_W/4, 15*SCREEN_H/16, al_color_name("white"),2.0);
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, (171*SCREEN_H / 200), ALLEGRO_ALIGN_CENTER, str2);
        al_flip_display();
    
}


static void create_button_pressed(ALLEGRO_FONT*font,uint8_t button,char *str0,char *str1,char *str2){
    switch(button){
        case 1:{
                al_draw_filled_rectangle(SCREEN_W/4, 7*SCREEN_H/16, 3*SCREEN_W/4, 9*SCREEN_H/16, al_color_name("grey"));
                al_draw_text(font, al_map_rgb(0,0,0), SCREEN_W / 2, (12*SCREEN_H /25), ALLEGRO_ALIGN_CENTER, str0);
        }
        break;
        case 2:{
                al_draw_filled_rectangle(SCREEN_W/4, 5*SCREEN_H/8, 3*SCREEN_W/4, 3*SCREEN_H/4, al_color_name("grey"));
                al_draw_text(font, al_map_rgb(0,0,0), SCREEN_W / 2, (267*SCREEN_H / 400), ALLEGRO_ALIGN_CENTER, str1);
        }
        break;
        case 3:{
                al_draw_filled_rectangle(SCREEN_W/4, 13*SCREEN_H/16, 3*SCREEN_W/4, 15*SCREEN_H/16, al_color_name("grey"));
                al_draw_text(font, al_map_rgb(0,0,0), SCREEN_W / 2, (171*SCREEN_H / 200), ALLEGRO_ALIGN_CENTER, str2);
        }
        break;
    }
    al_flip_display();
}