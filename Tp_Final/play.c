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
    int aux=0;
    difficulty = read_difficulty();
    if(difficulty!=EASY){
        if(difficulty!=NORMAL){
            if(difficulty!=HARD){
                fprintf(stderr,"Something happened, pleasy try it again latter");
                return CLOSE_DISPLAY;
            }
        }
    }
    //easy=30, medium=25, hard=20
    //idea: si es facil, puntaje final x1, medio x2, dificil x3 (Approved)
    
    next_level_animation(font, level);
    
    while(difficulty)
    {
        aux=move(sample, display,font,event_queue,timer,display_background, difficulty, &lifes, level);
        
        if(aux==CLOSE_DISPLAY||aux==RESET_GAME||aux==EXIT_MENU)
            return aux; 

        else if(!aux)
        {
            if(difficulty>12)
                difficulty--;
            next_level_animation(font,++level);   //mientras no este la pantalla que indica "siguiente nivel"
            if(lifes<3)
              lifes++;
        }
        else
        {
            difficulty = 0;
            lose_animation(font);
        }
    }
    return 0;
}
