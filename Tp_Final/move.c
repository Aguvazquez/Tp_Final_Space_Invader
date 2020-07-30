#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <allegro5/allegro.h>  
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_ttf.h> 
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h> 
#include "config.h"
#include "move.h"
#include "menus.h"

enum MYKEYS {KEY_LEFT, KEY_RIGHT, KEY_SPACE};

static uint16_t get_rand_num(uint8_t x);
/*Recibe un entero positivo y devuelve un entero aleatorio menor a dicho numero*/
static void score_to_str(uint16_t score,ALLEGRO_FONT**font);

int move(ALLEGRO_SAMPLE* sample[], ALLEGRO_DISPLAY** display,ALLEGRO_FONT *font[], ALLEGRO_EVENT_QUEUE** event_queue, ALLEGRO_TIMER **timer, ALLEGRO_BITMAP *display_background[], uint8_t difficulty, uint8_t* lifes, uint8_t level)
{
    uint8_t i, j, check, aux, accelerate=0, explosion_time=0;
    static uint8_t vida_bloques[4] = {30, 30, 30, 30};
    bool alien_change = false;
    float nave_x = SCREEN_W / 2.0 - 1.5*BASE_SIZE;
    float nave_y = SCREEN_H - 2*BASE_SIZE;
    float bullet_x, bullet_y;
    float bloques_x[4];
    float explosion_x, explosion_y;
    static uint16_t score=0;
    for(i=0; i<4; i++)
        bloques_x[i] = (1.5*i+1)*SCREEN_W / 6.5 - 2*BASE_SIZE;
    
    float bloques_y = 3*SCREEN_H / 4 + 2.5*BASE_SIZE;
    int8_t cant_aliens = N;
    //int8_t cant_aliens = 1;    //solo para facilitar debug
    
    //seteo de coordenadas iniciales de los aliens   
    float alien_x[N] = {3*SCREEN_W/15, 4*SCREEN_W/15, 5*SCREEN_W/15, 6*SCREEN_W/15, 7*SCREEN_W/15, 8*SCREEN_W/15, 9*SCREEN_W/15, 10*SCREEN_W/15, 11*SCREEN_W/15, 12*SCREEN_W/15,
                        3*SCREEN_W/15, 4*SCREEN_W/15, 5*SCREEN_W/15, 6*SCREEN_W/15, 7*SCREEN_W/15, 8*SCREEN_W/15, 9*SCREEN_W/15, 10*SCREEN_W/15, 11*SCREEN_W/15, 12*SCREEN_W/15,
                        3*SCREEN_W/15, 4*SCREEN_W/15, 5*SCREEN_W/15, 6*SCREEN_W/15, 7*SCREEN_W/15, 8*SCREEN_W/15, 9*SCREEN_W/15, 10*SCREEN_W/15, 11*SCREEN_W/15, 12*SCREEN_W/15,
                        3*SCREEN_W/15, 4*SCREEN_W/15, 5*SCREEN_W/15, 6*SCREEN_W/15, 7*SCREEN_W/15, 8*SCREEN_W/15, 9*SCREEN_W/15, 10*SCREEN_W/15, 11*SCREEN_W/15, 12*SCREEN_W/15,
                        3*SCREEN_W/15, 4*SCREEN_W/15, 5*SCREEN_W/15, 6*SCREEN_W/15, 7*SCREEN_W/15, 8*SCREEN_W/15, 9*SCREEN_W/15, 10*SCREEN_W/15, 11*SCREEN_W/15, 12*SCREEN_W/15};
    
    float alien_y[N] = {SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, 
                        SCREEN_H/5, SCREEN_H/5, SCREEN_H/5, SCREEN_H/5, SCREEN_H/5, SCREEN_H/5, SCREEN_H/5, SCREEN_H/5, SCREEN_H/5, SCREEN_H/5,
                        3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10,  
                        2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5,
                        SCREEN_H/2, SCREEN_H/2, SCREEN_H/2, SCREEN_H/2, SCREEN_H/2, SCREEN_H/2, SCREEN_H/2, SCREEN_H/2, SCREEN_H/2, SCREEN_H/2};
    
    float mystery_ship_y=SCREEN_H; 
    float mystery_ship_x=SCREEN_W; 
    
    float alien_bullets[2][N];
    
    for(i=0; i<N; i++)
    {
        alien_bullets[1][i]=SCREEN_H;
    }
    
    int8_t step = BASE_SIZE/2;
    bool key_pressed[3] = {false, false, false};    //estado de teclas, true cuando esta apretada
    bool redraw = false;
    bool do_exit = false;
    bool lock = false;
    bool lock_mystery_ship = false;
         
    al_set_target_bitmap(al_get_backbuffer(*display));
    al_start_timer(*timer);

    while (!do_exit && cant_aliens && *lifes) {
        ALLEGRO_EVENT ev;
        if (al_get_next_event(*event_queue, &ev)) //toma un evento de la cola.
        {
            if (ev.type == ALLEGRO_EVENT_TIMER) { //controles de movimiento de la nave

                if (key_pressed[KEY_LEFT] && nave_x >= MOVE_RATE)
                    nave_x -= MOVE_RATE;

                if (key_pressed[KEY_RIGHT] && nave_x <= SCREEN_W - 3*BASE_SIZE - MOVE_RATE)
                    nave_x += MOVE_RATE;
                
                if(aux>=difficulty)
                {
                    if(alien_change)
                        alien_change=false;
                    else
                        alien_change=true;
                    
                    if(!lock_mystery_ship){
                        if(!get_rand_num(20)){          //5% de probabilidad.
                            lock_mystery_ship=true;
                            mystery_ship_y=1.5*BASE_SIZE;
                        }
                    }
                      
                    for(i=0, check=0; i<N; i++)
                    {
                        if(alien_y[i] < SCREEN_H)
                            if(alien_x[i] >= SCREEN_W-2.5*BASE_SIZE || alien_x[i] <= BASE_SIZE/2) //revisa que no sobrepasen los extremos
                                check++;
                    }
                    if(check)
                    {
                        accelerate++;
                        if(accelerate >= 4 && difficulty >= 8){
                            difficulty -= 4;
                            accelerate = 0;
                        }
                        step *= -1;
                        for(i=0; i<N; i++)
                            alien_y[i] += BASE_SIZE;
                        
                    }
                    for(i=0; i<N; i++){
                        alien_x[i] += step;
                    }
                    
                    if(!get_rand_num(3)){               //33% de probabilidad
                        aux = get_rand_num(cant_aliens);
                        for(i=0; i<N; i++){          // para cada alien
                            if(alien_y[i]<SCREEN_H){        // si esta vivo
                                if(!(aux--)){
                                    if(alien_bullets[1][i]>=SCREEN_H){
                                        alien_bullets[0][i]=alien_x[i]+BASE_SIZE; //coordenada x
                                        alien_bullets[1][i]=alien_y[i]; //le asigno coordenada y del alien a la bala
                                    }
                                }
                            }
                        }
                    }
                    
                    aux=0;
                }
                
                if(lock_mystery_ship){
                        mystery_ship_x-=MOVE_RATE/2;
                    }
                redraw = true;
                aux++;
            }
            else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                return CLOSE_DISPLAY;

            else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (ev.keyboard.keycode) {

                    case ALLEGRO_KEY_LEFT:
                        key_pressed[KEY_LEFT] = true;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        key_pressed[KEY_RIGHT] = true;
                        break;       
                        
                    case ALLEGRO_KEY_SPACE:
                        if(!lock){
                            bullet_x = nave_x + 1.5*BASE_SIZE;    //setea la bala
                            bullet_y = nave_y;   
                            lock = true;   // solo dispara si no hay otra bala volando.
                            al_play_sample(sample[1], 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        }
                    break;
                }
            }
            else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
                switch (ev.keyboard.keycode) {

                    case ALLEGRO_KEY_LEFT:
                        key_pressed[KEY_LEFT] = false;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        key_pressed[KEY_RIGHT] = false;
                        break;
                    
                    case ALLEGRO_KEY_ESCAPE:{
                        switch(pause_menu(display,event_queue,font,display_background)){
                            case 0:{
                                cant_aliens = CLOSE_DISPLAY;
                                do_exit = true;
                            }
                            break;
                            case 1:     //Resume 
                            break;
                            case 2:{    //Reset level
                                cant_aliens= RESET_GAME;
                                do_exit=true;
                                
                            }
                            break;
                            case 3:{    //go back to main menu.
                                cant_aliens=EXIT_MENU;
                                do_exit = true;
                            }
                            break;
                            default:{
                                fprintf(stderr,"Something gone wrong");
                                return -1;
                            }
                            break;
                        }
                        
                    }   
                }
            }
        }

        if (redraw && al_is_event_queue_empty(*event_queue)) {
            
            redraw = false;
            al_draw_scaled_bitmap(display_background[11+level%5],0, 0, al_get_bitmap_width(display_background[11+level%5]), al_get_bitmap_height(display_background[11+level%5]), 
            0, 0, SCREEN_W, SCREEN_H, 0);
            al_draw_scaled_bitmap(display_background[2],0, 0, al_get_bitmap_width(display_background[2]), al_get_bitmap_height(display_background[2]), 
            nave_x, nave_y, 3*BASE_SIZE, 1.5*BASE_SIZE, 0);
            
            for(i=0; i<*lifes; i++)
                al_draw_scaled_bitmap(display_background[5],0, 0, al_get_bitmap_width(display_background[5]), al_get_bitmap_height(display_background[5]), 
                1.5*i*BASE_SIZE, 0, 2*BASE_SIZE, 2*BASE_SIZE, 0);
            
            for(i=0; i<4; i++)
                if(vida_bloques[i]>=20){
                    al_draw_filled_rectangle(bloques_x[i], bloques_y, bloques_x[i]+4*BASE_SIZE, bloques_y+BASE_SIZE, al_map_rgb(0, 255, 0));
                    al_draw_filled_rectangle(bloques_x[i], bloques_y+BASE_SIZE, bloques_x[i]+BASE_SIZE, bloques_y+2*BASE_SIZE, al_map_rgb(0, 255, 0));
                    al_draw_filled_rectangle(bloques_x[i]+3*BASE_SIZE, bloques_y+BASE_SIZE, bloques_x[i]+4*BASE_SIZE, bloques_y+2*BASE_SIZE, al_map_rgb(0, 255, 0));
                }
                else if(vida_bloques[i]>=10){
                    al_draw_filled_rectangle(bloques_x[i], bloques_y, bloques_x[i]+4*BASE_SIZE, bloques_y+BASE_SIZE, al_map_rgb(255, 255, 0));
                    al_draw_filled_rectangle(bloques_x[i], bloques_y+BASE_SIZE, bloques_x[i]+BASE_SIZE, bloques_y+2*BASE_SIZE, al_map_rgb(255, 255, 0));
                    al_draw_filled_rectangle(bloques_x[i]+3*BASE_SIZE, bloques_y+BASE_SIZE, bloques_x[i]+4*BASE_SIZE, bloques_y+2*BASE_SIZE, al_map_rgb(255, 255, 0));
                }
                else if(vida_bloques[i]){
                    al_draw_filled_rectangle(bloques_x[i], bloques_y, bloques_x[i]+4*BASE_SIZE, bloques_y+BASE_SIZE, al_map_rgb(255, 0, 0));
                    al_draw_filled_rectangle(bloques_x[i], bloques_y+BASE_SIZE, bloques_x[i]+BASE_SIZE, bloques_y+2*BASE_SIZE, al_map_rgb(255, 0, 0));
                    al_draw_filled_rectangle(bloques_x[i]+3*BASE_SIZE, bloques_y+BASE_SIZE, bloques_x[i]+4*BASE_SIZE, bloques_y+2*BASE_SIZE, al_map_rgb(255, 0, 0));
                }
                    
            for(i=0, check=0; i<N; i++)
            {
                if(alien_y[i] < SCREEN_H){
                        if(alien_change){
                            if(i<=((N/5)-1)){
                            al_draw_scaled_bitmap(display_background[8],0, 0, al_get_bitmap_width(display_background[8]), al_get_bitmap_height(display_background[8]), 
                            alien_x[i], alien_y[i], 2*BASE_SIZE, 2*BASE_SIZE,0); 
                        }
                        else if(i<=((3*N/5)-1)){
                            al_draw_scaled_bitmap(display_background[3],0, 0, al_get_bitmap_width(display_background[3]), al_get_bitmap_height(display_background[3]), 
                            alien_x[i], alien_y[i], 2*BASE_SIZE, 2*BASE_SIZE,0);
                        }
                        else{
                            al_draw_scaled_bitmap(display_background[9],0, 0, al_get_bitmap_width(display_background[9]), al_get_bitmap_height(display_background[9]), 
                            alien_x[i], alien_y[i], 2*BASE_SIZE, 2*BASE_SIZE,0);
                        }
                        
                    }
                    else{
                        
                        if(i<=((N/5)-1)){
                            al_draw_scaled_bitmap(display_background[7],0, 0, al_get_bitmap_width(display_background[7]), al_get_bitmap_height(display_background[7]), 
                            alien_x[i], alien_y[i], 2*BASE_SIZE, 2*BASE_SIZE,0); 
                        }
                        else if(i<=((3*N/5)-1)){
                            al_draw_scaled_bitmap(display_background[4],0, 0, al_get_bitmap_width(display_background[4]), al_get_bitmap_height(display_background[4]), 
                            alien_x[i], alien_y[i], 2*BASE_SIZE, 2*BASE_SIZE,0);
                        }
                        else{
                            al_draw_scaled_bitmap(display_background[10],0, 0, al_get_bitmap_width(display_background[10]), al_get_bitmap_height(display_background[10]), 
                            alien_x[i], alien_y[i], 2.5*BASE_SIZE, 2.5*BASE_SIZE,0); 
                        } 
                    }
                }
                if(alien_x[i] >= SCREEN_W-2*BASE_SIZE || alien_x[i] <= BASE_SIZE) //revisa que no sobrepasen los extremos
                    check++;
            }
            
            al_draw_scaled_bitmap(display_background[6],0, 0, al_get_bitmap_width(display_background[6]), al_get_bitmap_height(display_background[6]), 
            mystery_ship_x,mystery_ship_y, 2*BASE_SIZE, 2*BASE_SIZE,0);
           
            if(lock)
            {
                al_draw_rectangle(bullet_x-1, bullet_y, bullet_x+1, bullet_y+BASE_SIZE, al_map_rgb(255, 0, 0), 0);
                bullet_y -= 2*MOVE_RATE;        //actualiza la posicion de la bala en cada ciclo
                if(bullet_y <= bloques_y + BASE_SIZE)
                for(i=0, check=0; i<4; i++)
                {
                    if(vida_bloques[i]){
                        if(bullet_x>=bloques_x[i] && bullet_x<=bloques_x[i]+4*BASE_SIZE){
                            vida_bloques[i]--;
                            lock = false;
                        }
                    }
                }
            }
            
            if(bullet_y <= MOVE_RATE)           //si pega arriba desaparece la bala
                lock = false;
            
            for(i=0; i<N; i++){
                if(alien_y[i] < SCREEN_H){
                    if(bullet_y>=alien_y[i] && bullet_y<=alien_y[i]+2*BASE_SIZE){
                        if(bullet_x>=alien_x[i] && bullet_x<=alien_x[i]+2*BASE_SIZE){

                            cant_aliens--;
                            explosion_x = alien_x[i];
                            explosion_y = alien_y[i];
                            explosion_time = 10;
                            alien_y[i] = SCREEN_H;      //mueve el alien muerto fuera de la pantalla 
                            lock = false;
                            bullet_y = nave_y;
                            al_play_sample(sample[2], 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

                            if(i<=((N/5)-1)){
                                score+=15;
                            }
                            else if(i<=((3*N/5)-1)){
                                score+=10;
                            }
                            else{
                                score+=5;
                            }
                        }
                    }
                }
                
                for(j=0; j<4; j++){
                    if(vida_bloques[j]){
                        if((alien_bullets[1][i]+BASE_SIZE)>=bloques_y && (alien_bullets[1][i]+BASE_SIZE)<=bloques_y+BASE_SIZE && alien_bullets[0][i]>=bloques_x[j] && alien_bullets[0][i]<=bloques_x[j]+4*BASE_SIZE){
                            alien_bullets[1][i] = SCREEN_H;
                            vida_bloques[j]--;
                        }
                    }
                }
                
                if(alien_bullets[1][i]>=nave_y && alien_bullets[1][i]<=nave_y+BASE_SIZE && alien_bullets[0][i]>=nave_x && alien_bullets[0][i]<=nave_x+3*BASE_SIZE){
                    alien_bullets[1][i] = SCREEN_H;
                    (*lifes)--;
                    al_play_sample(sample[3], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    
                }
                
                if(alien_bullets[0][i]>=bullet_x-4 && alien_bullets[0][i]<=bullet_x+4 && alien_bullets[1][i]>=bullet_y && alien_bullets[1][i]<=bullet_y+BASE_SIZE){
                    lock = false;
                    bullet_y = SCREEN_H;
                    alien_bullets[1][i] = SCREEN_H;
                }
                
                al_draw_rectangle(alien_bullets[0][i]-1, alien_bullets[1][i], alien_bullets[0][i]+1, alien_bullets[1][i]+BASE_SIZE, al_map_rgb(255, 255, 255), 0);    //dibujo balas aliens
                alien_bullets[1][i] += 1.5*MOVE_RATE;        
                 
                if(alien_y[i]>3*SCREEN_H/4 && alien_y[i]<SCREEN_H)  //condicion de derrota
                    do_exit = true;
                
            }
            if(mystery_ship_x < SCREEN_W){
                if(bullet_y>=mystery_ship_y && bullet_y<=mystery_ship_y+2*BASE_SIZE){
                    if(bullet_x>=mystery_ship_x && bullet_x<=mystery_ship_x+2*BASE_SIZE){

                        mystery_ship_x = SCREEN_W;
                        lock_mystery_ship=false;
                        lock = false;
                        bullet_y = nave_y;
                        score+=5*get_rand_num(21);       //te puede sumar desde 0 a 100 puntos.
                            
                    }
                }
            }
            if(mystery_ship_x<0){
                mystery_ship_y = SCREEN_H;      //si la nave se sale de la pantalla , reseteo sus valores.
                mystery_ship_x = SCREEN_W;
                lock_mystery_ship=false;
            }
            
            if(explosion_time){
                al_draw_scaled_bitmap(display_background[17],0, 0, al_get_bitmap_width(display_background[17]), al_get_bitmap_height(display_background[17]), 
                explosion_x, explosion_y, 2.5*BASE_SIZE, 2.5*BASE_SIZE,0);
                explosion_time--;
            }
            score_to_str(score,font);
            al_flip_display();
        }
    }

    al_stop_timer(*timer);
    if(cant_aliens){
        for(i=0; i<4; i++)
            vida_bloques[i]=30;
        score=0;
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
static void score_to_str(uint16_t score, ALLEGRO_FONT**font){
     char str[13]={'S','C','O','R','E',':',' ',' ','0','0','0','0'};
     uint16_t aux=0,i,j;
     for(i=11,j=1; i>7; i--,j*=10){
        aux=score/j;
        str[i]=(char)(aux%10+ASCII);        
     }
     al_draw_text(font[0], al_map_rgb(255,255,255), SCREEN_W, BASE_SIZE/4, ALLEGRO_ALIGN_RIGHT, str);
     
}