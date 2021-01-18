/************************* Standard libraries **********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************/

/************************* Locals headers **************************************/

#include "config.h"
#include "menus.h"
#include "Top_Score.h"
#include "play.h"
#include "allegro_setup.h"

/*******************************************************************************/

#ifndef RASPBERRY

/***************************** Allegro libraries  ******************************/

#include <allegro5/allegro.h>  
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_ttf.h> 
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h> 

/*******************************************************************************/

/****************************Allegro global variables **************************/

extern  ALLEGRO_DISPLAY *display;
extern  ALLEGRO_EVENT_QUEUE *event_queue;
extern  ALLEGRO_TIMER *timer;
extern  ALLEGRO_FONT *font[FONTS]; 
extern  ALLEGRO_SAMPLE *samples[SAMPLES]; 
extern  ALLEGRO_BITMAP *display_background[BACKGROUNDS];

/*******************************************************************************/

/************************ Header of local functions ****************************/

/*
 * @Brief Crea un menú con 3 botones. 
 * @Param1: oración del primer botón.
 * @Param2: oración del segundo botón.
 * @Param3: oración del tercer botón.
 * @Param4: ...?
 * @Param5: indica si es menú de pausa o no.
 * @Return  CLOSE_DISPLAY cierra el juego.
 *          el número del botón oprimido.
 *          FATAL_ERROR si hubo un error.
 * @Comment esta función no detiene la música.
 */

static int8_t menu_display(char *str0, char *str1, char *str2, char flag, uint8_t pause);

/*
 * @Brief Permite cambiar la dificultad del juego. 
 * @Param1: oración del primer botón.
 * @Param2: oración del segundo botón.
 * @Param3: oración del tercer botón.
 * @Return  CLOSE_DISPLAY cierra el juego.
 *          1 = fácil.
 *          2 = medio.
 *          3 = difícil.
 *          FATAL_ERROR si hubo un error.
 */ 

static int8_t Difficulty(char *str0, char *str1, char *str2);

/*
 * @Brief crea los botones sin presionar.
 * @Param1: oración del primer botón.
 * @Param2: oración del segundo botón.
 * @Param3: oración del tercer botón.
 */

static void create_button_unpressed(char *str0, char *str1, char *str2);

/*
 * @Brief crea un botón presionado.
 * @Param1: botón presionado.
 * @Param2: oración del primer botón.
 * @Param3: oración del segundo botón.
 * @Param4: oración del tercer botón.
 */

static void create_button_pressed(uint8_t button, char *str0, char *str1, char *str2);

/*
 * @Brief muestra el top score en pantalla.
 * @Return CLOSE_DISPLAY si se cierra la pantalla.
 *         0 si se sale del top score.
 */

static int8_t Top_Score(void);

//Crea el botón de salida de top score sin presionar.

static void create_button_unpressed_top_score(void);

//Crea el botón de salida de top score presionado.

static void create_button_pressed_top_score(void);

//Crea la tabla del top score.

static void create_table_top_score(void);

//Imprime los nombres y puntajes del top score.

static void print_top_score(void);

/*******************************************************************************/

/*************************** Global fuctions ***********************************/

void main_menu (void){
    
    bool do_exit=false, flag=false, dont_play_song=false;
    int8_t aux=0;
    
    while(!do_exit){
        if(!flag){   
            if(!dont_play_song){
                aux = menu_display("PLAY", "DIFFICULTY", "TOP SCORE", 0, 0);
            }
            else{
                aux = menu_display("PLAY", "DIFFICULTY", "TOP SCORE", 1, 0);
            }    
            flag=true;
            dont_play_song=true;
        }
        
        switch(aux){
            case CLOSE_DISPLAY:{ 
                do_exit=true; 
                break;
            }
            case 1:{
                al_stop_samples();
                al_play_sample(samples[4], 0.25, 0.0, 1.0,
                                ALLEGRO_PLAYMODE_LOOP, NULL);    
                aux=play(ALLEG);    //aux obtendrá el valor correspondiente 
                                    //al terminar la partida.
                al_stop_samples();
                if(aux==CLOSE_DISPLAY || aux==FATAL_ERROR){
                    do_exit=true;
                }
                else if(aux==RESET_GAME){
                    aux=1;      //como si se oprimiera nuevamente 
                                //el primer botón "play".
                }
                else{
                    flag=false;
                    dont_play_song=false;
                }
                break;
            }
            case 2:{
                aux = Difficulty("EASY", "NORMAL", "HARD");
                flag=false;              
                if(aux==FATAL_ERROR){
                    fprintf(stderr, "Error al modificar dificultad.\n");
                    do_exit=true;
                }
                else if(aux==CLOSE_DISPLAY){
                    do_exit=true;
                }
                break;
            }
            case 3:{                
                aux = Top_Score();                
                flag=false;
                if(aux){
                    if(aux!=CLOSE_DISPLAY){ //salida inesperada.
                        fprintf(stderr, "Error al ver top score.\n");
                    }
                    do_exit=true;   //sale siempre que devuelva algo distinto de 0.
                }
                break;
            }
            default:{ 
                fprintf(stderr, "Error inesperado.\n");
                do_exit=true;
                break;
            }
        }
    }
}

