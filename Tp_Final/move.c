/************************* Standard libraries **********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

/*******************************************************************************/

/**************************** Local headers ************************************/

#include "config.h"
#include "move.h"
#include "GUI.h"
#include "joydrv.h"
#include "disdrv.h"
#include "termlib.h"
#include "libaudio.h"

/*******************************************************************************/

#ifndef RASPBERRY

/************************** Allegro libraries **********************************/

#include <allegro5/allegro.h>  
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_ttf.h> 
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h> 

/*******************************************************************************/

/*************************** Extern variables **********************************/

extern  ALLEGRO_DISPLAY *display;
extern  ALLEGRO_EVENT_QUEUE *event_queue;
extern  ALLEGRO_TIMER *timer;
extern  ALLEGRO_FONT *font[FONTS]; 
extern  ALLEGRO_SAMPLE *samples[SAMPLES];
extern  ALLEGRO_BITMAP *display_background[BACKGROUNDS];

/*******************************************************************************/

#endif

/*********************** Headers of local functions ****************************/

static void logical_move(bool* alien_change, bool* lock_mystery_ship, elements_t* alien_x, elements_t* alien_y, 
                        uint8_t* difficulty, elements_t* alien_bullets_x, elements_t* alien_bullets_y, 
                        uint8_t* accelerate, int8_t *step, int8_t cant_aliens);

static bool logical(bool* lock_mystery_ship, elements_t* mystery_ship_x, elements_t* alien_x, elements_t* alien_y,
                    elements_t* alien_bullets_x, elements_t* alien_bullets_y, int8_t* cant_aliens, elements_t* bullet_y,
                    elements_t* bullet_x, elements_t* explosion_x, elements_t* explosion_y, uint8_t* explosion_time, 
                    bool* lock, elements_t* nave_x, uint32_t* score, uint8_t* vida_bloques, elements_t* bloques_x, 
                    uint8_t* lives, uint8_t multiplier);

/*
 * @Brief recibe un entero positivo
 * @Return devuelve un entero positivo aleatorio menor a dicho numero
 */

static uint16_t get_rand_num(uint16_t x);

#ifndef RASPBERRY

/*
 * @Brief escribe el puntaje del jugador en pantalla en todo momento de la partida
 */

static void score_to_str(uint32_t* score);

#endif

/*******************************************************************************/

enum MYKEYS {LEFT, RIGHT, SPACE_UP, JOY_SWITCH};

uint8_t TimerTickRBP;
bool key_pressed[4] = {false, false, false, false};    //estado de teclas, true cuando esta apretada
bool event_change=false;

#ifdef RASPBERRY

void *Timer_rbp(){
    while(1){
        usleep(15000); //Para conseguir 60 FPS
        if(TimerTickRBP){
            TimerTickRBP--;
        }
    }
}

void *Joy_action(){
    jcoord_t coord;
    jswitch_t joy_switch;
    while(1){
        joy_update();
        coord=joy_get_coord();
        joy_switch=joy_get_switch();
        
        if(coord.x>=JOY_MAX_POS/2){
            key_pressed[RIGHT]=true;
            event_change=true;
        }
        else if(coord.x<=JOY_MAX_NEG/2){
            key_pressed[LEFT]=true;
            event_change=true;
        }
        else if(coord.y>=JOY_MAX_POS/2){
            key_pressed[SPACE_UP]=true;
            event_change=true;
        }
        else if(joy_switch){
           key_pressed[JOY_SWITCH]=true;
           event_change=true;
        }
        else{
            if(event_change){
                event_change=false;
                key_pressed[RIGHT]=false;
                key_pressed[LEFT]=false;
                key_pressed[SPACE_UP]=false;
                key_pressed[JOY_SWITCH]=false;
            }
        }
    }
}     

#endif

/******************************** Global functions *****************************/

