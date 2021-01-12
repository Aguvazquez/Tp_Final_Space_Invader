
/************************* Standard libraries **********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************/

/************************* Locals headers **************************************/

#include "config.h"
#include "menus.h"
#include "Top_Score.h"
#include "play.h"
#include "menu_display_allegro.h"
#include "allegro_setup.h"

/*******************************************************************************/

#ifndef RASPBERRY

/***************************** Allegro libraries  ******************************/

#include <allegro5/allegro.h>  
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_ttf.h> 
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h> 

/*******************************************************************************/

/****************************Allegro global variables **************************/

extern  ALLEGRO_DISPLAY * display;
extern  ALLEGRO_EVENT_QUEUE * event_queue;
extern  ALLEGRO_TIMER * timer;
extern  ALLEGRO_FONT *font[FONTS] ; 
extern  ALLEGRO_SAMPLE * samples[SAMPLES]; 
extern  ALLEGRO_BITMAP* display_background[BACKGROUNDS];

/*******************************************************************************/

/****************************Global fuctions************************************/
int main_menu (void){
    
    bool do_exit=false, flag=false, dont_play_song=false;
    int8_t aux=0;
    
    while(!do_exit){
        if(!flag){   
            if(!dont_play_song){
                aux = menu_display("PLAY", "DIFFICULTY", "TOP SCORE", 0, 0);
            }
            else{
                aux = menu_display("PLAY", "DIFFICULTY", "TOP SCORE", 1, 0);
            }    
        flag=true;
        dont_play_song=true;
        }
        
        switch(aux){
            case CLOSE_DISPLAY:{ 
                do_exit=true; 
                break;
            }
            case 1:{
                al_stop_samples();
                al_play_sample(samples[4], 0.25, 0.0, 1.0,
                                ALLEGRO_PLAYMODE_LOOP, NULL);    
                aux=play(ALLEG);    //aux obtendrá el valor correspondiente 
                                    //al terminar la partida.
                al_stop_samples();
                if(aux==CLOSE_DISPLAY){
                    do_exit=true;
                }
                else if(aux==RESET_GAME){
                    aux=1;      //como si se oprimiera nuevamente 
                                //el primer botón "play".
                }
                else{
                    flag=false;
                    dont_play_song=false;
                }
                break;
            }
            case 2:{
                aux = Difficulty("EASY", "NORMAL", "HARD");
                flag=false;              
                if(aux==FATAL_ERROR){
                    fprintf(stderr,"Error al modificar dificultad.\n");
                    return FATAL_ERROR;
                }
                else if(aux==CLOSE_DISPLAY){
                    do_exit=true;
                }
                break;
            }
            case 3:{                
                aux = Top_Score();                
                flag=false;
                if(!aux){
                    fprintf(stderr,"Error al abrir top score.\n");
                    return FATAL_ERROR;
                }
                else if(aux==2){ //Si fue un 2 --> se apreto la x del display
                    do_exit=true;
                }
                break;
            }
            default:{ 
                fprintf(stderr, "Error inesperado.\n");
                return FATAL_ERROR;
                break;
            }
        }
    }
    return aux;
}
    
int pause_menu(){
    int output;
    
    if((output=menu_display("RESUME","RESET GAME","EXIT",1,1))==-1){
        fprintf(stderr,"Hubo un error en el menù de pausa.");
        return FATAL_ERROR;
    }
    return output;
}

void next_level_animation(uint8_t level){
    char str[]={'L','E','V','E','L',' ',' ',' '};
    
    if(level>=10){
        str[6]=(char)((level/10)+ASCII);
    }
    str[7]=(char)((level%10)+ASCII);
    
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font[1], al_map_rgb(255, 255, 255), SCREEN_W / 2,
                            SCREEN_H / 3, ALLEGRO_ALIGN_CENTER, str);
    al_flip_display();
    al_rest(2.0);
}

void lose_animation( uint32_t score){
    char str1[]={"GAME OVER"};
    char str2[]={"YOUR SCORE IS: "};
    char str3[6]={' ',' ',' ',' ',' '};
    uint32_t aux=0,j;
    int8_t i;
    for(i=4,j=1; i>=0; i--,j*=10){ //Algoritmo int --> char
        aux=score/j;
        str3[i]=(char)(aux%10+ASCII);        
    }
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font[1], al_map_rgb(255, 255, 255), SCREEN_W/2, 
                            SCREEN_H/4, ALLEGRO_ALIGN_CENTER, str1); 
    al_draw_text(font[0], al_map_rgb(255, 255, 255), 23*SCREEN_W/40,
                            SCREEN_H/2, ALLEGRO_ALIGN_RIGHT, str2); 
    al_draw_text(font[0], al_map_rgb(255, 255, 255), 23*SCREEN_W/40,
                            SCREEN_H/2, ALLEGRO_ALIGN_LEFT, str3); 
    al_flip_display();
    al_rest(2.0);
}

#endif //RASPBERRY

char read_difficulty(void){
    
    FILE* fp;
    char output=0;
    fp=fopen(".Difficulty.txt","r");
    output=(fgetc(fp)-ASCII)*10; //Convierto en número.
    output+=(fgetc(fp)-ASCII);
    fclose(fp);
    return output;
}