int8_t pause_menu(void){    
    int8_t output = menu_display("RESUME", "RESET GAME", "EXIT", 1, 1);
    
    if(output == FATAL_ERROR){
        fprintf(stderr, "Hubo un error en el menú de pausa.");
    }
    return output;
}

/****************************Local fuctions************************************/

static int8_t menu_display(char *str0, char *str1, char *str2, char flag, uint8_t pause){
    
    bool do_exit=false, check=false, redraw=false;
    int8_t aux=0;
    
    /***********Mouse axes**********/
    float mouse_x=0, mouse_y=0;
    
    /**********Title and bakground menu.*********************/
    al_draw_scaled_bitmap(display_background[0], 0, 0, al_get_bitmap_width(display_background[0]), al_get_bitmap_height(display_background[0]), 
        0, 0, al_get_display_width(display), al_get_display_height(display), 0);
    if(pause){
        al_draw_text(font[1], al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 5, ALLEGRO_ALIGN_CENTER, "PAUSE MENU");
    }
    else{
        al_draw_scaled_bitmap(display_background[1], 0, 0, al_get_bitmap_width(display_background[1]), al_get_bitmap_height(display_background[1]), 
            0, 0, al_get_display_width(display), al_get_display_height(display) / 3, 0);
    }
    /********************************************************/
    
    if(!flag){
        al_play_sample(samples[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL); 
    }
    create_button_unpressed(str0, str1, str2);
    al_flip_display();
    
    while(!do_exit){ 
        ALLEGRO_EVENT ev;
        if(al_get_next_event(event_queue, &ev)){ //Toma un evento de la cola.
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                do_exit=true;
                aux=CLOSE_DISPLAY;
            }
            else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES){
                mouse_x = ev.mouse.x;
                mouse_y = ev.mouse.y;
            }
            else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                check=true;
            }
        }
        if(mouse_x >= SCREEN_W/4 && mouse_x <= 3*SCREEN_W/4){
            if(mouse_y >= 7*SCREEN_H/16 && mouse_y <= 9*SCREEN_H/16){
                if(check){
                    do_exit=true;
                    aux=1;  //primer botón
                }
                else{
                    create_button_pressed(1, str0, str1, str2);
                }
            }
            else if(mouse_y >= 5*SCREEN_H/8 && mouse_y <= 3*SCREEN_H/4){
                if(check){
                    do_exit=true;
                    aux=2;  //segundo botón
                }
                else{
                    create_button_pressed(2, str0, str1, str2);
                }
            }
            else if (mouse_y >= 13*SCREEN_H/16 && mouse_y <= 15*SCREEN_H/16){
                if(check){
                    do_exit=true;
                    aux=3;  //tercer botón
                }
                else{
                    create_button_pressed(3, str0, str1, str2);
                }
            }
            else{
                redraw=true;
            }
        }
        else{
            redraw=true;
        }
        if(redraw){
            create_button_unpressed(str0, str1, str2);
            redraw=false;
        }
        check=false;
    }
    return aux;
}

static int8_t Difficulty(char *str0, char *str1, char *str2){
    
    FILE* fp=fopen(".Difficulty.txt", "w");  //Creo el archivo difficulty en donde guardo el nivel de dificultad.
    
    if(!fp){
        return FATAL_ERROR;
    }

    int8_t aux = menu_display(str0, str1, str2, 1, 0);
    switch(aux){
        case CLOSE_DISPLAY:{
            break;
        }
        case 1:{
            fputs(EASY_CODE, fp);
            break;
        }
        case 2:{
            fputs(NORMAL_CODE, fp);
            break;
        }
        case 3:{
            fputs(HARD_CODE, fp);
            break;
        }
        default:{
            aux=FATAL_ERROR;
            break;
        }
    }

    fclose(fp);
    return aux;    
}

