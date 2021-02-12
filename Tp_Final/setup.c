/************************* Standard libraries **********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "config.h"
#include "back_end.h"

#ifdef RASPBERRY

#include "disdrv.h"
#include  "joydrv.h"
#include "audio.h"

#endif

/*******************************************************************************/

/*************************** Resource file definitions *************************/

#define SAMPLE1 ".allegro/samples/spaceinvader_theme.wav"
#define SAMPLE2 ".allegro/samples/shoot.wav"
#define SAMPLE3 ".allegro/samples/invaderkilled.wav"
#define SAMPLE4 ".allegro/samples/explosion.wav"
#define SAMPLE5 ".allegro/samples/game.wav"

#ifndef RASPBERRY

#define INVADER ".allegro/al_backgrounds/invaders.png"
#define SPACE_INVADER_LOGO ".allegro/al_backgrounds/space-invaders-logo.png"
#define SPACESHIP ".allegro/al_backgrounds/spaceship.png"
#define SAUCER2B ".allegro/al_backgrounds/saucer2b.png"
#define SAUCER2BB ".allegro/al_backgrounds/saucer2bb.png"
#define RED_HEART ".allegro/al_backgrounds/red-heart.png"
#define MYSTERYB ".allegro/al_backgrounds/mysteryb.png"
#define SAUCER3B ".allegro/al_backgrounds/saucer3b.png"
#define SAUCER3BB ".allegro/al_backgrounds/saucer3bb.png"
#define SAUCER1B ".allegro/al_backgrounds/saucer1b.png"
#define SAUCER1BB ".allegro/al_backgrounds/saucer1bb.png"
#define FONDO1 ".allegro/al_backgrounds/fondo1.jpg"
#define FONDO2 ".allegro/al_backgrounds/fondo2.jpg"
#define FONDO3 ".allegro/al_backgrounds/fondo3.jpg"
#define FONDO4 ".allegro/al_backgrounds/fondo4.jpg"
#define FONDO5 ".allegro/al_backgrounds/fondo5.jpeg"
#define BUM ".allegro/al_backgrounds/bum.png"
#define BLOQUE1 ".allegro/al_backgrounds/bloque_1.png"
#define BLOQUE2 ".allegro/al_backgrounds/bloque_2.png"
#define BLOQUE3 ".allegro/al_backgrounds/bloque_3.png"
#define INSTRUCTIONS_1 ".allegro/al_backgrounds/instructions_1.jpeg"
#define INSTRUCTIONS_2 ".allegro/al_backgrounds/instructions_2.jpeg"

#define SPACE_TTF ".allegro/ttf/space_invaders.ttf"

/*******************************************************************************/

/***************************** Allegro libraries *******************************/

#include <allegro5/allegro.h>  
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_ttf.h> 
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h> 
#include <allegro5/allegro_image.h> 

/*******************************************************************************/

/************************** Allegro global variables ***************************/

ALLEGRO_DISPLAY *display = NULL;

ALLEGRO_EVENT_QUEUE *event_queue = NULL;

ALLEGRO_TIMER *timer = NULL;

ALLEGRO_FONT *font[FONTS] = {NULL, NULL}; 
                                        
ALLEGRO_SAMPLE *samples[SAMPLES] = {NULL, NULL, NULL, NULL, NULL};

ALLEGRO_BITMAP *display_background[BACKGROUNDS] = {NULL, NULL, NULL, NULL, NULL,
         NULL, NULL, NULL, NULL,NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
         NULL, NULL};

/*******************************************************************************/

#else

Audio* audios[SAMPLES];

#endif  //RASPBERRY

/*******************************************************************************/

/*************************** Global functions **********************************/

