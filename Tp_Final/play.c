#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "play.h"
#include "config.h"
#include "move.h"
#include "menus.h"
#include"Top_Score.h"
#include "allegro_setup.h"

#ifndef RASPBERRY

#include <allegro5/allegro.h>  
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_ttf.h> 
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h> 

extern  ALLEGRO_DISPLAY * display;
extern  ALLEGRO_EVENT_QUEUE * event_queue;
extern  ALLEGRO_TIMER * timer;
extern  ALLEGRO_FONT *font[FONTS] ; //Para incluir mas de un tipo de letra , es decir mayusculas y bla bla bla
extern  ALLEGRO_SAMPLE * samples[SAMPLES]; //arreglo de canciones , para saber cuantas hay que iniciar.
extern  ALLEGRO_BITMAP* display_background[BACKGROUNDS]; // arreglo para incluir fondos.

#endif

int play(uint8_t mode)
{
    uint8_t level=1, difficulty, lifes=LIFES;
    int8_t aux=0, i;
    uint32_t score=0;
    char name[STR_LONG]={' ',' ',' ',' ',' ','\0'};
    difficulty = read_difficulty();
    if(difficulty!= EASY){
        if(difficulty!= NORMAL){
            if(difficulty!= HARD){
                fprintf(stderr,"Hubo un error");
                return CLOSE_DISPLAY;
            }
        }
    }
    
    //idea: si es facil, puntaje final x1, medio x2, dificil x3 (Approved)

#ifndef RASPBERRY

        next_level_animation(level);

#endif

    while(difficulty)
    {
        aux=move(difficulty, &lifes, level, &score, mode);
        
        if(aux==CLOSE_DISPLAY||aux==RESET_GAME||aux==EXIT_MENU)
            return aux; 

        else if(!aux)
        {
            if(difficulty>MAX_DIFFICULTY)
                difficulty--;
#ifndef RASPBERRY
                    next_level_animation(++level);   //mientras no este la pantalla que indica "siguiente nivel"
#endif
            if(lifes<LIFES)
              lifes++;
            else
                score+=100; //pasar de nivel con 3 vidas suma puntos
        }
        else
        {
            difficulty = 0;
            aux=get_top_score(score);
            
#ifndef RASPBERRY
            
                ALLEGRO_EVENT ev;
                lose_animation( score);                      
                if(aux){
                    for(i=0; i<STR_LONG; ){
                        al_clear_to_color(al_map_rgb(0, 0, 0));
                        al_draw_text(font[1], al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTER, name);
                        al_flip_display();
                        al_wait_for_event(event_queue, &ev);
                        if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
                            switch (ev.keyboard.keycode) {

                                case ALLEGRO_KEY_ENTER:
                                    i=STR_LONG;
                                    //name[i-1]='\0';
                                    break;

                                case ALLEGRO_KEY_BACKSPACE:
                                    name[i]=' ';
                                    if(i) i--;
                                    break;       

                                default:
                                    if(i<STR_LONG-1){
                                        if(ev.keyboard.keycode>=ALLEGRO_KEY_A && ev.keyboard.keycode<=ALLEGRO_KEY_Z){
                                            name[i++]=ev.keyboard.keycode-ALLEGRO_KEY_A+'A';
                                        }
                                        else if(ev.keyboard.keycode>=ALLEGRO_KEY_0 && ev.keyboard.keycode<=ALLEGRO_KEY_9){
                                            name[i++]=ev.keyboard.keycode-ALLEGRO_KEY_0+'0';
                                        }
                                    }
                                break;
                            }
                        }
                    }

                    put_on_top_score(score, name);
                }
#endif
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

int32_t string_to_number(char str[STR_LONG]){
    int8_t i;
    int32_t aux=0, j;
    for(i=4, j=1; i>=0; i--, j*=10){
        aux += (((int32_t)str[i]-48)*j);    
    }
    return aux;
}

char read_difficulty(void){
    
    FILE* fp;
    char difficulty=0;
    fp=fopen(".Difficulty.txt","r");
    difficulty=(fgetc(fp)-ASCII)*10;    //Convierto en la decena
    difficulty+=(fgetc(fp)-ASCII);      //Le sumo la unidad
    fclose(fp);
    return difficulty;
}