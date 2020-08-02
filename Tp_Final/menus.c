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
#include "menu_display_allegro.h"

/**********Header of locals fucntions*********/

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
 * @Coment This fuctions don't stop the music
 */ 
//static uint16_t Difficulty(ALLEGRO_DISPLAY**display ,ALLEGRO_SAMPLE *sample[],ALLEGRO_EVENT_QUEUE ** event_queue,ALLEGRO_FONT *font[],ALLEGRO_BITMAP*display_background[],char *str0, char*str1, char*str2);

/****************************Global fuctions***********************/
int main_menu (ALLEGRO_DISPLAY**display ,ALLEGRO_SAMPLE *sample[],ALLEGRO_EVENT_QUEUE ** event_queue,ALLEGRO_FONT *font[],ALLEGRO_BITMAP*display_background[],ALLEGRO_TIMER **timer){
    bool do_exit=false;
    int aux=0;
    bool flag = false,dont_play_song=false;
    
    
    while(!do_exit){
        if(!flag){
            
            if(!dont_play_song)
                aux=menu_display(display,sample,event_queue,font,display_background,"PLAY","DIFFICULTY","TOP SCORE",0,0);
            else
                aux=menu_display(display,sample,event_queue,font,display_background,"PLAY","DIFFICULTY","TOP SCORE",1,0);
            
        flag=true;
        dont_play_song=true;
        }
        switch(aux){
            case 0 : do_exit=true;//Exit value 
            break;
            case 1 :{
                
                al_stop_samples();
                al_play_sample(sample[4], 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                aux=play(sample, display,font,event_queue,timer,display_background);
                al_stop_samples();
                if(aux==CLOSE_DISPLAY){
                    do_exit=true;
                }
                else if(aux==RESET_GAME){
                    aux=1;
                }
                else{
                    flag=false;
                    dont_play_song=false;
                }
                
            }
            break;
            case 2 :{ 
               aux =Difficulty(display,sample,event_queue,font,display_background,"EASY","NORMAL","HARD");
               flag =false;              
               if(!aux){
                   fprintf(stderr,"Hubo un error tato , volve a descargar el archivo , gracias\n");
                   return -1;

               }
               else if(aux==CLOSE_DISPLAY) //Si fue un 2 , entonces se apreto la x del display
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
    
    if((output=menu_display(display,NULL,event_queue,font,display_background,"RESUME","RESET GAME","EXIT",1,1))==-1){
        fprintf(stderr,"Something happened , please try it again latter");
        return -1;
    }
    return output;
}
void next_level_animation(ALLEGRO_FONT *font[], uint8_t level){
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
void lose_animation(ALLEGRO_FONT *font[], uint32_t score){
    char str1[]={"GAME OVER"};
    char str2[]={"YOUR SCORE IS:"};
    char str3[6]={' ',' ',' ',' ',' '};
    uint32_t aux=0,i,j;
    for(i=5,j=1; i; i--,j*=10){
        aux=score/j;
        str3[i]=(char)(aux%10+ASCII);        
    }
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font[1], al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/4, ALLEGRO_ALIGN_CENTER, str1); 
    al_draw_text(font[0], al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2, ALLEGRO_ALIGN_CENTER, str2); 
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