int8_t move(uint8_t difficulty, uint8_t* lives, uint8_t level, uint32_t* score, uint8_t multiplier)
{
    //setup
    
    uint8_t i, j, aux, explosion_time=0, accelerate=0;
    static uint8_t vida_bloques[4] = {30, 30, 30, 30};
    int8_t cant_aliens=CANT_ALIENS, step;
    
    elements_t bullet_x=SCREEN_W, bullet_y, nave_x, bloques_x[4], explosion_x, explosion_y;
    elements_t alien_bullets_x[CANT_ALIENS], alien_bullets_y[CANT_ALIENS], mystery_ship_x;
    
    //bloques
    for(i=0; i<CANT_BLOQUES; i++){
        bloques_x[i] = PRIMER_BLOQUE + i*DISTANCIA_BLOQUES;
    }
    
    //nave
    nave_x = NAVE_X;
    
    //aliens
    elements_t alien_x[CANT_ALIENS];
    for(i=0; i<FILAS_ALIENS; i++){
        for(j=0; j<COLUMNAS_ALIENS; j++){
            alien_x[i*COLUMNAS_ALIENS + j] = PRIMERA_COLUMNA_ALIENS + j*DISTANCIA_ALIENS_X;
        }
    }
    
    elements_t alien_y[CANT_ALIENS];
    for(i=0; i<FILAS_ALIENS; i++){
        for(j=0; j<COLUMNAS_ALIENS; j++){
            alien_y[i*COLUMNAS_ALIENS + j] = PRIMERA_FILA_ALIENS + i*DISTANCIA_ALIENS_Y;
        }
    }
    
    mystery_ship_x=SCREEN_W; 
        
    for(i=0; i<CANT_ALIENS; i++)
    {
        alien_bullets_y[i] = SCREEN_H + BASE_SIZE;
    }
    
#ifdef RASPBERRY
    
    dcoord_t coord_nave, coord_bloques, coord_alien, coord_mystery_ship, coord_bullet, coord_alien_bullet;
    pthread_t Timer_RBP, Joy_Action;
    pthread_create(&Timer_RBP, NULL, Timer_rbp, NULL);
    pthread_create(&Joy_Action, NULL, Joy_action, NULL);

    int count=0;        //variable que regula el movimiento de nave misteriosa    
    step = BASE_SIZE;
    
#else
        
    step = BASE_SIZE/2;
    
#endif       
        
    bool redraw=false;
    bool do_exit=false, lock=false;
    bool lock_mystery_ship=false, alien_change=false;
    
    //game
    
#ifdef RASPBERRY  

    while(!do_exit && cant_aliens && *lives){
        if(!TimerTickRBP){
            TimerTickRBP = TIMERTICKRBP;
            if(key_pressed[LEFT] && nave_x>=MOVE_RATE){
                nave_x -= MOVE_RATE;
            }
            if(key_pressed[RIGHT] && (nave_x <= SCREEN_W-4*MOVE_RATE)){
                nave_x += MOVE_RATE;
            }
            if(aux >= difficulty){
                logical_move(&alien_change, &lock_mystery_ship, &alien_x[0], &alien_y[0], &difficulty, 
                             &alien_bullets_x[0], &alien_bullets_y[0], &accelerate, &step, cant_aliens);
                aux = 0;
            }
            if(lock_mystery_ship){
                count++;
                if(count>=3){
                    mystery_ship_x -= MOVE_RATE;
                    count=0;
                }
            }
            redraw = true;
            aux++;
        }
        if(key_pressed[SPACE_UP]){
            if(!lock){
                bullet_x = nave_x + BASE_SIZE; //setea la bala
                bullet_y = SCREEN_H + BASE_SIZE;
                lock = true; // solo dispara si no hay otra bala volando.
            }
        }
        if(key_pressed[JOY_SWITCH]){            
            aux=pause_menu_terminal();
            if(aux){
                do_exit=true;
                cant_aliens=aux;
                redraw = false;
            }
        }
        if(redraw){
            //fprintf(stderr,"%d ,%d ,%d ,%d\n ", vida_bloques[0], vida_bloques[1], vida_bloques[2], vida_bloques[3]);
	    //fprintf(stderr,"%d ,%d\n",bullet_x, bullet_y);
            show_on_terminal(*lives,*score);
            disp_clear();
            coord_nave.x=nave_x;
            coord_nave.y=NAVE_Y;
            coord_bloques.y=BLOQUES_Y;
            coord_mystery_ship.x=mystery_ship_x;
            coord_mystery_ship.y=MYSTERY_SHIP_Y;
            coord_bullet.x=bullet_x;
            coord_bullet.y=bullet_y;
            redraw=false;
            
            for(i=0; i<4; i++){
                switch(i){
                    case 0:{
                        break;
                    }
                    case 1:{
                        coord_nave.x++;
                        break;
                    }
                    case 2:{
                        coord_nave.x++;
                        break;
                    }
                    case 3:{
                        coord_nave.x--;
                        coord_nave.y--;
                        break;
                    }
                    default:{
                        fprintf(stderr, "Hubo un error al imprimir la nave.\n");
                        break;
                    }
                }
                disp_write(coord_nave, D_ON); 
            }
            for(i=0; i<CANT_BLOQUES; i++){
                if(vida_bloques[i]!=0){
                    coord_bloques.x=bloques_x[i];
                    disp_write(coord_bloques, D_ON);
                    coord_bloques.x++;
                    disp_write(coord_bloques, D_ON);
                }   
            }
            for(i=0; i<CANT_ALIENS; i++){
		//fprintf(stderr,"%d ,%d\n", alien_x[i], alien_y[i]);
                coord_alien.x=alien_x[i];
                coord_alien.y=alien_y[i];
                if(coord_alien.y<SCREEN_H){
                    disp_write(coord_alien, D_ON);
                }
                coord_alien_bullet.x=alien_bullets_x[i];
		coord_alien_bullet.y=alien_bullets_y[i];
		if(coord_alien_bullet.y<SCREEN_H && coord_alien_bullet.x<SCREEN_W){
		    disp_write(coord_alien_bullet, D_ON);
                }
                if(alien_bullets_y[i]<SCREEN_H){
                    alien_bullets_y[i]+=MOVE_RATE;
                }
            }
            if(coord_mystery_ship.x<SCREEN_W){
                disp_write(coord_mystery_ship, D_ON);
            }
            if(coord_bullet.y<SCREEN_H && coord_bullet.x<SCREEN_H){
                disp_write(coord_bullet, D_ON);
            }
            if(lock){
                bullet_y -= MOVE_RATE;  //actualiza la posicion de la bala en cada ciclo
                if(bullet_y == BLOQUES_Y){
                    for(i=0; i<4; i++){
                        if(vida_bloques[i]){
                            if(bullet_x==(bloques_x[i]+1) || bullet_x==bloques_x[i]){
                                vida_bloques[i]--;
                                lock = false;
				bullet_x=SCREEN_W;
                            }
                        }
                    }
                }
            }
            do_exit = logical(&lock_mystery_ship, &mystery_ship_x, &alien_x[0], &alien_y[0], 
                              &alien_bullets_x[0], &alien_bullets_y[0], &cant_aliens, &bullet_y, &bullet_x, 
                              &explosion_x, &explosion_y, &explosion_time, &lock, &nave_x,                    
                              score, &vida_bloques[0], &bloques_x[0], lives, multiplier);
            
            disp_update();
        }
        
    }
    
#else
    
    al_set_target_bitmap(al_get_backbuffer(display));
    al_start_timer(timer);
    while (!do_exit && cant_aliens && *lives) {
        ALLEGRO_EVENT ev;
        if(al_get_next_event(event_queue, &ev)){ //toma un evento de la cola.
            if(ev.type == ALLEGRO_EVENT_TIMER){ //controles de movimiento de la nave
                if(key_pressed[LEFT] && nave_x >= MOVE_RATE){
                    nave_x -= MOVE_RATE;
                }
                if(key_pressed[RIGHT] && nave_x <= SCREEN_W - 3*BASE_SIZE - MOVE_RATE){
                    nave_x += MOVE_RATE;
                }
                if(aux >= difficulty){
                    logical_move(&alien_change, &lock_mystery_ship, &alien_x[0], &alien_y[0], &difficulty, 
                             &alien_bullets_x[0], &alien_bullets_y[0], &accelerate, &step, cant_aliens);
                    aux = 0;
                }
                if(lock_mystery_ship){
                    mystery_ship_x -= MOVE_RATE/3;
                }
                redraw = true;
                aux++;
            } 
            else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                return CLOSE_DISPLAY;
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
                switch(ev.keyboard.keycode){
                    case ALLEGRO_KEY_LEFT:{
                        key_pressed[LEFT] = true;
                        break;
                    }
                    case ALLEGRO_KEY_RIGHT:{
                        key_pressed[RIGHT] = true;
                        break;
                    }
                    case ALLEGRO_KEY_SPACE:{
                        if(!lock){
                            bullet_x = nave_x + 1.5*BASE_SIZE; //setea la bala
                            bullet_y = NAVE_Y;
                            lock = true; // solo dispara si no hay otra bala volando.
                            al_play_sample(samples[1], 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        }
                        break;
                    }
                }
            } 
            else if(ev.type == ALLEGRO_EVENT_KEY_UP){
                switch(ev.keyboard.keycode){
                    case ALLEGRO_KEY_LEFT:{
                        key_pressed[LEFT] = false;
                        break;
                    }
                    case ALLEGRO_KEY_RIGHT:{
                        key_pressed[RIGHT] = false;
                        break;
                    }
                    case ALLEGRO_KEY_ESCAPE:{
                        switch(menu_display("RESUME", "RESET GAME", "EXIT", 1, 1)){ //menú de pausa
                            case 0:{
                                cant_aliens = CLOSE_DISPLAY;
                                do_exit = true;
                                break;
                            }
                            case 1:{ //Resume 
                                break;
                            }
                            case 2:{ //Reset level
                                cant_aliens = RESET_GAME;
                                do_exit = true;
                                break;
                            }
                            case 3:{ //go back to main menu.
                                cant_aliens = EXIT_MENU;
                                do_exit = true;
                                break;
                            }
                            default:{
                                fprintf(stderr, "Hubo un error en el menú de pausa.\n");
                                return FATAL_ERROR;
                                break;
                            }
                        }
                    }
                }
            }
        }

        if(redraw && al_is_event_queue_empty(event_queue)){
            redraw = false;
            
            if(lock){
                al_draw_rectangle(bullet_x-1, bullet_y, bullet_x+1, bullet_y+BASE_SIZE, al_map_rgb(255, 0, 0), 0);
                bullet_y -= 2*MOVE_RATE; //actualiza la posicion de la bala en cada ciclo
                if(bullet_y <= BLOQUES_Y+BASE_SIZE){
                    for(i=0; i<4; i++){
                        if(vida_bloques[i]){
                            if(bullet_x >= bloques_x[i] && bullet_x <= bloques_x[i]+4*BASE_SIZE){
                                vida_bloques[i]--;
                                lock = false;
                            }
                        }
                    }
                }
            }

            do_exit = logical(&lock_mystery_ship, &mystery_ship_x, &alien_x[0], &alien_y[0], &alien_bullets_x[0], 
                    &alien_bullets_y[0], &cant_aliens, &bullet_y, &bullet_x, &explosion_x, &explosion_y, &explosion_time, 
                    &lock, &nave_x, score, &vida_bloques[0], &bloques_x[0], lives, multiplier);
   
            score_to_str(score);   
        }
    }

    al_stop_timer(timer);

#endif
    
    if(cant_aliens){ // Reinicia la vida de los bloques.
        for(i=0; i<4; i++)
            vida_bloques[i]=30;
    }
    return cant_aliens;
}

