#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "config.h"
#include "move.h"
#include "menus.h"
#include "joydrv.h"
#include "disdrv.h"
#include "termlib.h"
#include "libaudio.h"
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

static void logical_move(bool* alien_change, bool* lock_mystery_ship, float* mystery_ship_x, float* mystery_ship_y,
                         float* alien_x, float* alien_y, uint8_t* accelerate, uint8_t* difficulty, float* alien_bullets_x, float* alien_bullets_y, int8_t *step,
                        int8_t cant_aliens, uint8_t* aux,uint8_t mode );

static bool logical(bool* lock_mystery_ship, float* mystery_ship_x, float* mystery_ship_y,
                    float* alien_x, float* alien_y,float* alien_bullets_x, float* alien_bullets_y, int8_t *step,int8_t* cant_aliens,
                    float* bullet_y, float* bullet_x, float* explosion_x, float* explosion_y, uint8_t* explosion_time,
                    bool* lock, float* nave_y, float* nave_x,uint32_t* score,uint8_t* vida_bloques,float* bloques_y, float* bloques_x,uint8_t* lives,
                    uint8_t mode);
#else
static void logical_move(bool* alien_change, bool* lock_mystery_ship, int* mystery_ship_x, int* mystery_ship_y,
                         int* alien_x, int* alien_y, uint8_t* accelerate, uint8_t* difficulty, int* alien_bullets_x, int* alien_bullets_y, int8_t *step,
                        int8_t cant_aliens, uint8_t* aux,uint8_t mode );

static bool logical(bool* lock_mystery_ship, int* mystery_ship_x, int* mystery_ship_y,
                    int* alien_x, int* alien_y,int* alien_bullets_x, int* alien_bullets_y, int8_t *step,int8_t* cant_aliens,
                    int* bullet_y, int* bullet_x, int* explosion_x, int* explosion_y, uint8_t* explosion_time,
                    bool* lock, int* nave_y, int* nave_x,uint32_t* score,uint8_t* vida_bloques,int* bloques_y, int* bloques_x,uint8_t* lives,
                    uint8_t mode);
#endif

enum MYKEYS {LEFT, RIGHT, SPACE_UP,JOY_SWITCH};

static uint16_t get_rand_num(uint8_t x);
/*Recibe un entero positivo y devuelve un entero aleatorio menor a dicho numero*/
static void score_to_str(uint32_t* score);

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

