/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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
#include <allegro5/allegro_image.h>
 
#include "config.h"  //header with defines 

uint16_t allegro_ini(ALLEGRO_DISPLAY** display,ALLEGRO_EVENT_QUEUE**  event_queue
                        ,ALLEGRO_FONT*font[],ALLEGRO_SAMPLE *sample[],ALLEGRO_TIMER **timer,
                        ALLEGRO_BITMAP* display_background[])
{
    uint16_t i;
    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return 0;
    }
    if (!al_install_mouse()) {
        fprintf(stderr, "failed to initialize the mouse!\n");
        al_uninstall_system();
        return 0;
    }
    if(!al_init_image_addon()){
        fprintf(stderr, "failed to initialize the image!\n");
        al_uninstall_system();
        al_uninstall_mouse();
        return 0;
    }
    
    if (!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_uninstall_mouse();
        return 0;
    }
    
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize primitives!\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_uninstall_mouse();
        al_uninstall_keyboard();        
        return 0;
    }
    
    if (!al_install_audio()) {
        fprintf(stderr, "failed to initialize audio!\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_uninstall_mouse();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        return 0;
    }

    if (!al_init_acodec_addon()) {
        fprintf(stderr, "failed to initialize audio codecs!\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_uninstall_mouse();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        al_uninstall_audio();
        return 0;
    }

    if (!al_reserve_samples(SAMPLES)) {
        fprintf(stderr, "failed to reserve samples!\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_uninstall_mouse();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        al_uninstall_audio();
        return 0;
    }
 
    *display = al_create_display(SCREEN_W, SCREEN_H); 
    if (!display) {
        fprintf(stderr, "failed to create display!\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_uninstall_mouse();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        al_uninstall_audio();
        return 0;
    }
    
    *event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_uninstall_mouse();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        al_uninstall_audio();
        al_destroy_display(*display);
        return 0;
    }
    
    al_init_font_addon(); 
    al_init_ttf_addon();
    font[0] = al_load_ttf_font("space_invaders.ttf", 22, 0); 
    font[1] = al_load_ttf_font("space_invaders.ttf", 40, 0);
    for(i=0;i<FONTS;i++){
        if (!font[i]) {
            fprintf(stderr, "No se pudo cargar una fuente.\n");
            al_uninstall_system();
            al_shutdown_image_addon();
            al_uninstall_mouse();
            al_uninstall_keyboard();
            al_shutdown_primitives_addon();
            al_uninstall_audio();
            al_destroy_display(*display);
            al_destroy_event_queue(*event_queue);
            return 0;
        }
    }
    
    sample[0] = al_load_sample("spaceinvader_theme.wav");
    sample[1] = al_load_sample("shoot.wav");
    sample[2] = al_load_sample("invaderkilled.wav");
    sample[3] = al_load_sample("explosion.wav");
    for(i=0;i<SAMPLES;i++){
        if (!sample) {
            fprintf(stderr,"Audio clip sample not loaded!\n");
            al_uninstall_system();
            al_shutdown_image_addon();
            al_uninstall_mouse();
            al_uninstall_keyboard();
            al_shutdown_primitives_addon();
            al_uninstall_audio();
            al_destroy_display(*display);
            al_destroy_event_queue(*event_queue);
            al_shutdown_ttf_addon();
            return 0;
        }
    }
    display_background[0] = al_load_bitmap("invaders.png");
    display_background[1] = al_load_bitmap("space-invaders-logo.png");
    display_background[2] = al_load_bitmap("baseshipa.png");
    display_background[3] = al_load_bitmap("saucer2b.png");
    display_background[4] = al_load_bitmap("saucer2bb.png");
    display_background[5] = al_load_bitmap("red-heart.png"); 
    display_background[6] = al_load_bitmap("mysteryb.png");
    display_background[7] = al_load_bitmap("saucer3b.png");
    display_background[8] = al_load_bitmap("saucer3bb.png");
    display_background[9] = al_load_bitmap("saucer1b.png");
    display_background[10] = al_load_bitmap("saucer1bb.png");
    display_background[11] = al_load_bitmap("fondo1.jpeg");
    display_background[12] = al_load_bitmap("fondo2.jpg");
    display_background[13] = al_load_bitmap("fondo3.jpg");
    display_background[14] = al_load_bitmap("fondo4.jpg");
    display_background[15] = al_load_bitmap("fondo5.jpg");
    for(i=0;i<BACKGROUNDS;i++){
    if (!display_background[i]) {
            fprintf(stderr,"background image not loaded!\n");
            al_uninstall_system();
            al_shutdown_image_addon();
            al_uninstall_mouse();
            al_uninstall_keyboard();
            al_shutdown_primitives_addon();
            al_uninstall_audio();
            al_destroy_display(*display);
            al_destroy_event_queue(*event_queue);
            al_shutdown_ttf_addon();
            return 0;
        }
    }
    *timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr,"Timer error!\n");
            al_uninstall_system();
            al_shutdown_image_addon();
            al_uninstall_mouse();
            al_uninstall_keyboard();
            al_shutdown_primitives_addon();
            al_uninstall_audio();
            al_destroy_display(*display);
            al_destroy_event_queue(*event_queue);
            al_shutdown_ttf_addon();
            return 0;
    }
    al_register_event_source(*event_queue, al_get_keyboard_event_source());
    al_register_event_source(*event_queue, al_get_display_event_source(*display));
    al_register_event_source(*event_queue, al_get_mouse_event_source());
    al_register_event_source(*event_queue, al_get_timer_event_source(*timer));
    
    
    return 1;
}
void allegro_shutdown(ALLEGRO_EVENT_QUEUE** event_queue,ALLEGRO_DISPLAY **display){
    
    al_uninstall_system();
    al_uninstall_mouse();
    al_uninstall_keyboard();
    al_shutdown_primitives_addon();
    al_shutdown_image_addon();
    al_uninstall_audio();
    /*al_destroy_display(*display); // No entiendo porque da segmentation fault , pero son estas dos lineas de codigo
    al_destroy_event_queue(*event_queue);*/
    al_shutdown_ttf_addon();
    
}