static int8_t Top_Score(void){
    
    uint8_t do_exit=false, check=false, redraw=false;
    int8_t aux=0;
    float mouse_x=0, mouse_y=0;
    
    al_draw_scaled_bitmap(display_background[0],0, 0, al_get_bitmap_width(display_background[0]), al_get_bitmap_height(display_background[0]), 
            0, 0, al_get_display_width(display), al_get_display_height(display), 0);

    al_draw_text(font[1], al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/6, ALLEGRO_ALIGN_CENTER, "TOP SCORE");
    
    create_button_unpressed_top_score();
    create_table_top_score();
    print_top_score();
    al_flip_display();
    
    while(!do_exit){ 
        ALLEGRO_EVENT ev;
        if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola
        {
            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                do_exit=true;
                aux=CLOSE_DISPLAY;
            }
            else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES){
                mouse_x = ev.mouse.x;
                mouse_y = ev.mouse.y;
            }
            else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                check=true;
           }
        }
        if(mouse_x>=13*SCREEN_W/16.0 && mouse_x<= 15*SCREEN_W/16.0){
            if(mouse_y>=13*SCREEN_H/16 && mouse_y<= 15*SCREEN_H/16){
                if(check){
                    do_exit=true;
                }
                else{
                    create_button_pressed_top_score();
                }
         
            }
            else{
                redraw=true;
            }
        }
        else{
            redraw=true;
        }
        if(redraw){
            create_button_unpressed_top_score();
            redraw=false;
        }
        check=false;
    }
    return aux;
}

static void print_top_score(void){
    
/* Cuando guardemos algo en el archivo de top score hay que guardarlo de la siguiente manera
 *                          score 
 *                          name
 *                          score 
 *                          name
 *                          ...
 * Si el nombre que elige la persona es menor a 5 letras, debe rellenar con espacios. 
 */

    uint8_t i;
    static FILE* fp;
    char str[STR_LONG];
    fp = fopen(".Top_Score.txt", "r"); // Con el punto se crea un archivo oculto.
    
    for(i=0; i<5; i++){
        //SCORE
        fgets(str,STR_LONG,fp);
        al_draw_text(font[0], al_map_rgb(255,255,255), 7*SCREEN_W/16, (21+4*i)*SCREEN_H/48, ALLEGRO_ALIGN_CENTER, str);
        fgetc(fp);  // "aumento" el fp a la siguiente linea 
        //NAME
        fgets(str,STR_LONG,fp);
        al_draw_text(font[0], al_map_rgb(255,255,255), 31*SCREEN_W/48, (21+4*i)*SCREEN_H/48, ALLEGRO_ALIGN_CENTER, str);
        fgetc(fp);
    }
}

static void create_button_unpressed(char *str0, char *str1, char *str2){
    
        al_draw_filled_rectangle(SCREEN_W/4, 7*SCREEN_H/16, 3*SCREEN_W/4, 9*SCREEN_H/16, al_color_name("black"));
        al_draw_rectangle(SCREEN_W/4, 7*SCREEN_H/16, 3*SCREEN_W/4, 9*SCREEN_H/16, al_color_name("white"),2.0);
        al_draw_text(font[0], al_map_rgb(255, 255, 255), SCREEN_W / 2, (12*SCREEN_H /25), ALLEGRO_ALIGN_CENTER, str0);
        
        al_draw_filled_rectangle(SCREEN_W/4, 5*SCREEN_H/8, 3*SCREEN_W/4, 3*SCREEN_H/4, al_color_name("black"));
        al_draw_rectangle(SCREEN_W/4, 5*SCREEN_H/8, 3*SCREEN_W/4, 3*SCREEN_H/4, al_color_name("white"),2.0);
        al_draw_text(font[0], al_map_rgb(255, 255, 255), SCREEN_W / 2, (267*SCREEN_H / 400), ALLEGRO_ALIGN_CENTER, str1);
        
        al_draw_filled_rectangle(SCREEN_W/4, 13*SCREEN_H/16, 3*SCREEN_W/4, 15*SCREEN_H/16, al_color_name("black"));
        al_draw_rectangle(SCREEN_W/4, 13*SCREEN_H/16, 3*SCREEN_W/4, 15*SCREEN_H/16, al_color_name("white"),2.0);
        al_draw_text(font[0], al_map_rgb(255, 255, 255), SCREEN_W / 2, (171*SCREEN_H / 200), ALLEGRO_ALIGN_CENTER, str2);
        
        al_flip_display();
    
}

static void create_button_pressed(uint8_t button, char *str0, char *str1, char *str2){
    switch(button){
        case 1:{
            al_draw_filled_rectangle(SCREEN_W/4, 7*SCREEN_H/16, 3*SCREEN_W/4, 9*SCREEN_H/16, al_color_name("grey"));
            al_draw_text(font[0], al_map_rgb(0,0,0), SCREEN_W / 2, (12*SCREEN_H /25), ALLEGRO_ALIGN_CENTER, str0);
            break;
        }
        case 2:{
            al_draw_filled_rectangle(SCREEN_W/4, 5*SCREEN_H/8, 3*SCREEN_W/4, 3*SCREEN_H/4, al_color_name("grey"));
            al_draw_text(font[0], al_map_rgb(0,0,0), SCREEN_W / 2, (267*SCREEN_H / 400), ALLEGRO_ALIGN_CENTER, str1);
            break;
        }
        case 3:{
            al_draw_filled_rectangle(SCREEN_W/4, 13*SCREEN_H/16, 3*SCREEN_W/4, 15*SCREEN_H/16, al_color_name("grey"));
            al_draw_text(font[0], al_map_rgb(0,0,0), SCREEN_W / 2, (171*SCREEN_H / 200), ALLEGRO_ALIGN_CENTER, str2);
            break;
        }
    }
    al_flip_display();
}

