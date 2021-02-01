/*******************************************************************************
 *                            Space Invaders                                   *
 *                         Trabajo Práctico Final                              *
 *                             Programación I                                  *
 *                                2020Q1                                       *
 * Desarrolladores: Mendizabal Francisco(61454), Vazquez Agustin(61420)        *
 * Todos los derechos reservados.                                              *
 *******************************************************************************/


/************************* Standard libraries **********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*******************************************************************************/

/************************* Locals headers **************************************/

#include "allegro_setup.h"
#include "config.h"
#include "menus.h"
#include "Top_Score.h"
#include "disdrv.h"
#include "joydrv.h"
#include "libaudio.h"
#include "play.h"

/*******************************************************************************/

#ifndef RASPBERRY //Condición de compilación, afecta a las librerias 
                  //y a las variables.

/***************************** Allegro libraries  ******************************/

#include <allegro5/allegro.h>  
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_ttf.h> 
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h>

/*******************************************************************************/

/**************************** Allegro global variables *************************/

ALLEGRO_DISPLAY *display = NULL;

ALLEGRO_EVENT_QUEUE *event_queue = NULL;

ALLEGRO_TIMER *timer = NULL;

ALLEGRO_FONT *font[FONTS] = {NULL, NULL}; 
                                        
ALLEGRO_SAMPLE *samples[SAMPLES] = {NULL, NULL, NULL, NULL, NULL};

ALLEGRO_BITMAP *display_background[BACKGROUNDS] = {NULL, NULL, NULL, NULL, NULL,
         NULL, NULL, NULL, NULL,NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

/*******************************************************************************/

#endif // RASPBERRY

/***************************** Main function ***********************************/

int main(void){
    
#ifdef RASPBERRY

    disp_init();
    joy_init();
    //init_sound();
    play(RBP);

#else
   
    if(create_Top_Score()){     //crea el archivo donde ubicaremos el
                                //top score, en caso de ser necesario.
        return EXIT_FAILURE;
    }
    
    if(allegro_ini()){      //inicialización de allegro
        fprintf(stderr, "Hubo un error de inicialización.\n");
        return EXIT_FAILURE;
    }
    
    main_menu();    //función que ejecuta el juego, hasta que es cerrado.
    
    allegro_shutdown(); //finalización de Allegro
   
#endif
    
    return EXIT_SUCCESS;
}

/******************************* END FILE **************************************/