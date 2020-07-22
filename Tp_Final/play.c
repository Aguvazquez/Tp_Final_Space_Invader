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

#define ALIEN 1
#define PRIMERA_FILA 2
#define ULTIMA_FILA 7
#define PRIMERA_COLUMNA 2
#define ULTIMA_COLUMNA 12
#define ESPACIO_ALIENS 1
#define BLOQUE 25
#define FILA_BLOQUES N-3
#define PRIMER_BLOQUE 2
#define ULTIMO_BLOQUE 12
#define ESPACIO_BLOQUES 3

uint8_t play(ALLEGRO_DISPLAY**display)
{
    uint8_t game[N][N], level=1;
    //while(condicion)
    {
        set_level(game, level);
        //print_game(game, display);
        move();
        //logic();
    }
    return 0;
}

void set_level(uint8_t game[N][N], uint8_t level)
{
    uint8_t i, j;
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
            game[i][j] = 0;
    }
    
    for(i=PRIMERA_FILA; i<=ULTIMA_FILA; i++)
    {
        for(j=PRIMERA_COLUMNA; j<=ULTIMA_COLUMNA; j+=ESPACIO_ALIENS+1)
            game[i][j] = ALIEN;
    }
    
    if(level==1)    //los bloques solo se inicializan al principio
    {
        for(i=FILA_BLOQUES, j=PRIMER_BLOQUE; j<=ULTIMO_BLOQUE; j+=ESPACIO_BLOQUES+1)
        {
            game[i][j++] = BLOQUE;
            game[i][j] = BLOQUE;
        }
    }
    
    //setear nave con puntero allegro bitmap
}

void print_game(uint8_t game[N][N], ALLEGRO_DISPLAY **display)
{
    al_clear_to_color(al_color_name("black"));
    uint8_t i, j;
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
        {
            if(game[i][j]==ALIEN)
                al_draw_filled_rectangle(j*SCREEN_W/16, i*SCREEN_H/16, (j+1)*SCREEN_W/16, (i+1)*SCREEN_H/16, al_color_name("green"));
            else if(game[i][j]==BLOQUE)
                al_draw_filled_rectangle(j*SCREEN_W/16, i*SCREEN_H/16, (j+1)*SCREEN_W/16, (i+1)*SCREEN_H/16, al_color_name("red"));
        }
    }
    al_flip_display();
    al_rest(3.0);
}