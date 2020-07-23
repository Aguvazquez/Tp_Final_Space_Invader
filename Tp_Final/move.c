#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <allegro5/allegro.h>  
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h>
#include "config.h"
#include "move.h"

enum MYKEYS {KEY_LEFT, KEY_RIGHT, KEY_SPACE};

static int create_bitmaps(ALLEGRO_BITMAP **bullet, ALLEGRO_BITMAP **alien, ALLEGRO_DISPLAY **display );

int move(ALLEGRO_DISPLAY** display, ALLEGRO_EVENT_QUEUE** event_queue, ALLEGRO_TIMER **timer, ALLEGRO_BITMAP *display_background[])
{
    
    ALLEGRO_BITMAP *nave = NULL;
    ALLEGRO_BITMAP *bullet = NULL;
    ALLEGRO_BITMAP *alien = NULL;
    
    float nave_x = SCREEN_W / 2.0 - ALIEN_SIZE;
    float nave_y = SCREEN_H - ALIEN_SIZE;
    float bullet_x, bullet_y;
    int cant_aliens = N;
    //int cant_aliens = 1;    //solo para facilitar debug
    
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
    
    uint8_t i, check, aux;
    int8_t step = ALIEN_SIZE/2;
    bool key_pressed[3] = {false, false, false};    //estado de teclas, true cuando esta apretada
    bool redraw = false;
    bool do_exit = false;
    bool lock = false;
    
    
    if(!create_bitmaps(&bullet,&alien,display)){
        fprintf(stderr,"fail to create bitmap");
        return -1;
    }

    
    al_start_timer(*timer);

    while (!do_exit || !cant_aliens) {
        ALLEGRO_EVENT ev;
        if (al_get_next_event(*event_queue, &ev)) //toma un evento de la cola.
        {
            if (ev.type == ALLEGRO_EVENT_TIMER) { //controles de movimiento de la nave

                if (key_pressed[KEY_LEFT] && nave_x >= MOVE_RATE)
                    nave_x -= MOVE_RATE;

                if (key_pressed[KEY_RIGHT] && nave_x <= SCREEN_W - 2*ALIEN_SIZE - MOVE_RATE)
                    nave_x += MOVE_RATE;
                
                if(aux>=25)
                {
                    for(i=0, check=0; i<N; i++)
                    {
                        if(alien_y[i] < SCREEN_H)
                            if(alien_x[i] >= SCREEN_W-2*ALIEN_SIZE || alien_x[i] <= ALIEN_SIZE) //revisa que no sobrepasen los extremos
                                check++;
                    }
                    if(check)
                    {
                        step *= -1;
                        for(i=0; i<N; i++)
                            alien_y[i] += ALIEN_SIZE;
                    }
                    for(i=0; i<N; i++)
                        alien_x[i] += step;
                    aux=0;
                }
                
                redraw = true;
                aux++;
            }
            else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                do_exit = true;

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
                            bullet_x = nave_x + ALIEN_SIZE;    //setea la bala
                            bullet_y = nave_y - ALIEN_SIZE;   
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
                    
                    case ALLEGRO_KEY_ESCAPE:
                        do_exit = true;
                        break;
                }
            }
        }

        if (redraw && al_is_event_queue_empty(*event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(display_background[2], nave_x, nave_y-ALIEN_SIZE, 0);
            for(i=0, check=0; i<N; i++)
            {
                if(alien_y[i] < SCREEN_H)
                    al_draw_bitmap(alien, alien_x[i], alien_y[i], 0);   //dibuja cada alien
                if(alien_x[i] >= SCREEN_W-2*ALIEN_SIZE || alien_x[i] <= ALIEN_SIZE) //revisa que no sobrepasen los extremos
                    check++;
            }
           
            
            if(lock)
            {
                al_draw_bitmap(bullet, bullet_x, bullet_y, 0);
                bullet_y -= 2*MOVE_RATE;        //actualiza la posicion de la bala en cada ciclo
            }
            if(bullet_y <= MOVE_RATE)
                lock = false;
            for(i=0; i<N; i++)
            {
                if(bullet_y>=alien_y[i] && bullet_y<=alien_y[i]+ALIEN_SIZE && bullet_x>=alien_x[i] && bullet_x<=alien_x[i]+ALIEN_SIZE)
                {
                    alien_y[i] = SCREEN_H;      //mueve el alien muerto fuera de la pantalla 
                    cant_aliens--;
                    lock = false;
                }
            }
            al_flip_display();
        }
    }

    al_destroy_bitmap(alien);
    al_stop_timer(*timer);
    return cant_aliens;
}

static int create_bitmaps(ALLEGRO_BITMAP **bullet,ALLEGRO_BITMAP **alien,ALLEGRO_DISPLAY **display ){
    
    
    *bullet = al_create_bitmap(1, ALIEN_SIZE);
    if (!bullet) {
        fprintf(stderr, "failed to create bullet bitmap!\n");
        return 0;
    }
    
    *alien = al_create_bitmap(ALIEN_SIZE, ALIEN_SIZE);
    if (!alien) {
        fprintf(stderr, "failed to create alien bitmap!\n");
        al_destroy_bitmap(*bullet);
        return 0;
    }
    al_set_target_bitmap(*bullet);
    al_clear_to_color(al_map_rgb(255, 255, 255));
    al_set_target_bitmap(*alien);
    al_clear_to_color(al_map_rgb(0, 255, 0));
    al_set_target_bitmap(al_get_backbuffer(*display));
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    return 1;
}