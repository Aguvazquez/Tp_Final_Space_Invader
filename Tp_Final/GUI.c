/***************************** Standard libraries ******************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************/

/******************************* Locals headers ********************************/

#include "config.h"
#include "GUI.h"
#include "disdrv.h"

/*******************************************************************************/

#ifndef RASPBERRY

/***************************** Allegro libraries *******************************/

#include <allegro5/allegro.h>  
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_ttf.h> 
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h> 

/*******************************************************************************/

/************************** Allegro global variables ***************************/

extern  ALLEGRO_DISPLAY *display;
extern  ALLEGRO_EVENT_QUEUE *event_queue;
extern  ALLEGRO_TIMER *timer;
extern  ALLEGRO_FONT *font[FONTS]; 
extern  ALLEGRO_SAMPLE *samples[SAMPLES]; 
extern  ALLEGRO_BITMAP *display_background[BACKGROUNDS];

/*******************************************************************************/

/************************* Header of local functions ***************************/

/*
 * @Brief crea los botones del menú (diseñado para 3 botones).
 * @Param1: oración del primer botón.
 * @Param2: oración del segundo botón.
 * @Param3: oración del tercer botón.
 * @Param4: color del botón.
 */

static void create_button(uint8_t button, char *str0, char *str1, char *str2, char *colour);

//Crea el botón de salida de top score del color requerido

static void create_button_top_score(char *colour);

//Crea la tabla del top score.

static void create_table_top_score(void);

//Escribe el puntaje del jugador en pantalla en todo momento de la partida.

static void score_to_str(uint32_t score);

/*******************************************************************************/

/******************************* Global fuctions *******************************/

