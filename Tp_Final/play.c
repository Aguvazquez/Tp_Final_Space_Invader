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


uint8_t play(ALLEGRO_DISPLAY**display,ALLEGRO_FONT *font[],ALLEGRO_EVENT_QUEUE **event_queue,ALLEGRO_TIMER **timer,ALLEGRO_BITMAP *display_background[])
{
    uint8_t game[N][N], level=1, difficulty=35;
    //difficulty = leer_archivo();
    //idea: si es facil, puntaje final x1, medio x2, dificil x3 (Approved)
    while(difficulty != 0)
    {
        if(!move(display,font,event_queue,timer,display_background, difficulty))
        {
            difficulty -= 1;
            al_rest(1.0);   //mientras no este la pantalla que indica "siguiente nivel"
            //next_level_pause();
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