static void create_button_unpressed_top_score(void){
    al_draw_filled_rectangle(13*SCREEN_W/16, 13*SCREEN_H/16, 15*SCREEN_W/16, 15*SCREEN_H/16, al_color_name("black"));
    al_draw_rectangle(13*SCREEN_W/16, 13*SCREEN_H/16, 15*SCREEN_W/16, 15*SCREEN_H/16, al_color_name("white"),2.0);
    al_draw_text(font[0], al_map_rgb(255, 255, 255), 7*SCREEN_W / 8, (171*SCREEN_H / 200), ALLEGRO_ALIGN_CENTER, "EXIT");
    al_flip_display();
}

static void create_button_pressed_top_score(void){
    al_draw_filled_rectangle(13*SCREEN_W/16, 13*SCREEN_H/16, 15*SCREEN_W/16, 15*SCREEN_H/16, al_color_name("grey"));
    al_draw_text(font[0], al_map_rgb(255, 255, 255), 7*SCREEN_W / 8, (171*SCREEN_H / 200), ALLEGRO_ALIGN_CENTER, "EXIT");
    al_flip_display();
}

static void create_table_top_score(void){
    //RECTANGULO BASE
    al_draw_filled_rectangle(SCREEN_W/4, SCREEN_H/3, 3*SCREEN_W/4, 5*SCREEN_H/6, al_color_name("black"));
    al_draw_rectangle(SCREEN_W/4, SCREEN_H/3, 3*SCREEN_W/4, 5*SCREEN_H/6, al_color_name("white"),2.0);
    
    //LINEAS VERTICALES 
    al_draw_line(13*SCREEN_W/24, SCREEN_H/3,13*SCREEN_W/24, 5*SCREEN_H/6, al_color_name("white"),2.0);
    al_draw_line(SCREEN_W/3, SCREEN_H/3,SCREEN_W/3, 5*SCREEN_H/6, al_color_name("white"),2.0);//vertical
    
    //SEPARADORES
    al_draw_rectangle(SCREEN_W/4, 5*SCREEN_H/12,3*SCREEN_W/4, SCREEN_H/2, al_color_name("white"),2.0);
    al_draw_rectangle(SCREEN_W/4, 7*SCREEN_H/12, 3*SCREEN_W/4, 2*SCREEN_H/3, al_color_name("white"),2.0);
    al_draw_rectangle(SCREEN_W/4, 3*SCREEN_H/4, 3*SCREEN_W/4, 5*SCREEN_H/6, al_color_name("white"),2.0);
    
    //TEXTO
    al_draw_text(font[0], al_map_rgb(255,255,255), 29*SCREEN_W / 96, (17*SCREEN_H /48), ALLEGRO_ALIGN_CENTER, "N°");
    al_draw_text(font[0], al_map_rgb(255,255,255), 29*SCREEN_W / 96, (7*SCREEN_H /16), ALLEGRO_ALIGN_CENTER, "1");
    al_draw_text(font[0], al_map_rgb(255,255,255), 29*SCREEN_W / 96, (25*SCREEN_H /48), ALLEGRO_ALIGN_CENTER, "2");
    al_draw_text(font[0], al_map_rgb(255,255,255), 29*SCREEN_W / 96, (29*SCREEN_H /48), ALLEGRO_ALIGN_CENTER, "3");
    al_draw_text(font[0], al_map_rgb(255,255,255), 29*SCREEN_W / 96, (11*SCREEN_H /16), ALLEGRO_ALIGN_CENTER, "4");
    al_draw_text(font[0], al_map_rgb(255,255,255), 29*SCREEN_W / 96, (37*SCREEN_H /48), ALLEGRO_ALIGN_CENTER, "5");
    al_draw_text(font[0], al_map_rgb(255,255,255), 7*SCREEN_W / 16, (17*SCREEN_H /48), ALLEGRO_ALIGN_CENTER, "SCORE");
    al_draw_text(font[0], al_map_rgb(255,255,255), 31*SCREEN_W / 48, (17*SCREEN_H /48), ALLEGRO_ALIGN_CENTER, "NAME");
    
    al_flip_display();
}

#endif //RASPBERRY

/****************************** END FILE ***************************************/