int8_t menu_display(char *str0, char *str1, char *str2, char flag, uint8_t pause){
    
    bool do_exit=false, check=false, redraw=false;
    int8_t aux=0, i;
    ALLEGRO_EVENT ev;
    
    //Mouse axes
    float mouse_x=0, mouse_y=0;
    
    //Title and bakground menu
    al_draw_scaled_bitmap(display_background[0], 0, 0, al_get_bitmap_width(display_background[0]),
                        al_get_bitmap_height(display_background[0]), 0, 0, SCREEN_W, SCREEN_H, 0);
    if(pause){
        al_draw_text(font[1], al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/5, ALLEGRO_ALIGN_CENTER, "PAUSE MENU");
    }
    else{
        al_draw_scaled_bitmap(display_background[1], 0, 0, al_get_bitmap_width(display_background[1]),
                            al_get_bitmap_height(display_background[1]), 0, 0, SCREEN_W, SCREEN_H/3, 0);
    }
    
    if(!flag){
        al_play_sample(samples[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL); 
    }
    for(i=1; i<=3; i++){
        create_button(i, str0, str1, str2, "black");
    }
    al_flip_display();
    
    while(!do_exit){ 
        if(al_get_next_event(event_queue, &ev)){
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
                    create_button(1, str0, str1, str2, "grey");
                }
            }
            else if(mouse_y >= 5*SCREEN_H/8 && mouse_y <= 3*SCREEN_H/4){
                if(check){
                    do_exit=true;
                    aux=2;  //segundo botón
                }
                else{
                    create_button(2, str0, str1, str2, "grey");
                }
            }
            else if (mouse_y >= 13*SCREEN_H/16 && mouse_y <= 15*SCREEN_H/16){
                if(check){
                    do_exit=true;
                    aux=3;  //tercer botón
                }
                else{
                    create_button(3, str0, str1, str2, "grey");
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
            for(i=1; i<=3; i++){
                create_button(i, str0, str1, str2, "black");
            }
            redraw=false;
        }
        check=false;
    }
    return aux;
}

/******************************* Local fuctions ********************************/

 int8_t Top_Score(void){
    
    uint8_t do_exit=false, check=false, redraw=false;
    int8_t aux=0;
    ALLEGRO_EVENT ev;
    float mouse_x=0, mouse_y=0;
    
    al_draw_scaled_bitmap(display_background[0], 0, 0, al_get_bitmap_width(display_background[0]), 
                        al_get_bitmap_height(display_background[0]), 0, 0, SCREEN_W, SCREEN_H, 0);

    al_draw_text(font[1], al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/6, ALLEGRO_ALIGN_CENTER, "TOP SCORE");
    
    create_button_top_score("black");
    create_table_top_score();
    print_top_score();
    al_flip_display();
    
    while(!do_exit){ 
        if (al_get_next_event(event_queue, &ev)){
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
                    create_button_top_score("grey");
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
            create_button_top_score("black");
            redraw=false;
        }
        check=false;
    }
    return aux;
}

static void create_button(uint8_t button, char *str0, char *str1, char *str2, char *colour){
    
    switch(button){
        case 1:
        {
            al_draw_filled_rectangle(SCREEN_W/4, 7*SCREEN_H/16, 3*SCREEN_W/4, 9*SCREEN_H/16, al_color_name(colour));
            al_draw_rectangle(SCREEN_W/4, 7*SCREEN_H/16, 3*SCREEN_W/4, 9*SCREEN_H/16, al_color_name("white"), 2.0);
            al_draw_text(font[0], al_map_rgb(255, 255, 255), SCREEN_W/2, 12*SCREEN_H/25, ALLEGRO_ALIGN_CENTER, str0);
            break;
        }
        case 2:
        {
            al_draw_filled_rectangle(SCREEN_W/4, 5*SCREEN_H/8, 3*SCREEN_W/4, 3*SCREEN_H/4, al_color_name(colour));
            al_draw_rectangle(SCREEN_W/4, 5*SCREEN_H/8, 3*SCREEN_W/4, 3*SCREEN_H/4, al_color_name("white"), 2.0);
            al_draw_text(font[0], al_map_rgb(255, 255, 255), SCREEN_W/2, 267*SCREEN_H/400, ALLEGRO_ALIGN_CENTER, str1);
            break;
        }
        case 3:
        {
            al_draw_filled_rectangle(SCREEN_W/4, 13*SCREEN_H/16, 3*SCREEN_W/4, 15*SCREEN_H/16, al_color_name(colour));
            al_draw_rectangle(SCREEN_W/4, 13*SCREEN_H/16, 3*SCREEN_W/4, 15*SCREEN_H/16, al_color_name("white"), 2.0);
            al_draw_text(font[0], al_map_rgb(255, 255, 255), SCREEN_W/2, 171*SCREEN_H/200, ALLEGRO_ALIGN_CENTER, str2);
            break;
        }
    }
    
    al_flip_display();
    
}

static void create_button_top_score(char *colour){

    al_draw_filled_rectangle(13*SCREEN_W/16, 13*SCREEN_H/16, 15*SCREEN_W/16, 15*SCREEN_H/16, al_color_name(colour));
    al_draw_rectangle(13*SCREEN_W/16, 13*SCREEN_H/16, 15*SCREEN_W/16, 15*SCREEN_H/16, al_color_name("white"), 2.0);
    al_draw_text(font[0], al_map_rgb(255, 255, 255), 7*SCREEN_W/8, 171*SCREEN_H/200, ALLEGRO_ALIGN_CENTER, "EXIT");
    al_flip_display();

}

static void create_table_top_score(void){
    //RECTANGULO BASE
    al_draw_filled_rectangle(SCREEN_W/4, SCREEN_H/3, 3*SCREEN_W/4, 5*SCREEN_H/6, al_color_name("black"));
    al_draw_rectangle(SCREEN_W/4, SCREEN_H/3, 3*SCREEN_W/4, 5*SCREEN_H/6, al_color_name("white"), 2.0);
    
    //LINEAS VERTICALES 
    al_draw_line(13*SCREEN_W/24, SCREEN_H/3,13*SCREEN_W/24, 5*SCREEN_H/6, al_color_name("white"), 2.0);
    al_draw_line(SCREEN_W/3, SCREEN_H/3,SCREEN_W/3, 5*SCREEN_H/6, al_color_name("white"), 2.0);//vertical
    
    //SEPARADORES
    al_draw_rectangle(SCREEN_W/4, 5*SCREEN_H/12,3*SCREEN_W/4, SCREEN_H/2, al_color_name("white"), 2.0);
    al_draw_rectangle(SCREEN_W/4, 7*SCREEN_H/12, 3*SCREEN_W/4, 2*SCREEN_H/3, al_color_name("white"), 2.0);
    al_draw_rectangle(SCREEN_W/4, 3*SCREEN_H/4, 3*SCREEN_W/4, 5*SCREEN_H/6, al_color_name("white"), 2.0);
    
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

void draw_world(uint8_t level, uint32_t score, uint8_t lives, uint8_t alien_change, elements_t nave_x, elements_t* bloques_x,
                uint8_t* vida_bloques, elements_t* alien_x, elements_t* alien_y, elements_t* alien_bullets_x, 
                elements_t* alien_bullets_y, elements_t bullet_x, elements_t bullet_y, elements_t mystery_ship_x, 
                elements_t explosion_x, elements_t explosion_y, uint8_t* explosion_time) {
    int8_t i;
    
    //fondo
    al_draw_scaled_bitmap(display_background[11 + level%5], 0, 0, al_get_bitmap_width(display_background[11 + level%5]),
            al_get_bitmap_height(display_background[11 + level%5]), 0, 0, SCREEN_W, SCREEN_H, 0);
    //nave
    al_draw_scaled_bitmap(display_background[2], 0, 0, al_get_bitmap_width(display_background[2]),
            al_get_bitmap_height(display_background[2]), nave_x, NAVE_Y, 3*BASE_SIZE, 1.5*BASE_SIZE, 0);
    //vidas
    for (i = 0; i < (lives); i++) {
        al_draw_scaled_bitmap(display_background[5], 0, 0, al_get_bitmap_width(display_background[5]),
                al_get_bitmap_height(display_background[5]), 1.5 * i*BASE_SIZE, 0, 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
    }
    //score
    score_to_str(score);
    //bloques
    for (i = 0; i < CANT_BLOQUES; i++) {
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
    }
    for (i = 0; i < CANT_ALIENS; i++) {
        //aliens
        if (alien_y[i] < SCREEN_H) {
            if (alien_change) {
                if (i < (CANT_ALIENS/5)) {
                    al_draw_scaled_bitmap(display_background[8], 0, 0, al_get_bitmap_width(display_background[8]),
                            al_get_bitmap_height(display_background[8]), alien_x[i], alien_y[i], 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
                }
                else if (i < (3 * CANT_ALIENS / 5)) {
                    al_draw_scaled_bitmap(display_background[3], 0, 0, al_get_bitmap_width(display_background[3]),
                            al_get_bitmap_height(display_background[3]), alien_x[i], alien_y[i], 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
                }
                else {
                    al_draw_scaled_bitmap(display_background[9], 0, 0, al_get_bitmap_width(display_background[9]),
                            al_get_bitmap_height(display_background[9]), alien_x[i], alien_y[i], 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
                }
            }
            else {
                if (i <= (CANT_ALIENS / 5)) {
                    al_draw_scaled_bitmap(display_background[7], 0, 0, al_get_bitmap_width(display_background[7]),
                            al_get_bitmap_height(display_background[7]), alien_x[i], alien_y[i], 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
                }
                else if (i < (3 * CANT_ALIENS / 5)) {
                    al_draw_scaled_bitmap(display_background[4], 0, 0, al_get_bitmap_width(display_background[4]),
                            al_get_bitmap_height(display_background[4]), alien_x[i], alien_y[i], 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
                }
                else {
                    al_draw_scaled_bitmap(display_background[10], 0, 0, al_get_bitmap_width(display_background[10]),
                            al_get_bitmap_height(display_background[10]), alien_x[i], alien_y[i], 2.5 * BASE_SIZE, 2.5 * BASE_SIZE, 0);
                }
            }
        }
       //balas de aliens
        al_draw_rectangle(alien_bullets_x[i] - BULLET_W/2, alien_bullets_y[i], alien_bullets_x[i] + BULLET_W/2,
                alien_bullets_y[i] + BASE_SIZE, al_map_rgb(255, 255, 255), 0); 
    }
    //nave misteriosa
    al_draw_scaled_bitmap(display_background[6], 0, 0, al_get_bitmap_width(display_background[6]),
            al_get_bitmap_height(display_background[6]), mystery_ship_x, MYSTERY_SHIP_Y, 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
    //explosiones
    if(*explosion_time){
                al_draw_scaled_bitmap(display_background[16], 0, 0, al_get_bitmap_width(display_background[16]), 
                    al_get_bitmap_height(display_background[16]), explosion_x, explosion_y, 2.5*BASE_SIZE, 2.5*BASE_SIZE, 0);
                (*explosion_time)--;
            }
    //bala
    if(bullet_y<NAVE_Y){
        al_draw_rectangle(bullet_x-BULLET_W/2, bullet_y, bullet_x+BULLET_W/2, bullet_y+BASE_SIZE, al_map_rgb(255, 0, 0), 0);
    }
    al_flip_display();
}

static void score_to_str(uint32_t score) {
    char str[] = {'S', 'C', 'O', 'R', 'E', ':', ' ', ' ', '0', '0', '0', '0', '0'};
    uint8_t i;
    uint32_t aux = 0, j;
    for (i = 12, j = 1; i > 7; i--, j *= 10) {
        aux = score / j;
        str[i] = (char) (aux % 10 + ASCII);
    }
    al_draw_text(font[0], al_map_rgb(255, 255, 255), SCREEN_W, BASE_SIZE / 4, ALLEGRO_ALIGN_RIGHT, str);
}
void next_level_animation(uint8_t level){

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

void lose_animation(uint32_t score){

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


#else

void show_on_terminal(uint8_t lives, uint32_t score){

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

void draw_world_rpi(elements_t nave_x, elements_t* bloques_x, uint8_t* vida_bloques, elements_t* alien_x, 
                    elements_t* alien_y, elements_t* alien_bullets_x, elements_t* alien_bullets_y, 
                    elements_t bullet_x, elements_t bullet_y, elements_t mystery_ship_x) {
    
    dcoord_t coord_nave, coord_bloques, coord_alien, coord_mystery_ship, coord_bullet, coord_alien_bullet;
    int i;
    disp_clear();
    coord_nave.x = nave_x;
    coord_nave.y = NAVE_Y;
    coord_bloques.y = BLOQUES_Y;
    coord_mystery_ship.x = mystery_ship_x;
    coord_mystery_ship.y = MYSTERY_SHIP_Y;
    coord_bullet.x = bullet_x;
    coord_bullet.y = bullet_y;

    //nave
    for (i = 0; i < 4; i++) {
        if(i==3) {
            coord_nave.y--;
            coord_nave.x -= 2;
            disp_write(coord_nave, D_ON);
        }
        else {
            disp_write(coord_nave, D_ON);
            coord_nave.x++;
        }
    }
    //bloques
    for (i = 0; i < CANT_BLOQUES; i++) {
        if (vida_bloques[i] != 0) {
            coord_bloques.x = bloques_x[i];
            disp_write(coord_bloques, D_ON);
            coord_bloques.x++;
            disp_write(coord_bloques, D_ON);
        }
    }
    for (i = 0; i < CANT_ALIENS; i++) {
        //aliens
        coord_alien.x = alien_x[i];
        coord_alien.y = alien_y[i];
        if (coord_alien.y < SCREEN_H) {
            disp_write(coord_alien, D_ON);
        }
        //balas de aliens
        coord_alien_bullet.x = alien_bullets_x[i];
        coord_alien_bullet.y = alien_bullets_y[i];
        if (coord_alien_bullet.y < SCREEN_H && coord_alien_bullet.x < SCREEN_W) {
            disp_write(coord_alien_bullet, D_ON);
        }
    }
    //nave misteriosa
    if (coord_mystery_ship.x < SCREEN_W) {
        disp_write(coord_mystery_ship, D_ON);
    }
    //bala
    if (coord_bullet.y < SCREEN_H && coord_bullet.x < SCREEN_H) {
        disp_write(coord_bullet, D_ON);
    }
}

#endif //RASPBERRY

void new_player_in_top(char name[STR_LONG]){
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
    for(i=0;i<STR_LONG;i++){
        c=getchar();
        if(c==127){ // 127 es el ASCII de delete
           if (i) { //si no es la primera letra
                name[--i] = ' '; //borra la letra anterior
            } else {
                name[i] = ' '; //sino borra la primera
            }
        }
        else if(c=='\n'){
            break; // se termina el algoritmo, pues el usuario decide mander ese nombre.
        }
            name[i]=c;
    }
#endif
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

/****************************** END FILE ***************************************/