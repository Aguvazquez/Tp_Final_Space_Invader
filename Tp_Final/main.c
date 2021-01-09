#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "allegro_setup.h"
#include "config.h"
#include "menus.h"
#include "Top_Score.h"
#include "disdrv.h"
#include "joydrv.h"
#include "libaudio.h"
#include "play.h"

#ifndef RASPBERRY

#include <allegro5/allegro.h>  
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_ttf.h> 
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h> 

ALLEGRO_DISPLAY * display = NULL;
ALLEGRO_EVENT_QUEUE * event_queue = NULL;
ALLEGRO_TIMER * timer = NULL;
ALLEGRO_FONT *font[FONTS] = {NULL, NULL}; //Para incluir mas de un tipo de letra , es decir mayusculas y bla bla bla
ALLEGRO_SAMPLE * samples[SAMPLES] = {NULL, NULL, NULL, NULL, NULL}; //arreglo de canciones , para saber cuantas hay que iniciar.
ALLEGRO_BITMAP* display_background[BACKGROUNDS] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}; // arreglo para incluir fondos.

#endif

int main(void) {

#ifdef RASPBERRY

    disp_init();
    joy_init();
    play(1);

#else
   
    if(!allegro_ini()){     //inicialización de allegro
        fprintf(stderr, "Hubo un error.\n");
        return FATAL_ERROR;
    }
    
    main_menu();    //función que ejecuta el juego, hasta que es cerrado.
    
    allegro_shutdown();
   
#endif
    
    return (EXIT_SUCCESS);
}