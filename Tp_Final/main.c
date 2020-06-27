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
#include "main_menu.h"
int main(void) {
    uint16_t aux=0;
    ALLEGRO_DISPLAY * display = NULL;
    ALLEGRO_EVENT_QUEUE * event_queue=NULL;
    ALLEGRO_TIMER * timer=NULL;
    ALLEGRO_FONT *font[]={NULL,NULL,NULL,NULL};//Para incluir mas de un tipo de letra , es decir mayusculas y bla bla bla
    ALLEGRO_SAMPLE * samples[]={NULL,NULL,NULL,NULL};//arreglo de canciones , para saber cuantas hay que iniciar.
    ALLEGRO_BITMAP* display_background[]={NULL,NULL};
    
    if(!allegro_ini(&display,&event_queue,&font,&samples,&timer,&display_background)){
        fprintf(stderr,"Hubo un error tato , volve a descargar el archivo , gracias\n");
    };
    
    aux=menu_display(&display,&samples[0],&event_queue,&font[0],&display_background);
    printf("%d",aux);
    return (EXIT_SUCCESS);
}
