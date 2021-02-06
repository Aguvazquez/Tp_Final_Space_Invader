/************************* Standard libraries **********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************/

/************************* Locals headers **************************************/

#include "config.h"
#include "GUI.h"
#include "Top_Score.h"
#include "play.h"
#include "allegro_setup.h"
#include "termlib.h"
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



//Crea el botón de salida de top score sin presionar.

static void create_button_unpressed_top_score(void);

//Crea el botón de salida de top score presionado.

static void create_button_pressed_top_score(void);

//Crea la tabla del top score.

static void create_table_top_score(void);



/*******************************************************************************/

/*************************** Global fuctions ***********************************/

int8_t menu_display(char *str0, char *str1, char *str2, char flag, uint8_t pause){
    
    bool do_exit=false, check=false, redraw=false;
    int8_t aux=0;
    
    /***********Mouse axes**********/
    float mouse_x=0, mouse_y=0;
    
    /**********Title and bakground menu.*********************/
    al_draw_scaled_bitmap(display_background[0], 0, 0, al_get_bitmap_width(display_background[0]), al_get_bitmap_height(display_background[0]),
        0, 0, al_get_display_width(display), al_get_display_height(display), 0);
    if(pause){
        al_draw_text(font[1], al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/5, ALLEGRO_ALIGN_CENTER, "PAUSE MENU");
    }
    else{
        al_draw_scaled_bitmap(display_background[1], 0, 0, al_get_bitmap_width(display_background[1]), al_get_bitmap_height(display_background[1]), 
            0, 0, al_get_display_width(display), al_get_display_height(display)/3, 0);
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

/****************************Local fuctions************************************/



 int8_t Top_Score(void){
    
    uint8_t do_exit=false, check=false, redraw=false;
    int8_t aux=0;
    float mouse_x=0, mouse_y=0;
    
    al_draw_scaled_bitmap(display_background[0], 0, 0, al_get_bitmap_width(display_background[0]), al_get_bitmap_height(display_background[0]),
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
        if(mouse_x>=13*SCREEN_W/16 && mouse_x<=15*SCREEN_W/16){
            if(mouse_y>=13*SCREEN_H/16 && mouse_y<=15*SCREEN_H/16){
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
            al_draw_text(font[0], al_map_rgb(0,0,0), SCREEN_W/2, (12*SCREEN_H /25), ALLEGRO_ALIGN_CENTER, str0);
            break;
        }
        case 2:{
            al_draw_filled_rectangle(SCREEN_W/4, 5*SCREEN_H/8, 3*SCREEN_W/4, 3*SCREEN_H/4, al_color_name("grey"));
            al_draw_text(font[0], al_map_rgb(0,0,0), SCREEN_W/2, (267*SCREEN_H/400), ALLEGRO_ALIGN_CENTER, str1);
            break;
        }
        case 3:{
            al_draw_filled_rectangle(SCREEN_W/4, 13*SCREEN_H/16, 3*SCREEN_W/4, 15*SCREEN_H/16, al_color_name("grey"));
            al_draw_text(font[0], al_map_rgb(0,0,0), SCREEN_W/2, (171*SCREEN_H/200), ALLEGRO_ALIGN_CENTER, str2);
            break;
        }
    }
    al_flip_display();
}

static void create_button_unpressed_top_score(void){
    al_draw_filled_rectangle(13*SCREEN_W/16, 13*SCREEN_H/16, 15*SCREEN_W/16, 15*SCREEN_H/16, al_color_name("black"));
    al_draw_rectangle(13*SCREEN_W/16, 13*SCREEN_H/16, 15*SCREEN_W/16, 15*SCREEN_H/16, al_color_name("white"), 2.0);
    al_draw_text(font[0], al_map_rgb(255, 255, 255), 7*SCREEN_W / 8, (171*SCREEN_H/200), ALLEGRO_ALIGN_CENTER, "EXIT");
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
void draw_world(uint8_t level,uint8_t lives,uint8_t alien_change,
                  elements_t nave_x,elements_t* bloques_x,uint8_t* vida_bloques ,
                  elements_t* alien_x,elements_t* alien_y,elements_t* alien_bullets_x,
                  elements_t* alien_bullets_y, elements_t bullet_x,elements_t bullet_y,elements_t mystery_ship_x,elements_t explosion_x,elements_t explosion_y,uint8_t* explosion_time) {
    int i,j;
    al_draw_scaled_bitmap(display_background[11 + level % 5], 0, 0, al_get_bitmap_width(display_background[11 + level % 5]),
            al_get_bitmap_height(display_background[11 + level % 5]), 0, 0, SCREEN_W, SCREEN_H, 0);
    al_draw_scaled_bitmap(display_background[2], 0, 0, al_get_bitmap_width(display_background[2]),
            al_get_bitmap_height(display_background[2]), nave_x, NAVE_Y, 3 * BASE_SIZE, 1.5 * BASE_SIZE, 0);

    for (i = 0; i < (lives); i++) {
        al_draw_scaled_bitmap(display_background[5], 0, 0, al_get_bitmap_width(display_background[5]),
                al_get_bitmap_height(display_background[5]), 1.5 * i*BASE_SIZE, 0, 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
    }
    for (i = 0; i < 4; i++)
        if (vida_bloques[i] >= 20) {
            al_draw_filled_rectangle(bloques_x[i], BLOQUES_Y, bloques_x[i] + 4 * BASE_SIZE, BLOQUES_Y + BASE_SIZE, al_map_rgb(0, 255, 0));
            al_draw_filled_rectangle(bloques_x[i], BLOQUES_Y + BASE_SIZE, bloques_x[i] + BASE_SIZE, BLOQUES_Y + 2 * BASE_SIZE, al_map_rgb(0, 255, 0));
            al_draw_filled_rectangle(bloques_x[i] + 3 * BASE_SIZE, BLOQUES_Y + BASE_SIZE, bloques_x[i] + 4 * BASE_SIZE, BLOQUES_Y + 2 * BASE_SIZE, al_map_rgb(0, 255, 0));
        }
        else if (vida_bloques[i] >= 10) {
            al_draw_filled_rectangle(bloques_x[i], BLOQUES_Y, bloques_x[i] + 4 * BASE_SIZE, BLOQUES_Y + BASE_SIZE, al_map_rgb(255, 255, 0));
            al_draw_filled_rectangle(bloques_x[i], BLOQUES_Y + BASE_SIZE, bloques_x[i] + BASE_SIZE, BLOQUES_Y + 2 * BASE_SIZE, al_map_rgb(255, 255, 0));
            al_draw_filled_rectangle(bloques_x[i] + 3 * BASE_SIZE, BLOQUES_Y + BASE_SIZE, bloques_x[i] + 4 * BASE_SIZE, BLOQUES_Y + 2 * BASE_SIZE, al_map_rgb(255, 255, 0));
        }
        else if (vida_bloques[i]) {
            al_draw_filled_rectangle(bloques_x[i], BLOQUES_Y, bloques_x[i] + 4 * BASE_SIZE, BLOQUES_Y + BASE_SIZE, al_map_rgb(255, 0, 0));
            al_draw_filled_rectangle(bloques_x[i], BLOQUES_Y + BASE_SIZE, bloques_x[i] + BASE_SIZE, BLOQUES_Y + 2 * BASE_SIZE, al_map_rgb(255, 0, 0));
            al_draw_filled_rectangle(bloques_x[i] + 3 * BASE_SIZE, BLOQUES_Y + BASE_SIZE, bloques_x[i] + 4 * BASE_SIZE, BLOQUES_Y + 2 * BASE_SIZE, al_map_rgb(255, 0, 0));
        }

    for (i = 0, j = 0; i < CANT_ALIENS; i++) {
        if (alien_y[i] < SCREEN_H) {
            if (alien_change) {
                if (i <= ((CANT_ALIENS / 5) - 1)) {
                    al_draw_scaled_bitmap(display_background[8], 0, 0, al_get_bitmap_width(display_background[8]),
                            al_get_bitmap_height(display_background[8]), alien_x[i], alien_y[i], 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
                }
                else if (i <= ((3 * CANT_ALIENS / 5) - 1)) {
                    al_draw_scaled_bitmap(display_background[3], 0, 0, al_get_bitmap_width(display_background[3]),
                            al_get_bitmap_height(display_background[3]), alien_x[i], alien_y[i], 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
                }
                else {
                    al_draw_scaled_bitmap(display_background[9], 0, 0, al_get_bitmap_width(display_background[9]),
                            al_get_bitmap_height(display_background[9]), alien_x[i], alien_y[i], 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
                }
            }
            else {
                if (i <= ((CANT_ALIENS / 5) - 1)) {
                    al_draw_scaled_bitmap(display_background[7], 0, 0, al_get_bitmap_width(display_background[7]),
                            al_get_bitmap_height(display_background[7]), alien_x[i], alien_y[i], 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
                }
                else if (i <= ((3 * CANT_ALIENS / 5) - 1)) {
                    al_draw_scaled_bitmap(display_background[4], 0, 0, al_get_bitmap_width(display_background[4]),
                            al_get_bitmap_height(display_background[4]), alien_x[i], alien_y[i], 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
                }
                else {
                    al_draw_scaled_bitmap(display_background[10], 0, 0, al_get_bitmap_width(display_background[10]),
                            al_get_bitmap_height(display_background[10]), alien_x[i], alien_y[i], 2.5 * BASE_SIZE, 2.5 * BASE_SIZE, 0);
                }
            }
        }
        if (alien_x[i] >= SCREEN_W - 2 * BASE_SIZE || alien_x[i] <= BASE_SIZE) { //revisa que no sobrepasen los extremos
            j++;
        }
        al_draw_rectangle(alien_bullets_x[i] - 1, alien_bullets_y[i], alien_bullets_x[i] + 1,
                alien_bullets_y[i] + BASE_SIZE, al_map_rgb(255, 255, 255), 0); //dibujo balas de aliens

        
    }

    al_draw_scaled_bitmap(display_background[6], 0, 0, al_get_bitmap_width(display_background[6]),
            al_get_bitmap_height(display_background[6]), mystery_ship_x, MYSTERY_SHIP_Y, 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
    
    if(*explosion_time){
                al_draw_scaled_bitmap(display_background[16], 0, 0, al_get_bitmap_width(display_background[16]), 
                    al_get_bitmap_height(display_background[16]), explosion_x, explosion_y, 2.5*BASE_SIZE, 2.5*BASE_SIZE, 0);
                (*explosion_time)--;
            }
    if(bullet_y<NAVE_Y){
        al_draw_rectangle(bullet_x-1, bullet_y, bullet_x+1, bullet_y+BASE_SIZE, al_map_rgb(255, 0, 0), 0);
    }
    al_flip_display();
}
#endif //RASPBERRY

 int8_t switch_difficulty(uint8_t option){

    FILE* fp=fopen(".Difficulty.txt", "w");  //Creo el archivo difficulty en donde guardo el nivel de dificultad.
    int8_t aux=EXIT_SUCCESS;
    
    if(!fp){
        return FATAL_ERROR;
    }
    switch(option){
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
            fprintf(stderr, "Dificultad mal configurada.\n");
            aux=FATAL_ERROR;
            break;
        }
    }

    fclose(fp);
    return aux;    
}

 void print_top_score(void){
    
    uint8_t i;
    static FILE* fp;
    char str[STR_LONG];
    fp = fopen(".Top_Score.txt", "r"); // Con el punto se crea un archivo oculto.
    
    for(i=0; i<5; i++){
        //SCORE
        fgets(str,STR_LONG,fp);
        
#ifndef RASPBERRY
        al_draw_text(font[0], al_map_rgb(255,255,255), 7*SCREEN_W/16, (21+4*i)*SCREEN_H/48, ALLEGRO_ALIGN_CENTER, str);
#else
        fprintf(stderr,"%d: %s",(i+1),str);
#endif //RASPBERRY
        
        fgetc(fp);  // "aumento" el fp a la siguiente linea 

        //NAME
        fgets(str,STR_LONG,fp);

#ifndef RASPBERRY
        al_draw_text(font[0], al_map_rgb(255,255,255), 31*SCREEN_W/48, (21+4*i)*SCREEN_H/48, ALLEGRO_ALIGN_CENTER, str);
#else
        fprintf(stderr,"    %s\n",str);
#endif //RASPBERRY

        fgetc(fp);
    }
}

void show_on_terminal(uint8_t lives, uint32_t score){
    uint8_t i;
    system("clear");
    fprintf(stderr, "******************************************");   //top side
    fprintf(stderr, "*\n*\n*\n");
    fprintf(stderr, "*       Score: %d\n", score);
    fprintf(stderr, "*\n");
    fprintf(stderr, "*       Lives: %d\n", lives);
    fprintf(stderr, "*\n*\n*\n");
    fprintf(stderr, "******************************************");   //bottom side
}


int pause_menu_terminal(void){
    uint8_t choice, c;
    int output=0;
    bool do_exit=false;
    system("clear");
    fprintf(stderr,"\t\tPAUSA.\n"); 
    
    while(!do_exit){
        fprintf(stderr, "Para continuar jugando pulse 1.\n");
        fprintf(stderr, "Para reiniciar la partida pulse 2.\n");
        fprintf(stderr, "Para salir del juego pulse 3.\n");
        while ((c = getchar()) != '\n') {
            choice = c;
        }
        if (choice == '1') {
            do_exit = true;          
        } else if (choice == '2') {
            output = RESET_GAME;
            do_exit = true;
        } else if (choice == '3') {
            output = EXIT_MENU;
            do_exit = true;
        } else {
            fprintf(stderr, "Por favor, introduzca un numero valido.\n");
        }
    }
    return output;
}
/****************************** END FILE ***************************************/