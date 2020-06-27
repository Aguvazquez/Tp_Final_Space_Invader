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
#include "Top_Score.h"
int main(void) {
    uint16_t aux=0;
    uint8_t do_exit=false;
    ALLEGRO_DISPLAY * display = NULL;
    ALLEGRO_EVENT_QUEUE * event_queue=NULL;
    ALLEGRO_TIMER * timer=NULL;
    ALLEGRO_FONT *font[]={NULL,NULL,NULL,NULL};//Para incluir mas de un tipo de letra , es decir mayusculas y bla bla bla
    ALLEGRO_SAMPLE * samples[]={NULL,NULL,NULL,NULL};//arreglo de canciones , para saber cuantas hay que iniciar.
    ALLEGRO_BITMAP* display_background[]={NULL,NULL};// arreglo para incluir fondos.
    
    if(!allegro_ini(&display,&event_queue,&font[0],&samples[0],&timer,&display_background[0])){
        fprintf(stderr,"Hubo un error tato , volve a descargar el archivo , gracias\n");
        return -1;
    };
    
    while(!do_exit){
    
        switch(menu_display(&display,&samples[0],&event_queue,&font[0],&display_background[0])){
            case 1 :{
                al_stop_samples();//PLAY 
                do_exit=true;
            }
            break;
            case 2 ://DIFFICULTY
            break;
            case 3 :{
                aux =Top_Score(&display,&samples[0],&event_queue,&font[0],&display_background[0]);
               if(!aux){
                   fprintf(stderr,"Hubo un error tato , volve a descargar el archivo , gracias\n");
                   return -1;

               }else if(aux==2) //Si fue un 2 , entonces se apreto la x del display
                   do_exit =true;
               

            }
            break;
            default :{ fprintf(stderr,"Hubo un error tato , volve a descargar el archivo , gracias\n");
                   return -1;
            }
            break;


        }
    }
    allegro_shutdown(&event_queue,&display);
    return (EXIT_SUCCESS);
}
