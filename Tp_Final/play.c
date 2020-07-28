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


int play(ALLEGRO_DISPLAY**display,ALLEGRO_FONT *font[],ALLEGRO_EVENT_QUEUE **event_queue,ALLEGRO_TIMER **timer,ALLEGRO_BITMAP *display_background[])
{
    uint8_t game[N][N], level=1, difficulty;
    int aux=0;
    difficulty = read_difficulty(); 
    //easy=30, medium=25, hard=20
    //idea: si es facil, puntaje final x1, medio x2, dificil x3 (Approved)
    
    next_level_animation(font, level);
    
    while(difficulty != 0)
    {
        aux=move(display,font,event_queue,timer,display_background, difficulty);
        
        if(aux==CLOSE_DISPLAY||aux==RESET_LEVEL){
            return aux;
        }
        else if(!aux)
        {
            difficulty -= 1;
            next_level_animation(font,level++);   //mientras no este la pantalla que indica "siguiente nivel"
            
            //if(life<3)
            //  life++;
        }
        else
        {
            difficulty = 0;
            //perdiste_kpo();
            //guardar_puntaje();
        }
    }
    return 0;
}
