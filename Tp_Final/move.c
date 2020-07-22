#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <allegro5/allegro.h>  
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h>

#define FPS    60.0
#define SCREEN_W  640
#define SCREEN_H  480
#define ALIEN_SIZE 25
#define MOVE_RATE  4.0
#define N 10

enum MYKEYS {
    KEY_LEFT, KEY_RIGHT, KEY_SPACE //arrow keys
};

int move()
{
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *nave = NULL;
    ALLEGRO_BITMAP *bullet = NULL;
    ALLEGRO_BITMAP *alien = NULL;
    
    float nave_x = SCREEN_W / 2.0 - ALIEN_SIZE;
    float nave_y = SCREEN_H - ALIEN_SIZE;
    float bullet_x, bullet_y;
    float alien_x[N] = {SCREEN_W/6.0, SCREEN_W/3.0, SCREEN_W/2.0, 2*SCREEN_W/3.0, 5*SCREEN_W/6.0,
                        SCREEN_W/6.0, SCREEN_W/3.0, SCREEN_W/2.0, 2*SCREEN_W/3.0, 5*SCREEN_W/6.0};
    float alien_y[N] = {SCREEN_H/4.0, SCREEN_H/4.0, SCREEN_H/4.0, SCREEN_H/4.0, SCREEN_H/4.0,
                        SCREEN_H/2.0, SCREEN_H/2.0, SCREEN_H/2.0, SCREEN_H/2.0, SCREEN_H/2.0, };
    int i;

    bool key_pressed[3] = {false, false, false}; //Estado de teclas, true cuando esta apretada
    bool redraw = false;
    bool do_exit = false;
    bool shoot = false;
    bool lock = false;
    
    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    if (!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        return -1;
    }

    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }

    nave = al_create_bitmap(2*ALIEN_SIZE, ALIEN_SIZE);
    if (!nave) {
        fprintf(stderr, "failed to create nave bitmap!\n");
        al_destroy_timer(timer);
        return -1;
    }
    
    bullet = al_create_bitmap(1, ALIEN_SIZE);
    if (!bullet) {
        fprintf(stderr, "failed to create bullet bitmap!\n");
        al_destroy_timer(timer);
        al_destroy_bitmap(nave);
        return -1;
    }
    
    alien = al_create_bitmap(ALIEN_SIZE, ALIEN_SIZE);
    if (!alien) {
        fprintf(stderr, "failed to create alien bitmap!\n");
        al_destroy_timer(timer);
        al_destroy_bitmap(nave);
        al_destroy_bitmap(bullet);
        return -1;
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_bitmap(nave);
        al_destroy_bitmap(bullet);
        al_destroy_bitmap(alien);
        al_destroy_timer(timer);
        return -1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(timer);
        al_destroy_bitmap(nave);
        al_destroy_bitmap(bullet);
        al_destroy_bitmap(alien);
        al_destroy_event_queue(event_queue);
        return -1;
    }

    al_set_target_bitmap(nave);
    al_clear_to_color(al_map_rgb(255, 0, 0));
    al_set_target_bitmap(bullet);
    al_clear_to_color(al_map_rgb(255, 255, 255));
    al_set_target_bitmap(alien);
    al_clear_to_color(al_map_rgb(0, 255, 0));
    al_set_target_bitmap(al_get_backbuffer(display));

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source()); //REGISTRAMOS EL TECLADO

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    al_start_timer(timer);

    while (!do_exit) {
        ALLEGRO_EVENT ev;
        if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola, VER RETURN EN DOCUMENT.
        {
            if (ev.type == ALLEGRO_EVENT_TIMER) {

                if (key_pressed[KEY_LEFT] && nave_x >= MOVE_RATE)
                    nave_x -= MOVE_RATE;

                if (key_pressed[KEY_RIGHT] && nave_x <= SCREEN_W - 2*ALIEN_SIZE - MOVE_RATE)
                    nave_x += MOVE_RATE;
               
                redraw = true;
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
                        if(!lock)
                            shoot = true;
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

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(nave, nave_x, nave_y, 0);
            for(i=0; i<N; i++)
            {
                al_draw_bitmap(alien, alien_x[i], alien_y[i], 0);
            }
            if(shoot)
            {
                bullet_x = nave_x + ALIEN_SIZE;
                bullet_y = nave_y - ALIEN_SIZE;   
                shoot = false;
                lock = true;
            }
            if(lock)
            {
                al_draw_bitmap(bullet, bullet_x, bullet_y, 0);
                bullet_y -= MOVE_RATE;
            }
            if(bullet_y <= MOVE_RATE)
                lock = false;
            for(i=0; i<N; i++)
            {
                if(bullet_y>=alien_y[i] && bullet_y<=alien_y[i]+ALIEN_SIZE && bullet_x>=alien_x[i] && bullet_x<=alien_x[i]+ALIEN_SIZE)
                {
                    alien_y[i] = SCREEN_H;
                    lock = false;
                }
            }
            al_flip_display();
        }
    }

    al_destroy_bitmap(nave);
    al_destroy_bitmap(nave);
    al_destroy_bitmap(alien);
    al_destroy_timer(timer);
    al_destroy_display(display);
    return 0;
}