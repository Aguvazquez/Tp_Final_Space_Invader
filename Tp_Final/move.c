#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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

static int create_bitmaps(ALLEGRO_BITMAP **bullet, ALLEGRO_BITMAP **bloque, ALLEGRO_DISPLAY **display );

int move(ALLEGRO_DISPLAY** display,ALLEGRO_FONT *font[], ALLEGRO_EVENT_QUEUE** event_queue, ALLEGRO_TIMER **timer, ALLEGRO_BITMAP *display_background[], uint8_t speed)
{
    
    ALLEGRO_BITMAP *nave = NULL;
    ALLEGRO_BITMAP *bullet = NULL;
    ALLEGRO_BITMAP *alien = NULL;
    ALLEGRO_BITMAP *bloque = NULL;
    
    uint8_t i, check, aux;
    uint8_t vida_bloques[4] = {25, 25, 25, 25}; // no puede ser static , porque si no los bloques no respawnean
    bool alien_change=false;
    float nave_x = SCREEN_W / 2.0 - BASE_SIZE;
    float nave_y = SCREEN_H - BASE_SIZE;
    float bullet_x, bullet_y;
    float bloques_x[4];
    
    for(i=0; i<4; i++)
        bloques_x[i] = (i+1)*SCREEN_W / 5 - 1.5*BASE_SIZE;
    
    float bloques_y = 3*SCREEN_H / 4 + 1.5*BASE_SIZE;
    uint8_t cant_aliens = N;
    //uint8_t cant_aliens = 1;    //solo para facilitar debug
    
    //seteo de coordenadas iniciales de los aliens
    
    float alien_x[N] = {3*SCREEN_W/11, 4*SCREEN_W/11, 5*SCREEN_W/11, 6*SCREEN_W/11, 7*SCREEN_W/11,8*SCREEN_W/11,
                        3*SCREEN_W/11, 4*SCREEN_W/11, 5*SCREEN_W/11, 6*SCREEN_W/11, 7*SCREEN_W/11,8*SCREEN_W/11,
                        3*SCREEN_W/11, 4*SCREEN_W/11, 5*SCREEN_W/11, 6*SCREEN_W/11, 7*SCREEN_W/11,8*SCREEN_W/11,
                        3*SCREEN_W/11, 4*SCREEN_W/11, 5*SCREEN_W/11, 6*SCREEN_W/11, 7*SCREEN_W/11,8*SCREEN_W/11,
                        3*SCREEN_W/11, 4*SCREEN_W/11, 5*SCREEN_W/11, 6*SCREEN_W/11, 7*SCREEN_W/11,8*SCREEN_W/11};
    
    float alien_y[N] = {SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, SCREEN_H/10, 
                        SCREEN_H/5, SCREEN_H/5, SCREEN_H/5, SCREEN_H/5, SCREEN_H/5, SCREEN_H/5,
                        3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10, 3*SCREEN_H/10,  
                        2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 2*SCREEN_H/5, 
                        SCREEN_H/2, SCREEN_H/2, SCREEN_H/2, SCREEN_H/2, SCREEN_H/2, SCREEN_H/2};
    
    int8_t step = BASE_SIZE/2;
    bool key_pressed[3] = {false, false, false};    //estado de teclas, true cuando esta apretada
    bool redraw = false;
    bool do_exit = false;
    bool lock = false;
    bool close_display= false ;
    
    
    if(!create_bitmaps(&bullet,&bloque,display)){
        fprintf(stderr,"fail to create bitmap");
        return -1;
    }

    
    al_start_timer(*timer);

    while (!do_exit && cant_aliens) {
        ALLEGRO_EVENT ev;
        if (al_get_next_event(*event_queue, &ev)) //toma un evento de la cola.
        {
            if (ev.type == ALLEGRO_EVENT_TIMER) { //controles de movimiento de la nave

                if (key_pressed[KEY_LEFT] && nave_x >= MOVE_RATE)
                    nave_x -= MOVE_RATE;

                if (key_pressed[KEY_RIGHT] && nave_x <= SCREEN_W - 2*BASE_SIZE - MOVE_RATE)
                    nave_x += MOVE_RATE;
                
                if(aux>=speed)
                {
                     if(alien_change)
                        alien_change=false;
                    else
                        alien_change=true;
            
                    for(i=0, check=0; i<N; i++)
                    {
                        if(alien_y[i] < SCREEN_H)
                            if(alien_x[i] >= SCREEN_W-2*BASE_SIZE || alien_x[i] <= BASE_SIZE) //revisa que no sobrepasen los extremos
                                check++;
                    }
                    if(check)
                    {
                        step *= -1;
                        for(i=0; i<N; i++)
                            alien_y[i] += BASE_SIZE;
                    }
                    for(i=0; i<N; i++)
                        alien_x[i] += step;
                    aux=0;
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
                            bullet_x = nave_x + BASE_SIZE;    //setea la bala
                            bullet_y = nave_y - BASE_SIZE;   
                            lock = true;   // solo dispara si no hay otra bala volando.
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
                                return CLOSE_DISPLAY;
                            }
                            break;
                            case 1:     //Resume 
                            break;
                            case 2:{    //Reset level
                                return RESET_LEVEL;
                            }
                            break;
                            case 3:{    //go back to main menu.
                                do_exit=true;
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
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_scaled_bitmap(display_background[2],0, 0, al_get_bitmap_width(display_background[2]), al_get_bitmap_height(display_background[2]), 
            nave_x, nave_y, 3*BASE_SIZE, 1.5*BASE_SIZE,0);
            //al_draw_bitmap(display_background[2], nave_x, nave_y-BASE_SIZE, 0);
            for(i=0; i<4; i++)
                if(vida_bloques[i])
                    al_draw_bitmap(bloque, bloques_x[i], bloques_y, 0);
            for(i=0, check=0; i<N; i++)
            {
                if(alien_y[i] < SCREEN_H){
                    if(alien_change){
                    al_draw_scaled_bitmap(display_background[3],0, 0, al_get_bitmap_width(display_background[3]), al_get_bitmap_height(display_background[3]), 
             alien_x[i], alien_y[i],2*BASE_SIZE, 2*BASE_SIZE,0); 
                    
                    }
                    else{
                        al_draw_scaled_bitmap(display_background[4],0, 0, al_get_bitmap_width(display_background[4]), al_get_bitmap_height(display_background[4]), 
             alien_x[i], alien_y[i],2*BASE_SIZE, 2*BASE_SIZE,0); 
                    
                    }
                }
                    //al_draw_bitmap(display_background[3], alien_x[i], alien_y[i], 0);   //dibuja cada alien
                if(alien_x[i] >= SCREEN_W-2*BASE_SIZE || alien_x[i] <= BASE_SIZE) //revisa que no sobrepasen los extremos
                    check++;
            }
           
            if(lock)
            {
                al_draw_bitmap(bullet, bullet_x, bullet_y, 0);
                bullet_y -= 2*MOVE_RATE;        //actualiza la posicion de la bala en cada ciclo
                for(i=0, check=0; i<4; i++)
                {
                    if(vida_bloques[i]){
                        if(nave_x>=bloques_x[i] && nave_x<=bloques_x[i]+3*BASE_SIZE){
                            vida_bloques[i]--;
                            lock = false;
                        }
                    }
                }
            }
            if(bullet_y <= MOVE_RATE)           //si pega arriba desaparece la bala
                lock = false;
            for(i=0; i<N; i++)
            {
                if(bullet_y>=alien_y[i] && bullet_y<=alien_y[i]+BASE_SIZE && bullet_x>=alien_x[i] && bullet_x<=alien_x[i]+BASE_SIZE)
                {
                    alien_y[i] = SCREEN_H;      //mueve el alien muerto fuera de la pantalla 
                    cant_aliens--;
                    lock = false;
                    bullet_y = nave_y;
                }
                if(alien_y[i]>3*SCREEN_H/4 && alien_y[i]<SCREEN_H)
                    do_exit = true;
            }
            al_flip_display();
        }
    }

    al_destroy_bitmap(alien);
    al_destroy_bitmap(bloque);
    al_stop_timer(*timer);
    return cant_aliens;
}

static int create_bitmaps(ALLEGRO_BITMAP **bullet,ALLEGRO_BITMAP **bloque,ALLEGRO_DISPLAY **display ){
    
    
    *bullet = al_create_bitmap(1, BASE_SIZE);
    if (!bullet) {
        fprintf(stderr, "failed to create bullet bitmap!\n");
        return 0;
    }
    
    

    *bloque = al_create_bitmap(3*BASE_SIZE, BASE_SIZE);
    if (!bloque) {
        fprintf(stderr, "failed to create alien bitmap!\n");
        al_destroy_bitmap(*bullet);
        return 0;
    }
    
    al_set_target_bitmap(*bullet);
    al_clear_to_color(al_map_rgb(255, 255, 255));
   // al_set_target_bitmap(*alien);
   // al_clear_to_color(al_map_rgb(0, 255, 0));
    al_set_target_bitmap(*bloque);
    al_clear_to_color(al_map_rgb(255, 0, 0));
    al_set_target_bitmap(al_get_backbuffer(*display));
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    return 1;
}