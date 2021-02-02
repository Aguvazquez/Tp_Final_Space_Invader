/************************* Standard libraries **********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*******************************************************************************/

/**************************** Local headers ************************************/

#include "play.h"
#include "config.h"
#include "move.h"
#include "menus.h"
#include "Top_Score.h"
#include "allegro_setup.h"

/*******************************************************************************/

#ifndef RASPBERRY

/************************* Allegro libraries ***********************************/

#include <allegro5/allegro.h>  
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_ttf.h> 
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h> 

/*******************************************************************************/

/************************** Extern variables ***********************************/

extern  ALLEGRO_DISPLAY *display;
extern  ALLEGRO_EVENT_QUEUE *event_queue;
extern  ALLEGRO_TIMER *timer;
extern  ALLEGRO_FONT *font[FONTS];
extern  ALLEGRO_SAMPLE *samples[SAMPLES];
extern  ALLEGRO_BITMAP *display_background[BACKGROUNDS];

/*******************************************************************************/

/***************************** Headers of local functions **********************/

/*
 * @Brief crea una animación que indica el nivel que está por empezar. 
 * @Param1: valor del nivel siguiente.
 */

static void next_level_animation(uint8_t level);

/*
 * @Brief indica el final de la partida, y el puntaje total obtenido. 
 * @Param1: puntaje obtenido.
 */

static void lose_animation(uint32_t score);
#endif
/*
 * @Brief recibe el nombre del jugador que acaba de entrar en el top score.
 * @Param1: arreglo donde se almacenará el nombre.
 */

static void new_player_in_top(char name[STR_LONG]);



/*
 * @Brief lee la dificultad, guardada en el archivo oculto. 
 * @Return  el valor leido.
 *          FATAL_ERROR si hay un error.
 */

static int8_t read_difficulty(void);

/*******************************************************************************/

/*************************** Global functions **********************************/

int8_t play(void)
{
    uint8_t level=1, lifes=LIFES, multiplier;
    int8_t aux=0, difficulty;
    uint32_t score=0;
    char name[STR_LONG]={' ',' ',' ',' ',' ','\0'};
    difficulty = read_difficulty();
    if(difficulty==EASY){
        multiplier=1;
    }
    else if(difficulty==NORMAL){
        multiplier=2;
    }
    else if(difficulty==HARD){
        multiplier=3;
    }
    else{
        fprintf(stderr, "Dificultad mal configurada.\n");
        return FATAL_ERROR;
    }

#ifndef RASPBERRY
    next_level_animation(level);
#endif

    while(difficulty){
        aux = move(difficulty, &lifes, level, &score, multiplier);
        if(aux==CLOSE_DISPLAY || aux==RESET_GAME || aux==EXIT_MENU){
            return aux; 
        }
        else if(!aux){  //no quedaron aliens vivos, entonces pasa al siguiente nivel

#ifndef RASPBERRY
            next_level_animation(++level);
#endif
            
            if(difficulty > MAX_DIFFICULTY){
                difficulty--;
            }
            if(lifes < LIFES){
                lifes++;
            }
            else{
                score+=100*multiplier; //pasar de nivel con 3 vidas suma puntos
            }
        }
        else{   //el jugador perdió la partida
            difficulty=0;   //permite salir del ciclo
            
#ifndef RASPBERRY
            
            lose_animation(score);
#endif
            aux = get_top_score(score);
            if(aux == FATAL_ERROR){
                return FATAL_ERROR;
            }
            else if(aux){   //si consiguió lugar en el top score
                new_player_in_top(name);
                put_on_top_score(score, name);                  
            }



        }
    }
    return EXIT_SUCCESS;
}

/*******************************************************************************/

/*************************** Local functions ***********************************/

static int8_t read_difficulty(void){
    
    FILE* fp = fopen(".Difficulty.txt", "r");
    if(!fp){
        fprintf(stderr, "Hubo un error al leer la dificultad.\n");
        return FATAL_ERROR;
    }
    int8_t difficulty;
    difficulty=(fgetc(fp)-ASCII)*10;    //Convierto en la decena
    difficulty+=(fgetc(fp)-ASCII);      //Le sumo la unidad
    fclose(fp);
    return difficulty;
}

#ifndef RASPBERRY

static void next_level_animation(uint8_t level){

    char str[]={'L','E','V','E','L',' ',' ',' '};
    
    if(level>=10){
        str[6]=(char)((level/10)+ASCII);    //escribe la decena del nivel en ASCII
    }
    str[7]=(char)((level%10)+ASCII);    //escribe la unidad del nivel en ASCII
    
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font[1], al_map_rgb(255, 255, 255), SCREEN_W/2, 2*SCREEN_H/5, ALLEGRO_ALIGN_CENTER, str);
    al_flip_display();
    al_rest(2.0);       //tiempo que dura la animación
}

static void lose_animation(uint32_t score){

    char str1[]={"GAME OVER"};
    char str2[]={"YOUR SCORE IS: "};
    char str3[STR_LONG]={' ',' ',' ',' ',' '};
    uint32_t aux=0, j;
    int8_t i;
    for(i = STR_LONG-2, j=1; i>=0; i--, j*=10){ //ya que el último dígito esta en la penúltima posición.
        aux=score/j;                            //Algoritmo int -> string
        str3[i]=(char)(aux%10+ASCII);        
    }
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font[1], al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/3, ALLEGRO_ALIGN_CENTER, str1); 
    al_draw_text(font[0], al_map_rgb(255, 255, 255), 23*SCREEN_W/40, SCREEN_H/2, ALLEGRO_ALIGN_RIGHT, str2); 
    al_draw_text(font[0], al_map_rgb(255, 255, 255), 23*SCREEN_W/40, SCREEN_H/2, ALLEGRO_ALIGN_LEFT, str3); 
    al_flip_display();
    al_rest(2.0);   //tiempo que dura la animación
}

#endif

static void new_player_in_top(char name[STR_LONG]){
#ifndef RASPBERRY
    ALLEGRO_EVENT ev;
    uint8_t i;

    for(i=0; i<STR_LONG; ){
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(font[1], al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/3, ALLEGRO_ALIGN_CENTER, "Escriba su nombre:");
        al_draw_text(font[1], al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2, ALLEGRO_ALIGN_CENTER, name);
        al_flip_display();
        al_wait_for_event(event_queue, &ev);
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_ENTER:{
                    i=STR_LONG; //para salir del ciclo
                    break;
                }
                case ALLEGRO_KEY_BACKSPACE:{
                    if(i){               //si no es la primera letra
                        name[--i]=' ';   //borra la letra anterior
                    }
                    else{
                        name[i]=' ';     //sino borra la primera
                    }
                    break;       
                }
                default:{
                    if(i < STR_LONG-1){
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
    }
#else
    uint8_t c,i;
    system("clear");
    fprintf(stderr,"Por favor, escriba su nombre(MAX 5 CARACTERES):\n");
    for(i=0;i<STR_LONG:i++){
        c=getchar();
        if(c==127){ // 127 es el ASCII de delete
           if (i) { //si no es la primera letra
                name[--i] = ' '; //borra la letra anterior
            } else {
                name[i] = ' '; //sino borra la primera
            }
        }
        else if(c=='\n'){
            break: // se termina el algoritmo, pues el usuario decide mander ese nombre.
        }
            name[i]=c;
    }
#endif
}


/***************************** END FILE ****************************************/