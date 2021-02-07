/*************************** Standard libraries ********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************/

/***************************** Locals headers **********************************/

#include "config.h"
#include "GUI.h"
#include "back_end.h"
#include "joydrv.h"
#include "disdrv.h"
#include "logic.h"
#include "libaudio.h"
/*******************************************************************************/

#ifndef RASPBERRY //Condición de compilación, afecta a las librerias 
                  //y a las variables.

/**************************** Allegro libraries ********************************/

#include <allegro5/allegro.h>  
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_ttf.h> 
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h>

/*******************************************************************************/

/************************* Allegro global variables ****************************/

extern  ALLEGRO_DISPLAY *display;
extern  ALLEGRO_EVENT_QUEUE *event_queue;
extern  ALLEGRO_TIMER *timer;
extern  ALLEGRO_FONT *font[FONTS]; 
extern  ALLEGRO_SAMPLE *samples[SAMPLES]; 
extern  ALLEGRO_BITMAP *display_background[BACKGROUNDS];

/*******************************************************************************/

/***************************** Global functions ********************************/

void main_menu(void) {

    bool do_exit = false, flag = false, dont_play_song = false;
    int8_t aux = 0;

    while (!do_exit) {
        if (!flag) {
            if (!dont_play_song) {
                aux = menu_display("PLAY", "DIFFICULTY", "TOP SCORE", 0, 0);
            } else {
                aux = menu_display("PLAY", "DIFFICULTY", "TOP SCORE", 1, 0);
            }
            flag = true;
            dont_play_song = true;
        }

        switch (aux) {
            case CLOSE_DISPLAY:
            {
                do_exit = true;
                break;
            }
            case 1:
            {
                al_stop_samples();
                al_play_sample(samples[4], 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                aux = play();           //aux obtendrá el valor correspondiente
                al_stop_samples();      //al terminar la partida.
                switch (aux) {
                    case 0: case EXIT_MENU:
                    {
                        flag = false;
                        dont_play_song = false;
                        break;
                    }
                    case CLOSE_DISPLAY:
                    {
                        do_exit = true;
                        break;
                    }
                    case RESET_GAME:
                    {
                        aux = 1; //como si se oprimiera "play" nuevamente
                        break;
                    }
                    default:
                    {
                        fprintf(stderr, "Hubo un error en la partida.\n");
                        do_exit = true;
                        break;
                    }
                }
                break;
            }
            case 2:
            {
                aux = menu_display("EASY", "NORMAL", "HARD", 0, 0);
                flag = false;
                switch (aux) {
                    case 1: case 2: case 3:
                    {
                        if (switch_difficulty(aux)) {
                            fprintf(stderr, "Hubo un error al modificar la dificultad.\n");
                        }
                        break;
                    }
                    case CLOSE_DISPLAY:
                    {
                        do_exit = true;
                        break;
                    }
                    default:
                    {
                        fprintf(stderr, "Hubo un error al modificar la dificultad.\n");
                        do_exit = true;
                        break;
                    }
                }
                break;
            }
            case 3:
            {
                aux = Top_Score();
                flag = false;
                if (aux) {
                    if (aux != CLOSE_DISPLAY) { //salida inesperada.
                        fprintf(stderr, "Error al ver top score.\n");
                    }
                    do_exit = true;     //sale siempre que devuelva algo distinto de 0.
                }
                break;
            }
            default:
            {
                fprintf(stderr, "Error inesperado en menú principal.\n");
                do_exit = true;
                break;
            }
        }
    }
}

#else

void main_menu(void) {

    uint8_t choice = 0, c = 0;
    bool do_exit = false, reset = false,dont_play_song=false;
    disp_init();
    joy_init();
    init_sound();
    system("clear");
    fprintf(stderr, "Bienvenido a Space Invaders.\n");

    while (!do_exit) {
        if(!dont_play_song){
            set_file_to_play("spaceinvader_theme.wav");
            play_sound();
        }
        if (!reset) {
            disp_clear();
            fprintf(stderr, "Para emepezar a jugar pulse 1.\n");
            fprintf(stderr, "Para elegir la dificultad pulse 2.\n");
            fprintf(stderr, "Para ver el top score pulse 3.\n");
            fprintf(stderr, "Para salir del juego pulse 4.\n");
        }
        while (!reset && (c = getchar()) != '\n') {
            choice = c;
        }
        reset = false;          //cancela la condicion de reset en caso
                                //que luego se desee volver al menú
        switch (choice) {
            dont_play_song=true;
            case '1': 
            {
                stop_sound();
                switch (play()) {
                    case 0: case EXIT_MENU:
                    {
                        system("clear");
                        break;  //al salir del switch se vuelve directamente al menú
                    }
                    case RESET_GAME:
                    {
                        reset = true; //para que entre a la partida automáticamente
                        break;
                    }
                    default:
                    {
                        fprintf(stderr, "Hubo un error en la partida.\n");
                        break;
                    }                    
                }
                dont_play_song=false;
                break;
            } 
            case '2':
            {
                while (!do_exit) {
                    fprintf(stderr, "Elija la dificultad:\n");
                    fprintf(stderr, "1: FACIL\n");
                    fprintf(stderr, "2: NORMAL\n");
                    fprintf(stderr, "3: DIFICIL\n");
                    while ((c = getchar()) != '\n') {
                        choice = c;
                    }
                    if (choice == '1' || choice == '2' || choice == '3') {
                        if (switch_difficulty(choice - ASCII)) {
                            fprintf(stderr, "Hubo un error al modificar la dificultad.\n");
                        }
                        do_exit = true; //para salir del menú de dificultad
                    } else {
                        fprintf(stderr, "Por favor ingrese un numero valido.\n");
                    }
                }
                do_exit = false;    //para no salir del menú principal
                break;
            }
            case '3':
            {
                print_top_score();
                break;
            } 
            case '4': 
            {
                do_exit = true;
                break;
            } 
            default:
            {
                fprintf(stderr, "Por favor, introduzca un numero valido.\n");
            }
        }
    }
}

#endif

/********************************** END FILE ***********************************/