int move(uint8_t difficulty, uint8_t* lives, uint8_t level, uint32_t* score,uint8_t mode)
{
    uint8_t i, j, check, aux, accelerate=0, explosion_time=0;
    static uint8_t vida_bloques[4] = {30, 30, 30, 30};
    int8_t cant_aliens = CANT_ALIENS, step;

#ifdef RASPBERRY
    int bullet_x=SCREEN_W, bullet_y,nave_x,nave_y,bloques_x[4], bloques_y,explosion_x, explosion_y,alien_bullets_x[CANT_ALIENS],alien_bullets_y[CANT_ALIENS],mystery_ship_x,mystery_ship_y;
    dcoord_t coord_nave,coord_bloques,coord_alien,coord_mystery_ship,coord_bullet,coord_alien_bullet;
    char songs[4][]={"game.wav","shoot.wav","invaderkilled.wav","explosion.wav"};
    pthread_t Timer_RBP,Joy_Action;
    pthread_create(&Timer_RBP,NULL,Timer_rbp,NULL);
    pthread_create(&Joy_Action,NULL,Joy_action,NULL);
    for(i=0; i<4; i++)
        bloques_x[i] = 1+4*i;
    bloques_y =SCREEN_H - 4;
    nave_x = SCREEN_W / 2 - 1;
    nave_y = SCREEN_H-1;
    //seteo de coordenadas iniciales de los aliens   
    int alien_x[CANT_ALIENS] = {3,5,7,9,11,13,
                        3,5,7,9,11,13,
                        3,5,7,9,11,13,
                        3,5,7,9,11,13};

    int alien_y[CANT_ALIENS] = {2,2,2,2,2,2,
                        4,4,4,4,4,4,
                        6,6,6,6,6,6,
                        8,8,8,8,8,8};
    step = MOVE_RATE;
#else 
   float bullet_x, bullet_y,nave_x,nave_y,bloques_x[4], bloques_y,explosion_x, explosion_y,alien_bullets_x[CANT_ALIENS],alien_bullets_y[CANT_ALIENS],mystery_ship_x,mystery_ship_y;
   for(i=0; i<4; i++)
        bloques_x[i] = (1.5*i+1)*SCREEN_W / 6.5 - 2*BASE_SIZE;
    bloques_y = 3*SCREEN_H / 4 + 2.5*BASE_SIZE;
    //int8_t cant_aliens = 1;    //solo para facilitar debug
    nave_x = SCREEN_W / 2.0 - 1.5*BASE_SIZE;
    nave_y = SCREEN_H - 2*BASE_SIZE;
    //seteo de coordenadas iniciales de los aliens   
    float alien_x[CANT_ALIENS] = {3*SCREEN_W/15, 4*SCREEN_W/15, 5*SCREEN_W/15, 6*SCREEN_W/15, 7*SCREEN_W/15, 8*SCREEN_W/15, 9*SCREEN_W/15, 10*SCREEN_W/15, 11*SCREEN_W/15, 12*SCREEN_W/15,
                  3*SCREEN_W/15, 4*SCREEN_W/15, 5*SCREEN_W/15, 6*SCREEN_W/15, 7*SCREEN_W/15, 8*SCREEN_W/15, 9*SCREEN_W/15, 10*SCREEN_W/15, 11*SCREEN_W/15, 12*SCREEN_W/15,
                  3*SCREEN_W/15, 4*SCREEN_W/15, 5*SCREEN_W/15, 6*SCREEN_W/15, 7*SCREEN_W/15, 8*SCREEN_W/15, 9*SCREEN_W/15, 10*SCREEN_W/15, 11*SCREEN_W/15, 12*SCREEN_W/15,
                  3*SCREEN_W/15, 4*SCREEN_W/15, 5*SCREEN_W/15, 6*SCREEN_W/15, 7*SCREEN_W/15, 8*SCREEN_W/15, 9*SCREEN_W/15, 10*SCREEN_W/15, 11*SCREEN_W/15, 12*SCREEN_W/15,
                  3*SCREEN_W/15, 4*SCREEN_W/15, 5*SCREEN_W/15, 6*SCREEN_W/15, 7*SCREEN_W/15, 8*SCREEN_W/15, 9*SCREEN_W/15, 10*SCREEN_W/15, 11*SCREEN_W/15, 12*SCREEN_W/15};

    float alien_y[CANT_ALIENS] = {SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, 
                  SCREEN_H/5, SCREEN_H/5, SCREEN_H/5, SCREEN_H/5, SCREEN_H/5, SCREEN_H/5, SCREEN_H/5, SCREEN_H/5, SCREEN_H/5, SCREEN_H/5,
                  3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10,  
                  2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5,
                  SCREEN_H/2, SCREEN_H/2, SCREEN_H/2, SCREEN_H/2, SCREEN_H/2, SCREEN_H/2, SCREEN_H/2, SCREEN_H/2, SCREEN_H/2, SCREEN_H/2};
    step = BASE_SIZE/2;
#endif       
    
    mystery_ship_y=SCREEN_H; 
    mystery_ship_x=SCREEN_W; 
    
    
    for(i=0; i<CANT_ALIENS; i++)
    {
        alien_bullets_y[i]=SCREEN_H;
    }
    
    

    bool redraw = false;
    bool do_exit = false;
    bool lock = false;
    bool lock_mystery_ship = false;
    bool alien_change = false;
    bool redraw_rbp= false;
    
#ifdef RASPBERRY
    //set_file_to_play(songs[0]);//Musica del juego.
    while (!do_exit && cant_aliens && *lives) {
        if (!TimerTickRBP) {
            TimerTickRBP = TIMERTICKRBP;
            if (key_pressed[LEFT] && nave_x >= MOVE_RATE)
                nave_x -= MOVE_RATE;

            if (key_pressed[RIGHT] && nave_x <= SCREEN_W - 4 * MOVE_RATE)
                nave_x += MOVE_RATE;

            if (aux >= difficulty) {
                logical_move(&alien_change, &lock_mystery_ship, &mystery_ship_x, &mystery_ship_y,
                        &alien_x[0], &alien_y[0], &accelerate, &difficulty, &alien_bullets_x[0], &alien_bullets_y[0], &step,
                        cant_aliens, &aux, mode);
                aux = 0;
            }

            if (lock_mystery_ship) {
                mystery_ship_x -= MOVE_RATE;
            }
            redraw_rbp = true;
            aux++;
        }
        if(key_pressed[SPACE_UP]){
            if(!lock) {
                bullet_x = nave_x + 1; //setea la bala
                bullet_y = SCREEN_H;
                lock = true; // solo dispara si no hay otra bala volando.
            }
        }
        if(key_pressed[JOY_SWITCH]){
            do_exit=true;
        }
        if(redraw_rbp){
            //fprintf(stderr,"%d ,%d ,%d ,%d\n ",vida_bloques[0],vida_bloques[1],vida_bloques[2],vida_bloques[3]);
	    //fprintf(stderr,"%d ,%d\n",bullet_x,bullet_y);
            disp_clear();
            coord_nave.x=nave_x;
            coord_nave.y=nave_y;
            coord_bloques.y=bloques_y;
            coord_mystery_ship.x=mystery_ship_x;
            coord_mystery_ship.y=mystery_ship_y;
            coord_bullet.x=bullet_x;
            coord_bullet.y=bullet_y;
            redraw_rbp=false;
            
            for(i=0;i<4;i++){
                switch (i){
                    case 0:
                    break;
                    case 1:coord_nave.x++;
                    break;
                    case 2:coord_nave.x++;
                    break;
                    case 3:{
                        coord_nave.x--;
                        coord_nave.y--;
                    }
                    break;
                    default:
                        fprintf(stderr,"Hubo un error en el print de la nave.");
                        
                }
                if(vida_bloques[i]!= 0){
                    coord_bloques.x=bloques_x[i];
                    disp_write(coord_bloques,D_ON);
                    coord_bloques.x++;
                    disp_write(coord_bloques,D_ON);
                }
                disp_write(coord_nave,D_ON);    
            }
            for(i=0;i<CANT_ALIENS;i++){
		//fprintf(stderr,"%d ,%d\n",alien_x[i],alien_y[i]);
                coord_alien.x=alien_x[i];
                coord_alien.y=alien_y[i];
                if(coord_alien.y<SCREEN_H)
                    disp_write(coord_alien,D_ON);
                coord_alien_bullet.x=alien_bullets_x[i];
		coord_alien_bullet.y=alien_bullets_y[i];
		if(coord_alien_bullet.y<SCREEN_H && coord_alien_bullet.x<SCREEN_W)
		    disp_write(coord_alien_bullet,D_ON);
                if(alien_bullets_y[i]<SCREEN_H)
                    alien_bullets_y[i]++;
            }
            if(coord_mystery_ship.x<SCREEN_W){
                disp_write(coord_mystery_ship,D_ON);
            }
            if(coord_bullet.y<SCREEN_H &&coord_bullet.x<SCREEN_H ){
                disp_write(coord_bullet,D_ON);
            }
            if (lock) {
                bullet_y -= MOVE_RATE; //actualiza la posicion de la bala en cada ciclo
                if (bullet_y == bloques_y)
                    for (i = 0, check = 0; i < 4; i++) {
                        if (vida_bloques[i]) {
                            if (bullet_x == (bloques_x[i]+1) || bullet_x == bloques_x[i]) {
                                vida_bloques[i]--;
                                lock = false;
				bullet_x=SCREEN_W;
                            }
                        }
                    }
            }
            do_exit = logical(&lock_mystery_ship, &mystery_ship_x, &mystery_ship_y, &alien_x[0], &alien_y[0], &alien_bullets_x[0], &alien_bullets_y[0], &step,
                    &cant_aliens, &bullet_y, &bullet_x, &explosion_x, &explosion_y, &explosion_time, &lock, &nave_y, &nave_x,
                    score, &vida_bloques[0], &bloques_y, &bloques_x[0], lives,mode);
            disp_update();
        }
    }
    
#else
    
    al_set_target_bitmap(al_get_backbuffer(display));
    al_start_timer(timer);
    while (!do_exit && cant_aliens && *lives) {
        ALLEGRO_EVENT ev;
        if (al_get_next_event(event_queue, &ev)) //toma un evento de la cola.
        {
            if (ev.type == ALLEGRO_EVENT_TIMER) { //controles de movimiento de la nave

                if (key_pressed[LEFT] && nave_x >= MOVE_RATE)
                    nave_x -= MOVE_RATE;

                if (key_pressed[RIGHT] && nave_x <= SCREEN_W - 3 * BASE_SIZE - MOVE_RATE)
                    nave_x += MOVE_RATE;

                if (aux >= difficulty) {
                    logical_move(&alien_change, &lock_mystery_ship, &mystery_ship_x, &mystery_ship_y,
                            &alien_x[0], &alien_y[0], &accelerate, &difficulty, &alien_bullets_x[0], &alien_bullets_y[0], &step,
                            cant_aliens, &aux, mode);
                    aux = 0;
                }

                if (lock_mystery_ship) {
                    mystery_ship_x -= MOVE_RATE / 3;
                }
                redraw = true;
                aux++;
            } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                return CLOSE_DISPLAY;

            else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (ev.keyboard.keycode) {

                    case ALLEGRO_KEY_LEFT:
                        key_pressed[LEFT] = true;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        key_pressed[RIGHT] = true;
                        break;

                    case ALLEGRO_KEY_SPACE:
                        if (!lock) {
                            bullet_x = nave_x + 1.5 * BASE_SIZE; //setea la bala
                            bullet_y = nave_y;
                            lock = true; // solo dispara si no hay otra bala volando.
                            al_play_sample(samples[1], 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        }
                        break;
                }
            } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
                switch (ev.keyboard.keycode) {

                    case ALLEGRO_KEY_LEFT:
                        key_pressed[LEFT] = false;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        key_pressed[RIGHT] = false;
                        break;

                    case ALLEGRO_KEY_ESCAPE:
                    {
                        switch (pause_menu()) {
                            case 0:
                            {
                                cant_aliens = CLOSE_DISPLAY;
                                do_exit = true;
                            }
                                break;
                            case 1: //Resume 
                                break;
                            case 2:
                            { //Reset level
                                cant_aliens = RESET_GAME;
                                do_exit = true;

                            }
                                break;
                            case 3:
                            { //go back to main menu.
                                cant_aliens = EXIT_MENU;
                                do_exit = true;
                            }
                                break;
                            default:
                            {
                                fprintf(stderr, "Something gone wrong");
                                return -1;
                            }
                                break;
                        }

                    }
                }
            }
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {

            redraw = false;
            al_draw_scaled_bitmap(display_background[11 + level % 5], 0, 0, al_get_bitmap_width(display_background[11 + level % 5]), al_get_bitmap_height(display_background[11 + level % 5]),
                    0, 0, SCREEN_W, SCREEN_H, 0);
            al_draw_scaled_bitmap(display_background[2], 0, 0, al_get_bitmap_width(display_background[2]), al_get_bitmap_height(display_background[2]),
                    nave_x, nave_y, 3 * BASE_SIZE, 1.5 * BASE_SIZE, 0);

            for (i = 0; i<*lives; i++)
                al_draw_scaled_bitmap(display_background[5], 0, 0, al_get_bitmap_width(display_background[5]), al_get_bitmap_height(display_background[5]),
                    1.5 * i * BASE_SIZE, 0, 2 * BASE_SIZE, 2 * BASE_SIZE, 0);

            for (i = 0; i < 4; i++)
                if (vida_bloques[i] >= 20) {
                    al_draw_filled_rectangle(bloques_x[i], bloques_y, bloques_x[i] + 4 * BASE_SIZE, bloques_y + BASE_SIZE, al_map_rgb(0, 255, 0));
                    al_draw_filled_rectangle(bloques_x[i], bloques_y + BASE_SIZE, bloques_x[i] + BASE_SIZE, bloques_y + 2 * BASE_SIZE, al_map_rgb(0, 255, 0));
                    al_draw_filled_rectangle(bloques_x[i] + 3 * BASE_SIZE, bloques_y + BASE_SIZE, bloques_x[i] + 4 * BASE_SIZE, bloques_y + 2 * BASE_SIZE, al_map_rgb(0, 255, 0));
                } else if (vida_bloques[i] >= 10) {
                    al_draw_filled_rectangle(bloques_x[i], bloques_y, bloques_x[i] + 4 * BASE_SIZE, bloques_y + BASE_SIZE, al_map_rgb(255, 255, 0));
                    al_draw_filled_rectangle(bloques_x[i], bloques_y + BASE_SIZE, bloques_x[i] + BASE_SIZE, bloques_y + 2 * BASE_SIZE, al_map_rgb(255, 255, 0));
                    al_draw_filled_rectangle(bloques_x[i] + 3 * BASE_SIZE, bloques_y + BASE_SIZE, bloques_x[i] + 4 * BASE_SIZE, bloques_y + 2 * BASE_SIZE, al_map_rgb(255, 255, 0));
                } else if (vida_bloques[i]) {
                    al_draw_filled_rectangle(bloques_x[i], bloques_y, bloques_x[i] + 4 * BASE_SIZE, bloques_y + BASE_SIZE, al_map_rgb(255, 0, 0));
                    al_draw_filled_rectangle(bloques_x[i], bloques_y + BASE_SIZE, bloques_x[i] + BASE_SIZE, bloques_y + 2 * BASE_SIZE, al_map_rgb(255, 0, 0));
                    al_draw_filled_rectangle(bloques_x[i] + 3 * BASE_SIZE, bloques_y + BASE_SIZE, bloques_x[i] + 4 * BASE_SIZE, bloques_y + 2 * BASE_SIZE, al_map_rgb(255, 0, 0));
                }

            for (i = 0, check = 0; i < CANT_ALIENS; i++) {
                if (alien_y[i] < SCREEN_H) {
                    if (alien_change) {
                        if (i <= ((CANT_ALIENS / 5) - 1)) {
                            al_draw_scaled_bitmap(display_background[8], 0, 0, al_get_bitmap_width(display_background[8]), al_get_bitmap_height(display_background[8]),
                                    alien_x[i], alien_y[i], 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
                        } else if (i <= ((3 * CANT_ALIENS / 5) - 1)) {
                            al_draw_scaled_bitmap(display_background[3], 0, 0, al_get_bitmap_width(display_background[3]), al_get_bitmap_height(display_background[3]),
                                    alien_x[i], alien_y[i], 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
                        } else {
                            al_draw_scaled_bitmap(display_background[9], 0, 0, al_get_bitmap_width(display_background[9]), al_get_bitmap_height(display_background[9]),
                                    alien_x[i], alien_y[i], 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
                        }

                    } else {

                        if (i <= ((CANT_ALIENS / 5) - 1)) {
                            al_draw_scaled_bitmap(display_background[7], 0, 0, al_get_bitmap_width(display_background[7]), al_get_bitmap_height(display_background[7]),
                                    alien_x[i], alien_y[i], 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
                        } else if (i <= ((3 * CANT_ALIENS / 5) - 1)) {
                            al_draw_scaled_bitmap(display_background[4], 0, 0, al_get_bitmap_width(display_background[4]), al_get_bitmap_height(display_background[4]),
                                    alien_x[i], alien_y[i], 2 * BASE_SIZE, 2 * BASE_SIZE, 0);
                        } else {
                            al_draw_scaled_bitmap(display_background[10], 0, 0, al_get_bitmap_width(display_background[10]), al_get_bitmap_height(display_background[10]),
                                    alien_x[i], alien_y[i], 2.5 * BASE_SIZE, 2.5 * BASE_SIZE, 0);
                        }
                    }
                }
                if (alien_x[i] >= SCREEN_W - 2 * BASE_SIZE || alien_x[i] <= BASE_SIZE) //revisa que no sobrepasen los extremos
                    check++;
                al_draw_rectangle(alien_bullets_x[i] - 1, alien_bullets_y[i], alien_bullets_x[i] + 1, alien_bullets_y[i] + BASE_SIZE, al_map_rgb(255, 255, 255), 0); //dibujo balas aliens
                alien_bullets_y[i] += 1.5 * MOVE_RATE;
            }

            al_draw_scaled_bitmap(display_background[6], 0, 0, al_get_bitmap_width(display_background[6]), al_get_bitmap_height(display_background[6]),
                    mystery_ship_x, mystery_ship_y, 2 * BASE_SIZE, 2 * BASE_SIZE, 0);

            if (lock) {
                al_draw_rectangle(bullet_x - 1, bullet_y, bullet_x + 1, bullet_y + BASE_SIZE, al_map_rgb(255, 0, 0), 0);
                bullet_y -= 2 * MOVE_RATE; //actualiza la posicion de la bala en cada ciclo
                if (bullet_y <= bloques_y + BASE_SIZE)
                    for (i = 0, check = 0; i < 4; i++) {
                        if (vida_bloques[i]) {
                            if (bullet_x >= bloques_x[i] && bullet_x <= bloques_x[i] + 4 * BASE_SIZE) {
                                vida_bloques[i]--;
                                lock = false;
                            }
                        }
                    }
            }

            do_exit = logical(&lock_mystery_ship, &mystery_ship_x, &mystery_ship_y, &alien_x[0], &alien_y[0], &alien_bullets_x[0], &alien_bullets_y[0], &step,
                    &cant_aliens, &bullet_y, &bullet_x, &explosion_x, &explosion_y, &explosion_time, &lock, &nave_y, &nave_x,
                    score, &vida_bloques[0], &bloques_y, &bloques_x[0], lives,mode);

            if (explosion_time) {
                al_draw_scaled_bitmap(display_background[16], 0, 0, al_get_bitmap_width(display_background[16]), al_get_bitmap_height(display_background[16]),
                        explosion_x, explosion_y, 2.5 * BASE_SIZE, 2.5 * BASE_SIZE, 0);
                explosion_time--;
            }
            score_to_str(score);
            al_flip_display();
        }
    }

    al_stop_timer(timer);

#endif
    
    if(cant_aliens){
        for(i=0; i<4; i++)
            vida_bloques[i]=30;
    }
    return cant_aliens;
}

static uint16_t get_rand_num(uint8_t x){
    
    static bool i=false ;
    if(!i){
        srand(time(NULL));
        i=true;
    }
    return (rand()%x);    
}

#ifndef RASPBERRY

static void score_to_str(uint32_t *score){
    char str[14]={'S','C','O','R','E',':',' ',' ','0','0','0','0','0'};
    uint32_t aux=0,i,j;
    for(i=12,j=1; i>7; i--,j*=10){
        aux=*score/j;
        str[i]=(char)(aux%10+ASCII);        
    }
    al_draw_text(font[0], al_map_rgb(255,255,255), SCREEN_W, BASE_SIZE/4, ALLEGRO_ALIGN_RIGHT, str);     
}

#endif

#ifndef RASPBERRY

static void logical_move(bool* alien_change, bool* lock_mystery_ship, float* mystery_ship_x, float* mystery_ship_y,
                         float* alien_x, float* alien_y, uint8_t* accelerate, uint8_t* difficulty,
                         float* alien_bullets_x, float* alien_bullets_y, int8_t *step,int8_t cant_aliens, uint8_t* aux,uint8_t mode){
#else

static void logical_move(bool* alien_change, bool* lock_mystery_ship, int* mystery_ship_x, int* mystery_ship_y,
        int* alien_x, int* alien_y, uint8_t* accelerate, uint8_t* difficulty, int* alien_bullets_x, int* alien_bullets_y, int8_t *step,
        int8_t cant_aliens, uint8_t* aux, uint8_t mode) {
    
#endif
    int check,i;
    if(*alien_change)
        *alien_change=false;
    else
        *alien_change=true;
                    
    if(!*lock_mystery_ship){
        if(!get_rand_num(20)){          //5% de probabilidad.
                *lock_mystery_ship=true;
                if(!mode){
                    *mystery_ship_y=1.5*BASE_SIZE;
                }else{
                    *mystery_ship_y=BASE_SIZE;
                }
                    
        }
    }
                      
    for(i=0, check=0; i<CANT_ALIENS; i++)
    {
        if(alien_y[i] < SCREEN_H){
            if(!mode){
                if(alien_x[i] >= SCREEN_W-2.5*BASE_SIZE || alien_x[i] <= BASE_SIZE/2) //revisa que no sobrepasen los extremos
                        check++;
            }else{
                if(alien_x[i] >= SCREEN_W-1 || alien_x[i] <= 0) //revisa que no sobrepasen los extremos
                        check++;
            }
        }
    }
    if(check)
    {
        (*accelerate)++;
        if((*accelerate) >= 4 && (*difficulty) >= 8){
            *difficulty -= 4;
            *accelerate = 0;
        }
        *step *= -1;
        for(i=0; i<CANT_ALIENS; i++)
        alien_y[i] += BASE_SIZE;
                        
    }
    for(i=0; i<CANT_ALIENS; i++){
        alien_x[i] += *step;
    }
                    
    if(!get_rand_num(3)){               //33% de probabilidad
        *aux = get_rand_num(cant_aliens);
        for(i=0; i<CANT_ALIENS; i++){          // para cada alien
            if(alien_y[i]<SCREEN_H){        // si esta vivo
                if(!((*aux)--)){
                        if(alien_bullets_y[i]>=SCREEN_H){
                            if(!mode)
                                alien_bullets_x[i]=alien_x[i]+BASE_SIZE; //coordenada x
                            else
                                alien_bullets_x[i]=alien_x[i]; //coordenada x
                            alien_bullets_y[i]=alien_y[i]; //le asigno coordenada y del alien a la bala
                        }
                }
            }
        }
    }
}
#ifndef RASPBERRY

static bool logical(bool* lock_mystery_ship, float* mystery_ship_x, float* mystery_ship_y,
        float* alien_x, float* alien_y, float* alien_bullets_x, float* alien_bullets_y, int8_t *step, int8_t* cant_aliens,
        float* bullet_y, float* bullet_x, float* explosion_x, float* explosion_y, uint8_t* explosion_time,
        bool* lock, float* nave_y, float* nave_x, uint32_t* score, uint8_t* vida_bloques, float* bloques_y, float* bloques_x, uint8_t* lives,
        uint8_t mode) {
#else

static bool logical(bool* lock_mystery_ship, int* mystery_ship_x, int* mystery_ship_y,
        int* alien_x, int* alien_y, int* alien_bullets_x, int* alien_bullets_y, int8_t *step, int8_t* cant_aliens,
        int* bullet_y, int* bullet_x, int* explosion_x, int* explosion_y, uint8_t* explosion_time,
        bool* lock, int* nave_y, int* nave_x, uint32_t* score, uint8_t* vida_bloques, int* bloques_y, int* bloques_x, uint8_t* lives,
        uint8_t mode) {
#endif
    int i,j;
    if(*bullet_y <= MOVE_RATE-2)           //si pega arriba desaparece la bala
        *lock = false;
    for(i=0; i<CANT_ALIENS; i++) {
        if (alien_y[i] < SCREEN_H) {
            if (!mode) {
                if (*bullet_y >= alien_y[i] && *bullet_y <= alien_y[i] + 2 * BASE_SIZE) {
                    if (*bullet_x >= alien_x[i] && *bullet_x <= alien_x[i] + 2 * BASE_SIZE) {
                        (*cant_aliens)--;
                        *explosion_x = alien_x[i];
                        *explosion_y = alien_y[i];
                        *explosion_time = 10;
                        alien_y[i] = SCREEN_H; //mueve el alien muerto fuera de la pantalla 
                        *lock = false;
                        *bullet_y = *nave_y;
#ifndef RASPBERRY
                        al_play_sample(samples[2], 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
#else
                        set_file_to_play(songs[2]);
                        play_sound();
#endif
                        if (i <= ((CANT_ALIENS / 5) - 1)) {
                            (*score) += 30;
                        } else if (i <= ((3 * CANT_ALIENS / 5) - 1)) {
                            (*score) += 20;
                        } else {
                            (*score) += 10;
                        }
                    }
                }
            } else {
                if (*bullet_y == alien_y[i] && *bullet_x == alien_x[i]) {
                    (*cant_aliens)--;
                    alien_y[i] = SCREEN_H; //mueve el alien muerto fuera de la pantalla 
                    *lock = false;
                    *bullet_y = *nave_y;

                    if (i <= ((CANT_ALIENS / 4) - 1)) {
                        (*score) += 30;
                    } else if (i <= ((3 * CANT_ALIENS / 4) - 1)) {
                        (*score) += 20;
                    } else {
                        (*score) += 10;
                    }
                    
                }
            }
        }


        for (j = 0; j < 4; j++) {
            if (vida_bloques[j]) {
                if ((alien_bullets_y[i] + BASE_SIZE) >= *bloques_y && (alien_bullets_y[i] + BASE_SIZE) <= *bloques_y + BASE_SIZE && alien_bullets_x[i] >= bloques_x[j] && alien_bullets_x[i] <= bloques_x[j] + 4 * BASE_SIZE) {
                    alien_bullets_y[i] = SCREEN_H;
                    vida_bloques[j]--;
                }
            }
        }

        if (alien_bullets_y[i] >= *nave_y && alien_bullets_y[i] <= *nave_y + BASE_SIZE && alien_bullets_x[i] >= *nave_x && alien_bullets_x[i] <= *nave_x + 3 * BASE_SIZE) {
            alien_bullets_y[i] = SCREEN_H;
            (*lives)--;
#ifndef RASPBERRY
                al_play_sample(samples[3], 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
#else
                set_file_to_play(songs[3]);
                play_sound();
#endif

        }
        if(!mode){
            if (alien_bullets_x[i] >= *bullet_x - 4 && alien_bullets_x[i] <= *bullet_x + 4 && alien_bullets_y[i] >= *bullet_y && alien_bullets_y[i] <= *bullet_y + BASE_SIZE) {
                *lock = false;
                *bullet_y = SCREEN_H;
                alien_bullets_y[i] = SCREEN_H;
            }
        }
        else{
            if (alien_bullets_x[i] == *bullet_x && alien_bullets_y[i]== *bullet_y) {
                *lock = false;
                *bullet_y = SCREEN_H;
                alien_bullets_y[i] = SCREEN_H;
            }
        }
            
        if (alien_y[i] > ((3 * SCREEN_H / 4)-1) && alien_y[i] < SCREEN_H) //condicion de derrota
            return true;

    }
            if(*mystery_ship_x < SCREEN_W){
#ifndef RASPBERRY
                    if(*bullet_y>=*mystery_ship_y && *bullet_y<=*mystery_ship_y+2*BASE_SIZE){
                        if(*bullet_x>=*mystery_ship_x && *bullet_x<=*mystery_ship_x+2*BASE_SIZE){
#else
                    if(*bullet_y==*mystery_ship_y){
                        if(*bullet_x==*mystery_ship_x ){
#endif
                            *mystery_ship_x = SCREEN_W;
                            *lock_mystery_ship=false;
                            *lock = false;
                            *bullet_y = *nave_y;
                            *score+=5*get_rand_num(21);       //te puede sumar desde 0 a 100 puntos.
#ifndef RASPBERRY
                            al_play_sample(samples[2], 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
#else
                            set_file_to_play(songs[2]);
                            play_sound();
#endif
                        }
                    }
                
            }
            if(*mystery_ship_x+2*BASE_SIZE<=0){
                *mystery_ship_y = SCREEN_H;      //si la nave se sale de la pantalla , reseteo sus valores.
                *mystery_ship_x = SCREEN_W;
                *lock_mystery_ship=false;
            }
    return false;
}
             