/******************************* Local functions *******************************/

static uint16_t get_rand_num(uint16_t x){
    
    static bool i=false;
    if(!i){
        srand(time(NULL));
        i=true;
    }
    return (rand()%x);    
}

#ifndef RASPBERRY

static void score_to_str(uint32_t *score){
    char str[]={'S','C','O','R','E',':',' ',' ','0','0','0','0','0'};
    uint8_t i;
    uint32_t aux=0, j;
    for(i=12, j=1; i>7; i--, j*=10){
        aux = (*score)/j;
        str[i]=(char)(aux%10 + ASCII);        
    }
    al_draw_text(font[0], al_map_rgb(255,255,255), SCREEN_W, BASE_SIZE/4, ALLEGRO_ALIGN_RIGHT, str);     
}

#endif

static void logical_move(bool* alien_change, bool* lock_mystery_ship, elements_t* alien_x, elements_t* alien_y, 
                        uint8_t* difficulty, elements_t* alien_bullets_x, elements_t* alien_bullets_y, 
                        uint8_t* accelerate, int8_t *step, int8_t cant_aliens){

    int8_t check, i, aux;
    
    if(*alien_change){
        *alien_change=false;
    }
    else{
        *alien_change=true;
    }
    if(!*lock_mystery_ship){
        if(!get_rand_num(20)){          //5% de probabilidad.
            *lock_mystery_ship=true;
        }
    }
    for(i=0, check=0; i<CANT_ALIENS; i++)
    {
        if(alien_y[i] < SCREEN_H){  //si están vivos

#ifndef RASPBERRY
            if(alien_x[i] >= SCREEN_W-2.5*BASE_SIZE || alien_x[i] <= BASE_SIZE/2){ //revisa que no sobrepasen los extremos
#else
            if(alien_x[i] == SCREEN_W-BASE_SIZE || alien_x[i] == 0){ //revisa que no sobrepasen los extremos
#endif  //RASPBERRY
                
                check++;
            }
        }
    }

    if(check){
        (*accelerate)++;
        if(*accelerate >= 3 && (*difficulty) >= MAX_DIFFICULTY-6){    //esto para que acelere dos veces
            *difficulty -= *accelerate;                               //si se juega con max dificultad
            *accelerate = 0;
        }
        *step *= -1;    //cambia el sentido del movimiento de los aliens
        for(i=0; i<CANT_ALIENS; i++)
        alien_y[i] += BASE_SIZE;
    }

    for(i=0; i<CANT_ALIENS; i++){
        alien_x[i] += *step;
    }
                    
    if(!get_rand_num(4)){               //25% de probabilidad
        aux = get_rand_num(cant_aliens);
        for(i=0; i<CANT_ALIENS; i++){       // para cada alien
            if(alien_y[i]<SCREEN_H){        // si esta vivo
                if(!((aux)--)){
                    if(alien_bullets_y[i]>=SCREEN_H){   //si no está disparando

#ifndef RASPBERRY
                        alien_bullets_x[i]=alien_x[i]+BASE_SIZE; //coordenada x de la bala
#else
                        alien_bullets_x[i]=alien_x[i]; //coordenada x de la bala
#endif  //RASPBERRY
                        
                    alien_bullets_y[i]=alien_y[i]; //coordenada y de la bala
                    }
                }
            }
        }
    }
}

static bool logical(bool* lock_mystery_ship, elements_t* mystery_ship_x, elements_t* alien_x, elements_t* alien_y,
                    elements_t* alien_bullets_x, elements_t* alien_bullets_y, int8_t* cant_aliens,
                    elements_t* bullet_y, elements_t* bullet_x, elements_t* explosion_x, elements_t* explosion_y,
                    uint8_t* explosion_time, bool* lock, elements_t* nave_x, uint32_t* score, uint8_t* vida_bloques,
                    elements_t* bloques_x, uint8_t* lives, uint8_t multiplier){

    int8_t i, j;
    if(*bullet_y <= MOVE_RATE-2)           //si pega arriba desaparece la bala
        *lock = false;
    for(i=0; i<CANT_ALIENS; i++){
        if(alien_y[i] < SCREEN_H){  //si el alien está vivo
            
#ifndef RASPBERRY
            if(*bullet_y >= alien_y[i] && *bullet_y <= alien_y[i] + 2*BASE_SIZE){
                if(*bullet_x >= alien_x[i] && *bullet_x <= alien_x[i] + 2*BASE_SIZE){
                    (*cant_aliens)--;           //muere un alien
                    *explosion_x = alien_x[i];
                    *explosion_y = alien_y[i];
                    *explosion_time = 10;
                    alien_y[i] = SCREEN_H; //mueve el alien muerto fuera de la pantalla 
                    *lock = false;
                    *bullet_y = NAVE_Y + BASE_SIZE;
                    al_play_sample(samples[2], 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    if(i < (CANT_ALIENS/FILAS_ALIENS)){
                        (*score) += 30*multiplier;
                    } 
                    else if(i < (3*CANT_ALIENS/FILAS_ALIENS)){
                        (*score) += 20*multiplier;
                    } 
                    else{
                        (*score) += 10*multiplier;
                    }
                }
            }
#else
            if(*bullet_y == alien_y[i] && *bullet_x == alien_x[i]){
                (*cant_aliens)--;   //muere un alien
                alien_y[i] = SCREEN_H; //mueve el alien muerto fuera de la pantalla 
                *lock = false;
                *bullet_y = NAVE_Y + BASE_SIZE;
                if(i < ((CANT_ALIENS/FILAS_ALIENS))){
                    (*score) += 30*multiplier;
                } 
                else if(i < (2*CANT_ALIENS/FILAS_ALIENS)){
                    (*score) += 20*multiplier;
                } 
                else{
                    (*score) += 10*multiplier;
                }
            }
#endif
            
        }            
        
        for(j=0; j<CANT_BLOQUES; j++){
            if(vida_bloques[j]){
#ifndef RASPBERRY
                if((alien_bullets_y[i]+BASE_SIZE) >= BLOQUES_Y && alien_bullets_y[i] <= BLOQUES_Y){
                    if(alien_bullets_x[i] >= bloques_x[j] && alien_bullets_x[i] <= bloques_x[j]+4*BASE_SIZE){
#else
                if((alien_bullets_y[i]) == BLOQUES_Y){
                    if(alien_bullets_x[i] == bloques_x[j] || alien_bullets_x[i] == bloques_x[j]+BASE_SIZE){
#endif
                        alien_bullets_y[i] = SCREEN_H + BASE_SIZE;
                        vida_bloques[j]--;
                    }
                }
            }
        }

#ifndef RASPBERRY
        if(alien_bullets_y[i] >= NAVE_Y && alien_bullets_y[i] <= NAVE_Y+BASE_SIZE){
            if(alien_bullets_x[i] >= *nave_x && alien_bullets_x[i] <= *nave_x+3*BASE_SIZE){
                al_play_sample(samples[3], 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
#else
        if(alien_bullets_y[i] == NAVE_Y){
            if(alien_bullets_x[i] == *nave_x && alien_bullets_x[i] < *nave_x+3*BASE_SIZE){
                //music();
#endif
                alien_bullets_y[i] = SCREEN_H + BASE_SIZE;                                  
                (*lives)--;
            }
        }
                
#ifndef RASPBERRY
        if(alien_bullets_x[i] >= *bullet_x-BULLET_W/2 && alien_bullets_x[i] <= *bullet_x+BULLET_W/2){
            if(alien_bullets_y[i] >= *bullet_y && alien_bullets_y[i] < *bullet_y+BASE_SIZE){
#else
        if(alien_bullets_x[i] == *bullet_x){
            if((alien_bullets_y[i] == *bullet_y)){  
#endif
                *lock = false;
                *bullet_y = SCREEN_H + BASE_SIZE;
                alien_bullets_y[i] = SCREEN_H + BASE_SIZE;
            }
        }
                    
        if(alien_y[i] >= (3*SCREEN_H/4) && alien_y[i] < SCREEN_H){ //condicion de derrota
            return true;
        }
    }
    
    if(*mystery_ship_x < SCREEN_W){

#ifndef RASPBERRY

        if(*bullet_y>=MYSTERY_SHIP_Y && *bullet_y<=MYSTERY_SHIP_Y+2*BASE_SIZE){
            if(*bullet_x>=*mystery_ship_x && *bullet_x<=*mystery_ship_x+2*BASE_SIZE){

#else
        if(*bullet_y == MYSTERY_SHIP_Y){
            if(*bullet_x == *mystery_ship_x){

#endif
                *mystery_ship_x = SCREEN_W;
                *lock_mystery_ship = false;
                *lock = false;
                *bullet_y = NAVE_Y + BASE_SIZE;
                *score += 5*multiplier*get_rand_num(21);       //te puede sumar desde 0 a 100 puntos.

#ifndef RASPBERRY
                al_play_sample(samples[2], 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
#else

#endif
            }
        }
    }
            
    if(*mystery_ship_x+2*BASE_SIZE <= 0){
        *mystery_ship_x = SCREEN_W;
        *lock_mystery_ship=false;
    }
    return false;
}
/********************************* END FILE ************************************/
void redraw_world(uint8_t level,uint8_t lives,uint8_t alien_change, elements_t nave_x,elements_t bloques_x,uint8_t vida_bloques ,elements_t alien_x,elements_t alien_y,elements_t alien_bullets_x, elements_t alien_bullets_y) {
    int i,j;
    al_draw_scaled_bitmap(display_background[11 + level % 5], 0, 0, al_get_bitmap_width(display_background[11 + level % 5]),
            al_get_bitmap_height(display_background[11 + level % 5]), 0, 0, SCREEN_W, SCREEN_H, 0);
    al_draw_scaled_bitmap(display_background[2], 0, 0, al_get_bitmap_width(display_background[2]),
            al_get_bitmap_height(display_background[2]), nave_x, NAVE_Y, 3 * BASE_SIZE, 1.5 * BASE_SIZE, 0);

    for (i = 0; i < (*lives); i++) {
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

        alien_bullets_y[i] += 1.5 * MOVE_RATE;
    }

    al_draw_scaled_bitmap(display_background[6], 0, 0, al_get_bitmap_width(display_background[6]),
            al_get_bitmap_height(display_background[6]), mystery_ship_x, MYSTERY_SHIP_Y, 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
    
    if(explosion_time){
                al_draw_scaled_bitmap(display_background[16], 0, 0, al_get_bitmap_width(display_background[16]), 
                    al_get_bitmap_height(display_background[16]), explosion_x, explosion_y, 2.5*BASE_SIZE, 2.5*BASE_SIZE, 0);
                explosion_time--;
            }
    }