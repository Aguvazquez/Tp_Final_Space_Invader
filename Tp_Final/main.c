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

#ifndef RASPBERRY

#include <allegro5/allegro.h>  
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_ttf.h> 
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h> 
    
#endif
/*
 * 
 */

#include "allegro_setup.h"
#include "config.h"
#include "menus.h"
#include "Top_Score.h"
int main(void) {

#ifdef RASPBERRY
    dips_init();
    joy_init();
    init_sound();
#else

   
    if(!allegro_ini()){
        fprintf(stderr,"Something happened, please try again later");
        return -1;
    }
    
    if(main_menu()== -1 ){
        fprintf(stderr,"Something happened, please try again later");
        return -1;
    }
    
    allegro_shutdown();
    
#endif
    return (EXIT_SUCCESS);
}
