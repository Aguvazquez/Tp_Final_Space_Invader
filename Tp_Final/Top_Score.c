#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "config.h"
#include "Top_Score.h"
#include "play.h"
#include "allegro_setup.h"

#ifndef RASPBERRY

#include <allegro5/allegro.h>  
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_ttf.h> 
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h> 

extern  ALLEGRO_DISPLAY * display;
extern  ALLEGRO_EVENT_QUEUE * event_queue;
extern  ALLEGRO_TIMER * timer;
extern  ALLEGRO_FONT *font[FONTS] ; //Para incluir mas de un tipo de letra , es decir mayusculas y bla bla bla
extern  ALLEGRO_SAMPLE * samples[SAMPLES]; //arreglo de canciones , para saber cuantas hay que iniciar.
extern  ALLEGRO_BITMAP* display_background[BACKGROUNDS]; // arreglo para incluir fondos.

#endif


uint8_t Create_Top_Score(){
    
    if(!fopen(".Top_Score.txt", "r+")){     //crea el archivo donde ubicaremos el top score.
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void put_on_top_score(uint32_t score, char *str){
    FILE* fp;
    uint32_t  j, aux=0;
    uint8_t i;
    char str1[6]={'0','0','0','0','0','\0'};
    for(i=4, j=1; i>=0; i--,j*=10){
        aux = score/j;
        str1[i]=(char)(aux%10+ASCII);        
    }
    fp = fopen(".Top_Score.txt","a");//Escribo al final del archivo.
    fputs(str1,fp);
    fputc('\n',fp);
    fputs(str,fp);
    fputc('\n',fp);
    fclose(fp);
    reorder_top_score();
}

void reorder_top_score(void){
    typedef struct{
	char score[STR_LONG];
        char name[STR_LONG];
        int32_t score_num;
        
    }user_t;
    FILE* fp;
    bool its_before_enter=0;
    char c;
    int all_names=6, i,j;
    user_t users[6],user_aux;
    user_t*puser=users;
    fp=fopen(".Top_Score.txt","r");
    
    while(all_names>0){   
        for(i=0;i<12;i++){
            c=fgetc(fp);
            if(c!='\n'){
                if(!its_before_enter){
                    puser->score[i]=c;
                }
                else{
                   puser->name[i-6]=c;
                }
            }
            else{
		if(!its_before_enter){
                    puser->score[i]='\0';
		    its_before_enter=1;
            	}
		else{
                    puser->name[i-6]='\0';
                    its_before_enter=0;
		}
            }
        }   
        ++puser;
        --all_names;
		
    }
    for(i=0;i<6;i++){
        users[i].score_num=string_to_number(users[i].score);
        //fprintf(stderr,"%d\n",users[i].score_num);
        //fprintf(stderr,"%s\n",users[i].score);
    }
    for(i=0;i<6;i++){
        for(j=1+i;j<6;j++){
            if((users[i].score_num)<(users[j].score_num)){
                user_aux=users[i];
                users[i]=users[j];
                users[j]=user_aux;
            }
        }
    }
    fclose(fp);
    fp=fopen(".Top_Score.txt","w");
    for(i=0;i<5;i++){               // Aca iria comparado con un 5 m para que ultimo no se copie , pero como estoy haciendo pruebas le puse que copie el 6to tmb
        fputs(users[i].score,fp);   //otro comentario a destacar , solo hay que llamar a esta funcion cuando el jugador ponga su nombre.
        fputc('\n',fp);
        fputs(users[i].name,fp);
        fputc('\n',fp);
    }
    fclose(fp);
    //fprintf(stderr,"%s\n",users[5].score); funciona , puse esta linea para poder probarlo en gedit.
}