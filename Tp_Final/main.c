/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: aguvazquez
 *
 * Created on 25 de junio de 2020, 12:31
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
/*
 * 
 */
#include "allegro_setup.h"
#include "config.h"
#include "menus.h"

int main(void) {
    uint16_t aux=0;
    bool do_exit=false;
    ALLEGRO_DISPLAY * display = NULL;
    ALLEGRO_EVENT_QUEUE * event_queue=NULL;
    ALLEGRO_TIMER * timer=NULL;
    ALLEGRO_FONT *font[FONTS]={NULL,NULL};//Para incluir mas de un tipo de letra , es decir mayusculas y bla bla bla
    ALLEGRO_SAMPLE * samples[SAMPLES]={NULL,NULL,NULL,NULL,NULL};//arreglo de canciones , para saber cuantas hay que iniciar.
    ALLEGRO_BITMAP* display_background[BACKGROUNDS]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
                                                     NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};// arreglo para incluir fondos.
    
    if(!allegro_ini(&display,&event_queue,&font[0],&samples[0],&timer,&display_background[0])){
        fprintf(stderr,"Something happened, please try again later");
        return -1;
    }
    
    if(main_menu(&display,&samples[0],&event_queue,&font[0],&display_background[0],&timer)== -1 ){
        fprintf(stderr,"Something happened, please try again later");
        return -1;
    }
    
    allegro_shutdown(&event_queue,&display);
    return (EXIT_SUCCESS);
}
