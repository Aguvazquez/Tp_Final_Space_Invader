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
#include "play.h"
#include "config.h"
#include "move.h"
#include "menus.h"


int play(ALLEGRO_SAMPLE* sample[], ALLEGRO_DISPLAY**display,ALLEGRO_FONT *font[],ALLEGRO_EVENT_QUEUE **event_queue,ALLEGRO_TIMER **timer,ALLEGRO_BITMAP *display_background[])
{
    uint8_t level=1, difficulty, lifes=3;
    int8_t aux=0;
    uint32_t score=0;
    char name[6];
    difficulty = read_difficulty();
    if(difficulty!=EASY){
        if(difficulty!=NORMAL){
            if(difficulty!=HARD){
                fprintf(stderr,"Something happened, pleasy try it again latter");
                return CLOSE_DISPLAY;
            }
        }
    }
    
    //idea: si es facil, puntaje final x1, medio x2, dificil x3 (Approved)
    
    next_level_animation(font, level);  
    
    while(difficulty)
    {
        aux=move(sample, display,font,event_queue,timer,display_background, difficulty, &lifes, level, &score);
        
        if(aux==CLOSE_DISPLAY||aux==RESET_GAME||aux==EXIT_MENU)
            return aux; 

        else if(!aux)
        {
            if(difficulty>12)
                difficulty--;
            next_level_animation(font,++level);   //mientras no este la pantalla que indica "siguiente nivel"
            if(lifes<3)
              lifes++;
            else
                score+=100; //pasar de nivel con 3 vidas suma puntos
        }
        else
        {
            difficulty = 0;
            lose_animation(font, score);
            if(aux=get_top_score(score)){
//                get_name(name);
//                submit_name(name, score, aux);
            }
        }
    }
    return 0;
}

uint8_t get_top_score(uint32_t score){  //devuelve la posicion del jugador si esta en el top 5, sino un 0
    static FILE* fp;
    char str[STR_LONG];
    uint8_t i;
    fp = fopen(".Top_Score.txt", "r");
    for(i=1; i<=5; i++){
        //SCORE
        fgets(str,STR_LONG,fp);
        if(string_to_number(str) < score)
            return i;
        fgetc(fp);  // "aumento" el fp a la siguiente linea 
        //NAME
        fgets(str,STR_LONG,fp);
        fgetc(fp);
    }
    fclose(fp);
    return 0;
}

uint32_t string_to_number(char str[6]){
    int8_t i, j, aux=0;
    for(i=5, j=1; i>=0; i--, j*=10){
        aux += (int8_t)((str[i]-ASCII)*j);
    }
    return aux;
}

void submit_name(char name[6], uint32_t score, uint8_t posicion){
    
}