uint8_t files_setup (void) {
    
    if (create_Top_Score()) {   //crea el archivo donde ubicaremos el
                                //top score, en caso de ser necesario.
        return EXIT_FAILURE;
    }
    if (create_difficulty()) {  //crea el archivo donde ubicaremos la
                                //dificultad, en caso de ser necesario.
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

#ifndef RASPBERRY

uint8_t allegro_ini(void)
{
    uint8_t i;
    if(!al_init()){
        fprintf(stderr, "failed to initialize allegro!\n");
        return EXIT_FAILURE;
    }
    if(!al_install_mouse()){
        fprintf(stderr, "failed to initialize the mouse!\n");
        al_uninstall_system();
        return EXIT_FAILURE;
    }
    if(!al_init_image_addon()){
        fprintf(stderr, "failed to initialize the image!\n");
        al_uninstall_system();
        al_uninstall_mouse();
        return EXIT_FAILURE;
    }
    if(!al_install_keyboard()){
        fprintf(stderr, "failed to initialize the keyboard!\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_uninstall_mouse();
        return EXIT_FAILURE;
    }
    if(!al_init_primitives_addon()){
        fprintf(stderr, "failed to initialize primitives!\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_uninstall_mouse();
        al_uninstall_keyboard();        
        return EXIT_FAILURE;
    }
    if(!al_install_audio()){
        fprintf(stderr, "failed to initialize audio!\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_uninstall_mouse();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        return EXIT_FAILURE;
    }
    if(!al_init_acodec_addon()){
        fprintf(stderr, "failed to initialize audio codecs!\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_uninstall_mouse();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        al_uninstall_audio();
        return EXIT_FAILURE;
    }
    if(!al_reserve_samples(SAMPLES)){
        fprintf(stderr, "failed to reserve samples!\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_uninstall_mouse();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        al_uninstall_audio();
        return EXIT_FAILURE;
    }
    
    display = al_create_display(SCREEN_W, SCREEN_H); 
    if(!display){
        fprintf(stderr, "failed to create display!\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_uninstall_mouse();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        al_uninstall_audio();
        return EXIT_FAILURE;
    }
    
    event_queue = al_create_event_queue();
    if(!event_queue){
        fprintf(stderr, "failed to create event_queue!\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_uninstall_mouse();
        al_uninstall_keyboard();
        al_shutdown_primitives_addon();
        al_uninstall_audio();
        al_destroy_display(display);
        return EXIT_FAILURE;
    }
    
    al_init_font_addon(); 
    al_init_ttf_addon();
    font[0] = al_load_ttf_font(SPACE_TTF, 24, 0);    //parámetros utilizados para definir las
    font[1] = al_load_ttf_font(SPACE_TTF, 50, 0);    //fuentes a nuestro gusto
    for(i=0; i<FONTS; i++){
        if(!font[i]){
            fprintf(stderr, "font not loaded!.\n");
            al_uninstall_system();
            al_shutdown_image_addon();
            al_uninstall_mouse();
            al_uninstall_keyboard();
            al_shutdown_primitives_addon();
            al_uninstall_audio();
            al_destroy_display(display);
            al_destroy_event_queue(event_queue);
            return EXIT_FAILURE;
        }
    }
    
    samples[0] = al_load_sample(SAMPLE1);
    samples[1] = al_load_sample(SAMPLE2);
    samples[2] = al_load_sample(SAMPLE3);
    samples[3] = al_load_sample(SAMPLE4);
    samples[4] = al_load_sample(SAMPLE5);
    for(i=0; i<SAMPLES; i++){
        if(!samples[i]){
            fprintf(stderr, "Audio clip sample not loaded!\n");
            al_uninstall_system();
            al_shutdown_image_addon();
            al_uninstall_mouse();
            al_uninstall_keyboard();
            al_shutdown_primitives_addon();
            al_uninstall_audio();
            al_destroy_display(display);
            al_destroy_event_queue(event_queue);
            al_shutdown_ttf_addon();
            return EXIT_FAILURE;
        }
    }
    
    display_background[0] = al_load_bitmap(INVADER);
    display_background[1] = al_load_bitmap(SPACE_INVADER_LOGO);
    display_background[2] = al_load_bitmap(SPACESHIP);
    display_background[3] = al_load_bitmap(SAUCER2B);
    display_background[4] = al_load_bitmap(SAUCER2BB);
    display_background[5] = al_load_bitmap(RED_HEART); 
    display_background[6] = al_load_bitmap(MYSTERYB);
    display_background[7] = al_load_bitmap(SAUCER3B);
    display_background[8] = al_load_bitmap(SAUCER3BB);
    display_background[9] = al_load_bitmap(SAUCER1B);
    display_background[10] = al_load_bitmap(SAUCER1BB);
    display_background[11] = al_load_bitmap(FONDO1);
    display_background[12] = al_load_bitmap(FONDO2);
    display_background[13] = al_load_bitmap(FONDO3);
    display_background[14] = al_load_bitmap(FONDO4);
    display_background[15] = al_load_bitmap(FONDO5);
    display_background[16] = al_load_bitmap(BUM);
    display_background[17] = al_load_bitmap(BLOQUE1);
    display_background[18] = al_load_bitmap(BLOQUE2);
    display_background[19] = al_load_bitmap(BLOQUE3);
    display_background[20] = al_load_bitmap(INSTRUCTIONS_1);
    display_background[21] = al_load_bitmap(INSTRUCTIONS_2);
    //display_background[17] = al_load_bitmap("platform4.png");
    for(i=0; i<BACKGROUNDS; i++){
    if(!display_background[i]){
            fprintf(stderr, "background image not loaded!\n");
            al_uninstall_system();
            al_shutdown_image_addon();
            al_uninstall_mouse();
            al_uninstall_keyboard();
            al_shutdown_primitives_addon();
            al_uninstall_audio();
            al_destroy_display(display);
            al_destroy_event_queue(event_queue);
            al_shutdown_ttf_addon();
            return EXIT_FAILURE;
        }
    }
    
    timer = al_create_timer(1.0/FPS);
    if(!timer){
        fprintf(stderr, "Timer error!\n");
            al_uninstall_system();
            al_shutdown_image_addon();
            al_uninstall_mouse();
            al_uninstall_keyboard();
            al_shutdown_primitives_addon();
            al_uninstall_audio();
            al_destroy_display(display);
            al_destroy_event_queue(event_queue);
            al_shutdown_ttf_addon();
            return EXIT_FAILURE;
    }
    
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    
    return EXIT_SUCCESS;
}

void allegro_shutdown(void){
    
    al_uninstall_system();
    al_uninstall_mouse();
    al_uninstall_keyboard();
    al_shutdown_primitives_addon();
    al_shutdown_image_addon();
    al_uninstall_audio();
    //al_destroy_display(display);          //No entiendo porque da segmentation fault, pero
    //al_destroy_event_queue(event_queue);  //son estas dos lineas de codigo
    al_shutdown_ttf_addon();
    
}

#else

int8_t rpi_ini(void){
    disp_init();
    joy_init();
    initAudio();
    if((audios[0]=createAudio(SAMPLE1,0,127))){
        if((audios[1]=createAudio(SAMPLE2,0,127))){
            if((audios[2]=createAudio(SAMPLE3,0,127))){
                if((audios[3]=createAudio(SAMPLE4,0,127))){
                    if((audios[4]=createAudio(SAMPLE5,0,127))){
                        return EXIT_SUCCESS;
                    }
                }
            }
        }
    }
    return EXIT_FAILURE;   
}

void rpi_shutdown(void){
    int i;
    for(i=0;i<SAMPLES;i++){
        freeAudio(audios[i]);
    }
    endAudio();
}

#endif //RASPBERRY

/***************************** END FILE ****************************************/